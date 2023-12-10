#include "Cpul/cpulFailure.h"

Failure::Failure(
    std::string theCondition,
    std::string theFileName,
    long theLineNumber,
    std::string inTestName)
    : condition(theCondition),
    fileName(theFileName),
    lineNumber(theLineNumber),
    isException(false),
    testName(inTestName)
{
}

std::ostream& operator<<(
    std::ostream& stream,
    Failure& failure)
{
    const std::string msg = failure.isException ? "Error: " : "Failure: ";
    stream << std::endl
    << failure.fileName
    << ":" << failure.lineNumber << " : " << failure.testName << " "
    << msg << failure.condition;
    return stream;
}
