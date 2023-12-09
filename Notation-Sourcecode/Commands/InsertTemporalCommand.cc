// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertTemporalCommand.h"

// Local
#include "Temporal.h"
#include "Score.h"

namespace komp
{
InsertTemporalCommand::~InsertTemporalCommand() = default;

InsertTemporalCommand::InsertTemporalCommand(CommandFactory& inFactory,
                                             ItemRegistry& inRegistry,
                                             ItemID inItemID,
                                             Measure& inMeasure,
                                             bool inDoOffsetItemTimes)
: Command("Insert Temporal", inFactory, inRegistry)
, mItemID{inItemID}
, mMeasure(inMeasure)
, mDoOffsetItemTimes(inDoOffsetItemTimes)
{
}

/// \brief Inserts the temporal
CommandResult
InsertTemporalCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    auto& temporal = mRegistry.getItem(mItemID).as<Temporal>();
    temporal.setIsQuiescent(false);
    mMeasure.insertItem(temporal, temporal.getTime());
    
    if (mDoOffsetItemTimes) {
        mMeasure.offsetItemTimes(temporal,
                                 temporal.getDuration().getClocks());
    }
    mMeasure.sort();
    
    std::vector<ItemID> temporals;
    temporals.push_back(temporal.getItemID());
    const CommandResult result{std::move(temporals)};
    return result;
}

/// \brief Removes the temporal
void
InsertTemporalCommand::undo()
{
    KOMP_LOG_COMMAND
    auto& temporal = mRegistry.getItem(mItemID).as<Temporal>();
    mMeasure.removeItem(temporal);
    
    if (mDoOffsetItemTimes) {
        mMeasure.offsetItemTimes(temporal,
                                 -1 * temporal.getDuration().getClocks());
    }
    
    mMeasure.sort();
}

/// \brief Re-inserts the temporal
void
InsertTemporalCommand::redo()
{
    KOMP_LOG_COMMAND
    execute();
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
