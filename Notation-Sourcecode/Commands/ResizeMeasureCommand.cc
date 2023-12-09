// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ResizeMeasureCommand.h"

// Local
#include "CommandFactory.h"
#include "Score.h"

namespace komp
{
    ResizeMeasureCommand::~ResizeMeasureCommand() = default;
    
    ResizeMeasureCommand::ResizeMeasureCommand(CommandFactory& inFactory,
                                               ItemRegistry& inRegistry,
                                               const Score& inScore,
                                               size_t inIndex,
                                               double inOriginalWidth,
                                               double inNewWidth)
    : komp::Command("Insert Measure", inFactory, inRegistry)
    , mScore(inScore)
    , mIndex(inIndex)
    , mOriginalWidth(inOriginalWidth)
    , mNewWidth(inNewWidth)
    , mOffset(mOriginalWidth - mNewWidth)
    {
    }
    
    CommandResult
    ResizeMeasureCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        setMeasureWidth(mNewWidth, -mOffset);
        return true;
    }
    
    void
    ResizeMeasureCommand::undo()
    {
        KOMP_LOG_COMMAND
        setMeasureWidth(mOriginalWidth, mOffset);
    }
    
    void
    ResizeMeasureCommand::redo()
    {
        KOMP_LOG_COMMAND
        execute();
        undoSubCommands();
    }
    
    void
    ResizeMeasureCommand::setMeasureWidth(double inWidth, double inOffset)
    {
        // Set the new width of measure
        const auto& measure = mScore.getMeasure(mIndex);
        const auto gap = measure->getGap();
        const auto noteSpace = inWidth - gap;
        measure->updateNoteSpace(noteSpace);
        
        // Adjust the position measures to the right of the measure
        auto iter = measure->iter() + 1;
        const auto iterEnd = mScore.getMeasuresEnd();
        for ( ; iter != iterEnd; ++iter) {
            const auto& moveMeasure = *iter;
            const auto x = moveMeasure->getX();
            const auto newX = x + inOffset;
            moveMeasure->setX(newX);
        }
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
