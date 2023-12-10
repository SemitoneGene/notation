// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// System
#include <sstream>

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

// Komp
#include "RelativePoint.h"
#include "Staff.h"

namespace
{
    inline komp::ScoreUPtr createOneNoteScore()
    {
        using namespace komp;
        auto score = komp::getBasicScore();
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.pitch.step = PitchStep::G;
        noteFactory.insertNote();
        score->layout();
        return score;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( SimpleConstructor, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        RelativePoint relativePoint{note};
        CHECK_DOUBLES_EQUAL( 0.0, relativePoint.getYSlots(), 0.01 );
    }

    TEST( ExplicitConstructor, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        RelativePoint relativePoint{note, 1.0, 2.0};
        CHECK_DOUBLES_EQUAL( 1.0, relativePoint.getX(), 0.01 );
        CHECK_DOUBLES_EQUAL( 2.0, relativePoint.getYSlots(), 0.01 );
    }
    
    TEST( GetSet, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        RelativePoint relativePoint{note, 1.0, 2.0};
        relativePoint.setX(10.0);
        relativePoint.setYSlots(11.0);
        CHECK_DOUBLES_EQUAL( 10.0, relativePoint.getX(), 0.01 );
        CHECK_DOUBLES_EQUAL( 11.0, relativePoint.getYSlots(), 0.01 );
    }
    
    TEST( GetYPixels, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        RelativePoint relativePoint{note, 3.0, 4.0};
        double spacing = Staff::getSpacing();
        double slotSize = spacing / 2.0;
        // slots go up, pixels go down, so flip the sign
        double expected = -1 * slotSize * relativePoint.getYSlots();
        CHECK_DOUBLES_EQUAL( expected, relativePoint.getYPixels(), 0.01 );
    }
    
    TEST( GetScoreX, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        RelativePoint relativePoint{note, 3.0, 4.0};
        double dataX = note.getLocation().getMeasure().getDataX();
        double expected = dataX + note.getX() + relativePoint.getX();
        CHECK_DOUBLES_EQUAL( expected, relativePoint.getScoreX(), 0.01 );
    }
    
    TEST( GetScoreY, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        const double thisTestsSlotValue = 1.0;
        RelativePoint relativePoint{note, 3.0, thisTestsSlotValue};
        const auto& staff = note.getLocation().getStaff();
        const double spacing = Staff::getSpacing();
        const double slotSize = spacing / 2.0;
        const double pixleDistanceFromNote = slotSize * thisTestsSlotValue;
        const double noteYPosition = staff.slotToPixel(note.getSlot());
        const double expected = noteYPosition - pixleDistanceFromNote;
        double actual = relativePoint.getScoreY();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    TEST( GetScoreYNeg, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        const double thisTestsSlotValue = -10.0;
        RelativePoint relativePoint{note, 3.0, thisTestsSlotValue};
        const auto& staff = note.getLocation().getStaff();
        const double spacing = Staff::getSpacing();
        const double slotSize = spacing / 2.0;
        const double pixleDistanceFromNote = slotSize * thisTestsSlotValue;
        const double noteYPosition = staff.slotToPixel(note.getSlot());
        const double expected = noteYPosition - pixleDistanceFromNote;
        double actual = relativePoint.getScoreY();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    TEST( GetScoreYZero, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        const double thisTestsSlotValue = 0.0;
        RelativePoint relativePoint{note, 3.0, thisTestsSlotValue};
        const auto& staff = note.getLocation().getStaff();
        const double spacing = Staff::getSpacing();
        const double slotSize = spacing / 2.0;
        const double pixleDistanceFromNote = slotSize * thisTestsSlotValue;
        const double noteYPosition = staff.slotToPixel(note.getSlot());
        const double expected = noteYPosition - pixleDistanceFromNote;
        double actual = relativePoint.getScoreY();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    TEST( GetScoreYFractional, RelativePoint )
    {
        using namespace komp;
        auto score = createOneNoteScore();
        auto& measure = score->getMeasuresFront();
        Note& note = measure->items<Note>().front();
        const double thisTestsSlotValue = 7.123;
        RelativePoint relativePoint{note, 3.0, thisTestsSlotValue};
        const auto& staff = note.getLocation().getStaff();
        const double spacing = Staff::getSpacing();
        const double slotSize = spacing / 2.0;
        const double pixleDistanceFromNote = slotSize * thisTestsSlotValue;
        const double noteYPosition = staff.slotToPixel(note.getSlot());
        const double expected = noteYPosition - pixleDistanceFromNote;
        double actual = relativePoint.getScoreY();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
