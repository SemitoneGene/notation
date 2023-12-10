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
#include "MxTupletSpec.h"
#include "Note.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace
{

    static const std::string fileName{"Lord Chord.xml"};

    const komp::ScoreUPtr getScore()
    {
       return komp::TestFileRepository::loadScore(fileName);
    }

    komp::ScoreUPtr getRoundTrippedScore()
    {
        using namespace komp;
        const auto original = TestFileRepository::loadScore(fileName);
        const auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*original.get()};
        fileMediator->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        return fileMediator->loadStream(iss);
    }

}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( LoadBeam_0_0, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_0_1, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 1;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_0_2, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 2;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_0_3, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 3;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_0_4, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 4;
        const auto temporalType = TemporalType::QUARTER;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_0_5, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 5;
        const auto temporalType = TemporalType::QUARTER;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_1_0, LordChord )
    {
        const int measureIndex = 1;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::HALF;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_1_1, LordChord )
    {
        const int measureIndex = 1;
        const int chordIndex = 1;
        const auto temporalType = TemporalType::HALF;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_3_6, LordChord )
    {
        const int measureIndex = 3;
        const int chordIndex = 6;
        const auto temporalType = TemporalType::SIXTEENTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_3_7, LordChord )
    {
        const int measureIndex = 3;
        const int chordIndex = 7;
        const auto temporalType = TemporalType::SIXTEENTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_3_8, LordChord )
    {
        const int measureIndex = 3;
        const int chordIndex = 8;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_5_0, LordChord )
    {
        const int measureIndex = 5;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::WHOLE;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = false;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( LoadBeam_6_0, LordChord )
    {
        const int measureIndex = 6;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::WHOLE;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = false;
        const bool expectedHasFlag = false;

        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    ////////////////////////////////////////////////////////////////////////////

    TEST( RoundTripBeam_0_0, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_0_1, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 1;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_0_2, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 2;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_0_3, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 3;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_0_4, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 4;
        const auto temporalType = TemporalType::QUARTER;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_0_5, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 5;
        const auto temporalType = TemporalType::QUARTER;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_1_0, LordChord )
    {
        const int measureIndex = 1;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::HALF;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_1_1, LordChord )
    {
        const int measureIndex = 1;
        const int chordIndex = 1;
        const auto temporalType = TemporalType::HALF;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_3_6, LordChord )
    {
        const int measureIndex = 3;
        const int chordIndex = 6;
        const auto temporalType = TemporalType::SIXTEENTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_3_7, LordChord )
    {
        const int measureIndex = 3;
        const int chordIndex = 7;
        const auto temporalType = TemporalType::SIXTEENTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_3_8, LordChord )
    {
        const int measureIndex = 3;
        const int chordIndex = 8;
        const auto temporalType = TemporalType::EIGHTH;
        const bool expectedHasBeam = true;
        const bool expectedHasStem = true;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_5_0, LordChord )
    {
        const int measureIndex = 5;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::WHOLE;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = false;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );

        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    TEST( RoundTripBeam_6_0, LordChord )
    {
        const int measureIndex = 6;
        const int chordIndex = 0;
        const auto temporalType = TemporalType::WHOLE;
        const bool expectedHasBeam = false;
        const bool expectedHasStem = false;
        const bool expectedHasFlag = false;

        const auto score = getRoundTrippedScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        CHECK( chord.getDuration().getTemporalType() == temporalType);
        CHECK( expectedHasBeam == chord.getIsBeamed() );
        CHECK( expectedHasStem == !chord.getFrames().stem.getIsEmpty() );
        CHECK( expectedHasFlag == !chord.getFrames().flags.getIsEmpty() );
        
        for (const Note& note : chord.getNotes()) {
            CHECK( expectedHasFlag == !note.getFrames().flags.getIsEmpty() );
        }
    }
    T_END;

    ////////////////////////////////////////////////////////////////////////////

    TEST( ChordFrameBottom_0_0, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 0;
        const int lowestNoteSlot = 2;
        const int expectedFrameBottomSlot = lowestNoteSlot - 1;
        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        const auto frame = chord.getFrame();
        const auto expected = chord.getLocation().getStaff().slotToPixel(expectedFrameBottomSlot);
        const auto actual = frame.bottom;
        CHECK_DOUBLES_EQUAL(expected, actual, 0.5);
    }
    T_END;

    TEST( ChordFrameBottom_0_1, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 1;
        const int lowestNoteSlot = -1;
        const int expectedFrameBottomSlot = lowestNoteSlot - 1;
        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        const auto frame = chord.getFrame();
        const auto expected = chord.getLocation().getStaff().slotToPixel(expectedFrameBottomSlot);
        const auto actual = frame.bottom;
        CHECK_DOUBLES_EQUAL(expected, actual, 0.5);
    }
    T_END;

    TEST( ChordFrameBottom_0_4, LordChord )
    {
        const int measureIndex = 0;
        const int chordIndex = 4;
        const int lowestNoteSlot = 2;
        const int expectedFrameBottomSlot = lowestNoteSlot - 1;
        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        const auto frame = chord.getFrame();
        const auto expected = chord.getLocation().getStaff().slotToPixel(expectedFrameBottomSlot);
        const auto actual = frame.bottom;
        CHECK_DOUBLES_EQUAL(expected, actual, 0.5);
    }
    T_END;

    TEST( ChordFrameBottom_1_1, LordChord )
    {
        const int measureIndex = 1;
        const int chordIndex = 1;
        const int lowestNoteSlot = 0;
        const int expectedFrameBottomSlot = lowestNoteSlot - 1;
        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        const auto frame = chord.getFrame();
        const auto expected = chord.getLocation().getStaff().slotToPixel(expectedFrameBottomSlot);
        const auto actual = frame.bottom;
        CHECK_DOUBLES_EQUAL(expected, actual, 0.5);
    }
    T_END;

    TEST( ChordFrameBottom_4_3, LordChord )
    {
        const int measureIndex = 4;
        const int chordIndex = 3;
        const auto& score = getScore();
        const auto chords = getItems<Chord>(*score.get(), measureIndex);
        const Chord& chord = chords.at(chordIndex);
        const auto frame = chord.getFrame();
        const auto bottomNote = chord.getBottomNote();
        const auto bottomNoteStem = chord.getBeam()->getStemFrame(chord);
        const auto expected = bottomNoteStem.bottom;
        const auto actual = frame.bottom;
        CHECK_DOUBLES_EQUAL(expected, actual, 0.5);
    }
    T_END;

#endif
};

    // Semitone Inc. Confidential Information.
    // TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
    // Reproduction in whole or in part without prior written permission of a
    // duly authorized representative is prohibited.

