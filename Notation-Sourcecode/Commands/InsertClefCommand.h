// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  Inserts a Clef and updates the times of subsequent temporals
    class InsertClefCommand : public Command
    {
    public:
        virtual ~InsertClefCommand();
        InsertClefCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          Measure& inMeasure,
                          ItemID inItemID);

        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        Measure& mMeasure;
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
