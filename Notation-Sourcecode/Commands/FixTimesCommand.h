// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"

namespace komp
{
    /// takes the first note (by x-position) in each track, hand, voice and
    /// sets it to zero, then proceeds rightward by x-position and cascade
    /// each temporal's time based on the duration of the temporal(s) that
    /// came before it.
    ///
    class FixTimesCommand : public Command
    {
    public:
        FixTimesCommand(CommandFactory& inFactory,
                        ItemRegistry& inRegistry,
                        Measure& inMeasure);

        virtual ~FixTimesCommand();
        
        virtual void undo() override;
        virtual void redo() override;

    private:
        void fixMeasure();
        void fixTrack(Track& inTrack);
        void fixHand(Track& inTrack, int inHand);
        void fixVoice(Track& inTrack, int inHand, int inVoice);
        void fixTemporals(const TemporalRefVec& inTemporals);

    private:
        Measure& mMeasure;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
