// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BracketTest.h"

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( Bracket01, Bracket )
    {
        const auto expected = getBracketScore();
        const auto actual = roundTripScore(expected);

        const auto& staffSystem = actual->getStaffSystems();
        const auto& brackets = staffSystem.front()->getBrackets();
        CHECK_EQUAL( 2, brackets.size() );

        const auto& firstBracket = brackets.at(0);
        CHECK( firstBracket->getType() == SystemBracketType::BRACE );
        CHECK_EQUAL( 0, firstBracket->getStartStaff().getIndex() );
        CHECK_EQUAL( 1, firstBracket->getStopStaff().getIndex() );

        const auto& stopBracket = brackets.at(1);
        CHECK( stopBracket->getType() == SystemBracketType::BRACKET );
        CHECK_EQUAL( 2, stopBracket->getStartStaff().getIndex() );
        CHECK_EQUAL( 4, stopBracket->getStopStaff().getIndex() );

    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

