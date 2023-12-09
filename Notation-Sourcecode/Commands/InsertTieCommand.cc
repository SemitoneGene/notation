// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertTieCommand.h"

// Local
#include "CommandFactory.h"
#include "CurveParams.h"
#include "ItemImpl.h"
#include "Score.h"
#include "Tie.h"
#include "Temporal.h"

namespace komp
{
    InsertTieCommand::InsertTieCommand(CommandFactory& inFactory,
                                       ItemRegistry& inRegistry,
                                       ItemID inFirst,
                                       ItemID inSecond)
    : komp::Command("Insert Tie", inFactory, inRegistry)
    , mTieID(0)
    , mFirstTemporalID(inFirst)
    , mSecondTemporalID(inSecond)
    {        
    }
    
    
    CommandResult
    InsertTieCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& first = mRegistry.getItem(mFirstTemporalID).as<Temporal>();
        auto& second = mRegistry.getItem(mSecondTemporalID).as<Temporal>();
        const CurveParams params{first, second};
        
        auto& item = mRegistry.createItem<Tie>(first.getLocation(), params);
        mTieID = item.getItemID();
        auto& m = first.getLocation().getMeasure();
        item.setIsQuiescent(false);
        m.insertItem(item, item.getTime());
        komp::ItemImpl::setNeedsDisplay(item);
        return true;
    }
    
    
    void
    InsertTieCommand::undo()
    {
        KOMP_LOG_COMMAND
        if (getSubCommandCount() == 0) {
            auto deleteCommand = mFactory.deleteItemCommand(mTieID);
            deleteCommand->execute();
            addSubCommand(std::move(deleteCommand));
        } else {
            redoSubCommands();
        }
    }
    
    
    void
    InsertTieCommand::redo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
