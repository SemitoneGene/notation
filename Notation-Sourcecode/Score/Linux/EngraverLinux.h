// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>

// Local
#include "Geometry.h"

namespace komp
{
    class EngraverLinux
    {
    public:
        static komp::Size MeasureText(std::string_view inFontName,
                                      const double inFontSize,
                                      const char* inText);
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
