// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// System
#include <sstream>

// Test Support
#include "TestSupport/TestFileRepository.h"
#include "BeamTestScores.h"
#include "BeamSlope.h"
#include "TestHelpers.h"

// Komp
#include "BeamIntf.h"
#include "FileMediatorFactory.h"
#include "TemporalConstants.h"


// Mx
#include "mx/api/DocumentManager.h"

namespace
{
    // TODO: Use font metrics
    static constexpr const double STEM_Y_NUDGE_SPACES = 0.2;

    const komp::StemPolicy AUTOMATIC_POLICY{komp::StemDirection::NONE, komp::Policy::AUTOMATIC};

    inline double upX()
    {
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE2(beamSpecs);
        const auto& beamSpec = beamSpecs.at(0);
        return beamSpec->getBeamedNote(0).getNote().getStemInfo().xUp;
    }
    
    inline double downX()
    {
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE2(beamSpecs);
        const auto& beamSpec = beamSpecs.at(0);
        return beamSpec->getBeamedNote(0).getNote().getStemInfo().xDown;
    }
    
    constexpr const double STEM_Y_NUDGE_SLOTS_ABOVE = STEM_Y_NUDGE_SPACES * 2.0;
    constexpr const double STEM_Y_NUDGE_SLOTS_BELOW = STEM_Y_NUDGE_SPACES * -2.0;
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    // Some tests will fail when improvements are made to the beaming code.  Tests
    // should be updated to reflect the desired values at that time

    TEST( Pivotal_145887973_BeamStemDrawingIssues_1, BeamTest )
    {
        const auto score = TestFileRepository::loadScore("BeamedChordStemDirectionIssue1.xml");
        const auto& measure = score->getMeasure(0);
        const auto notes = measure->items<Note>();
        const auto chords = measure->items<Chord>();
        const Chord& chord = chords.front();
        const auto beams = measure->items<BeamIntf>();
        const BeamIntf& beam = beams.front();
        CHECK( StemDirection::UP == beam.getStemPolicy().direction );
        CHECK( StemDirection::UP == chord.getStemPolicy().direction );

        Note* note = nullptr;

        for (Note& n : notes) {
            if (n.getPitch().step == PitchStep::E) {
                if (n.getPitch().octave == 5) {
                    note = &n;
                }
            }
        }

        KOMP_ASSERT(note);

        note->setSlot(10);
        CHECK( StemDirection::DOWN == beam.getStemPolicy().direction );
        CHECK( StemDirection::DOWN == chord.getStemPolicy().direction );
    }

    TEST( Pivotal_145887973_BeamStemDrawingIssues_2, BeamTest )
    {
        const auto score = TestFileRepository::loadScore("BeamedChordStemDirectionIssue2.xml");
        const auto& measure = score->getMeasure(0);
        const auto notes = measure->items<Note>();
        const auto chords = measure->items<Chord>();
        const Chord& chord = chords.front();
        const auto beams = measure->items<BeamIntf>();
        const BeamIntf& beam = beams.front();
        CHECK( StemDirection::DOWN == beam.getStemPolicy().direction );
        CHECK( StemDirection::DOWN == chord.getStemPolicy().direction );

        Note* note = nullptr;

        for (Note& n : notes) {
            if (n.getPitch().step == PitchStep::C) {
                if (n.getPitch().octave == 5) {
                    note = &n;
                }
            }
        }

        KOMP_ASSERT(note);

        note->setSlot(6);
        CHECK( StemDirection::UP == beam.getStemPolicy().direction );
        CHECK( StemDirection::UP == chord.getStemPolicy().direction );
    }

    TEST( E2_Spec000, BeamTest )
    {
        const auto x = upX();
        
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE2(beamSpecs);
        const auto& beamSpec = beamSpecs.at(0);
        CHECK_EQUAL( 2, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Above == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(7.0, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(7.0, beamLine.getPointB().getYSlots(), 0.1 );
        
        for (const auto& beamedNote : beamSpec->getBeamedNotes()) {
            CHECK( beamedNote->getDoDrawStem() );
            CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
            CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
            CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
            CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
            CHECK_DOUBLES_EQUAL( 7.0, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        }
    }
    
    TEST( E2_Spec066, BeamTest )
    {
        const double x = downX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE2(beamSpecs);
        const auto& beamSpec = beamSpecs.at(66);
        CHECK_EQUAL( 2, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Below == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-7.0, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-23.0, beamLine.getPointB().getYSlots(), 0.1 );

        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -8.0567, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -24.008, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
    }
    
    TEST( E3_Spec017, BeamTest )
    {
        const double x = upX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE3(beamSpecs);
        const auto& beamSpec = beamSpecs.at(17);
        CHECK_EQUAL( 3, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Above == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(7.0, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(7.0, beamLine.getPointB().getYSlots(), 0.1 );
        
        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 7.0, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 7.0, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(2);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 7.0, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
    }
    
    TEST( E3_Spec006, BeamTest )
    {
        const double x = downX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE3(beamSpecs);
        const auto& beamSpec = beamSpecs.at(6);
        CHECK_EQUAL( 3, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Below == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-7.0, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-23.0, beamLine.getPointB().getYSlots(), 0.1 );
        
        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -8.01, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -25.4929, beamedNote->getStemBeamsidePoint().getYSlots(), 0.2 );
        
        beamedNote = &beamSpec->getBeamedNote(2);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -24.010, beamedNote->getStemBeamsidePoint().getYSlots(), 0.2 );
    }
    
    TEST( E4_Spec032, BeamTest )
    {
        const double x = upX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE4(beamSpecs);
        const auto& beamSpec = beamSpecs.at(32);
        CHECK_EQUAL( 4, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Above == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(19.747, beamLine.getPointA().getYSlots(), 0.2 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL(20.497, beamLine.getPointB().getYSlots(), 0.2 );
        
        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( 19.6352, beamedNote->getStemBeamsidePoint().getYSlots(), 0.2 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( 4.20395, beamedNote->getStemBeamsidePoint().getYSlots(), 0.2 );
        
        beamedNote = &beamSpec->getBeamedNote(2);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( 20.8118, beamedNote->getStemBeamsidePoint().getYSlots(), 0.2 );
        
        beamedNote = &beamSpec->getBeamedNote(3);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.2 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.2 );
        CHECK_DOUBLES_EQUAL( 20.3805, beamedNote->getStemBeamsidePoint().getYSlots(), 0.2 );
    }
    
    TEST( E4_Spec009, BeamTest )
    {
        const double x = downX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE4(beamSpecs);
        const auto& beamSpec = beamSpecs.at(9);
        CHECK_EQUAL( 4, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Below == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-7.0, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-25.75, beamLine.getPointB().getYSlots(), 0.1 );
        
        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -8.0035, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -12.4842, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(2);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -24.066, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(3);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -26.75, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
    }
    
    TEST( E5_Spec012, BeamTest )
    {
        const double x = upX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE5(beamSpecs);
        const auto& beamSpec = beamSpecs.at(12);
        CHECK_EQUAL( 5, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Above == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(12.327, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(9.577, beamLine.getPointB().getYSlots(), 0.1 );
        
        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 12.309, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 4.51, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(2);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 4.816, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(3);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( 12.242, beamedNote->getStemBeamsidePoint().getYSlots(), 0.3 );
        
        beamedNote = &beamSpec->getBeamedNote(4);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.3 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_ABOVE, beamedNote->getStemNoteheadPoint().getYSlots(), 0.3 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.3 );
        CHECK_DOUBLES_EQUAL( 9.559, beamedNote->getStemBeamsidePoint().getYSlots(), 0.3 );
    }
    
    TEST( E5_Spec003, BeamTest )
    {
        const double x = downX();
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeE5(beamSpecs);
        const auto& beamSpec = beamSpecs.at(3);
        CHECK_EQUAL( 5, beamSpec->getBeamedNoteCount() );
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( komp::BeamPlacement::Below == beamSpec->getBeamPlacement() );
        CHECK_EQUAL( 1, beamSpec->getBeamLineCount());
        
        const auto& beamLine = beamSpec->getBeamLine(0);
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointA().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-7.0, beamLine.getPointA().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL(x, beamLine.getPointB().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL(-28.75, beamLine.getPointB().getYSlots(), 0.1 );
        
        auto beamedNote = &beamSpec->getBeamedNote(0);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -7.91719, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(1);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -17.5937, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(2);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -26.275, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(3);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -25.9761, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
        
        beamedNote = &beamSpec->getBeamedNote(4);
        CHECK( beamedNote->getDoDrawStem() );
        CHECK_DOUBLES_EQUAL( beamedNote->getNote().getStemInfo().thickness, beamedNote->getStemThickness(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemNoteheadPoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( STEM_Y_NUDGE_SLOTS_BELOW, beamedNote->getStemNoteheadPoint().getYSlots(), 0.1 );
        CHECK_DOUBLES_EQUAL( x, beamedNote->getStemBeamsidePoint().getX(), 0.1 );
        CHECK_DOUBLES_EQUAL( -29.65271, beamedNote->getStemBeamsidePoint().getYSlots(), 0.1 );
    }
    
    TEST( S4_Spec00, BeamTest )
    {
        komp::BeamSpecIntfUPtrVec beamSpecs;
        komp::makeS4(beamSpecs);
        const auto& beamSpec = beamSpecs.at(0);
        auto localPolicy = AUTOMATIC_POLICY;
        beamSpec->calculateBeamLines(localPolicy);
        CHECK( beamSpec != nullptr );
    }
    
    TEST( BeamChunkSaving_M00_N00, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 0;
        int expectedBeamCount = 2;
        
        using namespace komp;
        
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::forwardBroken;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N01, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 1;
        int expectedBeamCount = 1;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N02, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 2;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::backwardBroken;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N03, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 3;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::forwardBroken;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N04, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 4;
        int expectedBeamCount = 1;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N05, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 5;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N06, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 6;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N07, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 7;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N08, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 8;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N09, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 9;
        int expectedBeamCount = 1;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N10, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 10;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M00_N11, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 11;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N00, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 0;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N01, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 1;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::extend;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N02, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 2;
        int expectedBeamCount = 1;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N03, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 3;
        int expectedBeamCount = 1;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N04, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 4;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::backwardBroken;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N05, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 5;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::begin;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( BeamChunkSaving_M01_N06, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 6;
        int expectedBeamCount = 2;
        
        using namespace komp;
        komp::BeamSpecIntfUPtrVec beamSpecs;
        const auto score = komp::make16thFigures(beamSpecs);

        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::istringstream iss{oss.str()};
        auto& docMgr = mx::api::DocumentManager::getInstance();
        int docId = docMgr.createFromStream(iss);
        auto mxScoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        
        const auto& measure = mxScoreData.parts.at(0).measures.at(measureIndex);
        const auto& notes = measure.staves.at(0).voices.at(0).notes;
        const auto& note = notes.at(noteIndexInMeasure);
        const auto& beams = note.beams;
        CHECK_EQUAL(expectedBeamCount, beams.size());
        
        int beamIndex = -1;
        mx::api::Beam expected = mx::api::Beam::unspecified;
        mx::api::Beam actual = mx::api::Beam::unspecified;
        
        beamIndex = 0;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = mx::api::Beam::end;
        actual = beams.at(beamIndex);
        CHECK(expected == actual);
    }

const std::string beamChunkXml = R"(
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<!DOCTYPE score-partwise PUBLIC "-//Recordare//DTD MusicXML 3.0 Partwise//EN" "http://www.musicxml.org/dtds/partwise.dtd">
<score-partwise>
  <identification>
    <encoding>
      <software>komp</software>
      <supports type="yes" element="accidental" />
      <supports type="yes" element="beam" />
    </encoding>
    <miscellaneous>
      <miscellaneous-field name="komp-version-major">0</miscellaneous-field>
      <miscellaneous-field name="komp-version-minor">1</miscellaneous-field>
      <miscellaneous-field name="komp-version-patch">0</miscellaneous-field>
    </miscellaneous>
  </identification>
  <defaults>
    <scaling>
      <millimeters>8.46667003632</millimeters>
      <tenths>40</tenths>
    </scaling>
    <page-layout>
      <page-height>1300</page-height>
      <page-width>1000</page-width>
      <page-margins type="both">
        <left-margin>30</left-margin>
        <right-margin>60</right-margin>
        <top-margin>120</top-margin>
        <bottom-margin>6.67</bottom-margin>
      </page-margins>
    </page-layout>
  </defaults>
  <part-list>
    <score-part id="P1">
      <part-name print-object="no">Beam16thFigures</part-name>
    </score-part>
  </part-list>
  <part id="P1">
    <measure number="1">
      <attributes>
        <divisions>240</divisions>
        <time>
          <beats>4</beats>
          <beat-type>4</beat-type>
        </time>
        <clef>
          <sign>G</sign>
          <line>2</line>
        </clef>
      </attributes>
      <note>
        <pitch>
          <step>F</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">forward hook</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>120</duration>
        <voice>1</voice>
        <type>eighth</type>
        <beam number="1">continue</beam>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
      </note>
      <note>
        <pitch>
          <step>F</step>
          <octave>5</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">forward hook</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>180</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot />
        <beam number="1">end</beam>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>5</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">continue</beam>
        <beam number="2">continue</beam>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">continue</beam>
        <beam number="2">continue</beam>
      </note>
      <note>
        <pitch>
          <step>G</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
      </note>
      <note>
        <pitch>
          <step>A</step>
          <octave>5</octave>
        </pitch>
        <duration>120</duration>
        <voice>1</voice>
        <type>eighth</type>
        <beam number="1">begin</beam>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <octave>5</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">continue</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
      </note>
    </measure>
    <measure number="2">
      <note>
        <pitch>
          <step>B</step>
          <octave>5</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>C</step>
          <octave>6</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">continue</beam>
        <beam number="2">end</beam>
      </note>
      <note>
        <pitch>
          <step>D</step>
          <octave>6</octave>
        </pitch>
        <duration>120</duration>
        <voice>1</voice>
        <type>eighth</type>
        <beam number="1">end</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>6</octave>
        </pitch>
        <duration>180</duration>
        <voice>1</voice>
        <type>eighth</type>
        <dot />
        <beam number="1">begin</beam>
      </note>
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">backward hook</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>6</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>F</step>
          <octave>6</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
      </note>
      <note>
        <pitch>
          <step>F</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>6</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>F</step>
          <octave>6</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
      </note>
      <note>
        <pitch>
          <step>F</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">begin</beam>
        <beam number="2">begin</beam>
      </note>
      <note>
        <pitch>
          <step>E</step>
          <octave>4</octave>
        </pitch>
        <duration>60</duration>
        <voice>1</voice>
        <type>16th</type>
        <beam number="1">end</beam>
        <beam number="2">end</beam>
      </note>
    </measure>
  </part>
</score-partwise>
)";

    TEST( LoadBeamChunks_M00_N00, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 0;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote = bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = -1;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        beamIndex = 0;
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = BeamChunk::BrokenRight;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N01, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 1;
        int expectedBeamCount = 1;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = -1;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        beamIndex = 0;
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N02, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 2;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = -1;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        beamIndex = 0;
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = BeamChunk::BrokenLeft;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N03, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 3;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = -1;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        beamIndex = 0;
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
        
        beamIndex = 1;
        expected = BeamChunk::BrokenRight;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N04, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 4;
        int expectedBeamCount = 1;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = -1;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        beamIndex = 0;
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N05, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 5;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N06, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 6;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N07, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 7;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N08, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 8;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N09, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 9;
        int expectedBeamCount = 1;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N10, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 10;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M00_N11, BeamTest )
    {
        int measureIndex = 0;
        int noteIndexInMeasure = 11;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N00, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 0;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N01, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 1;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Continue;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N02, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 2;
        int expectedBeamCount = 1;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N03, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 3;
        int expectedBeamCount = 1;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N04, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 4;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::BrokenLeft;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N05, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 5;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::Begin;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    TEST( LoadBeamChunks_M01_N06, BeamTest )
    {
        int measureIndex = 1;
        int noteIndexInMeasure = 6;
        int expectedBeamCount = 2;
        
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{beamChunkXml};
        auto score = fileMediator->loadStream(iss);
        auto& measure = score->getMeasure(measureIndex);
        auto notes = measure->items<Note>(*score->getTracks().at(0).get());
        Note& note = notes.at(noteIndexInMeasure);
        auto beam = note.getBeam();
        CHECK(beam != nullptr);
        BeamedNoteIntf* beamedNote = nullptr;
        for (const auto& bn : beam->getBeamedNotes()) {
            if (bn->getNote().getItemID() == note.getItemID()) {
                beamedNote =bn.get();
            }
        }
        CHECK(beamedNote != nullptr);
        const auto& beamChunks = beamedNote->getBeamChunks();
        CHECK_EQUAL(expectedBeamCount, beamChunks.size());
        
        int beamIndex = 0;
        BeamChunk expected = BeamChunk::Unknown;
        BeamChunk actual = BeamChunk::Unknown;
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
        
        expected = BeamChunk::End;
        actual = beamChunks.at(beamIndex++);
        CHECK(expected == actual);
    }
    
    
    TEST( SixteenthBeamPlacement_S00_BN00, BeamTest )
    {
        int beamSpecIndex = 0;
        int beamedNoteIndex = 0;
        int expectedBeamCount = 2;
        bool isStemsUp = true;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);
        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.2);
        CHECK_DOUBLES_EQUAL(9.26472, beamLineP->getPointA().getYSlots(), 0.2);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(6.264722, beamLineP->getPointB().getYSlots(), 0.2);

        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.2);
        CHECK_DOUBLES_EQUAL(7.76472, beamLineP->getPointA().getYSlots(), 0.2);
        CHECK_DOUBLES_EQUAL(stemX + FRACTIONAL_BEAM_LENGTH, beamLineP->getPointB().getX(), 0.2);
        CHECK_DOUBLES_EQUAL(8.014722, beamLineP->getPointB().getYSlots(), 0.2);
    }
    
    
    TEST( SixteenthBeamPlacement_S03_BN01, BeamTest )
    {
        int beamSpecIndex = 3;
        int beamedNoteIndex = 1;
        int expectedBeamCount = 2;
        bool isStemsUp = false;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);

        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-16.0, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-6.0, beamLineP->getPointB().getYSlots(), 0.01);
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-10.0694, beamLineP->getPointA().getYSlots(), 0.2);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.2);
        CHECK_DOUBLES_EQUAL(-4.5138, beamLineP->getPointB().getYSlots(), 0.2);
    }
    
    
    TEST( SixteenthBeamPlacement_S05_BN01, BeamTest )
    {
        int beamSpecIndex = 5;
        int beamedNoteIndex = 1;
        int expectedBeamCount = 2;
        bool isStemsUp = false;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);

        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-15.0, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-6.0, beamLineP->getPointB().getYSlots(), 0.01);
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX - FRACTIONAL_BEAM_LENGTH, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-4.145833, beamLineP->getPointA().getYSlots(), 0.2);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.2);
        CHECK_DOUBLES_EQUAL(-4.5197, beamLineP->getPointB().getYSlots(), 0.2);
    }
    
    
    TEST( SixteenthBeamPlacement_S06_BN00, BeamTest )
    {
        int beamSpecIndex = 6;
        int beamedNoteIndex = 0;
        int expectedBeamCount = 2;
        bool isStemsUp = false;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);

        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-7, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-7, beamLineP->getPointB().getYSlots(), 0.01);
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-5.5000, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-5.5000, beamLineP->getPointB().getYSlots(), 0.01);
    }
    
    
    TEST( SixteenthBeamPlacement_S06_BN01, BeamTest )
    {
        int beamSpecIndex = 6;
        int beamedNoteIndex = 1;
        int expectedBeamCount = 2;
        bool isStemsUp = false;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);

        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-7, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-7, beamLineP->getPointB().getYSlots(), 0.01);
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-5.5000, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(-5.5000, beamLineP->getPointB().getYSlots(), 0.01);
    }
    
    
    TEST( SixteenthBeamPlacement_S07_BN00, BeamTest )
    {
        int beamSpecIndex = 7;
        int beamedNoteIndex = 0;
        int expectedBeamCount = 2;
        bool isStemsUp = true;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);

        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(7, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(7, beamLineP->getPointB().getYSlots(), 0.01);
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(5.50276, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(5.5000, beamLineP->getPointB().getYSlots(), 0.01);
    }
    
    
    TEST( SixteenthBeamPlacement_S07_BN01, BeamTest )
    {
        int beamSpecIndex = 7;
        int beamedNoteIndex = 1;
        int expectedBeamCount = 2;
        bool isStemsUp = true;
        
        using namespace komp;
        const double stemX = isStemsUp ? upX() : downX();
        const BeamPlacement expectedPlacement = isStemsUp ? BeamPlacement::Above : BeamPlacement::Below;
        komp::BeamSpecIntfUPtrVec specs;
        const auto score = komp::make16thFigures(specs);

        const auto& spec = specs.at(beamSpecIndex);
        auto localPolicy = AUTOMATIC_POLICY;
        spec->calculateBeamLines(localPolicy);
        const auto& beamedNote = spec->getBeamedNote(beamedNoteIndex);
        
        CHECK_EQUAL(expectedBeamCount, beamedNote.getCount());
        CHECK(expectedPlacement == spec->getBeamPlacement());
        
        BeamLineIntf* beamLineP = nullptr;
        int beamLineIndex = 0;
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(7, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(7, beamLineP->getPointB().getYSlots(), 0.01);
        
        beamLineP = &spec->getBeamLine(beamLineIndex++);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointA().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(5.50276, beamLineP->getPointA().getYSlots(), 0.01);
        CHECK_DOUBLES_EQUAL(stemX, beamLineP->getPointB().getX(), 0.01);
        CHECK_DOUBLES_EQUAL(5.5000, beamLineP->getPointB().getYSlots(), 0.01);
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
