// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "ChordSymbolParams.h"

namespace komp
{
    /// \brief  Inserts a Clef and updates the times of subsequent temporals
    class InsertChordSymbolCommand : public Command
    {
    public:
        virtual ~InsertChordSymbolCommand();
        InsertChordSymbolCommand(CommandFactory& inFactory,
                                 ItemRegistry& inRegistry,
                                 Measure& inMeasure,
                                 const ChordSymbolParams& inParams,
                                 bool inDoQuantize = true);

        virtual void undo() override;
        virtual void redo() override;

    private:
        virtual CommandResult executeImpl() override;
        ChordSymbolParams mParams;
        ItemID mItemID;
        Measure& mMeasure;
        bool mDoQuantize;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
