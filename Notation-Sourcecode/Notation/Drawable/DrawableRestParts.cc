// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableRestParts.h"

// System
#include <cmath>

// Local
#include "Preferences.h"
#include "Throw.h"
#include "Temporal.h"
#include "TemporalValue.h"

namespace komp
{

DrawableRestParts::DrawableRestParts()
{
}

void
DrawableRestParts::removeFromParent() const
{
    mNotehead.removeFromParent();
    mDrawableNoteDots.removeFromParent();
    mOutline.removeFromParent();
}

void
DrawableRestParts::restoreParent() const
{
    mNotehead.restoreParent();
    mDrawableNoteDots.restoreParent();
    mOutline.restoreParent();
}

Point
DrawableRestParts::getOffset() const
{
    return mOffset;
}


void
DrawableRestParts::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableRestParts::setZOrder(int inOrder)
{
    mNotehead.setZOrder(inOrder);
    mDrawableNoteDots.setZOrder(inOrder);
    mOutline.setZOrder(inOrder);
}

void
DrawableRestParts::setScale(double inScale)
{
    mNotehead.setScale(inScale);
    mDrawableNoteDots.setScale(inScale);
    mOutline.setScale(inScale);
}

void
DrawableRestParts::setRotation(double inDegrees)
{
}

Rect
DrawableRestParts::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableRestParts::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableRestParts::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableRestParts::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableRestParts::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableRestParts::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        
        DrawResult result;
        
        const auto headResult = getNoteheadMetrics(inPoint);
        const auto dotsResult = getDrawableNoteDotsMetrics(inPoint);
        
        result.frame.initializeOrUnion(headResult.frame);
        result.frame.initializeOrUnion(dotsResult.frame);

#ifdef KOMP_BUILD_APPLE
        const auto region = result.region;
        if (region) {
            ::CGPathAddRect(region.getMutable(), NULL, result.frame);
        }
#elif KOMP_BUILD_LINUX
        result.region.addRect(result.frame);
#endif
        result.anchor = inPoint;
        mCache.setCache(result, inPoint);
    }
    
    return mCache.getDrawResult(*this, inPoint);
    
}


void
DrawableRestParts::draw(const DrawParams& inDrawParams) const
{
    const auto metrics = getMetrics(inDrawParams.drawPoint);
    drawNotehead(inDrawParams);
    drawDrawableNoteDots(inDrawParams);
    drawDrawableOutline(metrics, inDrawParams);
}


void
DrawableRestParts::update(const Item& inItem)
{
    KOMP_SET_DIRTY
    
    const auto temporal = dynamic_cast<const Temporal*>(&inItem);
    if (!temporal->getIsChord()) {
        mDrawNotehead = true;
        mNotehead.update(*temporal);
    } else {
        mDrawNotehead = false;
        mNotehead.clearState();
    }
    
    const auto duration = temporal->getDuration();
    
    if (duration.getDots() > 0 &&
        !temporal->getIsRaw() &&
        !temporal->getIsChord()) {
        mDrawDrawableNoteDots = true;
        mDrawableNoteDots.update(*temporal);
    } else {
        mDrawDrawableNoteDots = false;
        mDrawableNoteDots.clearDots();
    }
}

void
DrawableRestParts::clearState()
{
    KOMP_SET_DIRTY
    setOffset(Point{0.0, 0.0});
    mDrawNotehead = false;
    mNotehead.clearState();
    mDrawDrawableNoteDots = false;
    mDrawableNoteDots.clearDots();
}


void
DrawableRestParts::setSpacesBeforeFirstDot(double inValue)
{
    KOMP_SET_DIRTY
    mDrawableNoteDots.setSpacesBeforeFirstDot(inValue);
}


void
DrawableRestParts::setSpacesBetweenDots(double inValue)
{
    KOMP_SET_DIRTY
    mDrawableNoteDots.setSpacesBetweenDots(inValue);
}


DrawResult
DrawableRestParts::getNoteheadMetrics(const Point& inPoint) const
{
    if (!mDrawNotehead) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mNotehead.getMetrics(drawPoint);
    
}

DrawResult
DrawableRestParts::getDrawableNoteDotsMetrics(const Point& inPoint) const
{
    if (!mDrawDrawableNoteDots) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mDrawableNoteDots.getMetrics(drawPoint);
}


void
DrawableRestParts::drawNotehead(const DrawParams& inDrawParams) const
{
    if (!mDrawNotehead) {
        return;
    }
    
    DrawParams drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    return mNotehead.draw(drawParams);
}


void
DrawableRestParts::drawDrawableNoteDots(const DrawParams& inDrawParams) const
{
    if (!mDrawDrawableNoteDots) {
        return;
    }
    
    DrawParams drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    mDrawableNoteDots.draw(drawParams);
}

void
DrawableRestParts::drawDrawableOutline(const DrawResult& inDrawResult,
                                       const DrawParams& inDrawParams) const
{
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();
        auto bounds = inDrawResult.frame;
        bounds.moveTo(0, 0);
        mOutline.setBounds(bounds);
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}


Rect
DrawableRestParts::getNoteheadFrame(const Point& inPoint) const
{
    return mNotehead.getFrame(inPoint);
}


Rect
DrawableRestParts::getNoteDotsFrame(const Point& inPoint) const
{
    return mDrawableNoteDots.getFrame(inPoint);
}


Region
DrawableRestParts::getNoteheadRegion(const Point& inPoint) const
{
    return mNotehead.getRegion(inPoint);
}


Region
DrawableRestParts::getNoteDotsRegion(const Point& inPoint) const
{
    return mDrawableNoteDots.getRegion(inPoint);
}


Point
DrawableRestParts::getNoteheadAnchor(const Point& inPoint) const
{
    return mNotehead.getAnchor(inPoint);
}


Point
DrawableRestParts::getNoteDotsAnchor(const Point& inPoint) const
{
    return mDrawableNoteDots.getAnchor(inPoint);
}

void
DrawableRestParts::observedItemChanged(const Item& observedItem,
                                       const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        update(observedItem);
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

