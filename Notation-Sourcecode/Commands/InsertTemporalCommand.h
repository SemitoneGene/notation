// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  Inserts a Note or Rest and updates the
    /// times of subsequent temporals
    ///
    class InsertTemporalCommand : public Command
    {
    public:
        virtual ~InsertTemporalCommand();
        InsertTemporalCommand(CommandFactory& inFactory,
                              ItemRegistry& inRegistry,
                              ItemID inItemID,
                              Measure& inMeasure,
                              bool inDoOffsetItemTimes = true);

        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        Measure& mMeasure;
        bool mDoOffsetItemTimes;
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
