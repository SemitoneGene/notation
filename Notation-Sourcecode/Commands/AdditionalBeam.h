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

    /// \brief  adds a secondary beam to some of the notes
    ///
    /// \discussion looks for the note which has the smallest number of beams.
    /// bumps up that notes number of beams (i.e. halves its duration), and
    /// any other notes that have the same number of beams.  This is used
    /// when a beam already exists, but the temporal types affected by the
    /// beam stroke are not all the same.  Relies on the BeamStrokeData
    /// input to identify whether or not the conditions are correct. Throws
    /// if the preconditions are not met (upon construction).
    ///
    class AdditionalBeam : public Command
    {
    public:
        AdditionalBeam(CommandFactory& inFactory,
                       ItemRegistry& inRegistry,
                       const BeamStrokeParams& inData);

        virtual ~AdditionalBeam();

        virtual void undo() override;
        virtual void redo() override;

    private:
        BeamStrokeParams mData;
        virtual CommandResult executeImpl() override;

    private:
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
