// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TestController.h"

// Local Frameworks
#include "Cpul/cpulTestTimer.h"
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// System
#include <sstream>

namespace komp
{
    TestController::TestController()
    : myUnsuccessfulTests()
    , myTotalRuntimeMilliseconds(0.0f)
    , myTotalTestCount(0)
    {
        
    }

    void
    TestController::runTests()
    {
        runTests(false);
    }
    
    void
    TestController::runTests(bool excludeSlowTests)
    {
        cpul::TestTimer timer;
        srand(::time_t(NULL));
        TestResult tr;
        
        if (excludeSlowTests) {
            myTotalTestCount = TestRegistry::runFastTests(tr);
        } else {
            myTotalTestCount = TestRegistry::runAllTests(tr);
        }
        
        timer.report( "Total runtime:" );
        myTotalRuntimeMilliseconds = static_cast<float>(timer.getMilliseconds());
        myUnsuccessfulTests.clear();
        for (const auto& f: tr.getFailures()) {
            UnsuccessfulTest info;
            info.testName = f.testName;
            info.failureDescription = f.condition;
            info.fileName = f.fileName;
            info.lineNumber = static_cast<int>(f.lineNumber);
            info.isException = f.isException;
            myUnsuccessfulTests.emplace_back(std::move(info));
        }
    }
    
    int
    TestController::getFailureCount() const
    {
        int failureCount = 0;
        for (const auto& f : getUnsuccessfulTests()) {
            if (!f.isException) {
                ++failureCount;
            }
        }
        return failureCount;
    }
    
    int
    TestController::getErrorCount() const
    {
        int errorCount = 0;
        for (const auto& f : getUnsuccessfulTests()) {
            if (f.isException) {
                ++errorCount;
            }
        }
        return errorCount;
    }
    
    const std::vector<UnsuccessfulTest>&
    TestController::getUnsuccessfulTests() const
    {
        return myUnsuccessfulTests;
    }
    
    float
    TestController::getTotalRuntimeMilliseconds() const
    {
        return myTotalRuntimeMilliseconds;
    }
    
    int
    TestController::getTotalTestCount() const
    {
        return myTotalTestCount;
    }
    
    std::string
    TestController::getTestRunResultMessage() const
    {
        std::stringstream ss;
        for (const auto& f : getUnsuccessfulTests()) {
            ss << f.fileName << ":" << f.lineNumber << " \n\n " << f.testName << " ";
            if (f.isException) {
                ss << "Error: ";
            } else {
                ss << "Failure: ";
            }
            ss << f.failureDescription << std::endl << std::endl;
        }
        ss << getTotalTestCount() << " tests\n";
        ss << getFailureCount() << " failures\n";
        ss << getErrorCount() << " errors" << std::endl;
        ss << "Total runtime: " << getTotalRuntimeMilliseconds() << " ms" << std::endl;
        return ss.str();
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
