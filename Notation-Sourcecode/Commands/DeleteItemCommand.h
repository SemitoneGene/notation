// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  deletes a simple item
    ///
    /// \discussion deletes a simple item that does not have
    /// any additional logic required
    ///
    class DeleteItemCommand : public Command
    {
    public:
        DeleteItemCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          Measure& inMeasure,
                          ItemID inItemID);
        
        virtual ~DeleteItemCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        ItemUPtr mMemento;
        Measure& mMeasure;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
