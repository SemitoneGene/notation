// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  swaps between to states for a single Item
    ///
    /// \discussion - TODO describe the slightly unusual usage
    ///
    class ItemStateCommand : public Command
    {
    public:
        ItemStateCommand(CommandFactory& inFactory,
                         ItemRegistry& inRegistry,
                         ItemUPtr inOriginalState,
                         ItemUPtr inAlteredState);
        virtual ~ItemStateCommand();

        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        ItemUPtr mOriginalState;
        ItemUPtr mAlteredState;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
