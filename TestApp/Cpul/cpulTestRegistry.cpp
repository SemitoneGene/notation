#include "Cpul/cpulTest.h"
#include "Cpul/cpulTestResult.h"
#include "Cpul/cpulTestRegistry.h"
#include "Cpul/cpulFailure.h"
#include <iostream>

void TestRegistry::addTest (Test *test) 
{
    instance ().add (test);
}

int TestRegistry::runAllTests (TestResult& result)
{
    return instance().run(false, result);
}

int TestRegistry::runFastTests (TestResult& result)
{
    return instance().run(true, result);
}

TestRegistry& TestRegistry::instance () {
    static TestRegistry registry;
    return registry;
}

void TestRegistry::add(Test *test) {
    tests.push_back (test);
}

int TestRegistry::run (bool excludeSlowTests, TestResult& result) {
    int testCount = 0;
    int errorCount = 0;
    result.startTests ();
    std::vector<Test *>::iterator it;
    for (it = tests.begin (); it != tests.end (); ++it) {
        
        if (excludeSlowTests && (*it)->getIsSlowTest()) {
            continue;
        }
        
        ++testCount;
        try {
            (*it)->run (result);
        }
        catch (std::exception &e) {
            ++errorCount;
            std::cout << std::endl
                      << (*it)->getFileName()
                      << "(" << (*it)->getLineNumber() << ") : "
                      << "Error: exception "
                      << "'" << e.what() << "'"
                      << " thrown in "
                      << (*it)->getName()
                      << std::endl;
            std::stringstream ss;
            ss << "uncaught std::exception occured '" << std::string{e.what()} << "'";
            Failure f{ss.str(), (*it)->getFileName(), (*it)->getLineNumber(), (*it)->getName()};
            f.isException = true;
            result.addFailure(std::move(f));
        }
        catch (...) {
            ++errorCount;
            std::cout << std::endl
                      << (*it)->getFileName()
                      << "(" << (*it)->getLineNumber() << ") : "
                      << "Error: unknown exception thrown in "
                      << (*it)->getName()
                      << std::endl;
            
            Failure f{"uncaught exception of unknown type occured", (*it)->getFileName(), (*it)->getLineNumber(), (*it)->getName()};
            f.isException = true;
            result.addFailure(std::move(f));
        }
    }
    result.endTests ();
    int failureCount = result.getFailureCount();
    if (failureCount > 0 || errorCount > 0) std::cout << std::endl;
    std::cout << testCount << " tests\n"
              << result.getFailureCount() << " failures\n"
              << errorCount << " errors" << std::endl;
    return testCount;
}



