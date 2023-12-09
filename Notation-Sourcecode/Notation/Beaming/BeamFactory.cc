// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeamFactory.h"

// Local
#include "Measure.h"
#include "Note.h"
#include "Rest.h"
#include "BeamSpec.h"
#include "BeamedNote.h"
#include "Beam.h"
#include "TemporalType.h"
#include "TemporalValue.h"
#include "Track.h"
#include "Score.h"

namespace komp
{

    BeamFactory::BeamFactory()
    {

    }


    BeamIntf&
    BeamFactory::beamSpan(ItemRegistry& inRegistry,
                             const Temporal& inTemporalA,
                             const Temporal& inTemporalB)
    {
        // Get the span of notes to beam
        const auto items = getFilteredTemporals(inTemporalA, inTemporalB);
        
        // Make sure that the items are not raw.  If they are, convert them
        // to eighth notes. This will avoid creating a beamed set of 1024 notes
        // (or whatever placeholder is being used for a raw note)
        for (Temporal& item : items) {
            if (item.getItemTraits().getItemType() == ItemType::Note) {
                auto& note = item.as<Note>();
                if (note.getIsRaw()) {
                    // Convert to eighth note
                    note.setIsRaw(false);
                    note.setDuration(note.getDuration().setTemporalType(TemporalType::EIGHTH));
                }
            }
        }
        
        auto beamSpec = createBeamSpec(items);
        auto& beam = createBeam(inRegistry, *beamSpec);
        insertBeam(beam);
        return beam;
    }
    
    
    void
    BeamFactory::autoBeam(ItemRegistry& inRegistry,
                             const Temporal& inTemporalA,
                             const Temporal& inTemporalB)
    {
        const auto temporals = getFilteredTemporals(inTemporalA, inTemporalB);
        autoBeam(inRegistry, temporals);
    }
    

    void
    BeamFactory::autoBeam(ItemRegistry& inRegistry,
                             const TemporalRefVec& inTemporals)
    {
        auto beamSpecs = groupBeats(inTemporals);
        for (const auto& beamSpec : beamSpecs) {
            auto& beam = createBeam(inRegistry, *beamSpec);
            insertBeam(beam);
        }
    }
    
    
    BeamSpecIntfUPtrVec
    BeamFactory::groupBeats(const TemporalRefVec& inTemporals) const
    {
        BeamSpecIntfUPtrVec beamSpecs;
        
        // abort if empty
        if (inTemporals.empty()) {
            return beamSpecs;
        }
        
        // copy the vector, ensuring that chords are included but their
        // constituant notes are note
        std::set<Temporal*> uniquenessHelper;
        TemporalRefVec temporals;
        
        for (Temporal& temporal : inTemporals) {
            if (temporal.getItemTraits().getItemType() == ItemType::Note) {
                auto& note = temporal.as<Note>();
                if (note.getIsChordMember()) {
                    auto chord = note.getChord();
                    if (chord) {
                        auto insertResult = uniquenessHelper.emplace(chord);
                        if (insertResult.second) {
                            temporals.emplace_back(*chord);
                            continue;
                        }
                    }
                } else {
                    auto insertResult = uniquenessHelper.emplace(&temporal);
                    if (insertResult.second) {
                        temporals.emplace_back(temporal);
                        continue;
                    }
                }
            } else {
                auto insertResult = uniquenessHelper.emplace(&temporal);
                if (insertResult.second) {
                    temporals.emplace_back(temporal);
                    continue;
                }
            }
        }

        // abort if empty
        if (temporals.empty()) {
            return beamSpecs;
        }
        
        Temporal& first = temporals.at(0);
        const auto& measure = first.getLocation().getMeasure();
        auto beatTimes = measure.getBeatGrouping().getBeatTimes();
        beatTimes.emplace_back(measure.getClocks());
        auto nextBeatMarker = beatTimes.cbegin();
        auto beatMarkerEnd = beatTimes.cend();
        
        int firstTemporalTime = first.getRoundedTime();
        
        // advance the beat marking sentinal to the correct starting point
        for( ; nextBeatMarker != beatMarkerEnd; ++nextBeatMarker) {
            if (*nextBeatMarker > firstTemporalTime) {
                break;
            }
        }
        
        TemporalRefVec currentBeatNotes;
        
        for (Temporal& temporal : temporals) {

            const auto temporalType = temporal.getDuration().getTemporalType();
            const auto isBeamableDuration = TemporalValue::getIsBeamable(temporalType);
            const auto isNewMeasure = (temporal.getLocation().getMeasure().getIndex() != measure.getIndex());
            const auto isNewBeat = (temporal.getRoundedTime() >= *nextBeatMarker);
            const auto isRest = temporal.getItemTraits().getItemType() == ItemType::Rest;
            
            if (isNewMeasure || isNewBeat || isRest || !isBeamableDuration) {
                if (currentBeatNotes.size() > 1) {
                    beamSpecs.emplace_back(createBeamSpec(currentBeatNotes));
                }
                currentBeatNotes.clear();
            }
            
            if (isNewMeasure) {
                const auto& newMeasure = temporal.getLocation().getMeasure();
                beatTimes = newMeasure.getBeatGrouping().getBeatTimes();
                beatTimes.emplace_back(newMeasure.getClocks());
                nextBeatMarker = beatTimes.cbegin();
                beatMarkerEnd = beatTimes.cend();
                // advance the beat marking sentinal to the correct starting point
                for( ; nextBeatMarker != beatMarkerEnd; ++nextBeatMarker) {
                    if (*nextBeatMarker > firstTemporalTime) {
                        break;
                    }
                }
            }
            
            if (!isNewMeasure) {
                if (isNewBeat) {
                    ++nextBeatMarker;
                    if (nextBeatMarker == beatMarkerEnd) {
                        --nextBeatMarker;
                    }
                }
            }
            
            if (!isRest && isBeamableDuration) {
                currentBeatNotes.emplace_back(temporal);
            } else {
                if (currentBeatNotes.size() > 1) {
                    beamSpecs.emplace_back(createBeamSpec(currentBeatNotes));
                }
                currentBeatNotes.clear();
            }
        }
        
        if (currentBeatNotes.size() > 1) {
            beamSpecs.emplace_back(createBeamSpec(currentBeatNotes));
        }
        
        return beamSpecs;
    }


    BeamSpecIntfUPtr
    BeamFactory::createBeamSpec(const TemporalRefVec& inTemporals) const
    {
        BeamSpecIntfUPtr beamSpec = std::make_unique<BeamSpec>();
        for (const auto& temporal : inTemporals) {
            BeamedNote beamedNote(temporal);
            beamSpec->addBeamedNote(beamedNote);
        }
        KOMP_ASSERT(beamSpec->getBeamedNotes().size() > 1);
        return beamSpec;
    }
    
    
    BeamSpecIntfUPtr
    BeamFactory::createBeamSpec() const
    {
        return std::make_unique<BeamSpec>();
    }


    BeamIntf&
    BeamFactory::createBeam(ItemRegistry& inRegistry,
                               const BeamSpecIntf& inSpec) const
    {
        KOMP_ASSERT(inSpec.getBeamedNoteCount() > 1);
        auto beamSpec = inSpec.clone();
        beamSpec->replaceChordedNotes();
        
        const auto& note = inSpec.getBeamedNote(0);
        const auto& loc = note.getNote().getLocation();
        return inRegistry.createItem<Beam>(loc, *beamSpec);
    }


    BeamedNoteIntfUPtr
    BeamFactory::createBeamedNote(Temporal& inTemporal)
    {
        if (inTemporal.getItemTraits().getItemType() == ItemType::Note) {
            const auto note = dynamic_cast<Note*>(&inTemporal);
            const auto chord = note->getChord();
            if (chord) {
                return std::make_unique<BeamedNote>(*chord);
            }
        }
        return std::make_unique<BeamedNote>(inTemporal);
    }


    void
    BeamFactory::insertBeam(BeamIntf& inBeam) const
    {
        const auto& beamedNotes = inBeam.getBeamedNotes();
        KOMP_ASSERT(beamedNotes.size() > 0);
        auto& measure = beamedNotes.at(0)->getNote().getLocation().getMeasure();
        inBeam.setIsQuiescent(false);
        measure.insertItem(inBeam, inBeam.getTime());
    }
    
    
    TemporalRefVec
    BeamFactory::getFilteredTemporals(const Temporal& inTemporalA,
                                         const Temporal& inTemporalB) const
    {
        const auto& locA = inTemporalA.getLocation();
        
        auto& track = locA.getTrack();
        const auto& measure = locA.getMeasure();
        const int voice = locA.getVoice();
        const int hand = locA.getHand();
        
        auto filter = [&](Item& filterItem) {
            
            const auto& filterLoc = filterItem.getLocation();

            if (&filterLoc.getTrack() != &track) {
                return false;
            }
            
            if (filterLoc.getMeasure().getIndex() != measure.getIndex()) {
                return false;
            }
            
            if (filterLoc.getHand() != hand) {
                return false;
            }
            
            if (filterLoc.getVoice() != voice) {
                return false;
            }
            
            if (filterItem.getRoundedTime() < inTemporalA.getRoundedTime()) {
                return false;
            }
            
            if (filterItem.getRoundedTime() > inTemporalB.getRoundedTime()) {
                return false;
            }
            
            if (filterItem.getItemTraits().getItemType() == ItemType::Rest) {
                return false;
            }
            
            if (filterItem.getItemTraits().getItemType() == ItemType::Note) {
                auto& note = filterItem.as<Note>();
                if (note.getIsChordMember()) {
                    return false;
                }
            }
            
            return true;
        };
        
        return measure.items<Temporal>(track, filter);
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
