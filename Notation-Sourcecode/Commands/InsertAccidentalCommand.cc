// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertAccidentalCommand.h"

// Local
#include "DeleteItemCommand.h"
#include "ItemImpl.h"
#include "RawAccidental.h"
#include "Score.h"
#include "Temporal.h"


namespace komp
{
    InsertAccidentalCommand::~InsertAccidentalCommand() = default;

    InsertAccidentalCommand::InsertAccidentalCommand(CommandFactory& inFactory,
                                                     ItemRegistry& inRegistry,
                                                     const MusicLocation& inMusicLocation,
                                                     const Point& inPoint,
                                                     AccidentalType inAccidentalType)
    : Command("Insert Accidental", inFactory, inRegistry)
    , mMusicLocation(inMusicLocation)
    , mScorePoint{inPoint}
    , mAccidentalType{inAccidentalType}
    , mAccidentalID{}
    {
    }


    CommandResult
    InsertAccidentalCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& raw = mRegistry.createItem<RawAccidental>(mRegistry,
                                                        mMusicLocation,
                                                        mAccidentalType);
        auto& measure = mMusicLocation.getMeasure();
        const auto measureDataX = measure.getDataX();
        const auto measureRelativeX = mScorePoint.x - measureDataX;
        raw.setX(measureRelativeX);
        const auto& staff = mMusicLocation.getStaff();
        const auto preciseSlot = staff.pixelToSlot(mScorePoint.y);
        const auto roundedSlot = roundToInt(preciseSlot);
        raw.setSlot(roundedSlot);
        raw.setIsQuiescent(false);
        measure.insertItem(raw);
        mAccidentalID = raw.getItemID();
        ItemImpl::setNeedsDisplay(raw);
        return true;
    }

    void
    InsertAccidentalCommand::undo()
    {
        KOMP_LOG_COMMAND

        if (getSubCommandCount() == 0) {
            auto deleteItemCommand = mFactory.deleteItemCommand(mAccidentalID);
            deleteItemCommand->execute();
            addSubCommand(std::move(deleteItemCommand));
        } else {
            redoSubCommands();
        }
    }


    void
    InsertAccidentalCommand::redo()
    {
        KOMP_LOG_COMMAND
        KOMP_ASSERT(getSubCommandCount() == 1);
        undoSubCommands();
    }
    
    bool
    InsertAccidentalCommand::containsItem(const Item& inItem) const {
        return inItem.getItemID() == mAccidentalID;
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
