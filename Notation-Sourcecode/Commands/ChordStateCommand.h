// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "Duration.h"
#include "StemPolicy.h"

namespace komp
{
    /// \brief  swaps between to states for a single Item
    ///
    /// \discussion - TODO describe the slightly unusual usage
    ///
    class ChordStateCommand : public Command
    {
    public:
        ChordStateCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          ItemUPtrVec&& inOriginalNoteMementos,
                          ItemUPtr inOriginalState,
                          ItemUPtr inAlteredState);
        virtual ~ChordStateCommand() = default;

        virtual void undo() override;
        virtual void redo() override;
        
    private:
        virtual CommandResult executeImpl() override;
        
        ItemID mItemID;
        std::vector<ItemID> mNoteIDs;
        ItemUPtr mOriginalState;
        ItemUPtr mAlteredState;
        Duration mOriginalDuration;
        Duration mAlteredDuration;
        double mOriginalX;
        double mAlteredX;
        StemPolicy mOriginalStemPolicy;
        StemPolicy mAlteredStemPolicy;
        ItemUPtrVec mOriginalNoteMementos;
        ItemUPtrVec mAlteredNoteMementos;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
