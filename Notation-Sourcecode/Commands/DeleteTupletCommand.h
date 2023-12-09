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
    /// \brief  Deletes a tuplet.
    ///
    class DeleteTupletCommand : public Command
    {
    public:
        virtual ~DeleteTupletCommand();
        DeleteTupletCommand(CommandFactory& inFactory,
                            ItemRegistry& inRegistry,
                            Measure& inMeasure,
                            const TupletIntf& inTuplet);

        virtual void undo() override;
        virtual void redo() override;

    private:
        Measure& mMeasure;
        ItemID mTupletID;
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
