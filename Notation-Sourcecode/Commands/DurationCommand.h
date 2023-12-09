// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "Duration.h"

namespace komp
{
    /// Changes the duration of a temporal, attempts to handle
    /// beams and chords correctly
    ///
    class DurationCommand : public Command
    {
    public:
        DurationCommand(CommandFactory& inFactory,
                        ItemRegistry& inRegistry,
                        Measure& inMeasure,
                        ItemID inItemID,
                        Duration inDuration);
        
        virtual ~DurationCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        void saveBeamState(BeamIntf& beam);
        void deleteBeam(BeamIntf& beam);
        void remakeBeam();
        
    private:
        ItemID mItemID;
        Duration mNewDuration;
        Measure& mMeasure;
        TemporalRefVec mBeamNotes;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
