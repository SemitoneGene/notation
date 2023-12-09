// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "StemType.h"

namespace komp
{
    /// creates a chord object comprised of the inNotes.  for least surprise, the
    /// inNotes should be of the same track, hand, measure, voice, duration/type
    ///
    class InsertChordCommand : public Command
    {
    public:
        InsertChordCommand(CommandFactory& inFactory,
                           ItemRegistry& inRegistry,
                           Measure& inMeasure,
                           const NoteRefVec& inNotes);
        
        InsertChordCommand(CommandFactory& inFactory,
                           ItemRegistry& inRegistry,
                           Measure& inMeasure,
                           const NoteRefVec& inNotes,
                           StemType inStemDirection);
        
        virtual ~InsertChordCommand();

        virtual void undo() override;
        virtual void redo() override;

    private:
        std::vector<ItemID> mNoteIDs;
        ItemID mChordID{std::numeric_limits<ItemID>::max()};
        Measure& mMeasure;
        CommandIntfUPtr mDeleteCommand{nullptr};
        int mHand{0};
        StemType mStemType;
        StemMode mStemMode{StemMode::AUTOMATIC};
        virtual CommandResult executeImpl() override;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
