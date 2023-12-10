// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "TupletRatio.h"
#include "Tuplet.h"
#include "BeamFactory.h"
#include "Duration.h"
#include "Measure.h"
#include "Note.h"
#include "FileMediatorFactory.h"
#include "TemporalConstants.h"


// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"

namespace
{

    //constexpr const double TOLERANCE = 5.0;

    inline komp::ScoreUPtr makeScore()
    {
        return komp::makeTupletScore1();
    }


    inline komp::TemporalRefVec getTemporals(komp::Measure& inMeasure)
    {
        using namespace komp;

        auto temporals = inMeasure.getTemporals();

        std::ranges::sort(temporals,
                          [&](const Temporal& lhs, const Temporal& rhs) {
            
            if (lhs.getTime() < rhs.getTime()) {
                return true;
            } else if (lhs.getTime() > rhs.getTime()) {
                return false;
            }
            
            if (lhs.getItemTraits().getItemType() == ItemType::Chord) {
                return true;
            } else if (rhs.getItemTraits().getItemType() == ItemType::Chord) {
                return false;
            }
            
            if (lhs.getSlot() < rhs.getSlot()) {
                return true;
            } else if (lhs.getSlot() > rhs.getSlot()) {
                return false;
            }
            
            return &lhs < &rhs;
        });
        
        return temporals;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( T00, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 0;
        const double expectedTime = 0.0;
        const double expectedClocks = 80.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{PitchStep::G, 5, 0};

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

    TEST( T01, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 1;
        const double expectedTime = 80.0;
        const double expectedClocks = 80.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{PitchStep::E, 5, 0};

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

    TEST( T03, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 3;
        const double expectedTime = 240.0;
        const double expectedClocks = 48.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{}; // rest

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

    TEST( T12, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 12;
        const double expectedTime = 560.0;
        const double expectedClocks = 80.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{}; // chord

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

    TEST( T13, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 13;
        const double expectedTime = 560.0;
        const double expectedClocks = 80.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{PitchStep::F, 4, 1};

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

    TEST( T14, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 14;
        const double expectedTime = 560.0;
        const double expectedClocks = 80.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{PitchStep::G, 4, 0};

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

    TEST( T15, PlaybackNote )
    {
        // expected values
        const int temporalIndex = 15;
        const double expectedTime = 560.0;
        const double expectedClocks = 80.0;
        const double expectedSustainClocks = expectedClocks * DEFAULT_DURATION_PERCENTAGE;
        const Pitch expectedPitch{PitchStep::B, 4, 0};

        // get the values
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const PlaybackNote& playbackNote = temporal.getPlaybackNote();
        const auto actualTime = playbackNote.getStartTime();
        const auto actualSustainClocks = playbackNote.getTimeDuration();
        const auto actualPitch = playbackNote.getPitch();

        // assert
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedSustainClocks, actualSustainClocks, 0.000001 );
        CHECK_EQUAL( expectedPitch, actualPitch );
    }
    T_END;

#endif
};

    // Semitone Inc. Confidential Information.
    // TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
    // Reproduction in whole or in part without prior written permission of a
    // duly authorized representative is prohibited.

