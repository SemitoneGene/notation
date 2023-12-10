// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "Mark.h"

// Test Support
#include "MxWedgeScore.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS    

    TEST( WaltzWedgeLoad, MxWedgeTest )
    {
        const auto score = TestFileRepository::loadScore("WaltzWedge.xml");
        const int measureIndex = 23;
        const int trackIndex = 3;
        const int handIndex = 0;
        const int voiceIndex = 0;
        const auto wedges = getItems<Wedge>(*score.get(), measureIndex, trackIndex, handIndex, voiceIndex);
        CHECK_EQUAL( 1, wedges.size() );
    }
    T_END;

    TEST( Assumptions, MxWedgeTest )
    {
        MxWedgeScore s;
        auto wedge = s.getWedge();
        CHECK_EQUAL(MxWedgeScore::slot, wedge.getSlot());
        CHECK_DOUBLES_EQUAL(static_cast<double>(MxWedgeScore::startTime), wedge.getTime(), 0.0001);
        CHECK_DOUBLES_EQUAL(static_cast<double>(MxWedgeScore::endTime), wedge.getEndTime(), 0.0001);
    }
    T_END;

    TEST( XmlRoundTrip, MxWords )
    {
        MxWedgeScore beforeObject;
        auto& beforeScore = beforeObject.getScore();
        auto before = beforeObject.getWedge();
        auto afterObject = beforeObject.roundTrip();
        auto after = afterObject.getWedge();
        auto& afterScore = afterObject.getScore();
        CHECK_EQUAL(MxWedgeScore::slot, after.getSlot());
        CHECK_DOUBLES_EQUAL(static_cast<double>(MxWedgeScore::startTime), after.getTime(), 0.0001);
        CHECK_DOUBLES_EQUAL(static_cast<double>(MxWedgeScore::endTime), after.getEndTime(), 0.0001);

        CHECK_DOUBLES_EQUAL(before.getX(), after.getX(), 0.0001);
        CHECK_EQUAL(before.getSlot(), after.getSlot());
        CHECK_DOUBLES_EQUAL(before.getScoreX(), after.getScoreX(), 0.0001);

        CHECK(WedgeType::DIMINUENDO == after.getWedgeType());

        // The stuff below is not related to Wedge Placement but deals with larger round trip bug regression

        auto beforeNotes = beforeScore->getMeasure(0)->items<Note>(*beforeScore->getTracks().front().get());
        auto afterNotes = afterScore->getMeasure(0)->items<Note>(*afterScore->getTracks().front().get());
        CHECK(afterNotes.size()>0);

        Note& beforeNote = beforeNotes.front();
        Note& afterNote = afterNotes.front();

        const auto beforeNoteX = beforeNote.getScoreX();
        const auto afterNoteX = afterNote.getScoreX();

        CHECK_DOUBLES_EQUAL(beforeNoteX, afterNoteX, 0.0001);

        auto beforeMeasureGap = beforeScore->getMeasure(0)->getGap();
        auto afterMeasureGap = afterScore->getMeasure(0)->getGap();

        CHECK_DOUBLES_EQUAL(beforeMeasureGap, afterMeasureGap, 0.0001);
    }
    T_END;
    
    TEST( XmlRoundTrip, MxWedgeTest )
    {
        MxWedgeScore beforeObject;
        auto& beforeScore = beforeObject.getScore();
        auto before = beforeObject.getWedge();
        auto afterObject = beforeObject.roundTrip();
        auto after = afterObject.getWedge();
        auto& afterScore = afterObject.getScore();
        CHECK_EQUAL(MxWedgeScore::slot, after.getSlot());
        CHECK_DOUBLES_EQUAL(static_cast<double>(MxWedgeScore::startTime), after.getTime(), 0.0001);
        CHECK_DOUBLES_EQUAL(static_cast<double>(MxWedgeScore::endTime), after.getEndTime(), 0.0001);

        CHECK_DOUBLES_EQUAL(before.getX(), after.getX(), 0.0001);
        CHECK_EQUAL(before.getSlot(), after.getSlot());
        CHECK_DOUBLES_EQUAL(before.getScoreX(), after.getScoreX(), 0.0001);
        
        CHECK(WedgeType::DIMINUENDO == after.getWedgeType());
        
        // The stuff below is not related to Wedge Placement but deals with larger round trip bug regression
        
        auto beforeNotes = beforeScore->getMeasure(0)->items<Note>(*beforeScore->getTracks().front().get());
        auto afterNotes = afterScore->getMeasure(0)->items<Note>(*afterScore->getTracks().front().get());
        CHECK(afterNotes.size()>0);
        
        const Note& beforeNote = beforeNotes.front();
        const Note& afterNote = afterNotes.front();
        
        auto beforeNoteX = beforeNote.getScoreX();
        auto afterNoteX = afterNote.getScoreX();
        
        CHECK_DOUBLES_EQUAL(beforeNoteX, afterNoteX, 0.0001);
        
        auto beforeMeasureGap = beforeScore->getMeasure(0)->getGap();
        auto afterMeasureGap = afterScore->getMeasure(0)->getGap();
        
        CHECK_DOUBLES_EQUAL(beforeMeasureGap, afterMeasureGap, 0.0001);
    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
