// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// System
#include <sstream>

// Komp
#include "Score.h"
#include "FileMediatorFactory.h"

// Mx
#include "mx/api/ScoreData.h"
#include "mx/api/DocumentManager.h"

namespace
{
    inline mx::api::ScoreData createScoreData()
    {
        using namespace mx::api;
        ScoreData scoreData;
        scoreData.encoding.software.emplace_back( "komp" );
        scoreData.encoding.miscelaneousFields.emplace_back( "komp-version-major", "1" );
        scoreData.encoding.miscelaneousFields.emplace_back( "komp-version-minor", "2" );
        scoreData.encoding.miscelaneousFields.emplace_back( "komp-version-patch", "3" );
        scoreData.parts.emplace_back( PartData{} );
        auto& part = scoreData.parts.back();
        part.name = "hello world";
        part.measures.emplace_back( MeasureData{} );
        auto& measure = part.measures.back();
        measure.staves.emplace_back( StaffData{} );
        return scoreData;
    }
    
    inline std::string getScoreAsMusicXML()
    {
        using namespace mx::api;
        std::stringstream oss;
        auto& mgr = DocumentManager::getInstance();
        const auto id = mgr.createFromScore( createScoreData() );
        mgr.writeToStream( id, oss );
        mgr.destroyDocument( id );
        return oss.str();
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS 
    TEST( VersionParsing, FileLoadResult )
    {
        using namespace komp;
        const std::string xml = getScoreAsMusicXML();
        std::istringstream iss{ xml };
        auto fileMediator = FileMediatorFactory::create( FileMediatorType::MusicXML );
        auto score = fileMediator->loadStream( iss );
        CHECK( score != nullptr );
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
