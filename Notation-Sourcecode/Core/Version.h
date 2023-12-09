
// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>

// Local
#include "VersionConstants.h"

namespace komp
{
    class Version
    {
    public:
        Version(int major, int minor, int patch);
        int major() const;
        int minor() const;
        int patch() const;
        
        std::string toString() const;
        static Version getCurrent();
        
    private:
        int mMajor;
        int mMinor;
        int mPatch;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
