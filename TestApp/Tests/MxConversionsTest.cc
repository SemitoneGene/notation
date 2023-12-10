// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Test Support
#include "BasicScore.h"

// Komp
#include "MxConverter.h"
#include "Clef.h"
#include "Constants.h"

#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

TEST( mxClef_to_komClef_Bass, MxConversions )
{
    const auto expected = komp::ClefType::F;
    mx::api::ClefData input;
    input.symbol = mx::api::ClefSymbol::f;
    input.line = 4;
    input.octaveChange = 0;
    const auto actual = komp::MxConverter::convertClefType( input );
    CHECK( expected == actual );
}
T_END


TEST( pixels_to_tenths, MxConversions )
{
    komp::MxConverterParams params;
    params.millimeters = 7.0f;
    params.tenths = 40.0f;
    params.pixelsPerInch = static_cast<float>(PIXELS_PER_INCH);
    komp::MxConverter converter{params};
    const float expected = static_cast<float>(40.0f / 7.0f * 25.4f);
    const float actual = static_cast<float>(converter.getTenthsPerInch());
    CHECK_DOUBLES_EQUAL(expected, actual, 0.00000f);
}
T_END


TEST( Score_Constructor, MxConversions )
{
    const auto score = komp::getBasicScore(1);
    using namespace komp;
    
    MxConverterParams params{*score.get()};
    MxConverter converter{params};
    
    const double expected = Staff::getSpacing() * 4.0;
    const double actual = static_cast<double>(converter.convertTenthsToPixels(40.0));
    CHECK_DOUBLES_EQUAL(expected, actual, 0.0001);
}
T_END

#endif

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
