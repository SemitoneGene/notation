// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp
#include "Score.h"
#include "ScoreCreationInfo.h"
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

namespace komp
{
inline ScoreUPtr makeBeamSaveScore0(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    
    outBeamSpecs.clear();
    
    komp::TrackInfoList tracks;
    
    komp::Instrument instrument;
    instrument.name = "Piano";
    komp::TrackInfo voiceTrackInfo(instrument);
    voiceTrackInfo.setName("BeamSaveScore0");
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
    
    Note* note = nullptr;
    BeamSpecIntfUPtr beamSpec = std::make_unique<BeamSpec>();
    
    /////////////////////////////////////////////////////////////
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.pitch.step = PitchStep::D;
    noteFactory.temporalType = TemporalType::EIGHTH;
    auto& reg = score->getItemRegistry();
    
    note = &noteFactory.insertNote();
    noteFactory.advancePosition();
    BeamedNote beamedNote{*note};
    beamSpec->addBeamedNote(std::move(beamedNote));
    
    auto& measure = beamSpec->getMeasure();
    auto& track = score->getTracks().front();
    const MusicLocation loc(measure, *track.get());
    
    /////////////////////////////////////////////////////////////
    for (int i = 1; i < 16; ++i) {
        
        note = &noteFactory.insertNote();
        noteFactory.advancePosition();
        auto innerNote = BeamedNote{*note};
        beamSpec->addBeamedNote(std::move(innerNote));
        
        if (i == 2 ||
            i == 5 ||
            i == 7 ||
            i == 13) {
            
            auto& beam = reg.createItem<Beam>(loc, *beamSpec);
            beam.setIsQuiescent(false);
            measure.insertItem(beam, beam.getTime());
            outBeamSpecs.emplace_back(std::move(beamSpec));
            beamSpec = std::make_unique<BeamSpec>();
        }
    }
    
    
    auto& beam = reg.createItem<Beam>(loc, *beamSpec);
    beam.setIsQuiescent(false);
    measure.insertItem(beam, beam.getTime());
    outBeamSpecs.emplace_back(std::move(beamSpec));
    
    score->layout();
    
    return score;
}

inline komp::ScoreUPtr
getBeamSaveScore0()
{
    komp::BeamSpecIntfUPtrVec beamSpecs;
    return makeBeamSaveScore0(beamSpecs);
}

inline komp::ScoreUPtr
makeBeamSaveScore1(komp::BeamSpecIntfUPtrVec& outBeamSpecs)
{
    using namespace komp;
    outBeamSpecs.clear();
    
    komp::TrackInfoList tracks;
    
    komp::Instrument instrument;
    instrument.name = "Piano";
    komp::TrackInfo voiceTrackInfo(instrument);
    voiceTrackInfo.setName("BeamSaveScore1");
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
    
    Note* note = nullptr;
    BeamSpecIntfUPtr beamSpec = std::make_unique<BeamSpec>();
    
    /////////////////////////////////////////////////////////////
    TestNoteFactory noteFactory{*score.get()};
    noteFactory.pitch.step = PitchStep::D;
    noteFactory.temporalType = TemporalType::THIRTY_SECOND;
    
    auto& reg = score->getItemRegistry();
    
    note = &noteFactory.insertNote();
    noteFactory.advancePosition();
    BeamedNote beamedNote{*note};
    beamedNote.setIsBroken(0, false);
    beamedNote.setIsBroken(1, false);
    beamedNote.setIsBroken(2, false);
    beamSpec->addBeamedNote(std::move(beamedNote));
    
    const auto& loc = beamedNote.getNote().getLocation();
    
    
    /////////////////////////////////////////////////////////////
    for (int i = 1; i < 32; ++i) {
        
        bool is16thBroken = i % 3;
        bool is32ndBroken = i % 4;
        
        note = &noteFactory.insertNote();
        noteFactory.advancePosition();
        auto innerNote = BeamedNote{*note};
        innerNote.setIsBroken(0, false);
        innerNote.setIsBroken(1, is16thBroken);
        innerNote.setIsBroken(2, is32ndBroken);
        beamSpec->addBeamedNote(std::move(innerNote));
        
        
        if (i % 6 == 0 && i < 30) {
            auto& beam = reg.createItem<Beam>(loc, *beamSpec);
            beam.setIsQuiescent(false);
            beamSpec->getMeasure().insertItem(beam, beam.getTime());
            outBeamSpecs.emplace_back(std::move(beamSpec));
            beamSpec = std::make_unique<BeamSpec>();
        }
    }
    
    auto& beam = reg.createItem<Beam>(loc, *beamSpec);
    beam.setIsQuiescent(false);
    beamSpec->getMeasure().insertItem(beam, beam.getTime());
    outBeamSpecs.emplace_back(std::move(beamSpec));
    
    score->layout();
    
    return score;
}

inline komp::ScoreUPtr getBeamSaveScore1()
{
    komp::BeamSpecIntfUPtrVec beamSpecs;
    return makeBeamSaveScore1(beamSpecs);
}

inline komp::ScoreUPtrVec getBeamSaveScores()
{
    komp::ScoreUPtrVec outScores;
    outScores.emplace_back(getBeamSaveScore0());
    outScores.emplace_back(getBeamSaveScore1());
    return outScores;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
