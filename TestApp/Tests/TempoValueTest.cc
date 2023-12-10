// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Komp
#include "TempoValue.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( Test001, TempoValue )
    {
        using namespace komp;
        const double bpm = 120.0;
        const auto beatType = TemporalType::QUARTER;
        const int beatTypeDots = 0;
        TempoValue tempoValue(bpm);
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getBpm(), 0.001 );
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getQpm(), 0.001 );
        CHECK( beatType == tempoValue.getBeatType() );
        CHECK_EQUAL( beatTypeDots, tempoValue.getBeatTypeDots() );
    }
    
    TEST( Test002, TempoValue )
    {
        using namespace komp;
        const double bpm = 111.3;
        const auto beatType = TemporalType::HALF;
        const int beatTypeDots = 0;
        TempoValue tempoValue(bpm, beatType);
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getBpm(), 0.001 );
        CHECK_DOUBLES_EQUAL( 222.6, tempoValue.getQpm(), 0.001 );
        CHECK( beatType == tempoValue.getBeatType() );
        CHECK_EQUAL( beatTypeDots, tempoValue.getBeatTypeDots() );
    }
    
    TEST( Test003, TempoValue )
    {
        using namespace komp;
        const double bpm = 100.0;
        const auto beatType = TemporalType::QUARTER;
        const int beatTypeDots = 1;
        TempoValue tempoValue(bpm, beatType, beatTypeDots);
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getBpm(), 0.001 );
        CHECK_DOUBLES_EQUAL( 150.0, tempoValue.getQpm(), 0.001 );
        CHECK( beatType == tempoValue.getBeatType() );
        CHECK_EQUAL( beatTypeDots, tempoValue.getBeatTypeDots() );
    }
    
    TEST( Test004, TempoValue )
    {
        using namespace komp;
        const double bpm = 100.0;
        const auto beatType = TemporalType::QUARTER;
        const int beatTypeDots = 2;
        TempoValue tempoValue(bpm, beatType, beatTypeDots);
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getBpm(), 0.001 );
        CHECK_DOUBLES_EQUAL( 175.0, tempoValue.getQpm(), 0.001 );
        CHECK( beatType == tempoValue.getBeatType() );
        CHECK_EQUAL( beatTypeDots, tempoValue.getBeatTypeDots() );
    }
    
    TEST( Test005, TempoValue )
    {
        using namespace komp;
        const double bpm = 100.0;
        const auto beatType = TemporalType::EIGHTH;
        const int beatTypeDots = 0;
        TempoValue tempoValue(bpm, beatType, beatTypeDots);
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getBpm(), 0.001 );
        CHECK_DOUBLES_EQUAL( 50.0, tempoValue.getQpm(), 0.001 );
        CHECK( beatType == tempoValue.getBeatType() );
        CHECK_EQUAL( beatTypeDots, tempoValue.getBeatTypeDots() );
    }
    
    TEST( Test006, TempoValue )
    {
        using namespace komp;
        const double bpm = 100.0;
        const auto beatType = TemporalType::EIGHTH;
        const int beatTypeDots = 1;
        TempoValue tempoValue(bpm, beatType, beatTypeDots);
        CHECK_DOUBLES_EQUAL( bpm, tempoValue.getBpm(), 0.001 );
        CHECK_DOUBLES_EQUAL( 75.0, tempoValue.getQpm(), 0.001 );
        CHECK( beatType == tempoValue.getBeatType() );
        CHECK_EQUAL( beatTypeDots, tempoValue.getBeatTypeDots() );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
