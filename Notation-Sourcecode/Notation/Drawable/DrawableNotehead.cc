// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableNotehead.h"

// System
#include <cmath>

// Local
#include "TemporalValue.h"
#include "Temporal.h"
#include "Throw.h"

namespace komp
{
DrawableNotehead::DrawableNotehead()
: mCache{}
{
}

DrawableNotehead::~DrawableNotehead() = default;

void
DrawableNotehead::removeFromParent() const
{
    mHead.removeFromParent();
}

void
DrawableNotehead::restoreParent() const
{
    mHead.restoreParent();
}

Point
DrawableNotehead::getOffset() const
{
    return mHead.getOffset();
}

void
DrawableNotehead::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mHead.setOffset(inOffset);
}

void
DrawableNotehead::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mHead.setZOrder(inOrder);
}

void
DrawableNotehead::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mHead.setScale(inScale);
}

void
DrawableNotehead::setRotation(double inDegrees)
{
}

Rect
DrawableNotehead::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}

double
DrawableNotehead::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableNotehead::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableNotehead::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableNotehead::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableNotehead::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        const auto result = mHead.getMetrics(inPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableNotehead::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mHead.draw(inDrawParams);
}


void
DrawableNotehead::update(const Temporal& inTemporal)
{
    if (inTemporal.getIsNote()) {
        updateNote(inTemporal);
    } else if (inTemporal.getIsRest()) {
        updateRest(inTemporal);
    } else {
        clearState();
    }
}


void
DrawableNotehead::clearState()
{
    KOMP_SET_DIRTY
    mHead.setText(DrawableChars{});
    mHead.setOffset(Point{0.0, 0.0});
}


void
DrawableNotehead::updateNote(const Temporal& inTemporal)
{
    if (areFloatsEqual(inTemporal.getFontSize(), mHead.getFontSize())
        && mHead.getText().size() == 1
        && mHead.getText()[0] == inTemporal.getGlyph()) {
        return;
    }
    
    KOMP_SET_DIRTY
    mHead.setFontSize(inTemporal.getFontSize());
    mHead.setText(DrawableChars{inTemporal.getGlyph()});
}


void
DrawableNotehead::updateRest(const Temporal& inTemporal)
{
    if (areFloatsEqual(inTemporal.getFontSize(), mHead.getFontSize())
        && mHead.getText().size() == 1
        && mHead.getText()[0] == inTemporal.getGlyph()) {
        return;
    }
    
    KOMP_SET_DIRTY
    mHead.setFontSize(inTemporal.getFontSize());
    mHead.setText(DrawableChars{inTemporal.getGlyph()});
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

