// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableNoteFlags.h"

// System
#include <cmath>

// Local
#include "Throw.h"
#include "TemporalValue.h"
#include "FontTheme.h"

namespace komp
{

DrawableNoteFlags::DrawableNoteFlags()
{
}

DrawableNoteFlags::~DrawableNoteFlags() = default;

void
DrawableNoteFlags::removeFromParent() const
{
    mFlags.removeFromParent();
}

void
DrawableNoteFlags::restoreParent() const
{
    mFlags.restoreParent();
}

Point
DrawableNoteFlags::getOffset() const
{
    return mFlags.getOffset();
}


void
DrawableNoteFlags::setOffset(const Point& inOffset)
{
    if (mFlags.getOffset() == inOffset) {
        return;
    }

    KOMP_SET_DIRTY
    mFlags.setOffset(inOffset);
}

void
DrawableNoteFlags::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mFlags.setZOrder(inOrder);
}

void
DrawableNoteFlags::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mFlags.setScale(inScale);
}

void
DrawableNoteFlags::setRotation(double inDegrees)
{
}

Rect
DrawableNoteFlags::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableNoteFlags::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableNoteFlags::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableNoteFlags::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableNoteFlags::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableNoteFlags::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        const auto result = mFlags.getMetrics(inPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableNoteFlags::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mFlags.draw(inDrawParams);
}


void
DrawableNoteFlags::update(const StemInfo& inInfo,
                          const StemPolicy& inPolicy,
                          const DrawableStem& inStem)
{
    if (!inInfo.hasFlag) {
        return;
    }
    
    KOMP_SET_DIRTY
    const auto stemFrame = inStem.getFrame(Point{});
    const auto stemEndY = inPolicy.getIsStemUp() ? stemFrame.top : stemFrame.bottom;
    const auto stemX = stemFrame.left;
    const auto flagX = stemX;
    const auto flagY = stemEndY;
    setOffset(Point{flagX, flagY});
    const auto stemDir = inPolicy.getIsStemUp() ? UpDown::UP : UpDown::DOWN;
    const auto flagType = TemporalValue::getTemporalFlagType(inInfo.temporalType,
                                                             stemDir);
    const auto flagChar = FontTheme::instance().FlagMap[flagType];
    mFlags.setText(DrawableChars{flagChar});
    mFlags.setFontSize(inInfo.flagsFontSize);
    
    // TODO: See if we can avoid doing all the work in this function
    // when the values haven't actually changed
}


void
DrawableNoteFlags::clearState()
{
    static const auto text = DrawableChars{};
    if (mFlags.getText() != text) {
        mFlags.setText(text);
        KOMP_SET_DIRTY
    }
    
    static const auto offset = Point{0.0, 0.0};
    if (mFlags.getOffset() != offset) {
        mFlags.setOffset(offset);
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

