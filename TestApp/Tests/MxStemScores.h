// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "FileMediatorFactory.h"
#include "BeamFactory.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{
    using namespace komp;
    
    inline ScoreUPtr makeMxStemTestQuartersScore()
    {
        auto score = getBasicScore(1);
        
        TestNoteFactory factory{*score.get()};
        factory.pitch.step = PitchStep::G;
        factory.pitch.octave = 5;
        
        factory.insertNote().setStemPolicy(StemPolicy{StemDirection::UP, Policy::SPECIFIED});
        factory.advancePosition();
        
        factory.insertNote().setStemPolicy(StemPolicy{StemDirection::UP, Policy::SPECIFIED});
        factory.advancePosition();
        
        factory.insertNote().setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::SPECIFIED});
        factory.advancePosition();
        
        factory.insertNote().setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::SPECIFIED});
        factory.advancePosition();

        score->layout();

        return score;
    }
    
    inline ScoreUPtr
    makeMxStemTestBeamScore()
    {
        auto score = getBasicScore(1);
        auto& reg = score->getItemRegistry();
        
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.pitch.step = PitchStep::G;
        noteFactory.pitch.octave = 5;
        noteFactory.temporalType = TemporalType::EIGHTH;
        
        BeamFactory beamFactory;
        
        auto a = noteFactory.insertNote();
        noteFactory.advancePosition();
        auto b = noteFactory.insertNote();
        noteFactory.advancePosition();
        auto& beam = beamFactory.beamSpan(reg, a, b);
        beam.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        
        a = noteFactory.insertNote();
        noteFactory.advancePosition();
        b = noteFactory.insertNote();
        noteFactory.advancePosition();
        beam = beamFactory.beamSpan(reg, a, b);
        beam.setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        
        a = noteFactory.insertNote();
        noteFactory.advancePosition();
        b = noteFactory.insertNote();
        noteFactory.advancePosition();
        beam = beamFactory.beamSpan(reg, a, b);
        beam.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        
        a = noteFactory.insertNote();
        noteFactory.advancePosition();
        b = noteFactory.insertNote();
        noteFactory.advancePosition();
        beam = beamFactory.beamSpan(reg, a, b);
        beam.setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        
        score->layout();
        
        return score;
    }

    inline ScoreUPtrVec getMxStemScores()
    {
        ScoreUPtrVec vec;
        vec.emplace_back(makeMxStemTestQuartersScore());
        vec.emplace_back(makeMxStemTestBeamScore());
        return vec;
    }
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
