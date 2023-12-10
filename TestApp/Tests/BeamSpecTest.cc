// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test Support
#include "TestSupport/TestFileRepository.h"
#include "BasicScore.h"
#include "TestNoteFactory.h"

// Komp
#include "BeamSpec.h"
#include "BeamedNote.h"
#include "Note.h"

namespace
{
    struct TestItems
    {
        komp::ScoreUPtr score;
        komp::NoteRefVec notes;
        
        TestItems()
        {
            using namespace komp;
            score = komp::getBasicScore(2);
            komp::TestNoteFactory noteFactory{*score.get()};
            noteFactory.temporalType = TemporalType::EIGHTH;
            
            for (int i = 0; i < 8; ++i) {
                noteFactory.pitch.step = static_cast<PitchStep>(i % 7);
                if (i % 7 == 0) {
                    ++noteFactory.pitch.octave;
                }
                notes.push_back(noteFactory.insertNote());
                noteFactory.advancePosition();
            }
            
            for (int i = 7; i >= 0; --i) {
                noteFactory.pitch.step = static_cast<PitchStep>(i % 7);
                if (i % 7 == 0) {
                    --noteFactory.pitch.octave;
                }
                notes.push_back(noteFactory.insertNote());
                noteFactory.advancePosition();
            }
            
            score->layout();
        }
    };
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    
    TEST(checkThatStemYIsNotTotallyBroken, BeamSpec)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(0);
        Note& b = t.notes.at(1);
        BeamSpec beamSpec;
        beamSpec.addBeamedNote(BeamedNote{a});
        beamSpec.addBeamedNote(BeamedNote{b});
        StemPolicy p;
        beamSpec.calculateBeamLines(p);
        const auto& beamedNoteA = beamSpec.getBeamedNote(0);
        const double y1 = beamedNoteA.getStemNoteheadPoint().getScoreY();
        const double y2 = beamedNoteA.getStemBeamsidePoint().getScoreY();
        
        CHECK( y1 != 0.0 );
        CHECK( y2 != 0.0 );
        CHECK( y1 != y2 );
        
        const auto& beamedNoteB = beamSpec.getBeamedNote(1);
        const double beamsideStemYForNoteB = beamedNoteB.getStemBeamsidePoint().getScoreY();
        CHECK( y2 != beamsideStemYForNoteB );
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
