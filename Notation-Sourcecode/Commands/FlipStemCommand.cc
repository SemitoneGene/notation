// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FlipStemCommand.h"

// Local
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Note.h"
#include "Score.h"
#include "Throw.h"

namespace komp
{
    FlipStemCommand::~FlipStemCommand() = default;
    
    FlipStemCommand::FlipStemCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     const Note& inNote)
    : komp::Command("Flip Stem", inFactory, inRegistry)
    , mTemporalID(inNote.getItemID())
    , mStemUp(inNote.getStemPolicy().getIsStemUp())
    {
    }
    
    CommandResult
    FlipStemCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(item.is<Temporal>());
        auto& note = mRegistry.getItem(mTemporalID).as<Note>();
        if (mStemUp) {
            note.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        } else {
            note.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        }
        ItemImpl::setNeedsDisplay(note);
        return true;
    }
        
    void
    FlipStemCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& noteItem = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(noteItem.getItemTraits().getItemType() == ItemType::Note);
        auto& note = noteItem.as<Note>();
        if (mStemUp) {
            note.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        } else {
            note.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        }
        undoSubCommands();
        ItemImpl::setNeedsDisplay(note);
    }
    
    void
    FlipStemCommand::redo()
    {
        KOMP_LOG_COMMAND
        auto& noteItem = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(noteItem.getItemTraits().getItemType() == ItemType::Note);
        auto& note = noteItem.as<Note>();
        if (mStemUp) {
            note.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        } else {
            note.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        }
        redoSubCommands();
        ItemImpl::setNeedsDisplay(note);
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
