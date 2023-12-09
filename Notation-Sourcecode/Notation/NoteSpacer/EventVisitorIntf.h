// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    class EventIntf;
    class Sim;

    class EventVisitorIntf
    {
    public:
        virtual ~EventVisitorIntf() = default;
        virtual void visit(EventIntf& ioEvent, Sim& ioSim) = 0;

        /// call before the first visit
        virtual void notifyBeginVisiting() = 0;

        /// call after the last visit
        virtual void notifyDoneVisiting() = 0;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
