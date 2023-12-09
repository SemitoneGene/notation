// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Geometry.h"

namespace komp
{
    enum class StemMode
    {
        FORCE_NONE,
        FORCE_UP,
        FORCE_DOWN,
        AUTOMATIC
    };

    enum class StemType
    {
        NONE,
        DOWN,
        UP
    };
    
    struct StemPlacement
    {
        Point upNW;
        Point upSE;
        Point downNW;
        Point downSE;
        
        bool isValid() const
        {
            if (upNW != POINT_ZERO) {
                return true;
            }
            
            if (upSE != POINT_ZERO) {
                return true;
            }

            if (downNW != POINT_ZERO) {
                return true;
            }
            
            if (downSE != POINT_ZERO) {
                return true;
            }

            return false;
        }
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
