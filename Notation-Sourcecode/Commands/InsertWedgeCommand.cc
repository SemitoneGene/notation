// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertWedgeCommand.h"

// Local
#include "Score.h"
#include "Wedge.h"
#include "CommandFactory.h"

namespace komp
{
    InsertWedgeCommand::~InsertWedgeCommand() = default;
    
    InsertWedgeCommand::InsertWedgeCommand(CommandFactory& inFactory,
                                           ItemRegistry& inRegistry,
                                           const SpannerParams& inParams,
                                           WedgeType inWedgeType,
                                           PlacementType inPlacementType)
    : komp::Command("Create Wedge", inFactory, inRegistry)
    , mWedgeID(0)
    , mSpannerParams(inParams)
    , mWedgeType(inWedgeType)
    , mPlacementType(inPlacementType)
    {
        
    }
    
    
    CommandResult
    InsertWedgeCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        
        const MusicLocation loc(*mSpannerParams.startMeasure,
                                *mSpannerParams.track);
        auto& wedge = mRegistry.createItem<Wedge>(loc,
                                                  WedgeParams{
            mSpannerParams,
            mWedgeType,
            mPlacementType
        });
        mWedgeID = wedge.getItemID();
        wedge.setIsQuiescent(false);
        mSpannerParams.startMeasure->insertItem(wedge, wedge.getTime());
        
        mSpannerParams = SpannerParams{};
        std::vector<ItemID> wedgeItemIDs;
        wedgeItemIDs.push_back(mWedgeID);
        CommandResult result{std::move(wedgeItemIDs)};
        return result;
    }
    
    
    void
    InsertWedgeCommand::undo()
    {
        KOMP_LOG_COMMAND
        if (getSubCommandCount() == 0) {
            auto deleteCommand = mFactory.deleteItemCommand(mWedgeID);
            deleteCommand->execute();
            addSubCommand(std::move(deleteCommand));
        } else {
            redoSubCommands();
        }
    }
    
    
    void
    InsertWedgeCommand::redo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
