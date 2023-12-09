// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableCurve.h"

// System
#include <cmath>

// Local
#include "Curve.h"
#include "DrawableContext.h"
#include "DrawableLayerFactory.h"
#include "Preferences.h"

namespace komp
{
static constexpr const double Thickness = 1.25;
static constexpr const double LineWidth = 0.5;

DrawableCurve::DrawableCurve()
: mOffset{0.0, 0.0}
, mZOrder{0}
, mCache{}
{
    
}

void
DrawableCurve::removeFromParent() const
{
    if (mLayer) {
        mLayer->removeFromParent();
        mOutline.removeFromParent();
        mDrawableCPStart.removeFromParent();
        mDrawableCPEnd.removeFromParent();
    }
}

void
DrawableCurve::restoreParent() const
{
    if (mLayer) {
        mLayer->restoreParent();
        mOutline.restoreParent();
        mDrawableCPStart.restoreParent();
        mDrawableCPEnd.restoreParent();
    }
}

Point
DrawableCurve::getOffset() const
{
    return mOffset;
}

void
DrawableCurve::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableCurve::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mZOrder = inOrder;
}

void
DrawableCurve::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mScale = inScale;
}

void
DrawableCurve::setRotation(double inDegrees)
{
}

Rect
DrawableCurve::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableCurve::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableCurve::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableCurve::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableCurve::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableCurve::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        
        DrawResult result;
        
#ifdef KOMP_BUILD_APPLE
        const komp::Region region = CGPathCreateMutable();
        const auto path = region.getMutable();
#elif KOMP_BUILD_LINUX
        komp::Region region = QPainterPath();
        auto& path = region.getPath();
#endif
        
#ifdef KOMP_BUILD_APPLE
        CGPathMoveToPoint(path,
                          NULL,
                          mLeft.x,
                          mLeft.y);
        
        CGPathAddCurveToPoint(path,
                              NULL,
                              mLeftInner.x, mLeftInner.y,
                              mRightInner.x, mRightInner.y,
                              mRight.x, mRight.y);
        
        CGPathAddCurveToPoint(path,
                              NULL,
                              mRightInner.x, mRightInner.y - Thickness,
                              mLeftInner.x, mLeftInner.y - Thickness,
                              mLeft.x, mLeft.y);
        
        result.region = ::CGPathCreateCopy(path);
        result.frame = CGPathGetBoundingBox(result.region);
#elif KOMP_BUILD_LINUX
        path.moveTo({mLeft.x, mLeft.y});
        path.cubicTo({mLeftInner.x, mLeftInner.y},
                     {mRightInner.x, mRightInner.y},
                     {mRight.x, mRight.y});
        path.cubicTo({mRightInner.x, mRightInner.y - Thickness},
                     {mLeftInner.x, mLeftInner.y - Thickness},
                     {mLeft.x, mLeft.y});
        
        result.region = region;
        result.frame = path.boundingRect();
#endif
        mCache.setCache(result, inPoint);
        
        if (mLayer) {
            removeFromParent();
            mLayer = nullptr;
            
            mOutline.removeFromParent();
            mDrawableCPStart.removeFromParent();
            mDrawableCPEnd.removeFromParent();
        }
    }
    
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableCurve::draw(const DrawParams& inDrawParams) const
{
    if (!mLayer) {
        DrawableLayerFactory factory;
        
        const auto metrics = getMetrics(inDrawParams.drawPoint);
        mLayer = factory.createCurve(inDrawParams.context,
                                     metrics.region);
        mLayer->restoreParent();
        mLayer->setLineWidth(LineWidth);
        mLayer->setFillColor(inDrawParams.colors.fillColor);
    }
    
#if 0
    if (Preferences::instance().getShowItemFrames()) {
        
        static constexpr const double CPSize = 2.0;
        static constexpr const double CPHalfSize = CPSize / 2.0;
        
        const auto curveFrame = Rect{mLeft.y, mLeft.x, mRight.y, mRight.x};
        mOutline.setBounds(curveFrame);
        
        const auto cpFrameStart = Rect(mLeftInner.y - CPHalfSize,
                                       mLeftInner.x - CPHalfSize,
                                       mLeftInner.y + CPHalfSize,
                                       mLeftInner.x + CPHalfSize);
        mDrawableCPStart.setBounds(cpFrameStart);
        
        const auto cpFrameEnd = Rect(mRightInner.y - CPHalfSize,
                                     mRightInner.x - CPHalfSize,
                                     mRightInner.y + CPHalfSize,
                                     mRightInner.x + CPHalfSize);
        mDrawableCPEnd.setBounds(cpFrameEnd);
        
        mOutline.restoreParent();
        mDrawableCPStart.restoreParent();
        mDrawableCPEnd.restoreParent();
        
        const DrawParams cparams {
            inDrawParams.context,
            Point{},
            inDrawParams.colors
        };
        mOutline.draw(cparams);
        mDrawableCPStart.draw(cparams);
        mDrawableCPEnd.draw(cparams);
    } else {
        mOutline.removeFromParent();
        mDrawableCPStart.removeFromParent();
        mDrawableCPEnd.removeFromParent();
    }
    
    mLayer->setZOrder(mZOrder);
    mLayer->setScale(mScale);
    mLayer->setFillColor(inDrawParams.colors.fillColor);
    mLayer->setStrokeColor(inDrawParams.colors.fillColor);
#endif
}

void
DrawableCurve::setPoints(const Point& inLeft,
                         const Point& inLeftInner,
                         const Point& inRightInner,
                         const Point& inRight)
{
    if (mLeft == inLeft
        && mLeftInner == inLeftInner
        && mRightInner == inRightInner
        && mRight == inRight) {
        return;
    }
    
    KOMP_SET_DIRTY
    mLeft = inLeft;
    mLeftInner = inLeftInner;
    mRightInner = inRightInner;
    mRight = inRight;
    
    removeFromParent();
    mOutline.removeFromParent();
    mDrawableCPStart.removeFromParent();
    mDrawableCPEnd.removeFromParent();
    
    mLayer = nullptr;
}

// MARK: - ItemObserver

void
DrawableCurve::observedItemChanged(const Item& observedItem,
                                   const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto curve = dynamic_cast<const Curve*>(&observedItem);
        Point start, cpOne, cpTwo, end;
        curve->getPoints(start, cpOne, cpTwo, end);
        setPoints(start, cpOne, cpTwo, end);
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

