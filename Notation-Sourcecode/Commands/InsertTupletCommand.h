// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "TupletIntf.h"

namespace komp
{
    /// \brief  Creates a tuplet and inserts it into the score
    ///
    class InsertTupletCommand : public Command
    {
    public:
        virtual ~InsertTupletCommand();
        InsertTupletCommand(CommandFactory& inFactory,
                            ItemRegistry& inRegistry,
                            Measure& inMeasure,
                            const TupletParams& inParams);

        virtual void undo() override;
        virtual void redo() override;

    private:
        Measure& mMeasure;
        TemporalRefVec mTemporals;
        TupletRatio mTupletRatio;
        ItemID mTupletID;
        CommandIntfUPtr mDeleteTupletCommand;
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
