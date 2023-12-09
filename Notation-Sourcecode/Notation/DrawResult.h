// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Geometry.h"

namespace komp
{
    struct DrawResult
    {
        /// during the draw operation the Drawable will calculate the Frame
        /// and output the value here. pixels
        Rect frame;

        /// during the draw operation the Drawable will calculate the Region
        /// and output the value here. pixels
        Region region;

        /// during the draw operation the Drawable will calculate the Anchor
        /// and output the value here. pixels
        Point anchor;

        DrawResult() : frame{}, region{}, anchor{0.0, 0.0} {}
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

