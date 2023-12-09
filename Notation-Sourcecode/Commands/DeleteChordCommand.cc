// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteChordCommand.h"

// Local
#include "Score.h"
#include "Chord.h"
#include "Note.h"
#include "BeamIntf.h"

namespace komp
{
    DeleteChordCommand::~DeleteChordCommand() = default;
    
DeleteChordCommand::DeleteChordCommand(CommandFactory& inFactory,
                                       ItemRegistry& inRegistry,
                                       const Chord& inChord,
                                       bool inDoDeleteNotes)
    : komp::Command("Delete Chord", inFactory, inRegistry)
    , mItemID(inChord.getItemID())
    , mMeasure(inChord.getLocation().getMeasure())
    , mDoDeleteNotes{inDoDeleteNotes}
    {
    }
    
    
    CommandResult
    DeleteChordCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& chord = mRegistry.getItem(mItemID).as<Chord>();
        auto notes = chord.getNotes();
        
        auto chordMemento = chord.getChordMemento();
        auto beam = chord.getBeam();
        if (beam) {
            auto deleteBeamCommand = mFactory.deleteBeamCommand(*beam);
            deleteBeamCommand->execute();
            addSubCommand(std::move(deleteBeamCommand));
        }

        mMeasure.removeItem(chord);

        for (const auto& note : notes) {
            chord.removeNote(note.get());
        }

        auto& alteredState = mRegistry.getItem(chordMemento.chordMemento->getItemID());
        auto chordMementoCmd = mFactory.chordStateCommand(std::move(chordMemento.noteMementos),
                                                          std::move(chordMemento.chordMemento),
                                                          alteredState.getMemento());
        addSubCommand(std::move(chordMementoCmd));

        if (mDoDeleteNotes) {
            for (const auto& note : notes) {
                auto noteDelete = mFactory.deleteTemporalCommand(note.get());
                noteDelete->execute();
                addSubCommand(std::move(noteDelete));
            }
        }

        return true;
    }
    
    
    void
    DeleteChordCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& chord = mRegistry.getItem(mItemID).as<Chord>();
        const bool wasQuiescent = chord.getIsQuiescent();
        chord.setIsQuiescent(true);
        undoSubCommands();
        mMeasure.insertItem(chord, chord.getTime());
        chord.setIsQuiescent(wasQuiescent);
    }

    
    void
    DeleteChordCommand::redo()
    {
        KOMP_LOG_COMMAND
        auto& chord = mRegistry.getItem(mItemID).as<Chord>();
        mMeasure.removeItem(chord);
        redoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
