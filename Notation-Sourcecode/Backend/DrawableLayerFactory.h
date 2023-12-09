// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableColor.h"
#ifdef KOMP_BUILD_APPLE
#include "DrawableLayerFactoryMacOS.h"
#elif KOMP_BUILD_LINUX
#include "DrawableLayerFactoryLinux.h"
#endif
#include "Forward.h"
#include "Geometry.h"

namespace komp
{
    class DrawableLayerFactory
    {
    public:
        DrawableLayerFactory();
        
        std::unique_ptr<DrawableLayer> createCurve(const DrawableContext& inContext,
                                                   const Region& inRegion) const;

        std::unique_ptr<DrawableLayer> createEllipse(const DrawableContext& inContext,
                                       double inWidth,
                                       double inHeight) const;

        std::unique_ptr<DrawableLayer> createLine(const DrawableContext& inContext,
                                                  const Point& inStart,
                                                  const Point& inEnd) const;

        std::unique_ptr<DrawableLayer> createRectangle(const DrawableContext& inContext,
                                                       const Rect& inBounds) const;

        std::unique_ptr<DrawableLayer> createText(const DrawableContext& inContext,
                                                  std::string_view inFontName,
                                                  const double inFontSize,
                                                  const char* inText,
                                                  const Point& inPosition,
                                                  const DrawableColor& inColor) const;
        
    private:
#ifdef KOMP_BUILD_APPLE
        DrawableLayerFactoryMacOS mDrawableLayerFactoryImpl;
#elif KOMP_BUILD_LINUX
        DrawableLayerFactoryLinux mDrawableLayerFactoryImpl;
#endif
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
