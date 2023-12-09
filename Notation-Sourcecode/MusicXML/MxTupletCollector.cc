// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// self
#include "MxTupletCollector.h"

// Local
#include "Forward.h"
#include "Constants.h"
#include "Measure.h"
#include "MusicLocation.h"
#include "MxConverter.h"
#include "Score.h"
#include "Throw.h"
#include "Temporal.h"
#include "TupletParams.h"
#include "Tuplet.h"

namespace komp
{
MxTupletCollector::MxTupletCollector()
: mOpenTuplets{}
, mClosedTuplets{}
{
    
}


void
MxTupletCollector::startTuplet(mx::api::TupletStart inStart,
                               mx::api::NoteData inFirstNote,
                               Temporal& inTemporal)
{
    if (mOpenTuplets.size() >= static_cast<size_t>(MAX_TUPLET_NESTING)) {
        LOG_F(WARNING, "encountered nested tuplets beyond this applications limit");
        mOpenTuplets.clear();
        return;
    }
    
    auto& loc = inTemporal.getLocation();
    MxTupletSpec spec(loc);
    spec.mxTupletStart = std::move(inStart);
    spec.mxFirstNote = std::move(inFirstNote);
    mOpenTuplets.emplace_back(std::move(spec));
    addToCurrentTuplet(inTemporal);
}


void
MxTupletCollector::addToCurrentTuplet(Temporal& inTemporal)
{
    if (mOpenTuplets.empty()) {
        LOG_F(WARNING, "attempted to add a note to a tuplet when no tuplets were active");
        return;
    }
    
    for (auto& spec : mOpenTuplets) {
        spec.temporals.emplace(&inTemporal);
        spec.replaceChordMembers();
    }
}


void
MxTupletCollector::stopTuplet(mx::api::TupletStop inStop,
                              Temporal& inTemporal)
{
    if (mOpenTuplets.empty()) {
        LOG_F(WARNING, "attempted to stop a tuplet when no tuplets were active");
        return;
    }
    
    addToCurrentTuplet(inTemporal);
    
    // if numberLevel is not available, use the most recent tuplet
    auto iter = mOpenTuplets.begin();
    
    // try to use numberLevel if it is available
    if (inStop.numberLevel > 0) {
        for (auto it = mOpenTuplets.begin(); it != mOpenTuplets.end(); ++it) {
            if (it->mxTupletStart.numberLevel == inStop.numberLevel) {
                iter = it;
                break;
            }
        }
    }
    
    iter->mxTupletStop = inStop;
    mClosedTuplets.push_back(*iter);
    mOpenTuplets.erase(iter);
}


MxTupletVec
MxTupletCollector::getTupletSpecs() const
{
    return mClosedTuplets;
}


void
MxTupletCollector::insertTuplets()
{
    warnIfOpenTupletsExist();
    
    auto iter = mClosedTuplets.begin();
    const auto end = mClosedTuplets.end();
    
    for ( ; iter != end; ++iter) {
        
        auto& tupletSpec = *iter;
        tupletSpec.replaceChordMembers();
        
        TupletParams params;
        
        if (!validateAndWarn(tupletSpec, params)) {
            continue;
        }
        
        insertTuplet(tupletSpec, std::move(params));
    }
}


void
MxTupletCollector::warnIfOpenTupletsExist() const
{
    if (mOpenTuplets.size() > 0) {
        LOG_F(WARNING, "unfinished tuplets still exist");
    }
}


bool
MxTupletCollector::validateAndWarn(const MxTupletSpec& inSpec,
                                   TupletParams& outParams) const
{
    const auto actualCount = inSpec.mxTupletStart.actualNumber;
    const auto actualType = parseActualType(inSpec);
    auto actualDots = std::max(inSpec.mxTupletStart.actualDots, 0);
    const auto normalCount = inSpec.mxTupletStart.normalNumber;
    const auto normalType = parseNormalType(inSpec, actualType);
    auto normalDots = std::max(inSpec.mxTupletStart.normalDots, 0);
    if (actualDots > MAX_DOTS) {
        actualDots = MAX_DOTS;
    } else if (actualDots < 0) {
        actualDots = 0;
    }
    if (normalDots > MAX_DOTS) {
        normalDots = MAX_DOTS;
    } else if (normalDots < 0) {
        normalDots = 0;
    }
    
    if (actualCount < 1) {
        LOG_F(INFO, "invalid actualCount");
        return false;
    }
    
    if (normalCount < 1) {
        LOG_F(INFO, "invalid normalCount");
        return false;
    }
    
    DurationCount actual{actualCount, actualType, actualDots};
    DurationCount normal{normalCount, normalType, normalDots};
    TupletRatio ratio{std::move(actual), std::move(normal)};
    outParams.tupletRatio = std::move(ratio);
    populateTemporals(inSpec, outParams);
    
    if (!outParams.getIsDurationValid()) {
        LOG_F(INFO, "tuplet specification and temporal duration total do not match");
        return false;
    }
    
    return true;
}

TemporalType
MxTupletCollector::parseActualType(const MxTupletSpec& inSpec) const
{
    auto mxActualType = inSpec.mxTupletStart.actualDurationName;
    
    if (mxActualType == mx::api::DurationName::unspecified) {
        mxActualType = inSpec.mxFirstNote.durationData.durationName;
    }
    
    return MxConverter::convertTemporalType(mxActualType);
}


TemporalType
MxTupletCollector::parseNormalType(const MxTupletSpec& inSpec,
                                   const TemporalType inActualType) const
{
    auto result = inActualType;
    auto mxType = inSpec.mxTupletStart.normalDurationName;
    
    if (mxType != mx::api::DurationName::unspecified) {
        result = MxConverter::convertTemporalType(mxType);
    }
    
    return result;
}


void
MxTupletCollector::populateTemporals(const MxTupletSpec& inSpec,
                                     TupletParams& outParams) const
{
    for (const auto& temporal : inSpec.temporals) {
        outParams.addTemporal(*temporal);
    }
}


void
MxTupletCollector::insertTuplet(const MxTupletSpec& inSpec,
                                TupletParams&& inParams) const
{
    auto& score = inSpec.measure.getScore();
    auto& registry = score.getItemRegistry();
    const MusicLocation loc(inSpec.measure, inSpec.track);
    auto& tuplet = registry.createItem<Tuplet>(loc, std::move(inParams));
    tuplet.setIsQuiescent(false);
    inSpec.measure.insertItem(tuplet);
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
