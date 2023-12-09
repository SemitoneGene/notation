// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "AccidentalType.h"
#include "Command.h"
#include "MusicLocation.h"

namespace komp
{
    /// \brief  Inserts a Note or Rest and updates the
    /// times of subsequent temporals
    ///
    class InsertAccidentalCommand : public Command
    {
    public:
        virtual ~InsertAccidentalCommand();
        InsertAccidentalCommand(CommandFactory& inFactory,
                                ItemRegistry& inRegistry,
                                const MusicLocation& inLocation,
                                const Point& inScorePoint,
                                AccidentalType inAccidentalType);

        virtual void undo() override;
        virtual void redo() override;

        virtual bool containsItem(const Item& inItem) const override;

    private:
        MusicLocation mMusicLocation;
        Point mScorePoint;
        AccidentalType mAccidentalType;
        ItemID mAccidentalID;
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
