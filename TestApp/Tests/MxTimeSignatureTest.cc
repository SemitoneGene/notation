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
#include "CurveScores.h"
#include "TestNoteFactory.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"
#include "TupletScores.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST(TimeSignatures, MxTimeSignature)
    {
        const auto score = getBasicScore(3, 3);
        auto& factory = score->getCommandFactory();
        
        TimeSignatureParams timeSignature;
        timeSignature.beatUnit = 1;
        timeSignature.beatsPerBar = 1;
        factory.changeTimeSignatureCommand(0, timeSignature)->execute();

        timeSignature.beatUnit = 2;
        timeSignature.beatsPerBar = 2;
        factory.changeTimeSignatureCommand(1, timeSignature)->execute();

        timeSignature.beatUnit = 6;
        timeSignature.beatsPerBar = 3;
        factory.changeTimeSignatureCommand(2, timeSignature)->execute();
        
        
        const auto result = roundTripScore(score);
        auto measure = result->getMeasure(0).get();
        CHECK_EQUAL(1, measure->getBeats());
        measure = result->getMeasure(1).get();
        CHECK_EQUAL(2, measure->getBeats());
        measure = result->getMeasure(2).get();
        CHECK_EQUAL(3, measure->getBeats());
    }
    
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
