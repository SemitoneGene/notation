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

// Komp
#include "BeatGrouping.h"
#include "TemporalValue.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( BeatGrouping3_4, BeatGrouping )
    {
        const int numerator = 3;
        const int denominator = 4;
        const int expectedBeatUnits = 3;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 1;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 2;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( BeatGrouping4_4, BeatGrouping )
    {
        const int numerator = 4;
        const int denominator = 4;
        const int expectedBeatUnits = 4;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 1;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 2;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 3;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( BeatGrouping3_8, BeatGrouping )
    {
        const int numerator = 3;
        const int denominator = 8;
        const int expectedBeatUnits = 1;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 1;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( BeatGrouping4_8, BeatGrouping )
    {
        const int numerator = 4;
        const int denominator = 8;
        const int expectedBeatUnits = 2;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 1;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( BeatGrouping5_8, BeatGrouping )
    {
        const int numerator = 5;
        const int denominator = 8;
        const int expectedBeatUnits = 2;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 1;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 1;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( BeatGrouping6_8, BeatGrouping )
    {
        const int numerator = 6;
        const int denominator = 8;
        const int expectedBeatUnits = 2;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 1;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 1;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::QUARTER;
        expectedDots = 1;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( BeatGrouping3_2, BeatGrouping )
    {
        const int numerator = 3;
        const int denominator = 2;
        const int expectedBeatUnits = 3;
        
        using namespace komp;
        auto beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const auto& beatUnits = beatGrouping.getBeatUnits();
        CHECK_EQUAL( expectedBeatUnits, beatGrouping.getBeatUnitCount() );
        CHECK_EQUAL( expectedBeatUnits, beatUnits.size() );
        auto beatUnit = beatGrouping.getBeatUnits().cbegin();
        auto expectedTemporalType = TemporalType::QUARTER;
        int expectedDots = 0;
        int beatIndex = 0;
        int expectedBeamCount = 0;
        
        beatIndex = 0;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::HALF;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
        
        beatIndex = 1;
        beatUnit = beatGrouping.getBeatUnits().cbegin() + beatIndex;
        expectedTemporalType = TemporalType::HALF;
        expectedDots = 0;
        expectedBeamCount = 0;
        CHECK( expectedTemporalType == beatUnit->getTemporalType() );
        CHECK_EQUAL( expectedDots, beatUnit->getDots() );
        CHECK_EQUAL( expectedBeamCount, beatUnit->getCount() )
        if (beatUnit->getCount()) {
            CHECK( beatUnit->getBeamBreaks().at(0) );
        }
    }
    
    TEST( getTotalClocks3_2, BeatGrouping )
    {
        using namespace komp;
        const int numerator = 3;
        const int denominator = 2;
        const int expectedClocks = numerator * static_cast<int>(TemporalValue::getValue(TemporalType::HALF));
        const BeatGrouping beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const int actualClocks = beatGrouping.getTotalClocks();
        CHECK_EQUAL( expectedClocks, actualClocks );
    }
    
    TEST( getTotalClocks9_8, BeatGrouping )
    {
        using namespace komp;
        const int numerator = 9;
        const int denominator = 8;
        const int expectedClocks = numerator * static_cast<int>(TemporalValue::getValue(TemporalType::EIGHTH));
        const BeatGrouping beatGrouping = BeatGrouping::getDefaultGroupingForTimeSignature(numerator, denominator);
        const int actualClocks = beatGrouping.getTotalClocks();
        CHECK_EQUAL( expectedClocks, actualClocks );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
