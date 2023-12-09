// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Version.h"

// System
#include <sstream>

namespace komp
{
    Version::Version(int major, int minor, int patch)
    : mMajor{major}
    , mMinor{minor}
    , mPatch{patch}
    {
        
    }
    
    int
    Version::major() const
    {
        return mMajor;
    }
    
    int
    Version::minor() const
    {
        return mMinor;
    }
    
    int
    Version::patch() const
    {
        return mPatch;
    }
    
    std::string
    Version::toString() const
    {
        std::stringstream ss;
        ss << major() << "."
           << minor() << "."
           << patch() << ".";
        return ss.str();
    }
    
    Version
    Version::getCurrent()
    {
        return Version{ KOMP_VERSION_MAJOR, KOMP_VERSION_MINOR, KOMP_VERSION_PATCH };
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
