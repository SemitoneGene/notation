// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Komp
#include "ChordHead.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    // these test cases are taken from Lord Chord.xml
    // the test name indicates M0 -> Measure Index 0
    // and E00 -> Event (i.e. chord, note or rest) Index

    TEST( Test_M0_E00, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{4, 0},
            ChordHead{5, 0},
            ChordHead{2, 0},
            ChordHead{6, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Right );
        CHECK( heads.at(3).side == ChordSide::Left );
    }

    TEST( Test_M0_E01, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{4, 0},
            ChordHead{-1, 0},
            ChordHead{4, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Right );
    }

    TEST( Test_M0_E02, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{1, 0},
            ChordHead{4, 0},
            ChordHead{2, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
    }

    TEST( Test_M0_E04, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{3, 0},
            ChordHead{2, 0},
            ChordHead{6, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
    }

    TEST( Test_M0_E05, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{0, 0},
            ChordHead{2, 0},
            ChordHead{4, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Left );
    }

    TEST( Test_M1_E00, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{3, 0},
            ChordHead{4, 0},
            ChordHead{5, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
    }

    TEST( Test_M1_E01, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{3, 0},
            ChordHead{0, 0},
            ChordHead{6, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Left );
    }

    TEST( Test_M2_E00, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{5, 0},
            ChordHead{1, 0},
            ChordHead{8, 0},
            ChordHead{7, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::DOWN);

        CHECK( heads.at(0).side == ChordSide::Right );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Right );
    }

    TEST( Test_M2_E01, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{1, 0},
            ChordHead{3, 0},
            ChordHead{6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::DOWN);

        CHECK( heads.at(0).side == ChordSide::Right );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Right );
        CHECK( heads.at(3).side == ChordSide::Right );
        CHECK( heads.at(4).side == ChordSide::Right );
    }

    TEST( Test_M2_E02, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{3, 0},
            ChordHead{4, 0},
            ChordHead{8, 0},
            ChordHead{9, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::DOWN);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Right );
    }

    TEST( Test_M3_E04, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{11, 0},
            ChordHead{10, 0},
            ChordHead{8, 0},
            ChordHead{7, 0},
            ChordHead{3, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::DOWN);

        CHECK( heads.at(0).side == ChordSide::Right );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Right );
        CHECK( heads.at(3).side == ChordSide::Left );
        CHECK( heads.at(4).side == ChordSide::Right );
    }

    TEST( Test_M4_E13, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{4, 0},
            ChordHead{5, 0},
            ChordHead{8, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::UP);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
    }

    // these are from Crazy Head Flippage.xml

    TEST( Test_CHF_M0, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{2, 0},
            ChordHead{4, 0},
            ChordHead{5, 0},
            ChordHead{6, 0},
            ChordHead{7, 0},
            ChordHead{8, 0},
            ChordHead{9, 0},
            ChordHead{10, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::NONE);

        CHECK( heads.at(0).side == ChordSide::Right );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Right );
        CHECK( heads.at(4).side == ChordSide::Left );
        CHECK( heads.at(5).side == ChordSide::Right );
        CHECK( heads.at(6).side == ChordSide::Left );
        CHECK( heads.at(7).side == ChordSide::Right );
    }

    TEST( Test_CHF_M1, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{2, 0},
            ChordHead{3, 0},
            ChordHead{4, 0},
            ChordHead{5, 0},
            ChordHead{6, 0},
            ChordHead{7, 0},
            ChordHead{8, 0},
            ChordHead{9, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::NONE);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Right );
        CHECK( heads.at(4).side == ChordSide::Left );
        CHECK( heads.at(5).side == ChordSide::Right );
        CHECK( heads.at(6).side == ChordSide::Left );
        CHECK( heads.at(7).side == ChordSide::Right );
    }

    TEST( Test_CHF_M2, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{2, 0},
            ChordHead{4, 0},
            ChordHead{6, 0},
            ChordHead{8, 0},
            ChordHead{9, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::NONE);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Left );
        CHECK( heads.at(4).side == ChordSide::Right );
    }

    TEST( Test_CHF_M6, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{2, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
            ChordHead{12, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::NONE);

        CHECK( heads.at(0).side == ChordSide::Right );
        CHECK( heads.at(1).side == ChordSide::Right );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Right );
    }

    TEST( Test_CHF_M7, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{-6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
            ChordHead{12, 0},
        };

        ChordHead::calculateHeadSidesNotPtr(heads, StemDirection::NONE);

        CHECK( heads.at(0).side == ChordSide::Left );
        CHECK( heads.at(1).side == ChordSide::Left );
        CHECK( heads.at(2).side == ChordSide::Left );
        CHECK( heads.at(3).side == ChordSide::Right );
    }



    TEST( Accidental_3, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{-6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
        };

        heads.at(0).accidentalFrame = Rect{63, 30, 70, 37};
        heads.at(1).accidentalFrame = Rect{69, 30, 76, 37};
        heads.at(2).accidentalFrame = Rect{80, 30, 87, 37};

        heads.at(0).accidentalOffset = -1000.0;
        heads.at(1).accidentalOffset = -1000.0;
        heads.at(2).accidentalOffset = -1000.0;

        ChordHead::calculateAccidentalPositionsNotPtr(heads);

        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(0).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1008.00, heads.at(1).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(2).accidentalOffset, 0.0001 );
    }



    TEST( Accidental_4, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{-6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
            ChordHead{12, 0},
        };

        heads.at(0).accidentalFrame = Rect{63, 30, 70, 37};
        heads.at(1).accidentalFrame = Rect{69, 30, 76, 37};
        heads.at(2).accidentalFrame = Rect{75, 30, 81, 37};
        heads.at(3).accidentalFrame = Rect{80, 30, 87, 37};

        heads.at(0).accidentalOffset = -1000.0;
        heads.at(1).accidentalOffset = -1000.0;
        heads.at(2).accidentalOffset = -1000.0;
        heads.at(3).accidentalOffset = -1000.0;

        ChordHead::calculateAccidentalPositionsNotPtr(heads);

        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(0).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1016.00, heads.at(1).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1008.00, heads.at(2).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(3).accidentalOffset, 0.0001 );
    }


    TEST( Accidental_5, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{-6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
            ChordHead{12, 0},
            ChordHead{13, 0},
        };

        heads.at(0).accidentalFrame = Rect{63, 30, 70, 37};
        heads.at(1).accidentalFrame = Rect{69, 30, 76, 37};
        heads.at(2).accidentalFrame = Rect{63, 30, 93, 37};
        heads.at(3).accidentalFrame = Rect{80, 30, 87, 37};
        heads.at(4).accidentalFrame = Rect{86, 30, 93, 37};

        heads.at(0).accidentalOffset = -1000.0;
        heads.at(1).accidentalOffset = -1000.0;
        heads.at(2).accidentalOffset = -1000.0;
        heads.at(3).accidentalOffset = -1000.0;
        heads.at(4).accidentalOffset = -1000.0;

        ChordHead::calculateAccidentalPositionsNotPtr(heads);

        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(0).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1008.00, heads.at(1).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1016.00, heads.at(2).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1008.00, heads.at(3).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(4).accidentalOffset, 0.0001 );
    }


    TEST( Accidental_3x, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{-6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
            ChordHead{12, 0},
            ChordHead{13, 0},
        };

        heads.at(0).accidentalFrame = Rect{};
        heads.at(1).accidentalFrame = Rect{};
        heads.at(2).accidentalFrame = Rect{63, 30, 81, 37};
        heads.at(3).accidentalFrame = Rect{80, 30, 87, 37};
        heads.at(4).accidentalFrame = Rect{86, 30, 93, 37};

        heads.at(0).accidentalOffset = -1000.0;
        heads.at(1).accidentalOffset = -1000.0;
        heads.at(2).accidentalOffset = -1000.0;
        heads.at(3).accidentalOffset = -1000.0;
        heads.at(4).accidentalOffset = -1000.0;

        ChordHead::calculateAccidentalPositionsNotPtr(heads);

        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(0).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(1).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(2).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1008.00, heads.at(3).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1000.00, heads.at(4).accidentalOffset, 0.0001 );
    }


    TEST( Accidental_X, ChordHead )
    {
        using namespace komp;
        ChordHeads heads =
        {
            ChordHead{-6, 0},
            ChordHead{9, 0},
            ChordHead{11, 0},
            ChordHead{12, 0},
            ChordHead{13, 0},
        };

        heads.at(0).accidentalFrame = Rect{63, 30, 70, 37};
        heads.at(1).accidentalFrame = Rect{69, 30, 76, 37};
        heads.at(2).accidentalFrame = Rect{63, 30, 93, 37};
        heads.at(3).accidentalFrame = Rect{80, 30, 87, 37};
        heads.at(4).accidentalFrame = Rect{86, 30, 93, 37};

        std::vector<Rect> noteheads;
        noteheads.emplace_back(65, 31, 66, 37);
        noteheads.emplace_back(73, 32, 74, 37);
        noteheads.emplace_back(77, 33, 78, 37);
        noteheads.emplace_back(83, 34, 84, 37);
        noteheads.emplace_back(90, 35, 91, 37);

        heads.at(0).noteheadFrames = noteheads;
        heads.at(1).noteheadFrames = noteheads;
        heads.at(2).noteheadFrames = noteheads;
        heads.at(3).noteheadFrames = noteheads;
        heads.at(4).noteheadFrames = noteheads;

        heads.at(0).accidentalOffset = -1000.0;
        heads.at(1).accidentalOffset = -1000.0;
        heads.at(2).accidentalOffset = -1000.0;
        heads.at(3).accidentalOffset = -1000.0;
        heads.at(4).accidentalOffset = -1000.0;

        ChordHead::calculateAccidentalPositionsNotPtr(heads);

        CHECK_DOUBLES_EQUAL( -1007.00, heads.at(0).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1015.00, heads.at(1).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1023.00, heads.at(2).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1011.00, heads.at(3).accidentalOffset, 0.0001 );
        CHECK_DOUBLES_EQUAL( -1003.00, heads.at(4).accidentalOffset, 0.0001 );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
