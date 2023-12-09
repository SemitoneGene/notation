// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "SimVisitorIntf.h"
#include "OverviewVisitor.h"
#include "PlaneID.h"

#include <set>

namespace komp
{
    /// \brief  The class is named MinimumMeasureWidthVisitor.
    ///
    /// \discussion The class named MinimumMeasureWidthVisitor does stuff.
    ///
    class MinimumMeasureWidthVisitor : public SimVisitorIntf
    {
    public:
        virtual ~MinimumMeasureWidthVisitor() = default;
        MinimumMeasureWidthVisitor(OverviewVisitor inOverviewVisitor, double inMinimumMeasureWidth);

    public:
        virtual void visit(Sim& ioSim) override;

    private:
        double mMinimumNoteSpace;
        bool mIsVisitCalled;
        OverviewVisitor mOverviewVisitor;
        double mFirstSimX;
        double mCurrentNoteSpace;
        double mMeasureStretchAmount;
        double mMeasureStretchRatio;
        double mStretchAreaSize;
        double mStetchedAreaGoalSize;
        double mStretchAreaRatio;
        bool mIsLastSimInLastBeat;

    private:
        void setup(Sim& ioSim);
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
