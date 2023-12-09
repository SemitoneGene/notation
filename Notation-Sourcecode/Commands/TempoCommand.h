// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "TempoMark.h"

namespace komp
{
    /// Changes the key signature (unless the new key signature is equal to the old)
    /// and updates the pitch of any affected notes.  Stops processing if a future
    /// key signature is encountered
    ///
    class TempoCommand : public Command
    {
    public:
        virtual ~TempoCommand();
        TempoCommand(CommandFactory& inFactory,
                     ItemRegistry& inRegistry,
                     Measure& inMeasure,
                     Track& inTrack,
                     double inTime,
                     TempoValue inTempoValue);

        virtual void undo() override;
        virtual void redo() override;

    private:
        Measure& mMeasure;
        Track& mTrack;
        double mTime;
        TempoValue mTempoValue;
        ItemID mInsertedMarkID;
        bool mIsMarkInserted;

    private:
        bool handleExistingMarks();
        void insertMark();
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
