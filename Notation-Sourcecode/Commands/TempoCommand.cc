// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TempoCommand.h"

// Local
#include "Temporal.h"
#include "Score.h"
#include "Track.h"
#include "TempoMark.h"

namespace komp
{
    TempoCommand::~TempoCommand() = default;

    TempoCommand::TempoCommand(CommandFactory& inFactory,
                               ItemRegistry& inRegistry,
                               Measure& inMeasure,
                               Track& inTrack,
                               double inTime,
                               TempoValue inTempoValue)
    : Command("Change Tempo", inFactory, inRegistry)
    , mMeasure(inMeasure)
    , mTrack(inTrack)
    , mTime(inTime)
    , mTempoValue(std::move(inTempoValue))
    , mInsertedMarkID(initializeItemID())
    , mIsMarkInserted(false)
    {

    }


    CommandResult
    TempoCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        if (!handleExistingMarks()) {
            insertMark();
        }

        return true;
    }


    void
    TempoCommand::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
        if (mIsMarkInserted) {
            auto& item = mRegistry.getItem(mInsertedMarkID);
            item.removeSelf();
        }
    }


    void
    TempoCommand::redo()
    {
        KOMP_LOG_COMMAND
        if (mIsMarkInserted) {
            auto& item = mRegistry.getItem(mInsertedMarkID);
            mMeasure.insertItem(item, mTime);
        }
        redoSubCommands();
    }


    bool
    TempoCommand::handleExistingMarks()
    {
        const auto allMarks = mMeasure.items<TempoMark>();
        auto isExistingMarkFound = false;
        for (TempoMark& mark : allMarks) {
            if (mark.getTime() == mTime) {
                isExistingMarkFound = true;
                auto memento = mark.getMemento();
                mark.setTempoValue(mTempoValue);
                auto stateCmd = mFactory.itemStateCommand(std::move(memento),
                                                          mark.getMemento());
                addSubCommand(std::move(stateCmd));
            }
        }

        return isExistingMarkFound;
    }


    void
    TempoCommand::insertMark()
    {
        const MusicLocation loc(mMeasure, mTrack);
        auto& mark = mRegistry.createItem<TempoMark>(loc, mTempoValue);
        mark.setTime(mTime);
        mInsertedMarkID = mark.getItemID();
        mIsMarkInserted = true;
        mMeasure.insertItem(mark);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
