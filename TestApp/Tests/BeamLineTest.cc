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
#include "BeamLine.h"
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
            komp::TestNoteFactory noteFactory{*score};
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
    TEST(constructorTemporalAssignments, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(0);
        Note& b = t.notes.at(1);
        BeamLine beamLine{a, b};
        CHECK_EQUAL( &a, &beamLine.getNoteA() );
        CHECK_EQUAL( &b, &beamLine.getNoteB() );
    }
    
    TEST(clone, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(0);
        Note& b = t.notes.at(1);
        BeamLine beamLine{a, b};
        BeamLineIntfUPtr clonedBeamLine = beamLine.clone();
        CHECK( &beamLine != clonedBeamLine.get() );
        CHECK_EQUAL( &beamLine.getNoteA(), &clonedBeamLine->getNoteA() );
        CHECK_EQUAL( &beamLine.getNoteB(), &clonedBeamLine->getNoteB() );
        CHECK_DOUBLES_EQUAL( beamLine.getPointA().getYSlots(), clonedBeamLine->getPointA().getYSlots(), 0.01 );
    }
    
    
    TEST(getSlope01, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(0);
        Note& b = t.notes.at(1);
        BeamLine beamLine{a, b};
        const auto slotSizeA = Staff::getSlotSize();
        const auto slotSizeB = Staff::getSlotSize();
        double aYSlots = 1.0;
        double bYSlots = -1.0;
        beamLine.getPointA().setYSlots(aYSlots);
        beamLine.getPointB().setYSlots(bYSlots);
        const double aX = a.getScoreX();
        const double bX = b.getScoreX();
        const double aY = a.getScoreY() - (slotSizeA * aYSlots);
        const double bY = b.getScoreY() - (slotSizeB * bYSlots);
        const double deltaX = bX - aX;
        const double deltaY = bY - aY;
        CHECK( deltaX != 0.0 );
        const double expected = deltaY / deltaX;
        const double actual = beamLine.getSlope();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    
    TEST(getSlope02, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(0);
        Note& b = t.notes.at(1);
        BeamLine beamLine{a, b};
        const auto slotSizeA = Staff::getSlotSize();
        const auto slotSizeB = Staff::getSlotSize();
        double aYSlots = 100.0;
        double bYSlots = -100.0;
        beamLine.getPointA().setYSlots(aYSlots);
        beamLine.getPointB().setYSlots(bYSlots);
        const double aX = a.getScoreX();
        const double bX = b.getScoreX();
        const double aY = a.getScoreY() - (slotSizeA * aYSlots);
        const double bY = b.getScoreY() - (slotSizeB * bYSlots);
        const double deltaX = bX - aX;
        const double deltaY = bY - aY;
        CHECK( deltaX != 0.0 );
        const double expected = deltaY / deltaX;
        const double actual = beamLine.getSlope();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    
    TEST(getSlope03, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(0);
        Note& b = t.notes.at(1);
        BeamLine beamLine{a, b};
        const auto slotSizeA = Staff::getSlotSize();
        const auto slotSizeB = Staff::getSlotSize();
        double aYSlots = -100.0;
        double bYSlots = 100.0;
        beamLine.getPointA().setYSlots(aYSlots);
        beamLine.getPointB().setYSlots(bYSlots);
        const double aX = a.getScoreX();
        const double bX = b.getScoreX();
        const double aY = a.getScoreY() - (slotSizeA * aYSlots);
        const double bY = b.getScoreY() - (slotSizeB * bYSlots);
        const double deltaX = bX - aX;
        const double deltaY = bY - aY;
        CHECK( deltaX != 0.0 );
        const double expected = deltaY / deltaX;
        const double actual = beamLine.getSlope();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    
    TEST(xOffset, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(3);
        Note& b = t.notes.at(14);
        BeamLine beamLine{a, b};
        const double aXOffset = 1.0;
        const double bXOffset = -2.0;
        beamLine.getPointA().setX( aXOffset );
        beamLine.getPointB().setX( bXOffset );
        const double aXPosition = a.getScoreX();
        const double bXPosition = b.getScoreX();
        double expected = aXPosition + aXOffset;
        double actual = beamLine.getPointA().getScoreX();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
        expected = bXPosition + bXOffset;
        actual = beamLine.getPointB().getScoreX();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }
    
    
    TEST(yOffset, BeamLine)
    {
        using namespace komp;
        TestItems t;
        Note& a = t.notes.at(6);
        Note& b = t.notes.at(7);
        BeamLine beamLine{a, b};
        const double yOffsetA = 10.1;
        const double yOffsetB = -3.3;
        beamLine.getPointA().setYSlots( yOffsetA );
        beamLine.getPointB().setYSlots( yOffsetB );
        const double slotSizeA = Staff::getSlotSize();
        const double slotSizeB = Staff::getSlotSize();
        double expected = a.getScoreY() - (slotSizeA * yOffsetA);
        double actual = beamLine.getPointA().getScoreY();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
        expected = b.getScoreY() - (slotSizeB * yOffsetB);
        actual = beamLine.getPointB().getScoreY();
        CHECK_DOUBLES_EQUAL( expected, actual, 0.01 );
    }

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
