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

// Komptest
#include "Directories.h"

// Komp
#include "InstrumentConfig.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( basicSurvival, InstrumentConfig )
    {
        using namespace komp;
        std::string configFilePath = getConfigDirectory() + std::string{"/instruments.json"};
        InstrumentConfig cfg;
        cfg.loadFromFile(configFilePath.c_str());
        CHECK(cfg.instruments.size() > 2);
    }
    
    TEST( fluteValues, InstrumentConfig )
    {
        using namespace komp;
        std::string configFilePath = getConfigDirectory() + std::string{"/instruments.json"};
        InstrumentConfig cfg;
        cfg.loadFromFile(configFilePath.c_str());
        auto iter = std::find_if(cfg.instruments.cbegin(),
                                 cfg.instruments.cend(),
                                 [&](const auto& i) {
            return i.second.name == "Flute";
        } );
        CHECK( iter != cfg.instruments.cend() );
        const auto& flute = iter->second;
        CHECK_EQUAL( "Flute", flute.name );
        CHECK_EQUAL( "Woodwinds", flute.family );
        CHECK_EQUAL( 1, flute.startingClefs.size() );
        CHECK( ClefType::G == flute.startingClefs.front() );
        CHECK_EQUAL( 1, flute.allowableClefs.size() );
        CHECK( ClefType::G == flute.allowableClefs.front() );
        CHECK( GroupBracketType::None == flute.instrumentBracket );
        CHECK_EQUAL( "C4", flute.getRangeLow().getString() );
        CHECK_EQUAL( "D7", flute.getRangeHigh().getString() );
    }

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

