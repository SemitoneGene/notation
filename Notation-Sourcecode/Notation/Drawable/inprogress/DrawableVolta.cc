// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableVolta.h"

// Local
#include "DrawableLine.h"
#include "Engraver.h"
#include "Staff.h"
#include "Temporal.h"
#include "Temporal.h"
#include "Throw.h"

namespace komp
{
DrawableVolta::DrawableVolta()
: mLines{}
, mCache{}
{
    
}

DrawableVolta::~DrawableVolta() = default;

DrawableUPtr
DrawableVolta::clone() const
{
    return std::make_unique<DrawableVolta>(*this);
}

void
DrawableVolta::removeFromParent() const
{
    mLines.removeFromParent();
}

void
DrawableVolta::restoreParent() const
{
    mLines.restoreParent();
}

Point
DrawableVolta::getOffset() const
{
    return mLines.getOffset();
}

void
DrawableVolta::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mLines.setOffset(inOffset);
}

void
DrawableVolta::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mLines.setZOrder(inOrder);
}

void
DrawableVolta::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mLines.setScale(inScale);
}

Rect
DrawableVolta::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}

double
DrawableVolta::getWidth() const
{
    return getFrame(Point{}).getWidth();
}

double
DrawableVolta::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableVolta::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableVolta::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableVolta::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        const auto result = mLines.getMetrics(inPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableVolta::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mLines.draw(inDrawParams);
}


//void
//DrawableVolta::update(const Temporal& inTemporal)
//{
//    if (!inTemporal.getIsNote()) {
//        clearState();
//        return;
//    }
//    
//    const auto& staff = inTemporal.getLocation().getStaff();
//    const auto slot = inTemporal.getSlot();
//    
//    if (!staff.getIsLegerAbove(slot) && !staff.getIsLegerBelow(slot)) {
//        clearState();
//        return;
//    }
//    
//    // TODO: See if we can avoid doing this if the values haven't changed
//    KOMP_SET_DIRTY
//    mLines.clearDrawables();
//    
//    const auto thickness = Engraver::instance().legerLineWidth();
//    const auto drawPoint = inTemporal.getDrawPoint();
//    const auto noteX = drawPoint.x;
//    const auto headWidth = inTemporal.getFrames().head.getWidth();
//    const auto legerLength = headWidth * 1.25;
//    const auto offset = (legerLength - headWidth) / 2.0;
//    const auto startX = noteX - offset;
//    DrawableLine legerLine;
//    legerLine.setLength(legerLength);
//    legerLine.setThickness(thickness);
//    legerLine.setDegrees(0.0);
//    
//    if (staff.getIsLegerAbove(slot)) {
//        int legerSlot = staff.getTopLineSlot() + 2;
//        for ( ; legerSlot <= slot; legerSlot += 2) {
//            const auto posY = staff.slotToPixel(legerSlot);
//            const auto relativeX = startX - drawPoint.x;
//            const auto relativeY = posY - drawPoint.y;
//            legerLine.setOffset(Point{relativeX, relativeY});
//            mLines.addDrawable(legerLine.clone());
//        }
//    }
//    
//    if (staff.getIsLegerBelow(slot)) {
//        int legerSlot = staff.getBottomLineSlot() - 2;
//        for ( ; legerSlot >= slot; legerSlot -= 2) {
//            const auto posY = staff.slotToPixel(legerSlot);
//            const auto relativeX = startX - drawPoint.x;
//            const auto relativeY = posY - drawPoint.y;
//            legerLine.setOffset(Point{relativeX, relativeY});
//            mLines.addDrawable(legerLine.clone());
//        }
//    }
//}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

