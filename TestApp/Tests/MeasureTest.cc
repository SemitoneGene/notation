// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test
#include "BasicScore.h"
#include "TestFileRepository.h"
#include "RoundTrip.h"

// Komp
#include "Score.h"
#include "Temporal.h"
#include "Measure.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS    
    TEST( getClocksPerMeasure4_4, MeasureTest )
    {
        using namespace komp;
        auto score = getBasicScore(1);
        const auto numerator = 4;
        const auto denominator = 4;
        const auto clocksPerQuarter = static_cast<int>(TIME_BASE);
        const auto expected = ( numerator * clocksPerQuarter * 4 ) / denominator;
        const auto& measure = score->getMeasure(0);
        measure->setMeter(numerator, denominator);
        const auto actual = static_cast<int>(measure->getClocks());
        CHECK_EQUAL(expected, actual);
    }
    
    TEST( getClocksPerMeasure3_4, MeasureTest )
    {
        using namespace komp;
        auto score = getBasicScore(1);
        const auto numerator = 3;
        const auto denominator = 4;
        const auto clocksPerQuarter = static_cast<int>(TIME_BASE);
        const auto expected = ( numerator * clocksPerQuarter * 4 ) / denominator;
        const auto& measure = score->getMeasure(0);
        measure->setMeter(numerator, denominator);
        const auto actual = static_cast<int>(measure->getClocks());
        CHECK_EQUAL(expected, actual);
    }
    
    TEST( getClocksPerMeasure5_8, MeasureTest )
    {
        using namespace komp;
        auto score = getBasicScore(1);
        const auto numerator = 5;
        const auto denominator = 8;
        const auto clocksPerQuarter = static_cast<int>(TIME_BASE);
        const auto expected = ( numerator * clocksPerQuarter * 4 ) / denominator;
        const auto& measure = score->getMeasure(0);
        measure->setMeter(numerator, denominator);
        const auto actual = static_cast<int>(measure->getClocks());
        CHECK_EQUAL(expected, actual);
    }
    
    TEST( getClocksPerMeasure3_16, MeasureTest )
    {
        using namespace komp;
        auto score = getBasicScore(1);
        const auto numerator = 5;
        const auto denominator = 8;
        const auto clocksPerQuarter = static_cast<int>(TIME_BASE);
        const auto expected = ( numerator * clocksPerQuarter * 4 ) / denominator;
        const auto& measure = score->getMeasure(0);
        measure->setMeter(numerator, denominator);
        const auto actual = static_cast<int>(measure->getClocks());
        CHECK_EQUAL(expected, actual);
    }

    TEST( DeleteMeasureTest, MeasureTest )
    {
        using namespace komp;
        const auto originalNumMeasures = 16;
        auto numMeasures = originalNumMeasures;
        auto score = TestFileRepository::loadScore("k001a_Articulations.xml");
        CHECK_EQUAL( originalNumMeasures, score->getMeasureCount());

        auto cmd = score->getCommandFactory().genericCommand("Blerp");

        for (int i = 16; i > 1; --i) {
            const int measureIndex = i/2;
            auto deleteMeasure = score->getCommandFactory().deleteMeasureCommand(static_cast<size_t>(measureIndex));
            deleteMeasure->execute();
            cmd->addSubCommand(std::move(deleteMeasure));
            CHECK_EQUAL(--numMeasures, score->getMeasureCount());

            const auto roundTrippedScore = roundTrip(score);
            CHECK_EQUAL(numMeasures, score->getMeasureCount());
        }

        cmd->undoSubCommands();
        CHECK_EQUAL( originalNumMeasures, score->getMeasureCount());
        const auto roundTrippedAgain = roundTrip(score);
        CHECK_EQUAL(originalNumMeasures, roundTrippedAgain->getMeasureCount());
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
