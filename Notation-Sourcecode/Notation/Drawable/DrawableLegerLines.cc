// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableLegerLines.h"

// Local
#include "DrawableLine.h"
#include "Engraver.h"
#include "Staff.h"
#include "Temporal.h"
#include "Temporal.h"
#include "Throw.h"

namespace komp
{
DrawableLegerLines::DrawableLegerLines()
: mLines{}
, mCache{}
{
    
}

DrawableLegerLines::~DrawableLegerLines() = default;

void
DrawableLegerLines::removeFromParent() const
{
    mLines.removeFromParent();
}

void
DrawableLegerLines::restoreParent() const
{
    mLines.restoreParent();
}

Point
DrawableLegerLines::getOffset() const
{
    return mLines.getOffset();
}

void
DrawableLegerLines::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mLines.setOffset(inOffset);
}

void
DrawableLegerLines::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mLines.setZOrder(inOrder);
}

void
DrawableLegerLines::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mLines.setScale(inScale);
}

void
DrawableLegerLines::setRotation(double inDegrees)
{
}

Rect
DrawableLegerLines::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}

double
DrawableLegerLines::getWidth() const
{
    return getFrame(Point{}).getWidth();
}

double
DrawableLegerLines::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableLegerLines::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableLegerLines::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableLegerLines::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        const auto result = mLines.getMetrics(inPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableLegerLines::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mLines.draw(inDrawParams);
}


void
DrawableLegerLines::update(const Temporal& inTemporal)
{
    if (!inTemporal.getIsNote()) {
        clearState();
        return;
    }
    
    const auto& staff = inTemporal.getLocation().getStaff();
    const auto slot = inTemporal.getSlot();
    
    if (!staff.getIsLegerAbove(slot) && !staff.getIsLegerBelow(slot)) {
        clearState();
        return;
    }
    
    // TODO: See if we can avoid doing this if the values haven't changed
    KOMP_SET_DIRTY
    mLines.clearDrawables();
    
    const auto thickness = Engraver::instance().legerLineWidth();
    const auto drawPoint = inTemporal.getDrawPoint();
    const auto noteX = drawPoint.x;
    const auto headWidth = inTemporal.getFrames().head.getWidth();
    const auto legerLength = headWidth * 1.25;
    const auto offset = (legerLength - headWidth) / 2.0;
    const auto startX = noteX - offset;
    
    if (staff.getIsLegerAbove(slot)) {
        int legerSlot = staff.getTopLineSlot() + 2;
        for ( ; legerSlot <= slot; legerSlot += 2) {
            const auto posY = staff.slotToPixel(legerSlot);
            const auto relativeX = startX - drawPoint.x;
            const auto relativeY = posY - drawPoint.y;
            auto legerLine = std::make_unique<DrawableLine>();
            legerLine->setLength(legerLength);
            legerLine->setThickness(thickness);
            legerLine->setDegrees(0.0);
            legerLine->setOffset(Point{relativeX, relativeY});
            mLines.addDrawable(std::move(legerLine));
        }
    }
    
    if (staff.getIsLegerBelow(slot)) {
        int legerSlot = staff.getBottomLineSlot() - 2;
        for ( ; legerSlot >= slot; legerSlot -= 2) {
            const auto posY = staff.slotToPixel(legerSlot);
            const auto relativeX = startX - drawPoint.x;
            const auto relativeY = posY - drawPoint.y;
            auto legerLine = std::make_unique<DrawableLine>();
            legerLine->setLength(legerLength);
            legerLine->setThickness(thickness);
            legerLine->setDegrees(0.0);
            legerLine->setOffset(Point{relativeX, relativeY});
            mLines.addDrawable(std::move(legerLine));
        }
    }
}


void
DrawableLegerLines::clearState()
{
    KOMP_SET_DIRTY
    mLines.clearDrawables();
    mLines.setOffset(Point{0.0, 0.0});
}


int
DrawableLegerLines::getLegerLineCount() const
{
    return mLines.getDrawableCount();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

