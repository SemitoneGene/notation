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
#include "BeamedNote.h"


namespace
{
    
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS    
    TEST(ConstructorEighth, BeamedNote)
    {
        using namespace komp;
        auto score = getBasicScore(1);
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.temporalType = TemporalType::EIGHTH;
        Note& note = noteFactory.insertNote();
        BeamedNote beamedNote{note};
        CHECK_EQUAL(1, beamedNote.getCount());
    }
    
    TEST(ConstructorSixteenth, BeamedNote)
    {
        using namespace komp;
        auto score = getBasicScore(1);
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        Note& note = noteFactory.insertNote();
        BeamedNote beamedNote{note};
        CHECK_EQUAL(2, beamedNote.getCount());
    }
    
    TEST(getSetIsBroken, BeamedNote)
    {
        using namespace komp;
        auto score = getBasicScore(1);
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        Note& note = noteFactory.insertNote();
        BeamedNote beamedNote{note};
        CHECK_EQUAL(false, beamedNote.getIsBroken(0));
        CHECK_EQUAL(false, beamedNote.getIsBroken(1));
        beamedNote.setIsBroken(1, true);
        CHECK_EQUAL(false, beamedNote.getIsBroken(0));
        CHECK_EQUAL(true, beamedNote.getIsBroken(1));
        beamedNote.setIsBroken(0, true);
        CHECK_EQUAL(true, beamedNote.getIsBroken(0));
        CHECK_EQUAL(true, beamedNote.getIsBroken(1));
        beamedNote.setIsBroken(1, false);
        CHECK_EQUAL(true, beamedNote.getIsBroken(0));
        CHECK_EQUAL(false, beamedNote.getIsBroken(1));
    }
    
    TEST(getNote, BeamedNote)
    {
        using namespace komp;
        auto score = getBasicScore(1);
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.temporalType = TemporalType::SIXTEENTH;
        Note& note = noteFactory.insertNote();
        BeamedNote beamedNote{note};
        CHECK_EQUAL(&note, &beamedNote.getNote());
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
