// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp
#include "Score.h"
#include "Track.h"
#include "TrackInfo.h"
#include "Clef.h"
#include "Temporal.h"
#include "Measure.h"
#include "Note.h"
#include "TemporalValue.h"
#include "TestNoteFactory.h"
#include "BeamSpec.h"
#include "BeamedNote.h"
#include "BeamLine.h"
#include "BeamLineIntf.h"
#include "Beam.h"
#include "BeamFactory.h"


namespace komp
{
inline komp::BeamSpecIntfUPtr
makeSomeBeamedEighthNotes(std::vector<int> midiPitches,
                          komp::TestNoteFactory& noteFactory)
{
    using namespace komp;
    
    auto& score = noteFactory.score;
    auto& reg = score.getItemRegistry();
    auto beamSpec = std::make_unique<BeamSpec>();
    
    for (auto midiPitch : midiPitches) {
        for (auto slot = -10; slot < 20; ++ slot) {
            const auto midiSlot = Pitch::midiNoteForSlot(slot, ClefType::G);
            const auto diff = midiPitch - midiSlot;
            if (diff > -2 && diff < 2) {
                noteFactory.pitch = Pitch{ClefType::G, slot, midiPitch};
                break;
            }
        }
        
        noteFactory.temporalType = TemporalType::EIGHTH;
        
        const auto note = noteFactory.insertNote();
        noteFactory.advancePosition();
        if (noteFactory.measureIndex > static_cast<int>(score.getMeasureCount()) - 1) {
            score.insertMeasure(noteFactory.measureIndex);
            const auto measureIndex = noteFactory.measureIndex - 1;
            const auto& measure = score.getMeasure(measureIndex);
            const auto timeSignatureTop = measure->getBeats();
            const auto timeSignatureBottom = measure->getBeatUnit();
            score.getMeasure(noteFactory.measureIndex)->setMeter(timeSignatureTop,
                                                                 timeSignatureBottom);
        }
        
        auto& beamNote = reg.getItem(note.getItemID());
        BeamedNote beamedNote{beamNote.as<Temporal>()};
        beamSpec->addBeamedNote(beamedNote);
    }
    
    if (beamSpec->getBeamedNoteCount() > 1) {
        auto& measure = beamSpec->getMeasure();
        const auto& track = score.getTracks().front();
        const MusicLocation loc(measure, *track.get());
        auto& beam = reg.createItem<Beam>(loc, *beamSpec);
        beam.setIsQuiescent(false);
        measure.insertItem(beam, beam.getTime());
    }
    return std::move(beamSpec);
}


inline komp::ScoreUPtr
makeE2(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
        
    komp::TrackInfoList tracks;
    
    komp::TrackInfo voiceTrackInfo;
    voiceTrackInfo.setName("BeamE2Test");
    voiceTrackInfo.addStaffInfo(komp::ClefType::G);
    tracks.push_back(voiceTrackInfo);
    
    komp::ScoreInfo scoreInfo;
    scoreInfo.trackInfoList = std::move(tracks);
    scoreInfo.pages = 1;
    scoreInfo.measuresPerLine = 4;
    scoreInfo.linesPerPage = 6;
    
    auto score = Score::create(scoreInfo);
    MeasureNumberInfo measureNumberInfo;
    measureNumberInfo.Type = MeasureNumberType::EVERY;
    score->setMeasureNumberInfo(measureNumberInfo);
    
    const auto d8th = TemporalType::EIGHTH;
    
    /////////////////////////////////////////////////////////////
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.pitch.step = PitchStep::D;
    
    noteFactory.temporalType = d8th;
    
    // repeated notes
    for (int i = 50; i < 102; ++i) {
        outBeamSpecs.emplace_back(makeSomeBeamedEighthNotes({i, i}, noteFactory));
    }
    
    int counter = 0;
    for (int firstNote = 54; firstNote < 98; ++firstNote) {
        for (int secondNote = 54; secondNote < 98; ++secondNote) {
            if (firstNote != secondNote) {
                if (counter % 11 == 0) {
                    outBeamSpecs.emplace_back(makeSomeBeamedEighthNotes({firstNote, secondNote}, noteFactory));
                }
            }
            ++counter;
        }
    }
    score->layout();
    
    return score;
}


inline komp::ScoreUPtr
makeE3(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    
    komp::TrackInfoList tracks;
    
    komp::TrackInfo voiceTrackInfo;
    voiceTrackInfo.setName("BeamE3Test");
    voiceTrackInfo.addStaffInfo(komp::ClefType::G);
    tracks.push_back(voiceTrackInfo);
    
    komp::ScoreInfo scoreInfo;
    scoreInfo.trackInfoList = std::move(tracks);
    scoreInfo.pages = 1;
    scoreInfo.measuresPerLine = 4;
    scoreInfo.linesPerPage = 6;
    
    int timeSignatureTop = 3;
    int timeSignatureBottom = 8;
    
    auto score = Score::create(scoreInfo);
    for (auto iter = score->getMeasuresBegin(); iter != score->getMeasuresEnd(); ++iter) {
        const auto& m = *iter;
        m->setMeter(timeSignatureTop, timeSignatureBottom);
        m->setTimeSignatureType(TimeSignatureType::NUMERIC);
    }
    
    MeasureNumberInfo measureNumberInfo;
    measureNumberInfo.Type = MeasureNumberType::EVERY;
    score->setMeasureNumberInfo(measureNumberInfo);
    
    const auto d8th = TemporalType::EIGHTH;
    
    /////////////////////////////////////////////////////////////
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.temporalType = d8th;
    
    int extremeLow = 54;
    int extremeHigh = 98;
    
    int counter = 0;
    for (int firstNote = extremeLow; firstNote < extremeHigh; ++firstNote) {
        for (int secondNote = extremeLow; secondNote < extremeHigh; ++secondNote) {
            for (int thirdNote = extremeLow; thirdNote < extremeHigh; ++thirdNote) {
                if (firstNote != secondNote) {
                    if (counter % 199 == 0) {
                        outBeamSpecs.emplace_back(makeSomeBeamedEighthNotes({firstNote, secondNote, thirdNote}, noteFactory));
                    }
                }
                ++counter;
            }
        }
    }
    score->layout();
    
    return score;
}


inline komp::ScoreUPtr
makeE4(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    
    komp::TrackInfoList tracks;
    
    komp::TrackInfo voiceTrackInfo;
    voiceTrackInfo.setName("BeamE4Test");
    voiceTrackInfo.addStaffInfo(komp::ClefType::G);
    tracks.push_back(voiceTrackInfo);
    
    komp::ScoreInfo scoreInfo;
    scoreInfo.trackInfoList = std::move(tracks);
    scoreInfo.pages = 1;
    scoreInfo.measuresPerLine = 4;
    scoreInfo.linesPerPage = 6;
    
    auto score = Score::create(scoreInfo);
    MeasureNumberInfo measureNumberInfo;
    measureNumberInfo.Type = MeasureNumberType::EVERY;
    score->setMeasureNumberInfo(measureNumberInfo);
    
    const auto d8th = TemporalType::EIGHTH;
    
    BeamSpec beamSpec;
    
    /////////////////////////////////////////////////////////////
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.pitch.step = PitchStep::D;
    
    noteFactory.temporalType = d8th;
    
    int total = 0;
    for (int firstNote = 55; firstNote < 95; firstNote += 5) {
        for (int secondNote = 55; secondNote < 95; secondNote += 10) {
            for (int thirdNote = 55; thirdNote < 95; thirdNote += 10) {
                for(int fourthNote = 55; fourthNote < 95; fourthNote += 5) {
                    if (total % 7 == 0) {
                        outBeamSpecs.emplace_back(makeSomeBeamedEighthNotes({firstNote, secondNote, thirdNote, fourthNote}, noteFactory));
                    }
                    ++total;
                }
            }
        }
    }
    score->layout();
    
    return score;
}


inline komp::ScoreUPtr
makeE5(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    
    komp::TrackInfoList tracks;
    
    komp::TrackInfo voiceTrackInfo;
    voiceTrackInfo.setName("BeamE5Test");
    voiceTrackInfo.addStaffInfo(komp::ClefType::G);
    tracks.push_back(voiceTrackInfo);
    
    komp::ScoreInfo scoreInfo;
    scoreInfo.trackInfoList = std::move(tracks);
    scoreInfo.pages = 1;
    scoreInfo.measuresPerLine = 4;
    scoreInfo.linesPerPage = 6;
    
    int timeSignatureTop = 5;
    int timeSignatureBottom = 8;
    
    auto score = Score::create(scoreInfo);
    for (auto iter = score->getMeasuresBegin(); iter != score->getMeasuresEnd(); ++iter) {
        const auto& m = *iter;
        m->setMeter(timeSignatureTop, timeSignatureBottom);
        m->setTimeSignatureType(TimeSignatureType::NUMERIC);
    }
    
    MeasureNumberInfo measureNumberInfo;
    measureNumberInfo.Type = MeasureNumberType::EVERY;
    score->setMeasureNumberInfo(measureNumberInfo);
    
    const auto d8th = TemporalType::EIGHTH;
    
    /////////////////////////////////////////////////////////////
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.temporalType = d8th;
    
    int extremeLow = 54;
    int extremeHigh = 98;
    
    long long counter = 0;
    for (int firstNote = extremeLow; firstNote < extremeHigh; ++firstNote) {
        for (int secondNote = extremeLow; secondNote < extremeHigh; ++secondNote) {
            for (int thirdNote = extremeLow; thirdNote < extremeHigh; ++thirdNote) {
                for (int fourthNote = extremeLow; fourthNote < extremeHigh; ++fourthNote) {
                    for (int fifthNote = extremeLow; fifthNote < extremeHigh; ++fifthNote) {
                        if (firstNote != secondNote) {
                            if (counter % 399277 == 0) {
                                outBeamSpecs.emplace_back(makeSomeBeamedEighthNotes({
                                    firstNote,
                                    secondNote,
                                    thirdNote,
                                    fourthNote,
                                    fifthNote}, noteFactory));
                            }
                        }
                        ++counter;
                    }
                }
            }
        }
    }
    score->layout();
    
    return score;
}

inline komp::ScoreUPtr
makeS4(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    
    komp::TrackInfoList tracks;
    
    komp::TrackInfo voiceTrackInfo;
    voiceTrackInfo.setName("BeamS4Test");
    voiceTrackInfo.addStaffInfo(komp::ClefType::G);
    tracks.push_back(voiceTrackInfo);
    
    komp::ScoreInfo scoreInfo;
    scoreInfo.trackInfoList = std::move(tracks);
    scoreInfo.pages = 1;
    scoreInfo.measuresPerLine = 4;
    scoreInfo.linesPerPage = 4;
    
    int timeSignatureTop = 1;
    int timeSignatureBottom = 4;
    
    auto score = Score::create(scoreInfo);
    for (auto iter = score->getMeasuresBegin(); iter != score->getMeasuresEnd(); ++iter) {
        const auto& m = *iter;
        m->setMeter(timeSignatureTop, timeSignatureBottom);
        m->setTimeSignatureType(TimeSignatureType::NUMERIC);
    }
    
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.temporalType = TemporalType::SIXTEENTH;
    noteFactory.pitch.step = PitchStep::F;
    
    auto& reg = score->getItemRegistry();
    
    for (int a=0; a < 2; ++a) {
        for (int b=0; b < 2; ++b) {
            for (int c=0; c < 2; ++c) {
                for (int d=0; d < 2; ++d) {
                    const bool bA = static_cast<bool>(a);
                    const bool bB = static_cast<bool>(b);
                    const bool bC = static_cast<bool>(c);
                    const bool bD = static_cast<bool>(d);
                    
                    BeamFactory beamFactory;
                    BeamSpecIntfUPtr beamSpec = std::make_unique<BeamSpec>();
                    
                    auto note = noteFactory.insertNote();
                    auto beamedNote01 = BeamedNote{note};
                    noteFactory.advancePosition();
                    beamedNote01.setIsBroken(1, bA);
                    beamSpec->addBeamedNote(std::move(beamedNote01));
                    
                    note = noteFactory.insertNote();
                    auto beamedNote02 = BeamedNote{note};
                    noteFactory.advancePosition();
                    beamedNote02.setIsBroken(1, bB);
                    beamSpec->addBeamedNote(std::move(beamedNote02));
                    
                    note = noteFactory.insertNote();
                    auto beamedNote03 = BeamedNote{note};
                    noteFactory.advancePosition();
                    beamedNote03.setIsBroken(1, bC);
                    beamSpec->addBeamedNote(std::move(beamedNote03));
                    
                    note = noteFactory.insertNote();
                    auto beamedNote04 = BeamedNote{note};
                    noteFactory.advancePosition();
                    beamedNote04.setIsBroken(1, bD);
                    beamSpec->addBeamedNote(std::move(beamedNote04));
                    
                    const auto& loc = beamedNote04.getNote().getLocation();
                    auto& beam = reg.createItem<Beam>(loc, *beamSpec);
                    beam.setIsQuiescent(false);
                    beamSpec->getMeasure().insertItem(beam, beam.getTime());
                    outBeamSpecs.emplace_back(std::move(beamSpec));
                }
            }
        }
    }
    
    score->layout();
    
    return score;
}


inline komp::ScoreUPtr
make16thFigures(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    
    komp::TrackInfoList tracks;
    
    komp::TrackInfo voiceTrackInfo;
    voiceTrackInfo.setName("Beam16thFigures");
    voiceTrackInfo.addStaffInfo(komp::ClefType::G);
    tracks.push_back(voiceTrackInfo);
    
    komp::ScoreInfo scoreInfo;
    scoreInfo.trackInfoList = std::move(tracks);
    scoreInfo.pages = 1;
    scoreInfo.measuresPerLine = 2;
    scoreInfo.linesPerPage = 1;
    
    int timeSignatureTop = 4;
    int timeSignatureBottom = 4;
    
    auto score = Score::create(scoreInfo);
    for (auto iter = score->getMeasuresBegin(); iter != score->getMeasuresEnd(); ++iter) {
        const auto& m = *iter;
        m->setMeter(timeSignatureTop, timeSignatureBottom);
        m->setTimeSignatureType(TimeSignatureType::NUMERIC);
    }
    
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.temporalType = TemporalType::SIXTEENTH;
    noteFactory.pitch.step = PitchStep::F;
    auto& reg = noteFactory.score.getItemRegistry();
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();
        
        BeamedNote beamedNote{noteFactory.insertNote()};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote));
        noteFactory.pitch.step = PitchStep::E;
        noteFactory.temporalType = TemporalType::EIGHTH;
        auto note = noteFactory.insertNote();
        auto beamedNote01 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        noteFactory.pitch.step = PitchStep::C;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        note = noteFactory.insertNote();
        auto beamedNote03 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote03));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam01 = reg.createItem<Beam>(loc, *beamSpec);
        beam01.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam01, beam01.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::F;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote04 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote04));
        
        noteFactory.pitch.step = PitchStep::E;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::EIGHTH;
        noteFactory.dots = 1;
        note = noteFactory.insertNote();
        auto beamedNote05 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote05));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam02 = reg.createItem<Beam>(loc, *beamSpec);
        beam02.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam02, beam02.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::D;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote06 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote06));
        
        noteFactory.pitch.step = PitchStep::C;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote07 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote07));
        
        noteFactory.pitch.step = PitchStep::D;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote08 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote08));
        
        noteFactory.pitch.step = PitchStep::G;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote09 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote09));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam03 = reg.createItem<Beam>(loc, *beamSpec);
        beam03.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam03, beam03.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::A;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::EIGHTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote10 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote10));
        
        noteFactory.pitch.step = PitchStep::C;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote11 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote11));
        
        noteFactory.pitch.step = PitchStep::D;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote13 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote13));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam04 = reg.createItem<Beam>(loc, *beamSpec);
        beam04.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam04, beam04.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::B;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote));
        
        noteFactory.pitch.step = PitchStep::C;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote01 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        noteFactory.pitch.step = PitchStep::D;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::EIGHTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote02 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote02));
                
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam05 = reg.createItem<Beam>(loc, *beamSpec);
        beam05.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam05, beam05.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::E;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::EIGHTH;
        noteFactory.dots = 1;
        auto note = noteFactory.insertNote();
        auto beamedNote01 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        noteFactory.pitch.step = PitchStep::B;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote02 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote02));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam06 = reg.createItem<Beam>(loc, *beamSpec);
        beam06.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam06, beam06.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
        
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::E;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote));
        
        noteFactory.pitch.step = PitchStep::F;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote01 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam07 = reg.createItem<Beam>(loc, *beamSpec);
        beam07.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam07, beam07.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::F;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote));
        
        noteFactory.pitch.step = PitchStep::E;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote01 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam08 = reg.createItem<Beam>(loc, *beamSpec);
        beam08.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam08, beam08.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();

        noteFactory.pitch.step = PitchStep::E;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote));
        
        noteFactory.pitch.step = PitchStep::F;
        noteFactory.pitch.octave = 6;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote01 =BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        auto& beam09 = reg.createItem<Beam>(loc, *beamSpec);
        beam09.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam09, beam09.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
        
    }
    
    {
        auto beamSpec = std::make_unique<BeamSpec>();
        
        noteFactory.pitch.step = PitchStep::F;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        auto note = noteFactory.insertNote();
        auto beamedNote = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote));
        
        auto& measure = beamSpec->getMeasure();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(measure, *track.get());

        noteFactory.pitch.step = PitchStep::E;
        noteFactory.pitch.octave = 4;
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        noteFactory.dots = 0;
        note = noteFactory.insertNote();
        auto beamedNote01 = BeamedNote{note};
        noteFactory.advancePosition();
        beamSpec->addBeamedNote(std::move(beamedNote01));
        
        
        auto& beam10 = reg.createItem<Beam>(loc, *beamSpec);
        beam10.setIsQuiescent(false);
        beamSpec->getMeasure().insertItem(beam10, beam10.getTime());
        outBeamSpecs.emplace_back(std::move(beamSpec));
    }
    
    score->layout();
    
    return score;
}

inline komp::ScoreUPtrVec getBeamTestScores()
{
    
    komp::ScoreUPtrVec outScores;
    
    komp::BeamSpecIntfUPtrVec E2Specs;
    outScores.emplace_back(makeE2(E2Specs));
    
    komp::BeamSpecIntfUPtrVec E3Specs;
    outScores.emplace_back(makeE3(E3Specs));
    
    komp::BeamSpecIntfUPtrVec E4Specs;
    outScores.emplace_back(makeE4(E4Specs));
    
    komp::BeamSpecIntfUPtrVec E5Specs;
    outScores.emplace_back(makeE5(E5Specs));
    
    komp::BeamSpecIntfUPtrVec S4Specs;
    outScores.emplace_back(makeS4(S4Specs));
    
    komp::BeamSpecIntfUPtrVec theSpecs;
    outScores.emplace_back(make16thFigures(theSpecs));
    
    return outScores;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
