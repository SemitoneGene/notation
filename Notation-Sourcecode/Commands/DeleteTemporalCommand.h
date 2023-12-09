// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  Deletes a Note or Rest
    ///
    class DeleteTemporalCommand : public Command
    {
    public:
        virtual ~DeleteTemporalCommand();
        DeleteTemporalCommand(CommandFactory& inFactory,
                              ItemRegistry& inRegistry,
                              const Temporal& inTemporal);

        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        ItemUPtr mArchivedTemporal;
        Measure& mMeasure;
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
