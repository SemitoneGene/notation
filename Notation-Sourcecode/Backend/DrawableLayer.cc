// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLayer.h"

// Local
#include "DrawableColor.h"
#include "DrawableContext.h"
#ifdef KOMP_BUILD_APPLE
#include "DrawableLayerMacOS.h"
#elif KOMP_BUILD_LINUX
#include "DrawableLayerLinux.h"
#endif

namespace komp
{
DrawableLayer::DrawableLayer(const DrawableContext& inParent)
: mParent{inParent}
{
#ifdef KOMP_BUILD_APPLE
    mDrawableLayerImpl = std::make_unique<DrawableLayerMacOS>();
#elif KOMP_BUILD_LINUX
    mDrawableLayerImpl = std::make_unique<DrawableLayerLinux>();
#endif
    mParent.addLayer(this);
}

DrawableLayer::DrawableLayer(const DrawableContext& inParent,
                             const Region& inPath)
: mParent{inParent}
{
#ifdef KOMP_BUILD_APPLE        
    mDrawableLayerImpl = std::make_unique<DrawableLayerMacOS>(inPath);
#elif KOMP_BUILD_LINUX
    mDrawableLayerImpl = std::make_unique<DrawableLayerLinux>(inPath);
#endif        
    mParent.addLayer(this);
}

DrawableLayer::DrawableLayer(const DrawableContext& inParent,
                             DrawableContextItem* inItem)
: mParent{inParent}
{
#ifdef KOMP_BUILD_APPLE
    mDrawableLayerImpl = std::make_unique<DrawableLayerMacOS>(inItem);
#elif KOMP_BUILD_LINUX
    mDrawableLayerImpl = std::make_unique<DrawableLayerLinux>(inItem);
#endif
    mParent.addLayer(this);
}

DrawableLayer::~DrawableLayer()
{
    mDrawableLayerImpl->removeFromParent();
}

void
DrawableLayer::removeFromParent()
{
    mDrawableLayerImpl->removeFromParent();
}

void
DrawableLayer::restoreParent()
{
    mParent.addLayer(this);
}

void
DrawableLayer::setLineWidth(double inWidth)
{
    mDrawableLayerImpl->setLineWidth(inWidth);
}

void
DrawableLayer::setMiterLimit(double inLimit)
{
    mDrawableLayerImpl->setMiterLimit(inLimit);
}

void
DrawableLayer::setFillColor(const DrawableColor& inColor)
{
    mDrawableLayerImpl->setFillColor(inColor);
}

void
DrawableLayer::disableFillColor()
{
    mDrawableLayerImpl->disableFillColor();
}

void
DrawableLayer::setStrokeColor(const DrawableColor& inColor)
{
    mDrawableLayerImpl->setStrokeColor(inColor);
}

void
DrawableLayer::disableStrokeColor()
{
    mDrawableLayerImpl->disableStrokeColor();
}

void
DrawableLayer::moveTo(const Point& inPosition)
{
    mDrawableLayerImpl->moveTo(inPosition);
}

void
DrawableLayer::moveBy(const Point& inAmount)
{
    mDrawableLayerImpl->moveBy(inAmount);
}

void
DrawableLayer::setZOrder(int inOrder)
{
    mDrawableLayerImpl->setZOrder(inOrder);
}

void
DrawableLayer::setScale(double inScale)
{
    mDrawableLayerImpl->setScale(inScale);
}

void
DrawableLayer::setRotation(double inDegrees)
{
    mDrawableLayerImpl->setRotation(inDegrees);
}

Region
DrawableLayer::getPath() const
{
    return mDrawableLayerImpl->getPath();
}

Rect
DrawableLayer::getBounds() const
{
    return mDrawableLayerImpl->getBounds();
}

DrawableContextItem*
DrawableLayer::getLayer() const {
    return mDrawableLayerImpl->getLayer();
}

bool
DrawableLayer::isValid() const
{
    return mDrawableLayerImpl->getLayer() != nullptr;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
