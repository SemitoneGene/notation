// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TestSupport/TestFile.h"
#include "Throw.h"

namespace komp
{
TestFile::TestFile(std::string mainDirectory,
                   std::string subDirectory,
                   std::string fileName,
                   bool isLoadFailureExpected)
: mIsLoadFailureExpected(isLoadFailureExpected)
{
    KOMP_ASSERT(mainDirectory.size() > 0);
    mMainDirectory = mainDirectory;
    
    mSubdirectory = subDirectory;
    
    KOMP_ASSERT(fileName.size() > 0);
    mFilename = fileName;
    
    size_t lastDotPosition = fileName.find_last_of('.');
    mFilenamePart = fileName.substr(0, lastDotPosition);
    mExtension = fileName.substr(lastDotPosition + 1, fileName.size() - lastDotPosition - 1);
    
    mFullPath = std::string{ mMainDirectory } + std::string{ mSubdirectory } + std::string{ mFilename };
}

std::string_view
TestFile::getFilename() const
{
    return mFilename;
}

std::string_view
TestFile::getSubdirectory() const
{
    return mSubdirectory;
}

std::string_view
TestFile::getFilenamePart() const
{
    return mFilenamePart;
}

std::string_view
TestFile::getExtension() const
{
    return mExtension;
}

std::string_view
TestFile::getFullPath() const
{
    return mFullPath;
}

const bool
TestFile::isLoadFailureExpected() const
{
    return mIsLoadFailureExpected;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
