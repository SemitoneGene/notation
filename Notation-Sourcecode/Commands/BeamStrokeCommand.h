// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"
#include "BeamStrokeParams.h"

namespace komp
{

    /// \brief  adds a beam to a span of notes
    ///
    /// \discussion does a lot of different stuff based on input contitions
    ///
    class BeamStrokeCommand : public Command
    {
    public:
        BeamStrokeCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          const BeamStrokeParams& inData);
        
        virtual ~BeamStrokeCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        BeamStrokeParams mData;
        
    private:
        void handleAdditionalBeam();
        void handleConnectingBeam();
        void handleHalvingBeam();
        void handleFractionalBeam();
        void getRebeamables(TemporalRefVec& outRebeamables, BeamIntfRefVec& outBeams);
        void fixTimes();
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
