// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    class InsertSlurCommand : public Command
    {
    public:
        InsertSlurCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          ItemID inStart,
                          ItemID inLast);
        
        virtual ~InsertSlurCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mSlurID;
        ItemID mFirstTemporalID;
        ItemID mLastTemporalID;
        virtual CommandResult executeImpl() override;
        
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
