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
    /// \brief  The class is named IdealWidthsVisitor.
    ///
    /// \discussion The class named IdealWidthsVisitor does stuff.
    ///
    class IdealWidthsVisitor : public SimVisitorIntf
    {
    public:
        virtual ~IdealWidthsVisitor() = default;
        IdealWidthsVisitor(OverviewVisitor inOverviewVisitor);

    public:
        virtual void visit(Sim& ioSim) override;

    private:
        OverviewVisitor mOverviewVisitor;
        bool mIsVisitCalled;

    private:
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
