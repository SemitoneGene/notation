// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableStem.h"

// System
#include <cmath>

// Local
#include "Throw.h"
#include "TemporalValue.h"
#include "TemporalImpl.h"
#include "Staff.h"

namespace komp
{
DrawableStem::DrawableStem()
: mLine{}
, mCache{}
{
    
}

DrawableStem::~DrawableStem() = default;


void
DrawableStem::removeFromParent() const
{
    mLine.removeFromParent();
}

void
DrawableStem::restoreParent() const
{
    mLine.restoreParent();
}

Point
DrawableStem::getOffset() const
{
    return mLine.getOffset();
}

void
DrawableStem::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mLine.setOffset(inOffset);
}

void
DrawableStem::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mLine.setZOrder(inOrder);
}

void
DrawableStem::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mLine.setScale(inScale);
}


void
DrawableStem::setRotation(double inDegrees)
{
}

Rect
DrawableStem::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableStem::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableStem::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableStem::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableStem::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableStem::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        const auto result = mLine.getMetrics(inPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableStem::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mLine.draw(inDrawParams);
}


void
DrawableStem::update(const StemInfo& inInfo, const StemPolicy& inPolicy)
{
    KOMP_SET_DIRTY
    if (!inPolicy.getHasStem()) {
        clearState();
        return;
    }
    
    if (inPolicy.direction == StemDirection::BOTH) {
        KOMP_THROW("StemDirection::BOTH is not implemented");
    }
    
    setDirection(inInfo, inPolicy);
    
    if (inInfo.thickness != mLine.getThickness()) {
        mLine.setThickness(inInfo.thickness);
    }
    
    setLength(inInfo, inPolicy);
}


void
DrawableStem::clearState()
{
    KOMP_SET_DIRTY
    mLine.clearState();
}


void
DrawableStem::setDirection(const StemInfo& inInfo, const StemPolicy& inPolicy)
{
    KOMP_SET_DIRTY
    mStemInfo = inInfo;
    mStemInfo.staff = nullptr; // we don't need this
    mStemPolicy = inPolicy;
    const bool isUp = inPolicy.getIsStemUp();
    const auto offset = isUp ? 0 : inInfo.thickness / 2.0;
    
    if (isUp) {
        mLine.setDegrees(270.0);
        setOffset(Point{inInfo.xUp, inInfo.yUp});
    } else {
        mLine.setDegrees(90.0);
        setOffset({inInfo.xDown + offset, inInfo.yDown});
    }
}


void
DrawableStem::setLength(const StemInfo& inInfo, const StemPolicy& inPolicy)
{
    KOMP_SET_DIRTY
    mStemInfo = inInfo;
    mStemInfo.staff = nullptr; // we don't need this
    mStemPolicy = inPolicy;
    if (!inPolicy.getHasStem()) {
        mLine.setLength(0.0);
        return;
    }
    
    const auto isUp = inPolicy.getIsStemUp();
    const auto idealSlots = inInfo.idealLength * 2;
    const auto direction = isUp ? 1 : -1;
    double endSlot = static_cast<double>(inInfo.escapeSlot) + (direction * idealSlots);
    
    if (isUp && endSlot < 6.0) {
        endSlot = 6.0;
    } else if (!isUp && endSlot > 6.0) {
        endSlot = 6.0;
    }
    
    const auto numFlags = TemporalValue::getNumFlags(inInfo.temporalType);
    const auto slotSize = Staff::getSlotSize();
    
    // trial and error magic
    if (numFlags == 1 && inInfo.hasFlag) {
        endSlot += -1 * direction * (0.153 / slotSize);
    } else if (numFlags == 2 && inInfo.hasFlag) {
        if (isUp) {
            endSlot += -1 * direction * (0.034 / slotSize);
        } else {
            endSlot += -1 * direction * (0.153 / slotSize);
        }
    }  else if (numFlags == 3 && inInfo.hasFlag) {
        if (isUp) {
            endSlot += -1 * direction * (0.533 / slotSize);
        } else {
            endSlot += -1 * direction * (0.924 / slotSize);
        }
    }
    
    const auto attachScoreY = inInfo.temporalY + (isUp ? inInfo.yUp : inInfo.yDown);
    const auto endScoreY = inInfo.staff->slotToPixel(endSlot);
    const auto stemLength = std::abs(attachScoreY - endScoreY);
    mLine.setLength(stemLength);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

