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
#include "Mark.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( xDefault, MarkPosition )
    {
        auto score = TestFileRepository::loadScore("Mark Position.xml");
        auto xml = saveScore(score);
        auto rtscore = roundTripScore(score);
        auto rtmarks = getItems<Mark>(*rtscore.get(), 0, 0, 0, 0);
        Mark& rtmark = rtmarks.at(0);
        const auto& rtPoint = rtmark.getRelativePoint();
        CHECK_DOUBLES_EQUAL(0.7055, rtPoint.getX(), 0.0001);
    }
    T_END;

    TEST( yDefault, MarkPosition )
    {
        auto score = TestFileRepository::loadScore("Mark Position.xml");
        auto xml = saveScore(score);
        auto rtscore = roundTripScore(score);
        auto rtmarks = getItems<Mark>(*rtscore.get(), 0, 0, 0, 0);
        Mark& rtmark = rtmarks.at(0);
        const auto& rtPoint = rtmark.getRelativePoint();
        CHECK_DOUBLES_EQUAL(-1.8624, rtPoint.getYSlots(), 0.0001);
    }
    T_END;

    TEST( Placement01, FermataPlacement )
    {
        auto score = TestFileRepository::loadScore("k002a_Fermatas.xml");
        auto xml = saveScore(score);
        auto rtscore = roundTripScore(score);
        auto rtmarks = getItems<Mark>(*rtscore.get(), 0, 0, 0, 0);
        Mark& rtmark = rtmarks.at(0);
        CHECK_DOUBLES_EQUAL(4.5255, rtmark.getRelativePoint().getYSlots(), 0.0001);
    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

