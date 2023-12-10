// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include <string>
#include <vector>

namespace komp
{

    class TestFile
    {
    public:
        
        // mainDirectory should be followed by a '/'
        // subDirectory can be empty, but if it is not
        // then it should be followed by a '/'
        TestFile(std::string mainDirectory,
                 std::string subDirectory,
                 std::string fileName,
                 bool isLoadFailureExpected);
        
        std::string_view getFilename() const;
        std::string_view getSubdirectory() const;
        std::string_view getFilenamePart() const;
        std::string_view getExtension() const;
        std::string_view getFullPath() const;
        const bool isLoadFailureExpected() const;
    
    private:
        std::string mFilename;
        std::string mSubdirectory;
        std::string mFilenamePart;
        std::string mExtension;
        std::string mFullPath;
        std::string mMainDirectory;
        bool mIsLoadFailureExpected;
    };
    
    using TestFiles = std::vector<TestFile>;
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
