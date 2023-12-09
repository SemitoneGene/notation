// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    class InsertTieCommand : public Command
    {
    public:
        InsertTieCommand(CommandFactory& inFactory,
                         ItemRegistry& inRegistry,
                         ItemID inStart,
                         ItemID inSecondNote);
        
        virtual ~InsertTieCommand() = default;
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mTieID;
        ItemID mFirstTemporalID;
        ItemID mSecondTemporalID;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
