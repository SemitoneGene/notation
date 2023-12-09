// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "AddMarkCommand.h"

// Local
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Mark.h"
#include "Measure.h"
#include "Temporal.h"
#include "Throw.h"

namespace komp
{
    AddMarkCommand::~AddMarkCommand() = default;
    
    /// \brief The basic command to be subclassed
AddMarkCommand::AddMarkCommand(CommandFactory& inFactory,
                               ItemRegistry& inRegistry,
                               ItemID inItemID,
                               komp::MarkType inMarkType,
                               komp::PlacementType inPlacementType)
    : komp::Command("Add Mark", inFactory, inRegistry)
    , mRegistry{inRegistry}
    , mMarkID(0)
    , mTemporalID(inItemID)
    , mMarkType(inMarkType)
    , mPlacementType(inPlacementType)
    {
        
    }
    
    
    CommandResult
    AddMarkCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        const auto& item = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(item.is<Temporal>());
        auto& temporal = mRegistry.getItem(mTemporalID).as<Temporal>();
        auto& mark = temporal.getAttachments().addMark(mRegistry,
                                                       temporal,
                                                       mMarkType,
                                                       mPlacementType);
        mMarkID = mark.getItemID();
        ItemImpl::setNeedsDisplay(mark);
        return true;
    }
    
    
    void
    AddMarkCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& noteItem = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(noteItem.is<Temporal>());
        auto& temporal = noteItem.as<Temporal>();
        auto& markItem = mRegistry.getItem(mMarkID);
        KOMP_ASSERT(markItem.getItemTraits().getItemType() == ItemType::Mark);
        auto& mark = markItem.as<Mark>();
        temporal.getAttachments().removeMark(mark);
        auto& measure = mark.getLocation().getMeasure();
        measure.removeItem(mark);
        undoSubCommands();
    }

    
    void
    AddMarkCommand::redo()
    {
        KOMP_LOG_COMMAND
        auto& noteItem = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(noteItem.is<Temporal>());
        auto& temporal = noteItem.as<Temporal>();
        auto& markItem = mRegistry.getItem(mMarkID);
        KOMP_ASSERT(markItem.getItemTraits().getItemType() == ItemType::Mark);
        auto& mark = markItem.as<Mark>();
        auto& attachments = temporal.getAttachments();
        attachments.addMark(mRegistry, temporal, mark);
        mark.setIsQuiescent(false);
        auto& measure = temporal.getLocation().getMeasure();
        measure.insertItem(mark, mark.getTime());
        redoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
