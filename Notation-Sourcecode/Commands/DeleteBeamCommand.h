// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"
#include "Chord.h"

namespace komp
{
    /// \brief  deletes a beam
    ///
    class DeleteBeamCommand : public Command
    {
    public:
        DeleteBeamCommand(CommandFactory& inFactory,
                          ItemRegistry& inRegistry,
                          const BeamIntf& inBeam);
        
        virtual ~DeleteBeamCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        Measure* mMeasure;
        ItemUPtr mBeamMemento;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
