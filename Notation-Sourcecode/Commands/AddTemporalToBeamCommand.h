// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"
#include "StemDirection.h"

namespace komp
{
    class AddTemporalToBeamCommand : public Command
    {
    public:
        AddTemporalToBeamCommand(CommandFactory& inFactory,
                                 ItemRegistry& inRegistry,
                                 Measure& inMeasure,
                                 ItemID inBeamID,
                                 ItemID inTemporalID);

        virtual ~AddTemporalToBeamCommand();
        virtual void undo() override;
        virtual void redo() override;

    private:
        ItemID mBeamID;
        ItemID mTemporalID;
        Measure& mMeasure;
        virtual CommandResult executeImpl() override;

    private:

    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
