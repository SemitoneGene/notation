// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLine.h"

// System
#include <cmath>
#include <numbers>

// Local
#include "DrawableContext.h"
#include "DrawableLayerFactory.h"
#include "Engraver.h"

namespace komp
{
DrawableLine::DrawableLine()
: mOffset{0.0, 0.0}
, mZOrder{0}
, mLength{0.0}
, mThickness{0.0}
, mDegrees{0}
, mCache{}
{
    
}

void
DrawableLine::removeFromParent() const
{
    if (mLayer) {
        mLayer->removeFromParent();
    }
}

void
DrawableLine::restoreParent() const
{
    if (mLayer) {
        mLayer->restoreParent();
    }
}

Point
DrawableLine::getOffset() const
{
    return mOffset;
}

void
DrawableLine::clearState()
{
    setLength(0.0);
    setDegrees(0.0);
    setThickness(0.0);
    removeFromParent();
    mLayer = nullptr;
}

void
DrawableLine::setOffset(const Point& inOffset)
{
    if (inOffset == mOffset) {
        return;
    }

    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableLine::setZOrder(int inOrder)
{
    if (inOrder == mZOrder) {
        return;
    }

    KOMP_SET_DIRTY
    mZOrder = inOrder;
}

void
DrawableLine::setScale(double inScale)
{
    if (inScale == mScale) {
        return;
    }

    KOMP_SET_DIRTY
    mScale = inScale;
}

void
DrawableLine::setRotation(double inDegrees)
{
}

Rect
DrawableLine::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableLine::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableLine::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableLine::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableLine::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableLine::getMetrics(const Point& inPoint) const
{
    if (mLength == 0) {
        return DrawResult{};
    }
    
    if (mCache.getIsDirty()) {
        
        Point pointA = inPoint;
        pointA.x += getOffset().x;
        pointA.y += getOffset().y;
        
        Point pointB;
        pointB.x = pointA.x + getLength() * std::cos(getRadians());
        pointB.y = pointA.y + getLength() * std::sin(getRadians());
        
        DrawResult drawResult;
#ifdef KOMP_BUILD_APPLE
        const auto thickness = getThickness();
        const komp::Region region = CGPathCreateMutable();
        const auto path = region.getMutable();
        ::CGPathMoveToPoint(path, NULL, pointA.x, pointA.y);
        ::CGPathAddLineToPoint(path, NULL, pointB.x, pointB.y);
        ::CGPathCloseSubpath(path);
        drawResult.region = ::CGPathCreateCopyByStrokingPath(path,
                                                             NULL,
                                                             thickness,
                                                             kCGLineCapButt,
                                                             kCGLineJoinMiter,
                                                             CGFloat{0.0});
        
        drawResult.frame = CGPathGetBoundingBox(drawResult.region);
        if (drawResult.frame.top > drawResult.frame.bottom) {
            std::swap(drawResult.frame.top, drawResult.frame.bottom);
        }
        
        if (drawResult.frame.right < drawResult.frame.left) {
            std::swap(drawResult.frame.right, drawResult.frame.left);
        }
#elif KOMP_BUILD_LINUX        
        drawResult.frame = {pointA, pointB};
        if (drawResult.frame.top > drawResult.frame.bottom) {
            std::swap(drawResult.frame.top, drawResult.frame.bottom);
        }
        
        if (drawResult.frame.right < drawResult.frame.left) {
            std::swap(drawResult.frame.right, drawResult.frame.left);
        }
        drawResult.region.addRect(drawResult.frame);
#endif
        mCache.setCache(drawResult, inPoint);
    }
    
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableLine::draw(const DrawParams& inDrawParams) const
{
    if (mLength == 0) {
        return;
    }
    
    auto offset = getOffset();
    if (mScale != 1.0) {
        offset.scaleBy(mScale);
    }
    auto drawPoint = inDrawParams.drawPoint;
    drawPoint.moveBy(offset);
    
    if (!mLayer) {
        
        const auto radians = getRadians();
        const double thickness = getThickness();
        
        const Point dest {
            mLength * std::cos(radians),
            mLength * std::sin(radians)
        };
        
        DrawableLayerFactory factory;
        mLayer = factory.createLine(inDrawParams.context,
                                    Point(),
                                    dest);
        mLayer->restoreParent();
        mLayer->disableFillColor();
        mLayer->setLineWidth(thickness);
    }
    
    mLayer->setZOrder(mZOrder);
    mLayer->setScale(mScale);
    mLayer->moveTo(drawPoint);
    mLayer->setStrokeColor(inDrawParams.colors.strokeColor);
}


void
DrawableLine::setLength(double inValue)
{
    if (inValue == mLength) {
        return;
    }
    
    KOMP_SET_DIRTY
    mLength = std::max(0.0, inValue);
    removeFromParent();
    mLayer = nullptr;
}


double
DrawableLine::getLength() const
{
    return mLength;
}


void
DrawableLine::setThickness(double inValue)
{
    if (inValue == mThickness) {
        return;
    }

    KOMP_SET_DIRTY
    mThickness = std::max(0.0, inValue);
}


double
DrawableLine::getThickness() const
{
    return mThickness;
}


void
DrawableLine::setDegrees(double inValue)
{
    if (inValue == mDegrees) {
        return;
    }

    KOMP_SET_DIRTY
    inValue = std::fmod(inValue, 360.0);
    
    if (inValue < 0) {
        inValue += 360.0;
    }
    
    mDegrees = inValue;
    
    removeFromParent();
    mLayer = nullptr;
}


double
DrawableLine::getDegrees() const
{
    return mDegrees;
}


static constexpr const double radiansPerDegree = std::numbers::pi / 180.0;

double
DrawableLine::getRadians() const
{
    return getDegrees() * radiansPerDegree;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

