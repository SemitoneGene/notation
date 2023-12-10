// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Duration.h"
#include "Measure.h"
#include "Staff.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"

namespace
{
    inline komp::ScoreUPtr makeScore()
    {
        return komp::makeTupletScore1();
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( getStaffLineCount, Staff )
    {
        const int expected = 5;

        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto temporals = measure->getTemporals();
        const Temporal& firstTemporal = temporals.front();
        const Staff& staff = firstTemporal.getLocation().getStaff();
        const auto actual = staff.getStaffLineCount();

        CHECK_EQUAL( expected, actual );
    }
    T_END;

    TEST( getIsLegerAbove, Staff )
    {
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto temporals = measure->getTemporals();
        const Temporal& firstTemporal = temporals.front();
        const Staff& staff = firstTemporal.getLocation().getStaff();

        bool expected = false;

        for (int slot = -36; slot < 36; ++slot) {
            const bool actual = staff.getIsLegerAbove(slot);
            CHECK_EQUAL( expected, actual );
            expected = (slot + 1) >= 12;
        }
    }
    T_END;


    TEST( getIsLegerBelow, Staff )
    {
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto temporals = measure->getTemporals();
        const Temporal& firstTemporal = temporals.front();
        const Staff& staff = firstTemporal.getLocation().getStaff();

        bool expected = true;

        for (int slot = -36; slot < 36; ++slot) {
            const bool actual = staff.getIsLegerBelow(slot);
            CHECK_EQUAL( expected, actual );
            expected = (slot + 1) <= 0;
        }
    }
    T_END;

#endif
};

    // Semitone Inc. Confidential Information.
    // TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
    // Reproduction in whole or in part without prior written permission of a
    // duly authorized representative is prohibited.
