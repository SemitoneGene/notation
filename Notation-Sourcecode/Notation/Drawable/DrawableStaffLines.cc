// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableStaffLines.h"

// Local
#include "DrawableLine.h"
#include "Engraver.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "Throw.h"

namespace komp
{
DrawableStaffLines::DrawableStaffLines()
: mLines{}
, mCache{}
{
    
}

DrawableStaffLines::~DrawableStaffLines() = default;

void
DrawableStaffLines::removeFromParent() const
{
    mLines.removeFromParent();
}

void
DrawableStaffLines::restoreParent() const
{
    mLines.restoreParent();
}

Point
DrawableStaffLines::getOffset() const
{
    return mLines.getOffset();
}

void
DrawableStaffLines::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mLines.setOffset(inOffset);
}

void
DrawableStaffLines::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mLines.setZOrder(inOrder);
}

void
DrawableStaffLines::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mLines.setScale(inScale);
}

void
DrawableStaffLines::setRotation(double inDegrees)
{
}

Rect
DrawableStaffLines::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}

double
DrawableStaffLines::getWidth() const
{
    return getFrame(Point{}).getWidth();
}

double
DrawableStaffLines::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableStaffLines::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableStaffLines::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableStaffLines::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        auto result = mLines.getMetrics(inPoint);
        result.anchor = inPoint;
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableStaffLines::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mLines.draw(inDrawParams);
}

void
DrawableStaffLines::update(const Staff& inStaff)
{
    const auto length = inStaff.getStaffSystem()->getWidth();
    const auto thickness = Engraver::instance().staffLineWidth();
    const auto count = inStaff.getStaffLineCount();
    const auto spacing = inStaff.getSpacing();
    
    // Handle percussion and tablature staves
    auto initialOffset = 0;
    if (count == 1) {
        initialOffset = 2 * spacing;
    }

    for (auto index = 0; index < count; index++) {
        const auto offset = initialOffset + (index * spacing);
        auto line = std::make_unique<DrawableLine>();
        line->setThickness(thickness);
        line->setLength(length);
        line->setOffset({0, offset});
        mLines.addDrawable(std::move(line));
    }
}

void
DrawableStaffLines::clearState()
{
    if (mLines.getDrawableCount() != 0 || mLines.getOffset() != POINT_ZERO) {
        KOMP_SET_DIRTY
        mLines.clearDrawables();
        mLines.setOffset(Point{0.0, 0.0});
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

