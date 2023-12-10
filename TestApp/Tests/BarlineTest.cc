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
#include "TestHelpers.h"
#include "BasicScore.h"

// Komp
#include "Measure.h"

// Mx
#include "mx/api/DocumentManager.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( BarlineTest, Barline)
    {
        const int measureIndex = 0;
        const auto leftExpected = BarlineType::LEFT_REPEAT;
        const auto rightExpected = BarlineType::RIGHT_REPEAT;

        const auto originalScore = getBasicScore();
        const auto& originalMeasure = originalScore->getMeasure(measureIndex);
        auto& cmdFact = originalScore->getCommandFactory();
        auto barlineCommand = cmdFact.barlinesCommand(*originalMeasure,
                                                      leftExpected,
                                                      rightExpected);
        barlineCommand->execute();

        const auto resultScore = roundTripScore(originalScore);
        const auto& resultMeasure = resultScore->getMeasure(0);
        const auto leftActual = resultMeasure->getLeftBarline();
        const auto rightActual = resultMeasure->getRightBarline();

        CHECK( leftExpected == leftActual );
        CHECK( rightExpected == rightActual );

    }
    T_END;

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
