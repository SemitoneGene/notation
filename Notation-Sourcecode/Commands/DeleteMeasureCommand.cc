// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteMeasureCommand.h"

// Local
#include "CommandFactory.h"
#include "Score.h"

namespace komp
{
    DeleteMeasureCommand::~DeleteMeasureCommand() = default;
    
DeleteMeasureCommand::DeleteMeasureCommand(CommandFactory& inFactory,
                                           ItemRegistry& inRegistry,
                                           Score& inScore,
                                           size_t inIndex)
    : komp::Command("Delete Measure", inFactory, inRegistry)
    , mScore(inScore)
    , mIndex(inIndex)
    , mMeasure{nullptr}
    {
    }
    
    CommandResult
    DeleteMeasureCommand::executeImpl()
    {
        KOMP_LOG_COMMAND

        mMeasure = mScore.deleteMeasure(mIndex);
        redoSubCommands();
        
        mScore.layout();
        mScore.draw();
        
        return true;
    }
    
    void
    DeleteMeasureCommand::undo()
    {
        KOMP_LOG_COMMAND
        
        mScore.insertMeasure(std::move(mMeasure), mIndex);
        mMeasure = nullptr;
        
        redoSubCommands();
        
        mScore.layout();
        mScore.draw();
    }
    
    void
    DeleteMeasureCommand::redo()
    {
        KOMP_LOG_COMMAND
        execute();
        undoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
