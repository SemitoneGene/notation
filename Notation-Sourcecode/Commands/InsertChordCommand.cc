// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertChordCommand.h"

// Local
#include "Chord.h"
#include "ItemImpl.h"
#include "Note.h"
#include "Score.h"
#include "Throw.h"


namespace komp
{
    InsertChordCommand::~InsertChordCommand() = default;

    InsertChordCommand::InsertChordCommand(CommandFactory& inFactory,
                                           ItemRegistry& inRegistry,
                                           Measure& inMeasure,
                                           const NoteRefVec& inNotes)
    : InsertChordCommand{inFactory, inRegistry, inMeasure, inNotes, StemType::UP}
    {
    }
    
    
    InsertChordCommand::InsertChordCommand(CommandFactory& inFactory,
                                           ItemRegistry& inRegistry,
                                           Measure& inMeasure,
                                           const NoteRefVec& inNotes,
                                           StemType inStemDirection)
    : komp::Command("Insert Chord", inFactory, inRegistry)
    , mMeasure(inMeasure)
    , mStemType(inStemDirection)
    {
        if (inStemDirection == StemType::UP) {
            mStemMode = StemMode::FORCE_UP;
        } else if (inStemDirection == StemType::DOWN) {
            mStemMode = StemMode::FORCE_DOWN;
        } else if (inStemDirection == StemType::NONE) {
            mStemMode = StemMode::FORCE_NONE;
        } else {
            mStemMode = StemMode::AUTOMATIC;
        }
        
        const Note& first = inNotes.front();
        mHand = first.getLocation().getHand();
        
        for (const Note& note : inNotes) {
            KOMP_ASSERT(mHand == note.getLocation().getHand());
            mNoteIDs.emplace_back(note.getItemID());
        }
    }


    CommandResult
    InsertChordCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        
        if (mNoteIDs.empty()) {
            LOG_F(WARNING, "Note IDs are empty");
            return false;
        }
        
        NoteRefVec notes;
        std::vector<std::pair<ItemUPtr, ItemRef>> mementos;
        Track* track = nullptr;
        
        for (const auto i : mNoteIDs) {
            auto& item = mRegistry.getItem(i);
            KOMP_ASSERT(item.getItemTraits().getItemType() == ItemType::Note);
            auto& note = item.as<Note>();
            notes.push_back(note);
            track = &note.getLocation().getTrack();
            mementos.emplace_back(note.getMemento(), note);

            Chord* existingChord = nullptr;

            if (note.getIsChordMember()) {
                existingChord = note.getChord();
            }

            if (existingChord) {
                auto removeChordCommand = mFactory.deleteChordCommand(*existingChord,
                                                                      false);
                removeChordCommand->execute();
                addSubCommand(std::move(removeChordCommand));
            }
        }
 
        if (!track) {
            LOG_F(WARNING, "Invalid track");
            return false;
        }

        const MusicLocation loc(mMeasure, *track, mHand);
        auto& newChord = mRegistry.createItem<Chord>(loc, notes);
        mChordID = newChord.getItemID();

        if (mStemMode != StemMode::AUTOMATIC) {
            if (mStemType == StemType::UP) {
                newChord.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
            } else if (mStemType == StemType::DOWN) {
                newChord.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
            }
        }
        
        for (const auto& pair : mementos) {
            auto stateCmd = mFactory.itemStateCommand(pair.first->getMemento(),
                                                      pair.second.get().getMemento());
            addSubCommand(std::move(stateCmd));
        }

        newChord.setIsQuiescent(false);
        mMeasure.insertItem(newChord, newChord.getTime());
        return true;
    }


    void
    InsertChordCommand::undo()
    {
        KOMP_LOG_COMMAND
        if (!mDeleteCommand) {
            auto& chord = mRegistry.getItem(mChordID).as<Chord>();
            mDeleteCommand = mFactory.deleteChordCommand(chord, false);
            mDeleteCommand->execute();
        } else {
            mDeleteCommand->redo();
        }
        undoSubCommands();
    }


    void
    InsertChordCommand::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
        if (mDeleteCommand) {
            mDeleteCommand->undo();
        }
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
