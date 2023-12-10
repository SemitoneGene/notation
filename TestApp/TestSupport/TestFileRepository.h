// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include "TestSupport/TestFile.h"

#include <memory>
#include <string>
#include <set>

// Local
#include "Forward.h"
#include "InstrumentConfig.h"


namespace komp
{
    class TestFileRepository
    {
    public:
        
        static const TestFiles& getAllFiles();
        
        // filter out the files excluded in the list defined below
        static TestFiles getNonExcludedFiles();
        
        
        static komp::ScoreUPtr loadScore(std::string fileName);
        static komp::ScoreUPtr loadScore(std::string fileName, const InstrumentConfig config);
        
    private:
        TestFileRepository();
        void initializeFileList();
        static const TestFileRepository& getInstance();
        static const TestFiles& getFiles();
        
    private:
        TestFiles myTestFiles;
        std::set<std::string> myMxLoadFailureFiles;
        std::set<std::string> myExcludedFiles;
    };

    // list files here that are expected to throw expections in Mx during load
    // for example a file with no part ID would be expected to cause Mx to throw
    const std::set<std::string> expectedMxLoadFailureFiles =
    {
        "ly41g_PartNoId.xml",
    };
    
    // list files here that we wish to exclude from test runs for any reason
    // for example if a file is causing komp to crash and we want to fix the
    // problem later, list the file here along with a comment about why it it
    // being excluded from the test runs
    const std::set<std::string> excludedFiles =
    {
        // SIGABRT - Spatial::setX
        "ezinar.xml",
        
        // SIGABRT - Spatial::setX
        // "freezing.xml",
        
        // SIGABRT - Spatial::setX
        // "FreezingSmall.xml",
        
        // Too huge and slow
        "logic01a_homoSapiens.xml",
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
