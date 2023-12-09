// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertMeasureCommand.h"

// Local
#include "CommandFactory.h"
#include "Score.h"

namespace komp
{
InsertMeasureCommand::~InsertMeasureCommand() = default;

InsertMeasureCommand::InsertMeasureCommand(CommandFactory& inFactory,
                                           ItemRegistry& inRegistry,
                                           Score& inScore,
                                           size_t inIndex)
: komp::Command("Insert Measure", inFactory, inRegistry)
, mScore(inScore)
, mIndex(inIndex)
{
}

CommandResult
InsertMeasureCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    
    if (mMeasure) {
        mScore.insertMeasure(std::move(mMeasure), mIndex);
        mMeasure = nullptr;
    } else {
        
        // Insert the measure for the first time
        const auto& measure = mScore.insertMeasure(mIndex);
        
        const auto index = measure->getIndex();
        if (index == 0) {
            
            // Always show the meter in the first measure
            measure->setMeterVisible(true);
            
            // Use the time signature from the following measure
            if (mScore.getMeasureCount() > index) {
                
                const auto& nextMeasure = mScore.getMeasure(index);
                measure->setMeter(nextMeasure->getBeats(),
                                  nextMeasure->getBeatUnit());
                nextMeasure->setMeterVisible(false);
                measure->setMeterVisible(true);
            }
            
        } else {
            
            // Use the time signature from the previous measure
            const auto& previousMeasure = mScore.getMeasure(index - 1);
            measure->setMeter(previousMeasure->getBeats(),
                              previousMeasure->getBeatUnit());
        }
    }
    
    mScore.layout();
    mScore.draw();
    
    return true;
}

void
InsertMeasureCommand::undo()
{
    KOMP_LOG_COMMAND
    mScore.deleteMeasure(mIndex);
    redoSubCommands();
    mScore.layout();
    mScore.draw();
}

void
InsertMeasureCommand::redo()
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
