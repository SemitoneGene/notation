// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLayerFactory.h"

// Local
#include "DrawableLayer.h"
#ifdef KOMP_BUILD_APPLE
#include "DrawableLayerFactoryMacOS.h"
#elif KOMP_BUILD_LINUX
#include "DrawableLayerFactoryLinux.h"
#endif

namespace komp
{
    DrawableLayerFactory::DrawableLayerFactory()
    {
    }
    
    std::unique_ptr<DrawableLayer>
    DrawableLayerFactory::createCurve(const DrawableContext& inContext,
                                      const Region& inRegion) const
    {
        return mDrawableLayerFactoryImpl.createCurve(inContext, inRegion);
    }

    std::unique_ptr<DrawableLayer>
    DrawableLayerFactory::createEllipse(const DrawableContext& inContext,
                                        double inWidth,
                                        double inHeight) const
    {
        return mDrawableLayerFactoryImpl.createEllipse(inContext,
                                                        inWidth,
                                                        inHeight);
    }

    std::unique_ptr<DrawableLayer>
    DrawableLayerFactory::createLine(const DrawableContext& inContext,
                                     const Point& inStart,
                                     const Point& inEnd) const
    {
        return mDrawableLayerFactoryImpl.createLine(inContext,
                                                     inStart,
                                                     inEnd);
    }
    
    std::unique_ptr<DrawableLayer>
    DrawableLayerFactory::createRectangle(const DrawableContext& inContext,
                                          const Rect& inBounds) const {
        return mDrawableLayerFactoryImpl.createRectangle(inContext, inBounds);
    }

    std::unique_ptr<DrawableLayer>
    DrawableLayerFactory::createText(const DrawableContext& inContext,
                                     std::string_view inFontName,
                                     const double inFontSize,
                                     const char* inText,
                                     const Point& inPosition,
                                     const DrawableColor& inColor) const
    {
        return mDrawableLayerFactoryImpl.createText(inContext,
                                                     inFontName,
                                                     inFontSize,
                                                     inText,
                                                     inPosition,
                                                     inColor);
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
