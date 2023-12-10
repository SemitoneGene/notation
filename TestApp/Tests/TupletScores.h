// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "MeasureLocation.h"
#include "BeamFactory.h"
#include "Tuplet.h"
#include "Rest.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{
    using namespace komp;

    inline void makeTuplet(Measure& inMeasure,
                           const std::vector<TestNote>& inNotes,
                           const TupletRatio& inTupletRatio,
                           TestNoteFactory& testNoteFactory)
    {
        TemporalRefVec tupletNotes;
        TemporalRefVec beamNotes;
        NoteRefVec chordNotes;
        testNoteFactory.autoAdvanceMeasure = false;

        auto& score = testNoteFactory.score;
        const auto& track = score.getTracks().front();
        auto& registry = score.getItemRegistry();

        for (const auto& testNote : inNotes) {
            if (testNote.isRest) {
                if (beamNotes.size() > 1) {
                    BeamFactory beamFactory;
                    auto beamSpec = beamFactory.createBeamSpec(beamNotes);
                    auto& beam = beamFactory.createBeam(registry, *beamSpec);
                    beamFactory.insertBeam(beam);
                }
                beamNotes.clear();
                
                const MusicLocation loc(inMeasure, *track.get());
                Rest& rest = registry.createItem<Rest>(loc, testNote.temporalType);
                komp::Duration duration{testNote.temporalType};
                duration.setDots(testNote.dots);
                rest.setDuration(duration);
                rest.setTime(testNoteFactory.time);
                rest.setIsQuiescent(false);
                score.getMeasure(testNoteFactory.measureIndex)->insertItem(rest);
                tupletNotes.emplace_back(rest);
                testNoteFactory.temporalType = testNote.temporalType;
                testNoteFactory.dots = testNote.dots;
                testNoteFactory.setEntryPositionBasedOnMeasureContents();
            } else {
                Chord* chord = nullptr;
                Note* note = nullptr;
                testNoteFactory.temporalType = testNote.temporalType;
                testNoteFactory.dots = testNote.dots;
                for (const auto& pitch : testNote.pitches) {
                    testNoteFactory.pitch = pitch;
                    const auto acc = AccidentalInfo::getAccidentalType(pitch.alter);
                    if (acc != AccidentalType::NATURAL) {
                        testNoteFactory.accidental = acc;
                    } else {
                        testNoteFactory.accidental = AccidentalType::NONE;
                    }
                    Note& insertedNote = testNoteFactory.insertNote();
                    chordNotes.emplace_back(insertedNote);
                    note = &insertedNote;
                }

                if (chordNotes.size() >= 2) {
                    const MusicLocation loc(inMeasure, *track.get());
                    chord = &registry.createItem<Chord>(loc, chordNotes);
                    chord->setIsQuiescent(false);
                    score.getMeasure(testNoteFactory.measureIndex)->insertItem(*chord);
                }

                if (TemporalValue::getNumFlags(testNote.temporalType) >= 1) {
                    if (chord) {
                        beamNotes.emplace_back(*chord);
                        tupletNotes.emplace_back(*chord);
                    } else {
                        beamNotes.emplace_back(*note);
                        tupletNotes.emplace_back(*note);
                    }
                } else {
                    if (beamNotes.size() > 1) {
                        BeamFactory beamFactory;
                        auto beamSpec = beamFactory.createBeamSpec(beamNotes);
                        auto& beam = beamFactory.createBeam(registry, *beamSpec);
                        beamFactory.insertBeam(beam);
                    }
                    beamNotes.clear();
                }

                testNoteFactory.setEntryPositionBasedOnMeasureContents();
                chordNotes.clear();

            } // if testNote !isRest

        } // for each testNote

        if (beamNotes.size() > 1) {
            BeamFactory beamFactory;
            auto beamSpec = beamFactory.createBeamSpec(beamNotes);
            auto& beam = beamFactory.createBeam(registry, *beamSpec);
            beamFactory.insertBeam(beam);
        }
        beamNotes.clear();

        if (tupletNotes.size() > 0) {
            TupletParams tparams;
            tparams.tupletRatio = inTupletRatio;
            tparams.setTemporals(tupletNotes);
            const MusicLocation loc(inMeasure, *track.get());
            auto& tuplet = registry.createItem<Tuplet>(loc, tparams);
            const auto& measure = score.getMeasure(testNoteFactory.measureIndex);
            tuplet.setIsQuiescent(false);
            measure->insertItem(tuplet);
        }
    }

    inline ScoreUPtr makeTupletScore1()
    {
        auto score = getBasicScore(1);
        const auto& track = score->getTracks().at(0);
        auto& measure = *score->getMeasures().front().get();

        TestNoteFactory factory{*score.get()};
        NoteRefVec notes;
        
        factory.temporalType = TemporalType::EIGHTH;
        factory.pitch.step = PitchStep::G;
        factory.pitch.octave = 5;
        notes.push_back(factory.insertNote());
        factory.setEntryPositionBasedOnMeasureContents();
        
        factory.pitch.step = PitchStep::E;
        notes.push_back(factory.insertNote());
        factory.setEntryPositionBasedOnMeasureContents();
        
        factory.pitch.step = PitchStep::C;
        notes.push_back(factory.insertNote());
        factory.setEntryPositionBasedOnMeasureContents();
        
        BeamFactory beamFactory;
        TemporalRefVec temporals;
        for (const auto& note : notes) {
            temporals.emplace_back(note);
        }
        
        auto& registry = score->getItemRegistry();
        auto& commandFactory = score->getCommandFactory();
        
        auto beamSpec = beamFactory.createBeamSpec(temporals);
        auto& beam = beamFactory.createBeam(registry, *beamSpec);
        beamFactory.insertBeam(beam);
        
        TemporalRefVec wtemporals;
        for (const auto& note : notes) {
            wtemporals.emplace_back(note);
        }
        
        TupletParams tupletParams;
        tupletParams.setTemporals(std::move(wtemporals));
        tupletParams.tupletRatio = TupletRatio{DurationCount{3, TemporalType::EIGHTH, 0},
                                               DurationCount{2, TemporalType::EIGHTH, 0}};
        const MusicLocation loc(measure, *track.get());
        auto& tuplet = registry.createItem<Tuplet>(loc, tupletParams);
        tuplet.setIsQuiescent(false);
        measure.insertItem(tuplet);

        auto& nextMeasure = *score->getMeasure(factory.measureIndex).get();
        auto fixTimes = commandFactory.fixTimesCommand(nextMeasure);
        fixTimes->execute();
        factory.setEntryPositionBasedOnMeasureContents();

        {
            std::vector<TestNote> testNotes;

            {
                TestNote testNote;
                testNote.isRest = true;
                testNote.temporalType = TemporalType::SIXTEENTH;
                testNote.pitches.emplace_back(Pitch{});
                testNotes.push_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::SIXTEENTH;
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::E;
                testNotes.push_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::SIXTEENTH;
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::F;
                testNotes.push_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::SIXTEENTH;
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::G;
                testNotes.push_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::SIXTEENTH;
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::A;
                ++testNote.pitches.at(0).octave;
                testNotes.push_back(testNote);
            }
            
            const DurationCount five{5, TemporalType::SIXTEENTH, 0};
            const DurationCount four{4, TemporalType::SIXTEENTH, 0};
            const TupletRatio ratio{five, four};
            
            makeTuplet(nextMeasure, testNotes, ratio, factory);
        }
        
        {
            std::vector<TestNote> testNotes;
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::EIGHTH;
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::A;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(1).step = PitchStep::E;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(2).step = PitchStep::F;
                
                testNotes.emplace_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::EIGHTH;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::B;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(1).step = PitchStep::F;
                testNote.pitches.at(1).alter = 1;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(2).step = PitchStep::G;
                
                testNotes.emplace_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::EIGHTH;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::D;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(1).step = PitchStep::G;
                testNote.pitches.at(1).alter = 1;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(2).step = PitchStep::E;
                
                testNotes.emplace_back(testNote);
            }
            
            const DurationCount three{3, TemporalType::EIGHTH, 0};
            const DurationCount two{2, TemporalType::EIGHTH, 0};
            const auto ratio = TupletRatio{three, two};
            
            makeTuplet(nextMeasure, testNotes, ratio, factory);
            fixTimes = commandFactory.fixTimesCommand(nextMeasure);
            fixTimes->execute();
            factory.setEntryPositionBasedOnMeasureContents();
        }
        
        {
            std::vector<TestNote> testNotes;
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::EIGHTH;
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::A;

                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(1).step = PitchStep::E;

                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(2).step = PitchStep::F;

                testNotes.emplace_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::EIGHTH;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(0).step = PitchStep::B;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(1).step = PitchStep::F;
                
                testNote.pitches.emplace_back(Pitch{});
                testNote.pitches.at(2).step = PitchStep::G;
                
                testNotes.emplace_back(testNote);
            }
            
            {
                TestNote testNote;
                testNote.temporalType = TemporalType::EIGHTH;
                testNote.isRest = true;
                testNotes.emplace_back(testNote);
            }
            
            const DurationCount three{3, TemporalType::EIGHTH, 0};
            const DurationCount two{2, TemporalType::EIGHTH, 0};
            const auto ratio = TupletRatio{three, two};

            makeTuplet(nextMeasure, testNotes, ratio, factory);

            fixTimes = commandFactory.fixTimesCommand(nextMeasure);
            fixTimes->execute();
            factory.setEntryPositionBasedOnMeasureContents();
        }
                
        score->layout();
        
        return score;
    }
    
    inline ScoreUPtr makeTupletScore2()
    {
        auto score = getBasicScore(1);
        return score;
    }
    
    inline ScoreUPtrVec getTupletScores()
    {
        ScoreUPtrVec vec;
        vec.emplace_back(makeTupletScore1());
        //vec.emplace_back(makeTupletScore2());
        return vec;
    }
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
