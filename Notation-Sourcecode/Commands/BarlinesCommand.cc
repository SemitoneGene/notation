// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BarlinesCommand.h"

// Local
#include "Measure.h"
#include "Score.h"
#include "Temporal.h"

namespace komp
{
    BarlinesCommand::BarlinesCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     Measure& inMeasure,
                                     BarlineType inLeftBarline,
                                     BarlineType inRightBarline)
    : Command("Change Barline", inFactory, inRegistry)
    , mMeasure(inMeasure)
    , mUpdatedLeftBarline(inLeftBarline)
    , mUpdatedRightBarline(inRightBarline)
    {
        mOriginalLeftBarline = mMeasure.getLeftBarline();
        mOriginalRightBarline = mMeasure.getRightBarline();
    }
    
    BarlinesCommand::~BarlinesCommand() = default;
    
    
    CommandResult
    BarlinesCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        
        mMeasure.setLeftBarline(mUpdatedLeftBarline);
        mMeasure.setRightBarline(mUpdatedRightBarline);
        mMeasure.updateSpacing();
        
        auto& score = mMeasure.getScore();
        score.draw();

        return true;
    }
    
    
    void
    BarlinesCommand::undo()
    {
        KOMP_LOG_COMMAND

        mMeasure.setLeftBarline(mOriginalLeftBarline);
        mMeasure.setRightBarline(mOriginalRightBarline);
        undoSubCommands();
    }
        
    void
    BarlinesCommand::redo()
    {
        KOMP_LOG_COMMAND
        execute();
        redoSubCommands();
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
