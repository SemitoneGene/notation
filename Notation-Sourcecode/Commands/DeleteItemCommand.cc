// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteItemCommand.h"

// Local
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Measure.h"
#include "Score.h"

namespace komp
{
DeleteItemCommand::~DeleteItemCommand() = default;

DeleteItemCommand::DeleteItemCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     Measure& inMeasure,
                                     ItemID inItemID)
: komp::Command("Delete Item", inFactory, inRegistry)
, mItemID{inItemID}
, mMeasure{inMeasure}
{
}


CommandResult
DeleteItemCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    auto& item = mRegistry.getItem(mItemID);
    
    auto& selection = getScore().getSelection();
    if (selection->contains(item)) {
        selection->removeItem(item);
    }
    
    mMemento = item.getMemento();
    mMeasure.removeItem(item);
    return true;
}


void
DeleteItemCommand::undo()
{
    KOMP_LOG_COMMAND
    KOMP_ASSERT(mMemento);
    auto& item = mRegistry.getItem(mItemID);
    item.restoreMemento(*mMemento);
    mMeasure.insertItem(item, item.getTime());
    mMemento = nullptr;
    undoSubCommands();
    ItemImpl::setNeedsDisplay(item);
}


void
DeleteItemCommand::redo()
{
    KOMP_LOG_COMMAND
    KOMP_ASSERT(mMemento == nullptr);
    execute();
    redoSubCommands();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
