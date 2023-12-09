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
    class AddMarkCommand : public Command
    {
    public:
        AddMarkCommand(CommandFactory& inFactory,
                       ItemRegistry& inRegistry,
                       ItemID inItemID,
                       komp::MarkType inMarkType,
                       komp::PlacementType inPlacementType);
        
        virtual ~AddMarkCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemRegistry& mRegistry;
        ItemID mMarkID;
        ItemID mTemporalID;
        MarkType mMarkType;
        PlacementType mPlacementType;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
