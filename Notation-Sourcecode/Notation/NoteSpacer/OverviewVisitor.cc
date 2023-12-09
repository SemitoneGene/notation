// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "OverviewVisitor.h"

// Local
#include "EventIntf.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Constants.h"
#include "Throw.h"

#include <cmath>

namespace komp
{

// i.e. default to whole note
static constexpr const double CLOCKS_PER_WHOLE_NOTE = static_cast<double>(TIME_BASE) * 4.0;
static constexpr const double DEFAULT_DURATION_WHOLE_NOTES = 1.0;
static constexpr const double DEFAULT_GOURLAY_K = 5.0;
static constexpr const double EIGHTH_NOTE_DURATION_WHOLE_NOTES = 1.0 / 8.0;

OverviewVisitor::OverviewVisitor(Measure& inMeasure)
: mIsVisitCalled{false}
, mMinDurationWholeNotes{-1.0}
, mMaxDurationWholeNotes{-1.0}
, mGourlayK{DEFAULT_GOURLAY_K}
, mNumEvents{0}
, mNumSims{0}
, mPreviousSim{nullptr}
, mPlaneIDs{}
, mMeasure{inMeasure}
{
    
}


void
OverviewVisitor::visit(EventIntf& ioEvent, Sim& ioSim)
{
    double durationClocks = 0.0;
    auto& item = ioEvent.getItem();
    
    if (item.is<Temporal>()) {
        auto& temporal = item.as<Temporal>();
        durationClocks = temporal.getDuration().getClocks();
    } else {
        const auto& next = ioEvent.getNextPlanarEvent();
        
        if (next) {
            const auto itemTime = item.getTime();
            const auto nextTime = next->getItem().getTime();
            durationClocks = nextTime - itemTime;
        }
    }
    
    // normalize to fractions of a quarter-note for gourlay spacing algorithm
    const double durationWholeNotes = durationClocks <= 0.0 ? DEFAULT_DURATION_WHOLE_NOTES : durationClocks / CLOCKS_PER_WHOLE_NOTE;
    
    if (!mIsVisitCalled) {
        mMinDurationWholeNotes = durationWholeNotes;
        mMaxDurationWholeNotes = durationWholeNotes;
        mIsVisitCalled = true;
        return;
    } else {
        if (durationWholeNotes < mMinDurationWholeNotes) {
            mMinDurationWholeNotes = durationWholeNotes;
        }
        
        if (durationWholeNotes > mMaxDurationWholeNotes) {
            mMaxDurationWholeNotes = durationWholeNotes;
        }
    }
    
    ++mNumEvents;
    
    if (&ioSim != mPreviousSim) {
        ++mNumSims;
        mPreviousSim = &ioSim;
    }
    
    mPlaneIDs.insert(ioEvent.getPlaneID());
}


void
OverviewVisitor::notifyBeginVisiting()
{
    
}


void
OverviewVisitor::notifyDoneVisiting()
{
    mIsVisitCalled = true;
    
    if (mMinDurationWholeNotes <= 0) {
        mMinDurationWholeNotes = DEFAULT_DURATION_WHOLE_NOTES;
    }
    
    if (mMaxDurationWholeNotes <= 0) {
        mMaxDurationWholeNotes = DEFAULT_DURATION_WHOLE_NOTES;
    }
    
    mPreviousSim = nullptr;
    
    calculateGourlayK();
}


double
OverviewVisitor::getMinDurationWholeNotes() const
{
    if (mMinDurationWholeNotes <= 0) {
        return DEFAULT_DURATION_WHOLE_NOTES;
    }
    
    return mMinDurationWholeNotes;
}


double
OverviewVisitor::getMaxDurationWholeNotes() const
{
    if (mMaxDurationWholeNotes <= 0) {
        return DEFAULT_DURATION_WHOLE_NOTES;
    }
    
    return mMaxDurationWholeNotes;
    
}

const double
OverviewVisitor::getClocksPerWholeNote() const
{
    return CLOCKS_PER_WHOLE_NOTE;
}



const double
OverviewVisitor::getIdealSpaceForDuration(double inDurationWholeNotes) const
{
    KOMP_ASSERT(inDurationWholeNotes > 0.0);
    KOMP_ASSERT(std::isfinite(inDurationWholeNotes));
    const double log2Dur = std::log2(inDurationWholeNotes);
    const double result = log2Dur + getGourlayK();
    return result;
}


const double
OverviewVisitor::getGourlayK() const
{
    return mGourlayK;
}


const Measure&
OverviewVisitor::getMeasure() const
{
    return mMeasure;
}


void
OverviewVisitor::calculateGourlayK()
{
    if (!mIsVisitCalled) {
        return;
    }
    
    const auto minDurWholeNotes = std::min(getMinDurationWholeNotes(), EIGHTH_NOTE_DURATION_WHOLE_NOTES);
    const auto log2OfMin = std::log2(minDurWholeNotes);
    mGourlayK = 2 - log2OfMin;
    KOMP_ASSERT(mGourlayK > 0.0);
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
