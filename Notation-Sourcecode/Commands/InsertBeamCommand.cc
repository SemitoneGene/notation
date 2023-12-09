// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertBeamCommand.h"

// Local
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Note.h"
#include "BeamFactory.h"
#include "BeamFactory.h"
#include "Rest.h"
#include "Score.h"
#include "TemporalValue.h"

namespace komp
{
InsertBeamCommand::InsertBeamCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     const std::vector<ItemID>& inItems)
: komp::Command{"Insert Beam Command", inFactory, inRegistry}
, mBeamID{initializeItemID()}
{
    for (auto& itemID : inItems) {
        auto& item = inRegistry.getItem(itemID);
        KOMP_ASSERT(item.is<Temporal>());
        const auto& temporal = item.as<Temporal>();
        if (temporal.getIsChordMember()) {
            mItemIDs.emplace(itemID);
        } else if (!temporal.getIsRest()) {
            mItemIDs.emplace(itemID);
        }
    }
}


InsertBeamCommand::InsertBeamCommand(CommandFactory& inFactory,
                                     ItemRegistry& inRegistry,
                                     const std::vector<ItemID>& inItems,
                                     StemDirection inStemDirection)
: InsertBeamCommand{inFactory, inRegistry, inItems}
{
    switch (inStemDirection)
    {
    case StemDirection::UP:
    case StemDirection::DOWN:
        {
            mStemDirection = inStemDirection;
            mIsStemDirectionGiven = true;
            break;
        }
        
    default:
        break;
    }
}


CommandResult
InsertBeamCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    std::set<Temporal*> beamables;
    std::set<Note*> notes;
    std::set<Rest*> rests;
    std::set<Chord*> chords;
    std::set<BeamIntf*> beams;
    
    populateSets(beamables,
                 notes,
                 rests,
                 chords,
                 beams);
    
    if (beamables.size() < 2) {
        return false;
    }
    
    validate(beamables);
    
    createMementos(beamables,
                   notes,
                   rests,
                   chords,
                   beams);
    
    deleteBeams(beams);
    correctTemporalTypes(beamables);
    insertBeam(beamables);
    mIsExecuteCalled = true;
    return true;
}


void
InsertBeamCommand::undo()
{
    KOMP_LOG_COMMAND
    if (!mIsExecuteCalled) {
        return;
    }
    
    if (!mIsUndoCalled || !mDeleteBeamCommand) {
        auto& item = mRegistry.getItem(mBeamID);
        KOMP_ASSERT(item.is<BeamIntf>());
        auto& beam = item.as<BeamIntf>();
        mDeleteBeamCommand = mFactory.deleteBeamCommand(beam);
        mDeleteBeamCommand->execute();
    } else {
        mDeleteBeamCommand->redo();
    }
    
    undoSubCommands();
    mIsUndoCalled = true;
}


void
InsertBeamCommand::redo()
{
    KOMP_LOG_COMMAND
    KOMP_ASSERT(mIsUndoCalled);
    KOMP_ASSERT(mDeleteBeamCommand);
    redoSubCommands();
    mDeleteBeamCommand->undo();
}


void
InsertBeamCommand::populateSets(std::set<Temporal*>& beamables,
                                std::set<Note*>& notes,
                                std::set<Rest*>& rests,
                                std::set<Chord*>& chords,
                                std::set<BeamIntf*>& beams) const
{
    for (const auto& itemID : mItemIDs) {
        
        auto& item = mRegistry.getItem(itemID);
        KOMP_ASSERT(item.is<Temporal>());
        auto& temporal = item.as<Temporal>();
        
        if (temporal.getIsChord()) {
            
            KOMP_ASSERT(temporal.getItemTraits().getItemType() == ItemType::Chord);
            auto& chord = temporal.as<Chord>();
            chords.emplace(&temporal.as<Chord>());
            processBeam(chord.getBeam(),
                        beamables,
                        notes,
                        rests,
                        chords,
                        beams);
            for (Note& note : chord.getNotes()) {
                notes.emplace(&note);
            }
            beamables.emplace(&chord);
            
        } else if (temporal.getIsRest()) {
            
            KOMP_ASSERT(temporal.getItemTraits().getItemType() == ItemType::Rest);
            auto& rest = temporal.as<Rest>();
            rests.emplace(&rest);
            processTemporal(temporal,
                            beamables,
                            notes,
                            rests,
                            chords,
                            beams);
            
        } else if (temporal.getIsNote()) {
            
            KOMP_ASSERT(temporal.getItemTraits().getItemType() == ItemType::Note);
            auto& note = temporal.as<Note>();
            notes.emplace(&note);
            processTemporal(temporal,
                            beamables,
                            notes,
                            rests,
                            chords,
                            beams);
        }
    }
}


void
InsertBeamCommand::validate(const std::set<Temporal*>& beamables) const
{
    KOMP_ASSERT(beamables.size() >= 2);
    const auto& measure = (*beamables.cbegin())->getLocation().getMeasure();
    for (const auto& temporal : beamables) {
        KOMP_ASSERT(temporal->getLocation().getMeasure().getIndex() == measure.getIndex());
    }
}


void
InsertBeamCommand::createMementos(const std::set<Temporal*>& beamables,
                                  const std::set<Note*>& notes,
                                  const std::set<Rest*>& rests,
                                  const std::set<Chord*>& chords,
                                  const std::set<BeamIntf*>& beams)
{
    using MementoPair = std::pair<ItemUPtr, ItemRef>;
    std::vector<MementoPair> mementoPairs;
    
    for (auto i : chords) {
        auto& item = i->as<Item>();
        mementoPairs.emplace_back(item.getMemento(), item);
    }
    
    for (auto i : notes) {
        auto& item = i->as<Item>();
        mementoPairs.emplace_back(item.getMemento(), item);
    }
    
    for (const auto i : rests) {
        auto& item = i->as<Item>();
        mementoPairs.emplace_back(item.getMemento(), item);
    }
    
    for (auto i : beams) {
        auto& item = i->as<Item>();
        mementoPairs.emplace_back(item.getMemento(), item);
    }
    
    for (const auto& pair : mementoPairs) {
        auto command = mFactory.itemStateCommand(pair.first->getMemento(),
                                                 pair.second.get().getMemento());
        addSubCommand(std::move(command));
    }
}


void
InsertBeamCommand::deleteBeams(const std::set<BeamIntf*>& beams)
{
    for (const auto& beam : beams) {
        auto command = mFactory.deleteBeamCommand(*beam);
        command->execute();
        addSubCommand(std::move(command));
    }
}

void
InsertBeamCommand::processBeam(BeamIntf* inBeam,
                               std::set<Temporal*>& ioBeamables,
                               std::set<Note*>& ioNotes,
                               std::set<Rest*>& ioRests,
                               std::set<Chord*>& ioChords,
                               std::set<BeamIntf*>& ioBeams) const
{
    if (inBeam) {
        
        ioBeams.emplace(inBeam);
        
        for (const auto& beamedNote : inBeam->getBeamedNotes()) {
            auto& t = beamedNote->getNote();
            if (t.getIsChordMember()) {
                auto anotherChordTemporal = t.getChord();
                KOMP_ASSERT(anotherChordTemporal->getItemTraits().getItemType() == ItemType::Chord);
                auto& anotherChord = anotherChordTemporal->as<Chord>();
                ioChords.emplace(&anotherChord);
                ioBeamables.emplace(&anotherChord);
            } else {
                if (t.getIsChord()) {
                    ioChords.emplace(&t.as<Chord>());
                    ioBeamables.emplace(&t);
                } else if (t.getIsNote()) {
                    ioNotes.emplace(&t.as<Note>());
                    ioBeamables.emplace(&t);
                } else if (t.getIsRest()) {
                    ioRests.emplace(&t.as<Rest>());
                    ioBeamables.emplace(&t);
                }
            }
        }
    }
}

void
InsertBeamCommand::processTemporal(Temporal& inTemporal,
                                   std::set<Temporal*>& ioBeamables,
                                   std::set<Note*>& ioNotes,
                                   std::set<Rest*>& ioRests,
                                   std::set<Chord*>& ioChords,
                                   std::set<BeamIntf*>& ioBeams) const
{
    processBeam(inTemporal.getBeam(),
                ioBeamables,
                ioNotes,
                ioRests,
                ioChords,
                ioBeams);
        
    if (inTemporal.getIsChordMember()) {
        auto chordTemporal = inTemporal.getChord();
        KOMP_ASSERT(chordTemporal->getItemTraits().getItemType() == ItemType::Chord);
        auto& chord = chordTemporal->as<Chord>();
        ioChords.emplace(&chord);
    } else {
        ioBeamables.emplace(&inTemporal);
    }
}

void
InsertBeamCommand::correctTemporalTypes(const std::set<Temporal*>& beamables)
{
    for (const auto& temporal : beamables) {
        auto duration = temporal->getDuration();
        const auto ttype = duration.getTemporalType();
        if (temporal->getIsRaw()) {
            duration.setTemporalType(TemporalType::EIGHTH);
            temporal->setIsRaw(false);
            temporal->setDuration(duration);
        } else if (TemporalValue::getNumFlags(ttype) < 1) {
            duration.setTemporalType(TemporalType::EIGHTH);
            temporal->setDuration(duration);
        }
    }
}


void
InsertBeamCommand::insertBeam(const std::set<Temporal*>& beamables)
{
    KOMP_ASSERT(beamables.size() >= 2);
    
    TemporalRefVec temporals;
    for (const auto& temporal : beamables) {
        temporals.emplace_back(*temporal);
    }
    
    sortItems(temporals);
    
    BeamFactory f;
    auto beamSpec = f.createBeamSpec(temporals);
    KOMP_ASSERT(beamSpec->getBeamedNoteCount() >= 2);
    auto& beam = f.createBeam(mRegistry, *beamSpec);
    const Temporal& first = temporals.front();
    auto& measure = first.getLocation().getMeasure();
    beam.setIsQuiescent(false);
    measure.insertItem(beam);
    if (mIsStemDirectionGiven) {
        beam.setStemPolicy(StemPolicy{mStemDirection, Policy::AUTOMATIC});
    }
    
    mBeamID = beam.getItemID();
    auto fixTimes = mFactory.fixTimesCommand(measure);
    fixTimes->execute();
    ItemImpl::setNeedsDisplay(beam);
    addSubCommand(std::move(fixTimes));
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
