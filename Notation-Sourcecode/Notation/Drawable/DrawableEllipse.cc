// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableEllipse.h"

// System
#include <cmath>

// Local
#include "DrawableContext.h"
#include "DrawableLayerFactory.h"
#include "Engraver.h"

namespace komp
{
DrawableEllipse::DrawableEllipse()
: mOffset{0.0, 0.0}
, mZOrder{0}
, mWidth{0.0}
, mHeight{0.0}
, mCache{}
{
    
}

DrawableEllipse::DrawableEllipse(double inWidth, double inHeight)
: mOffset{0.0, 0.0}
, mZOrder{0}
, mWidth{inWidth}
, mHeight{inHeight}
, mCache{}
{
    
}

void
DrawableEllipse::removeFromParent() const
{
    if (mLayer) {
        mLayer->removeFromParent();
    }
}

void
DrawableEllipse::restoreParent() const
{
    if (mLayer) {
        mLayer->restoreParent();
    }
}

Point
DrawableEllipse::getOffset() const
{
    return mOffset;
}


void
DrawableEllipse::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableEllipse::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mZOrder = inOrder;
}

void
DrawableEllipse::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mScale = inScale;
}

void
DrawableEllipse::setRotation(double inDegrees)
{
}

Rect
DrawableEllipse::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableEllipse::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableEllipse::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableEllipse::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableEllipse::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableEllipse::getMetrics(const Point& inPoint) const
{
    if (mWidth == 0 || mHeight == 0) {
        return DrawResult{};
    }
    
    if (mCache.getIsDirty()) {
        
        DrawResult drawResult;
        auto drawPoint = inPoint;
        drawPoint.x += mOffset.x;
        drawPoint.y += mOffset.y;
        
        const auto ellipse = Rect(drawPoint.y,
                                  drawPoint.x,
                                  drawPoint.y + mHeight,
                                  drawPoint.x + mWidth);        
#ifdef KOMP_BUILD_APPLE
        drawResult.region = ::CGPathCreateWithEllipseInRect(ellipse, NULL);
        drawResult.frame = CGPathGetBoundingBox(drawResult.region);
#elif KOMP_BUILD_LINUX
        drawResult.region.getPath().addEllipse(ellipse);
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
DrawableEllipse::draw(const DrawParams& inDrawParams) const
{
    if (mWidth == 0 || mHeight == 0) {
        return;
    }
    
    getMetrics(inDrawParams.drawPoint);
    const auto& offset = getOffset();
    auto drawPoint = inDrawParams.drawPoint;
    drawPoint.moveBy(offset);
    
    if (!mLayer) {
        
        DrawableLayerFactory factory;
        mLayer = factory.createEllipse(inDrawParams.context,
                                       mWidth,
                                       mHeight);
        mLayer->disableStrokeColor();
        mLayer->restoreParent();
    }
    
    mLayer->setZOrder(mZOrder);
    mLayer->setScale(mScale);
    mLayer->moveTo(drawPoint);
    mLayer->setFillColor(inDrawParams.colors.fillColor);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

