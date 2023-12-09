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
        /// During the metrics operation the Drawable will calculate the Frame
        /// and output the value here.
        Rect frame;

        /// During the metrics operation the Drawable will calculate the Region
        /// and output the value here.
        Region region;

        /// During the metrics operation the Drawable will calculate the Anchor
        /// and output the value here.
        Point anchor;
                
        DrawResult()
                : frame{}
                , region{}
                , anchor{0.0, 0.0}
        {}

        DrawResult(const DrawResult& other)
        : frame{other.frame}
#ifdef KOMP_BUILD_APPLE
        , region{::CGPathCreateMutableCopy(other.region.getMutable())}
#elif KOMP_BUILD_LINUX
        , region{other.region}
#endif
        , anchor(other.anchor)
        {
        }

        DrawResult&
        operator=(const DrawResult& other)
        {
            frame = other.frame;
#ifdef KOMP_BUILD_APPLE
            region = ::CGPathCreateMutableCopy(other.region.getMutable());
#elif KOMP_BUILD_LINUX
            region = other.region;
#endif
            anchor = other.anchor;
            return *this;
        }

        DrawResult(DrawResult&&) = default;
        DrawResult& operator=(DrawResult&&) = default;

        inline DrawResult&
        moveBy(double inXAmount, double inYAmount)
        {
            // Adjust the frame and anchor.  There is no need to modify
            // the region as it will be drawn using the anchor point.
            frame.moveBy(inXAmount, inYAmount);
            anchor.moveBy(inXAmount, inYAmount);
            return *this;
        }
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

