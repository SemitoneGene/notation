// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertClefCommand.h"

// Local
#include "Clef.h"
#include "Score.h"

namespace komp
{
InsertClefCommand::~InsertClefCommand() = default;

InsertClefCommand::InsertClefCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     Measure& inMeasure,
                                     ItemID inItemID)
: Command("Insert Clef", inFactory, inRegistry)
, mItemID{inItemID}
, mMeasure(inMeasure)
{
}

/// \brief Inserts the clef
CommandResult
InsertClefCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    auto& clef = mRegistry.getItem(mItemID).as<Clef>();
    clef.setIsQuiescent(false);
    mMeasure.insertItem(clef, clef.getTime());
    mMeasure.sort();
    return true;
}

/// \brief Removes the clef
void
InsertClefCommand::undo()
{
    KOMP_LOG_COMMAND
    auto& clef = mRegistry.getItem(mItemID).as<Clef>();
    mMeasure.removeItem(clef);
    
    const auto measureIndex = mMeasure.getIndex();
    if (measureIndex > 0) {
        const auto& score = mMeasure.getScore();
        const auto& previous = score.getMeasure(measureIndex - 1);
        previous->setCourtesyGap(0);
    }
    
    mMeasure.sort();
}

/// \brief Re-inserts the clef
void
InsertClefCommand::redo()
{
    KOMP_LOG_COMMAND
    execute();
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
