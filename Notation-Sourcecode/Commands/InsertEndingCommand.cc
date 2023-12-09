// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertEndingCommand.h"

// Local
#include "Ending.h"
#include "Measure.h"
#include "Score.h"

namespace komp
{
InsertEndingCommand::InsertEndingCommand(CommandFactory& inFactory,
                                         ItemRegistry& inRegistry,
                                         Measure& inMeasure,
                                         const EndingParams& inParams)
    : Command("Insert Ending", inFactory, inRegistry)
    , mMeasure(inMeasure)
    , mParams(inParams)
    , mItemID(initializeItemID())
    {
    }
    
    InsertEndingCommand::~InsertEndingCommand() = default;
    
    CommandResult
    InsertEndingCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        
        // The measure passed in indicates the end of ending one.
        EndingElement begin;
        begin.measure = &mMeasure;
        begin.type = EndingType::BEGIN;
        begin.status = EndingStatus::DEFAULT;
        begin.ending = 1;
        mParams.elements.emplace_back(begin);
        
        // Find the next measure
        const auto nextIndex = mMeasure.getIndex() + 1;
        const auto& score = mMeasure.getScore();
        if (nextIndex < score.getMeasureCount()) {
            
            // The next measure is the beginning of ending two
            EndingElement next;
            next.measure = score.getMeasure(nextIndex).get();
            next.type = EndingType::END;
            next.status = EndingStatus::DEFAULT;
            next.ending = 2;
            mParams.elements.emplace_back(next);
            
        } else {
            LOG_F(WARNING, "Incomplete ending");
        }
        
        const MusicLocation loc(mMeasure, *mParams.track);
        auto& ending = mRegistry.createItem<Ending>(loc, mParams);
        mMeasure.insertItem(ending);
        mItemID = ending.getItemID();
        
        return true;
    }
    
    void
    InsertEndingCommand::undo()
    {
        KOMP_LOG_COMMAND
        if (getSubCommandCount() == 0) {
            auto deleteCommand = mFactory.deleteItemCommand(mItemID);
            deleteCommand->execute();
            addSubCommand(std::move(deleteCommand));
        } else {
            redoSubCommands();
        }
    }
    
    void
    InsertEndingCommand::redo()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mItemID);
        mMeasure.insertItem(item);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
