// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ItemStateCommand.h"

// Local
#include "Score.h"
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Throw.h"
#include "Chord.h"
#include "ChordStateCommand.h"

namespace komp
{    
    ItemStateCommand::~ItemStateCommand() = default;
    
    ItemStateCommand::ItemStateCommand(CommandFactory& inFactory,
                                       ItemRegistry& inRegistry,
                                       ItemUPtr inOriginalState,
                                       ItemUPtr inAlteredState)
    : komp::Command("Item State Change", inFactory, inRegistry)
    , mItemID(inOriginalState->getItemID())
    , mOriginalState(std::move(inOriginalState))
    , mAlteredState(std::move(inAlteredState))
    {
        KOMP_ASSERT(mOriginalState->getItemID() == mAlteredState->getItemID());
    }


    CommandResult
    ItemStateCommand::executeImpl()
    {
        KOMP_LOG_COMMAND;
        auto& registryItem = mRegistry.getItem(mItemID);
        registryItem.update();
        return false;
    }
    
    void
    ItemStateCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& registryItem = mRegistry.getItem(mItemID);
        registryItem.restoreMemento(*mOriginalState);
        undoSubCommands();
        registryItem.update();
    }
    
    /// \brief default implementation - most subclasses will override this method
    void
    ItemStateCommand::redo()
    {
        KOMP_LOG_COMMAND
        auto& registryItem = mRegistry.getItem(mItemID);
        registryItem.restoreMemento(*mAlteredState);
        redoSubCommands();
        registryItem.update();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
