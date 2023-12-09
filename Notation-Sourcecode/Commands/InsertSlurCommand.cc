// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertSlurCommand.h"

// Local
#include "CommandFactory.h"
#include "ItemImpl.h"
#include "Score.h"
#include "Slur.h"
#include "Temporal.h"


namespace komp
{
    InsertSlurCommand::~InsertSlurCommand() = default;
    
InsertSlurCommand::InsertSlurCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     ItemID inStart,
                                     ItemID inLast)
    : komp::Command("Insert Slur", inFactory, inRegistry)
    , mSlurID(0)
    , mFirstTemporalID(inStart)
    , mLastTemporalID(inLast)
    {
        
    }
    
    
    CommandResult
    InsertSlurCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& first = mRegistry.getItem(mFirstTemporalID).as<Temporal>();
        auto& second = mRegistry.getItem(mLastTemporalID).as<Temporal>();
        
        const auto& loc = first.getLocation();
        
        const CurveParams params{
            first,
            second,
            PlacementType::AUTOMATIC
        };
        auto& item = mRegistry.createItem<Slur>(loc, params);
        mSlurID = item.getItemID();
        auto& m = loc.getMeasure();
        item.setIsQuiescent(false);
        m.insertItem(item, item.getTime());
        komp::ItemImpl::setNeedsDisplay(item);
        return true;
    }
    
    
    void
    InsertSlurCommand::undo()
    {
        KOMP_LOG_COMMAND
        if (getSubCommandCount() == 0) {
            auto deleteCommand = mFactory.deleteItemCommand(mSlurID);
            deleteCommand->execute();
            addSubCommand(std::move(deleteCommand));
        } else {
            redoSubCommands();
        }
    }
    
    
    void
    InsertSlurCommand::redo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
