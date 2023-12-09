// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableColors.h"
#include "DrawableContext.h"

namespace komp
{
    struct DrawParams
    {
        /// The DrawableContext* where the Drawable will be drawn
        const DrawableContext& context;

        /// The point at which the Drawable should be drawn. pixels
        Point drawPoint;
        
        /// The colors for stroke and fill
        DrawableColors colors;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

