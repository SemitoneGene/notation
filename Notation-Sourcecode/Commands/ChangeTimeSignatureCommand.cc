// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChangeTimeSignatureCommand.h"

// Local
#include "CommandFactory.h"
#include "Score.h"

namespace komp
{
    ChangeTimeSignatureCommand::~ChangeTimeSignatureCommand() = default;
    
    ChangeTimeSignatureCommand::ChangeTimeSignatureCommand(CommandFactory& inFactory,
                                                           ItemRegistry& inRegistry,
                                                           Score& inScore,
                                                           size_t inMeasureIndex,
                                                           const TimeSignatureParams& inParams)
    : komp::Command("Change Time Signature", inFactory, inRegistry)
    , mScore(inScore)
    , mMeasureIndex(inMeasureIndex)
    , mSpan(1)
    , mParams(inParams)
    , mPreviousParams(inParams)
    , mShow(true)
    {
        // Save the current information
        const auto& measure = mScore.getMeasure(mMeasureIndex);
        mPreviousParams = measure->getTimeSignatureParams();
        
        // Check if we need to set the visibility
        if (mMeasureIndex == 0) {
            mShow = true;
        } else {
            const auto& previousMeasure = mScore.getMeasure(mMeasureIndex - 1);
            const auto& previousParams = previousMeasure->getTimeSignatureParams();
            mShow = previousParams.beatsPerBar != mParams.beatsPerBar
                 || previousParams.beatUnit != mParams.beatUnit;
        }
        
        // Determine the measure span to change.  We will keep applying the new
        // values until we hit a visible key signature that differs
        const auto totalMeasures = mScore.getMeasureCount();
        
        for (int index = mMeasureIndex + 1; index < totalMeasures; ++index) {
            
            const auto& nextMeasure = mScore.getMeasure(index);
            const auto& nextParams = nextMeasure->getTimeSignatureParams();
            const bool changed = nextParams.beatsPerBar != mParams.beatsPerBar
                              || nextParams.beatUnit != mParams.beatUnit;
            
            if (nextMeasure->getMeterVisible() && changed) {
                break;
            }
            ++mSpan;
        }
    }
    
    CommandResult
    ChangeTimeSignatureCommand::executeImpl()
    {
        KOMP_LOG_COMMAND

        // Apply the time signature change to the measure span
        const auto lastIndex = mMeasureIndex + mSpan;
        for (auto index = mMeasureIndex; index < lastIndex; ++index) {

            const auto& measure = mScore.getMeasure(index);
            measure->setTimeSignatureType(mParams.type);
            measure->setMeter(mParams.beatsPerBar, mParams.beatUnit);
            
            if (index == mMeasureIndex) {
                measure->setMeterVisible(mShow);
            } else {
                measure->setMeterVisible(false);
            }            
            measure->layout();
        }
        
        mScore.layout();
       
        return true;
    }
    
    void
    ChangeTimeSignatureCommand::undo()
    {
        KOMP_LOG_COMMAND
        
        // Apply the time signature change to the measure span
        const auto lastIndex = mMeasureIndex + mSpan;
        for (int index = mMeasureIndex; index < lastIndex; ++index) {
            
            const auto& measure = mScore.getMeasure(index);
            measure->setTimeSignatureType(mPreviousParams.type);
            measure->setMeter(mPreviousParams.beatsPerBar, mPreviousParams.beatUnit);
            
            if (index == mMeasureIndex) {
                measure->setMeterVisible(!mShow);
            } else {
                measure->setMeterVisible(false);
            }
        }
        redoSubCommands();
    }
    
    void
    ChangeTimeSignatureCommand::redo()
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
