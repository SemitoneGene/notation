#include "Cpul/cpulTestResult.h"
#include "Cpul/cpulFailure.h"
#include <iostream>

TestResult::TestResult()
:
    failureCount (0)
{
}

void TestResult::testWasRun()
{
}

void TestResult::startTests ()
{
}

void TestResult::addFailure (Failure failure)
{
    std::cout << failure << std::endl;
    failureCount++;
    myFailures.emplace_back(std::move(failure));
}

void TestResult::endTests ()
{
}

int TestResult::getFailureCount() const
{
    return failureCount;
}

const std::vector<Failure>& TestResult::getFailures() const
{
    return myFailures;
}
