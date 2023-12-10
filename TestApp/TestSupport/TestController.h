// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>
#include <vector>

namespace komp
{
    
    // in the output string, a 'failure' is defined as a test with a failed assertion
    // an 'error' is a test that emmited an uncaught exception.
    
    struct UnsuccessfulTest
    {
        std::string testName;
        std::string failureDescription;
        std::string fileName;
        int lineNumber;
        bool isException;
    };
    
    class TestController
    {
    public:
        TestController();
        void runTests();
        void runTests(bool excludeSlowTests);
        
        // returns the count of tests with failed assertions
        int getFailureCount() const;

        // returns the count of tests with uncaught exceptions
        int getErrorCount() const;
        
        // returns the total count of both failures and errors
        int getUnsuccessfulTestCount() const;
        
        // the total number of tests that were run
        int getTotalTestCount() const;
        
        const std::vector<UnsuccessfulTest>& getUnsuccessfulTests() const;
        float getTotalRuntimeMilliseconds() const;
        std::string getTestRunResultMessage() const;
    private:
        std::vector<UnsuccessfulTest> myUnsuccessfulTests;
        float myTotalRuntimeMilliseconds;
        int myTotalTestCount;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
