// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "EventVisitorIntf.h"
#include "PlaneID.h"

#include <set>

namespace komp
{
    /// \brief  The class is named CollisionVisitor.
    ///
    /// \discussion The class named CollisionVisitor does stuff.
    ///
    class CollisionVisitor : public EventVisitorIntf
    {
    public:
        virtual ~CollisionVisitor() = default;
        CollisionVisitor();

    public:
        virtual void visit(EventIntf& ioEvent, Sim& ioSim) override;
        virtual void notifyBeginVisiting() override;
        virtual void notifyDoneVisiting() override;

        double getMeasureNoteSpace() const;

    private:
        bool mIsVisitCalled;
        double mMeasureNoteSpace;
        Sim* mPreviousSim;

    private:
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
