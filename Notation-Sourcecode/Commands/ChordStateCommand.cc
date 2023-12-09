// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordStateCommand.h"

// Local
#include "Chord.h"
#include "Score.h"
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Throw.h"

namespace komp
{    
    ChordStateCommand::ChordStateCommand(CommandFactory& inFactory,
                                         ItemRegistry& inRegistry,
                                         ItemUPtrVec&& inOriginalNoteMementos,
                                         ItemUPtr inOriginalState,
                                         ItemUPtr inAlteredState)
    : komp::Command("Chord State Change", inFactory, inRegistry)
    , mItemID(inOriginalState->getItemID())
    , mOriginalState(std::move(inOriginalState))
    , mAlteredState(std::move(inAlteredState))
    , mOriginalNoteMementos(std::move(inOriginalNoteMementos))
    , mAlteredNoteMementos()
    {
        const auto& original = dynamic_cast<Chord*>(mOriginalState.get());
        KOMP_ASSERT(original);
        mOriginalDuration = original->getDuration();
        mOriginalStemPolicy = original->getStemPolicy();
        mOriginalX = original->getX();
        
        const auto& altered = dynamic_cast<Chord*>(mAlteredState.get());
        KOMP_ASSERT(altered);
        mAlteredDuration = altered->getDuration();
        mAlteredStemPolicy = altered->getStemPolicy();
        mAlteredX = altered->getX();

        for (const auto& note : mOriginalNoteMementos) {
            mNoteIDs.push_back(note->getItemID());
        }

        for (const Note& note : altered->getNotes()) {
            mAlteredNoteMementos.emplace_back(note.getMemento());
        }
        
        KOMP_ASSERT(mOriginalState->getItemID() == mAlteredState->getItemID());
    }


    CommandResult
    ChordStateCommand::executeImpl()
    {
        KOMP_LOG_COMMAND;
        return false;
    }
    
    void
    ChordStateCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& registryItem = mRegistry.getItem(mItemID);
        registryItem.restoreMemento(*mOriginalState);
        const auto& mementos = mOriginalNoteMementos;
        for (const auto noteID : mNoteIDs) {
            auto iter = std::find_if(mementos.cbegin(),
                                     mementos.cend(),
                                     [&](const ItemUPtr& inNote) {
                return inNote->getItemID() == noteID;
            });
            
            if (iter == mementos.cend()) {
                continue;
            }

            auto& memento = *iter;
            auto& registryNote = mRegistry.getItem(noteID);
            KOMP_ASSERT(registryNote.getItemTraits().getItemType() == ItemType::Note);
            auto& note = registryNote.as<Note>();
            note.restoreMemento(*memento);
            ItemImpl::setNeedsDisplay(note);
        }
        KOMP_ASSERT(registryItem.getItemTraits().getItemType() == ItemType::Chord);
        auto& chord = registryItem.as<Chord>();
        const auto wasQuiescent = chord.getIsQuiescent();
        chord.setIsQuiescent(true);
        chord.setDuration(mOriginalDuration);
        chord.setX(mOriginalX);
        chord.setStemPolicy(mOriginalStemPolicy);
        undoSubCommands();
        chord.setIsQuiescent(wasQuiescent);
        ItemImpl::setNeedsDisplay(chord);
    }
    
    void
    ChordStateCommand::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
        auto& registryItem = mRegistry.getItem(mItemID);
        registryItem.restoreMemento(*mAlteredState);
        const auto& mementos = mAlteredNoteMementos;
        for (const auto& ID : mNoteIDs) {
            auto iter = std::find_if(mementos.cbegin(),
                                     mementos.cend(),
                                     [&](const ItemUPtr& inNote) {
                return inNote->getItemID() == ID;
            });
            if (iter == mementos.cend()) {
                continue;
            }

            auto& memento = *iter;
            auto& registryNote = mRegistry.getItem(ID);
            KOMP_ASSERT(registryNote.getItemTraits().getItemType() == ItemType::Note);
            auto& note = registryNote.as<Note>();
            note.restoreMemento(*memento);
            ItemImpl::setNeedsDisplay(note);
        }
        KOMP_ASSERT(registryItem.getItemTraits().getItemType() == ItemType::Chord);
        auto& chord = registryItem.as<Chord>();
        chord.setDuration(mAlteredDuration);
        chord.setX(mAlteredX);
        chord.setStemPolicy(mAlteredStemPolicy);
        ItemImpl::setNeedsDisplay(chord);
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
