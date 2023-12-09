// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TupletParams.h"

// Local
#include "Chord.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{

TupletParams::TupletParams()
: tupletRatio
{
    DurationCount {
        3,
        TemporalType::EIGHTH,
        0
    },
    DurationCount {
        2,
        TemporalType::EIGHTH, 0
    }
}
, mTemporals{}
{
}

bool
TupletParams::getIsDurationValid() const
{
    if (mTemporals.empty()) {
        return false;
    }
    
    const auto voice = (*mTemporals.cbegin())->getLocation().getVoice();
    
    double totalDuration = 0.0;
    for (const auto temporal : mTemporals) {
        
        if (temporal->getLocation().getVoice() != voice) {
            LOG_F(ERROR, "tuplet cannot span voices");
            return false;
        }
        
        totalDuration += temporal->getDuration().getUntupletedClocks();
    }
    
    const int roundedTotal = static_cast<int>(std::ceil(totalDuration - 0.5));
    const int actual = tupletRatio.getActual().getRoundedClocks();
    return roundedTotal == actual;
}


void
TupletParams::addTemporal(Temporal& inTemporal)
{
    mTemporals.insert(&inTemporal);
    correctForChords();
}


void
TupletParams::setTemporals(const TemporalRefVec& inTemporals)
{
    mTemporals.clear();
    
    for (Temporal& temporal : inTemporals) {
        mTemporals.emplace(&temporal);
    }
    
    correctForChords();
}


TemporalRefVec
TupletParams::getTemporals() const
{
    TemporalRefVec result;
    
    for (const auto& temporal : mTemporals) {
        result.emplace_back(*temporal);
    }
    
    return result;
}


void
TupletParams::correctForChords()
{
    std::set<Temporal*> newSet;
    
    for (const auto temporal : mTemporals) {
        if (temporal->getIsChordMember()) {
            auto chord = temporal->getChord();
            newSet.emplace(chord);
        } else {
            newSet.insert(temporal);
        }
    }
    
    std::swap(newSet, mTemporals);
}


DurationCount
TupletParams::calculateDurationCount(const TemporalRefVec& inTemporals,
                                     TemporalType inPreferedTemporalType)
{
    return DurationCount{1, TemporalType::WHOLE, 1};
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
