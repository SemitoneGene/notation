// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MinimumMeasureWidthVisitor.h"

// Local
#include "EventIntf.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Constants.h"
#include "Throw.h"
#include "Sim.h"
#include "Staff.h"
#include "Measure.h"

#include <cmath>

namespace komp
{
    MinimumMeasureWidthVisitor::MinimumMeasureWidthVisitor(OverviewVisitor inOverviewVisitor, double inMinimumMeasureWidth)
    : mMinimumNoteSpace(inMinimumMeasureWidth)
    , mIsVisitCalled{false}
    , mOverviewVisitor{std::move(inOverviewVisitor)}
    , mFirstSimX{NAN}
    , mCurrentNoteSpace{NAN}
    , mMeasureStretchAmount{NAN}
    , mMeasureStretchRatio{NAN}
    , mStretchAreaSize{NAN}
    , mStetchedAreaGoalSize{NAN}
    , mStretchAreaRatio{NAN}
    , mIsLastSimInLastBeat{false}
    {

    }


    void
    MinimumMeasureWidthVisitor::visit(Sim& ioSim)
    {
        if (!mIsVisitCalled) {
            mIsVisitCalled = true;
            setup(ioSim);
        } else {
            if (mStretchAreaRatio == 1.0) {
                return;
            }

            const auto oldX = ioSim.getX();
            const auto newX = oldX * mStretchAreaRatio;
            ioSim.setX(newX);
        }
    }


    void
    MinimumMeasureWidthVisitor::setup(Sim& ioSim)
    {
        mFirstSimX = ioSim.getX();
        auto lastSim = &ioSim;

        while (lastSim->getNextSim() != nullptr) {
            lastSim = lastSim->getNextSim();
        }

        KOMP_ASSERT(lastSim);

        const auto& measure = mOverviewVisitor.getMeasure();
        const auto ticksPerBeat = measure.getTicksPerBeat();
        const auto totalTicks = measure.getClocks();
        const auto lastBeatClocks = totalTicks - ticksPerBeat;
        mIsLastSimInLastBeat = static_cast<double>(lastSim->getTime()) >= lastBeatClocks;

        if (mIsLastSimInLastBeat) {
            const auto rightSpace = lastSim->getLargestRightWidth();
            mCurrentNoteSpace = rightSpace + lastSim->getX();
            mMeasureStretchAmount = mMinimumNoteSpace - mCurrentNoteSpace;
        } else {
            const double lastSimTime = lastSim->getTime();
            const double lastSimX = lastSim->getX();
            const double lastSimWidth = lastSim->getIdealWidth();
            const double lastSimDurationClocks = lastSim->getDuration();
            const double measureRemainingClocks = totalTicks - lastSimTime;
            KOMP_ASSERT(lastSimDurationClocks > 0);
            const double pixelsPerTick = lastSimWidth / lastSimDurationClocks;
            const double distanceToEnd = measureRemainingClocks * pixelsPerTick;
            const double lastSimMaxRightWidth = lastSim->getLargestRightWidth();
            const double calculatedNoteSpace = lastSimX + distanceToEnd;
            const double minimumNoteSpace = lastSimX + lastSimMaxRightWidth;
            mCurrentNoteSpace = std::max(minimumNoteSpace, calculatedNoteSpace);
        }

        if (mMeasureStretchAmount > 0 && mCurrentNoteSpace > 0 && &ioSim != lastSim) {
            mMeasureStretchRatio = mMinimumNoteSpace / mCurrentNoteSpace;
            mStretchAreaSize = mCurrentNoteSpace - ioSim.getX();
            mStetchedAreaGoalSize = mMinimumNoteSpace - ioSim.getX();
            KOMP_ASSERT(mStretchAreaSize != 0.0);
            mStretchAreaRatio = mStetchedAreaGoalSize / mStretchAreaSize;
        } else {
            mMeasureStretchAmount = 0.0;
            mMeasureStretchRatio = 1.0;
            mStretchAreaSize = mMinimumNoteSpace;
            mStetchedAreaGoalSize = mMinimumNoteSpace;
            mStretchAreaRatio = 1.0;
        }
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
