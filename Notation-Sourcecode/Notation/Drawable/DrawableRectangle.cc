// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableRectangle.h"

// System
#include <cmath>

// Local
#include "DrawableContext.h"
#include "DrawableLayerFactory.h"
#include "Engraver.h"

namespace komp
{

DrawableRectangle::DrawableRectangle(bool inFill, bool inStroke)
: mFill{inFill}
, mStroke{inStroke}
, mCache{}
{
}

DrawableRectangle::DrawableRectangle(const Rect& inBounds,
                                     bool inFill,
                                     bool inStroke)
: mOffset{0.0, 0.0}
, mZOrder{0}
, mFill{inFill}
, mStroke{inStroke}
, mBounds{inBounds}
, mCache{}
{
    
}

void
DrawableRectangle::removeFromParent() const
{
    if (mLayer) {
        mLayer->removeFromParent();
    }
}

void
DrawableRectangle::restoreParent() const
{
    if (mLayer) {
        mLayer->restoreParent();
    }
}

Point
DrawableRectangle::getOffset() const
{
    return mOffset;
}


void
DrawableRectangle::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableRectangle::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mZOrder = inOrder;
}

void
DrawableRectangle::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mScale = inScale;
}

void
DrawableRectangle::setRotation(double inDegrees)
{
}

Rect
DrawableRectangle::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableRectangle::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableRectangle::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableRectangle::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableRectangle::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableRectangle::getMetrics(const Point& inPoint) const
{
    if (mBounds.getIsEmpty()) {
        return DrawResult{};
    }
    
    if (mCache.getIsDirty()) {
        
        DrawResult drawResult;
        auto drawPoint = inPoint;
        drawPoint.x += mOffset.x;
        drawPoint.y += mOffset.y;

#ifdef KOMP_BUILD_APPLE
        drawResult.region = CGPathCreateWithRect(mBounds, nullptr);
        drawResult.frame = CGPathGetBoundingBox(drawResult.region);
#elif KOMP_BUILD_LINUX
        drawResult.region = QPainterPath();
        drawResult.region.addRect(mBounds);
        drawResult.frame = drawResult.region.getBounds();
#endif
        if (drawResult.frame.top > drawResult.frame.bottom) {
            std::swap(drawResult.frame.top, drawResult.frame.bottom);
        }
        
        if (drawResult.frame.right < drawResult.frame.left) {
            std::swap(drawResult.frame.right, drawResult.frame.left);
        }
        mCache.setCache(drawResult, inPoint);
    }
    
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableRectangle::setBounds(const Rect& inBounds)
{
    KOMP_SET_DIRTY
    mBounds = inBounds;
    removeFromParent();
    mLayer = nullptr;
}

void
DrawableRectangle::draw(const DrawParams& inDrawParams) const
{
    if (mBounds.getIsEmpty()) {
        return;
    }
    
    getMetrics(inDrawParams.drawPoint);
    const auto& offset = getOffset();
    auto drawPoint = inDrawParams.drawPoint;
    drawPoint.moveBy(offset);
    
    if (!mLayer) {
        DrawableLayerFactory factory;
        mLayer = factory.createRectangle(inDrawParams.context, mBounds);
        mLayer->restoreParent();
    }
    
    mLayer->setZOrder(mZOrder);
    mLayer->setScale(mScale);
    mLayer->moveTo(drawPoint);

    if (mFill) {
        mLayer->setFillColor(inDrawParams.colors.fillColor);
    } else {
        mLayer->disableFillColor();
    }

    if (mStroke) {
        mLayer->setStrokeColor(inDrawParams.colors.strokeColor);
        mLayer->setLineWidth(mStrokeThickness);
    } else {
        mLayer->disableStrokeColor();
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

