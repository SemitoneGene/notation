// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  adds a mark to the note or rest
    ///
    class DeleteMarkCommand : public Command
    {
    public:
        DeleteMarkCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          const Mark& inMark);
        
        virtual ~DeleteMarkCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
