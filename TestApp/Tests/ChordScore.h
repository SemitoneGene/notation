// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "Chord.h"
#include "BeamFactory.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{
    using namespace komp;
    
    inline ScoreUPtr makeChordScore1()
    {
        auto score = getBasicScore(1);
        auto& reg = score->getItemRegistry();

        TestNoteFactory factory{*score.get()};
        NoteRefVec chordNotes;
        
        ////////////////////////////////////////////////////////////////////////////////////////
        factory.pitch.step = PitchStep::G;
        factory.pitch.octave = 5;
        chordNotes.push_back(factory.insertNote());
        
        factory.pitch.step = PitchStep::E;
        chordNotes.push_back(factory.insertNote());

        factory.pitch.step = PitchStep::C;
        chordNotes.push_back(factory.insertNote());

        factory.advancePosition();
        
        auto& measure = score->getMeasure(0);
        auto& track = score->getTracks().front();
        const MusicLocation loc(*measure.get(), *track.get());
        auto& chord01 = reg.createItem<Chord>(loc, chordNotes);
        chordNotes.clear();
        auto time = chord01.getTime();
        chord01.setIsQuiescent(false);
        score->getMeasure(0)->insertItem(chord01, time);
        
        ////////////////////////////////////////////////////////////////////////////////////////
        
        TemporalRefVec beamNotes;
        Note* note;
        
        factory.temporalType = TemporalType::EIGHTH;
        
        factory.pitch.step = PitchStep::G;
        factory.pitch.octave = 5;
        note = &factory.insertNote();
        beamNotes.push_back(*note);
        chordNotes.push_back(*note);
        
        factory.pitch.step = PitchStep::F;
        factory.pitch.octave = 5;
        note = &factory.insertNote();
        beamNotes.push_back(*note);
        chordNotes.push_back(*note);
        
        factory.pitch.step = PitchStep::E;
        factory.pitch.octave = 5;
        note = &factory.insertNote();
        beamNotes.push_back(*note);
        chordNotes.push_back(*note);
        
        factory.advancePosition();
        
        auto& chord02 = reg.createItem<Chord>(loc, chordNotes);
        chordNotes.clear();
        chord02.setIsQuiescent(false);
        measure->insertItem(chord02, chord02.getTime());
        
        factory.pitch.step = PitchStep::C;
        factory.pitch.octave = 4;
        note = &factory.insertNote();
        beamNotes.push_back(*note);
        chordNotes.push_back(*note);
        
        factory.pitch.step = PitchStep::A;
        factory.pitch.octave = 4;
        note = &factory.insertNote();
        beamNotes.push_back(*note);
        chordNotes.push_back(*note);
        
        factory.pitch.step = PitchStep::B;
        factory.pitch.octave = 4;
        note = &factory.insertNote();
        beamNotes.push_back(*note);
        chordNotes.push_back(*note);
        
        factory.advancePosition();
        
        auto& chord03 = reg.createItem<Chord>(loc, chordNotes);
        chordNotes.clear();
        time = chord03.getTime();
        chord03.setIsQuiescent(false);
        measure->insertItem(chord03, time);
        
        BeamFactory beamFactory;
        beamFactory.autoBeam(reg, beamNotes);
        
        score->layout();
        
        return score;
    }
    
    inline ScoreUPtr makeChordScore2()
    {
        auto score = getBasicScore(1);
        return score;
    }
    
    inline ScoreUPtrVec getChordScores()
    {
        ScoreUPtrVec vec;
        vec.emplace_back(makeChordScore1());
        //vec.emplace_back(makeChordScore2());
        return vec;
    }
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
