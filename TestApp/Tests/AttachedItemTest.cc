// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test Support
#include "AttachedItemScore.h"
#include "BasicScore.h"
#include "TestNoteFactory.h"

// Komp
#include "AttachedItem.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( Test001, AttachedItem )
    {
        using namespace komp;
        auto score = getAttachedItemScore();
        const auto& measure = score->getMeasuresFront();
        auto& track = score->getTracks().at(0);
        const auto& items = measure->items<AttachedItem>(*track.get());
        CHECK(items.size() > 0);
        const AttachedItem& mark = items.at(0);
        const auto& rp = mark.getRelativePoint();
        CHECK_DOUBLES_EQUAL( -3.2299, rp.getX(), 0.0001 );
        CHECK_DOUBLES_EQUAL( -10.2104, rp.getYSlots(), 0.0001 );
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
