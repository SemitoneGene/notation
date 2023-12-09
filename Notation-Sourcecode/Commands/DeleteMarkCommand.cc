// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteMarkCommand.h"

// Local
#include "Score.h"
#include "ItemRegistry.h"
#include "Temporal.h"
#include "Mark.h"
#include "Throw.h"

namespace komp
{
    DeleteMarkCommand::~DeleteMarkCommand() = default;
    
DeleteMarkCommand::DeleteMarkCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     const Mark& inMark)
    : komp::Command("Delete Mark", inFactory, inRegistry)
    , mItemID(inMark.getItemID())
    {
        
    }
    
    
    CommandResult
    DeleteMarkCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mItemID);
        KOMP_ASSERT(item.getItemTraits().getItemType() == ItemType::Mark);
        auto& mark = item.as<Mark>();
        auto& spatial = mark.getRelativePoint().getItem();
        KOMP_ASSERT(spatial.is<Temporal>());
        auto& temporal = spatial.as<Temporal>();
        auto& measure = mark.getLocation().getMeasure();
        temporal.getAttachments().removeMark(mark);
        measure.removeItem(mark);
        return true;
    }
    
    
    void
    DeleteMarkCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mItemID);
        KOMP_ASSERT(item.getItemTraits().getItemType() == ItemType::Mark);
        auto& mark = item.as<Mark>();
        auto& spatial = mark.getRelativePoint().getItem();
        KOMP_ASSERT(spatial.is<Temporal>());
        auto& temporal = spatial.as<Temporal>();
        temporal.getAttachments().addMark(mRegistry, temporal, mark);
        auto& measure = mark.getLocation().getMeasure();
        measure.insertItem(mark, mark.getTime());
        undoSubCommands();
    }
    
    
    void
    DeleteMarkCommand::redo()
    {
        KOMP_LOG_COMMAND
        execute();
        redoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
