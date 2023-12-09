// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// deletes a chord but leaves the unchorded notes in the measure
    ///
    class DeleteChordCommand : public Command
    {
    public:
        DeleteChordCommand(CommandFactory& inFactory,
                           ItemRegistry& inRegistry,
                           const Chord& inChord,
                           bool inDoDeleteNotes);
        
        virtual ~DeleteChordCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mItemID;
        Measure& mMeasure;
        bool mDoDeleteNotes;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
