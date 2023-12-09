// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableWedge.h"

// System
#include <cmath>

// Local
#include "DrawableContext.h"
#include "DrawableLine.h"
#include "Engraver.h"
#include "Preferences.h"
#include "Wedge.h"

namespace komp
{
DrawableWedge::DrawableWedge()
{
}

void
DrawableWedge::removeFromParent() const
{
    mDrawables.removeFromParent();
    mOutline.removeFromParent();
}

void
DrawableWedge::restoreParent() const
{
    mDrawables.restoreParent();
    mOutline.restoreParent();
}

Point
DrawableWedge::getOffset() const
{
    return mDrawables.getOffset();
}


void
DrawableWedge::setOffset(const Point& inOffset)
{
    mDrawables.setOffset(inOffset);
    mOutline.setOffset(inOffset);
}

void
DrawableWedge::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
    mOutline.setZOrder(inOrder);
}

void
DrawableWedge::setScale(double inScale)
{
    mDrawables.setScale(inScale);
    mOutline.setScale(inScale);
}

void
DrawableWedge::setRotation(double inDegrees)
{
}

Rect
DrawableWedge::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}


double
DrawableWedge::getWidth() const
{
    return mDrawables.getWidth();
}


double
DrawableWedge::getHeight() const
{
    return mDrawables.getHeight();
}


Region
DrawableWedge::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}


Point
DrawableWedge::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}

DrawResult
DrawableWedge::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}

void
DrawableWedge::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
    
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}
//            const Rect frameRect(top, left, bottom, right);
//    const auto middle = mBottom - halfHeight;

//            if (selected()) {
//
//                // Draw the resizing handles
//                // TODO: This will not work when we support multiple systems per page
//                const auto handleTop = (middle - HALF_HANDLE_SIZE);
//                const auto handleBottom = (middle + HALF_HANDLE_SIZE + 1);
//
//                // Left side
//                mHandleLeft = frameRect;
//                mHandleLeft.top = handleTop;
//                mHandleLeft.right = mHandleLeft.left + HANDLE_SIZE + 1;
//                mHandleLeft.bottom = handleBottom;
//
//                // Right side
//                mHandleRight = frameRect;
//                mHandleRight.top = handleTop;
//                mHandleRight.left = mHandleRight.right - (HANDLE_SIZE + 1);
//                mHandleRight.bottom = handleBottom;
//
//                if (mType == WedgeType::CRESCENDO) {
//
//                    // Adjust the right side offset
//                    mHandleRight.left += HANDLE_OFFSET;
//                    mHandleRight.right += HANDLE_OFFSET;
//                } else {
//
//                    // Adjust the left side offset
//                    mHandleLeft.left -= HANDLE_OFFSET;
//                    mHandleLeft.right -= HANDLE_OFFSET;
//                }
//
//                // Draw the handles
//                inContext->fillRoundRect(mHandleLeft);
//                inContext->fillRoundRect(mHandleRight);
//            }
//
//            if (Preferences::instance().getShowItemFrames()) {
//                inContext->setLineWidth(1.0);
//                inContext->setStrokeColor(0.0, 0.185, 0.573, 1.0);
//                inContext->strokeRect(frameRect);
//            }


//#ifdef KOMP_BUILD_APPLE
//            const komp::Region region = CGPathCreateMutable();
//            const auto path = region.getMutable();
//            CGPathMoveToPoint(path,
//                              NULL,
//                              mLeft.x,
//                              mLeft.y);
//
//            CGPathAddCurveToPoint(path,
//                                  NULL,
//                                  mLeftInner.x, mLeftInner.y,
//                                  mRightInner.x, mRightInner.y,
//                                  mRight.x, mRight.y);
//
//            CGPathAddCurveToPoint(path,
//                                  NULL,
//                                  mRightInner.x, mRightInner.y,
//                                  mLeftInner.x, mLeftInner.y,
//                                  mLeft.x, mLeft.y);
//
//            ::CGPathCloseSubpath(path);
//
//            drawResult.region = ::CGPathCreateCopyByStrokingPath(path,
//                                                                 NULL,
//                                                                 1.0,
//                                                                 kCGLineCapButt,
//                                                                 kCGLineJoinMiter,
//                                                                 CGFloat{0.0});
//
//            drawResult.frame = CGPathGetBoundingBox(drawResult.region);
//
//            DrawableLayerFactory factory;
//            drawResult.layer = factory.createCurve(path);
//            drawResult.layer->setFillColor(inDrawParams.colors.fillColor);
//            drawResult.layer->setStrokeColor(inDrawParams.colors.fillColor);
//            drawResult.layer->setLineWidth(1.0);
//#else
//            drawResult.region = nullptr;
//#endif

void
DrawableWedge::configure(WedgeType inType, Rect inFrame, double inThickness)
{
    mDrawables.clearDrawables();
    
    mType = inType;
    mFrame = inFrame;
    mThickness = inThickness;
    
    const auto halfHeight = mFrame.getHalfHeight();
    const auto middle = mFrame.top + halfHeight;
    
    const Point origin{ mFrame.left, middle};

    {
        const auto rightBottom = mFrame.rightBottom();
        const auto length = origin.distance(rightBottom);
        const auto degrees = origin.degrees(rightBottom);
        
        auto line = std::make_unique<DrawableLine>();
        line->setLength(length);
        line->setDegrees(degrees);
        line->setThickness(mThickness);
        if (mType == WedgeType::DIMINUENDO) {
            line->setOffset({0, -halfHeight});
        }
        mDrawables.addDrawable(std::move(line));
    }
    
    {
        const auto rightTop = mFrame.rightTop();
        const auto length = origin.distance(rightTop);
        const auto degrees = origin.degrees(rightTop);
        
        auto line = std::make_unique<DrawableLine>();
        line->setLength(length);
        line->setDegrees(degrees);
        line->setThickness(mThickness);
        if (mType == WedgeType::DIMINUENDO) {
            line->setOffset({0, halfHeight});
        }
        mDrawables.addDrawable(std::move(line));
    }
        
    mOutline.setBounds(getFrame(Point{}));
}

// MARK: - ItemObserver

void
DrawableWedge::observedItemChanged(const Item& observedItem,
                                   const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto wedge = dynamic_cast<const Wedge*>(&observedItem);
        configure(wedge->getWedgeType(),
                  wedge->getPositions(),
                  Engraver::instance().wedgeLineWidth());
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

