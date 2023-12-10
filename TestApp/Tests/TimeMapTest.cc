// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "TimeMap.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    using namespace komp;

    TEST( helloWorld, TimeMap )
    {
        TimeMap tm{};
        auto expected = -1;
        auto actual = tm.getClocks();
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( setWidthNoPoints, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(10.0);
        auto expected = 10.0;
        auto actual = tm.getWidth();
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( setClocksNoPoints, TimeMap )
    {
        TimeMap tm{};
        tm.setClocks(12.0);
        auto expected = 12.0;
        auto actual = tm.getClocks();
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( getIsSetFalse, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(11.1);
        CHECK( !tm.getIsSet() );
    }
    T_END
    
    TEST( getIsSetTrue, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(11.1);
        tm.setClocks(199);
        CHECK( tm.getIsSet() );
    }
    T_END
    
    TEST( addPointNotSet, TimeMap )
    {
        TimeMap tm{};
        CHECK( !tm.addPoint(100, 38.2) );
    }
    T_END
    
    TEST( addPointIsSet, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(11.1);
        tm.setClocks(199);
        CHECK( tm.addPoint(100, 10.0) );
    }
    T_END
    
    TEST( addPointBadPoint, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(11.1);
        tm.setClocks(199);
        CHECK( tm.addPoint(100, 10.0) );
        CHECK( !tm.addPoint(99, 10.1) );
    }
    T_END
    
    TEST( getTimeForX_test01, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(250, 25.0) );
        CHECK( tm.addPoint(500, 50.0) );
        CHECK( tm.addPoint(750, 75.0) );
        int expected = 300;
        int actual = tm.getTimeForX(30.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( getTimeForX_test02, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        int expected = 150;
        int actual = tm.getTimeForX(90.5);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( getTimeForX_test03, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        int expected = 650;
        int actual = tm.getTimeForX(96.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( getXForTime_test01, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(250, 25.0) );
        CHECK( tm.addPoint(500, 50.0) );
        CHECK( tm.addPoint(750, 75.0) );
        double expected = 30.0;
        double actual = tm.getXForTime(300);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( getXForTime_test02, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        double expected = 90.5;
        double actual = tm.getXForTime(150);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( getXForTime_test03, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        double expected = 96.0;
        double actual = tm.getXForTime(650);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( rescaleWidth_getTimeForX_test01, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(250, 25.0) );
        CHECK( tm.addPoint(500, 50.0) );
        CHECK( tm.addPoint(750, 75.0) );
        tm.setWidth(1000.0);
        int expected = 300;
        int actual = tm.getTimeForX(300.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( rescaleWidth_getTimeForX_test02, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setWidth(1000.0);
        int expected = 150;
        int actual = tm.getTimeForX(905.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( rescaleWidth_getTimeForX_test03, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setWidth(1000.0);
        int expected = 650;
        int actual = tm.getTimeForX(960.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( rescaleWidth_getXForTime_test01, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(250, 25.0) );
        CHECK( tm.addPoint(500, 50.0) );
        CHECK( tm.addPoint(750, 75.0) );
        tm.setWidth(1000.0);
        double expected = 300.0;
        double actual = tm.getXForTime(300);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( rescaleWidth_getXForTime_test02, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setWidth(1000.0);
        double expected = 905.0;
        double actual = tm.getXForTime(150);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( rescaleWidth_getXForTime_test03, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setWidth(1000.0);
        double expected = 960.0;
        double actual = tm.getXForTime(650);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( rescaleTime_getTimeForX_test01, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(250, 25.0) );
        CHECK( tm.addPoint(500, 50.0) );
        CHECK( tm.addPoint(750, 75.0) );
        tm.setClocks(100);
        int expected = 30;
        int actual = tm.getTimeForX(30.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( rescaleTime_getTimeForX_test02, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setClocks(100);
        int expected = 15;
        int actual = tm.getTimeForX(90.5);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( rescaleTime_getTimeForX_test03, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setClocks(100);
        int expected = 65;
        int actual = tm.getTimeForX(96.0);
        CHECK_EQUAL( expected, actual );
    }
    T_END
    
    TEST( rescaleTime_getXForTime_test01, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(250, 25.0) );
        CHECK( tm.addPoint(500, 50.0) );
        CHECK( tm.addPoint(750, 75.0) );
        tm.setClocks(100);
        double expected = 30.0;
        double actual = tm.getXForTime(30);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( rescaleTime_getXForTime_test02, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setClocks(100);
        double expected = 90.5;
        double actual = tm.getXForTime(15);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END
    
    TEST( rescaleTime_getXForTime_test03, TimeMap )
    {
        TimeMap tm{};
        tm.setWidth(100.0);
        tm.setClocks(1000);
        CHECK( tm.addPoint(100, 90.0) );
        CHECK( tm.addPoint(200, 91.0) );
        CHECK( tm.addPoint(300, 92.0) );
        tm.setClocks(100);
        double expected = 96.0;
        double actual = tm.getXForTime(65);
        CHECK_DOUBLES_EQUAL( expected, actual, 0.000001 );
    }
    T_END

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
