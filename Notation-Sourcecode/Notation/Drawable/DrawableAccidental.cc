// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableAccidental.h"

// System
#include <cmath>

// Local
#include "FontTheme.h"
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{
DrawableAccidental::DrawableAccidental()
{
}

DrawableAccidental::DrawableAccidental(const Item& inItem)
{
}

DrawableAccidental::~DrawableAccidental() = default;

void
DrawableAccidental::removeFromParent() const
{
    mText.removeFromParent();
}

void
DrawableAccidental::restoreParent() const
{
    mText.restoreParent();
}

Point
DrawableAccidental::getOffset() const
{
    return mText.getOffset();
}


void
DrawableAccidental::setOffset(const Point& inOffset)
{
    if (inOffset == mText.getOffset()) {
        return;
    }
    
    KOMP_SET_DIRTY
    mText.setOffset(inOffset);
}

void
DrawableAccidental::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mText.setZOrder(inOrder);
}

void
DrawableAccidental::setScale(double inScale)
{
    mText.setScale(inScale);
}

void
DrawableAccidental::setRotation(double inDegrees)
{
}

Rect
DrawableAccidental::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableAccidental::getWidth() const
{
    return mCache.getWidth(*this);
}


double
DrawableAccidental::getHeight() const
{
    return mCache.getHeight(*this);
}


Region
DrawableAccidental::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableAccidental::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableAccidental::getMetrics(const Point& inPoint) const
{
    return mText.getMetrics(inPoint);
}


void
DrawableAccidental::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mText.draw(inDrawParams);
}

void
DrawableAccidental::clearState()
{
    auto isDirty = false;
    
    const auto text = DrawableChars{};
    if (text != mText.getText()) {
        mText.setText(text);
        isDirty = true;
    }

    const auto offset = Point{0.0, 0.0};
    if (offset != mText.getOffset()) {
        mText.setOffset(offset);
        isDirty = true;
    }

    if (isDirty) {
        KOMP_SET_DIRTY
    }
}


void
DrawableAccidental::setFontSize(double inFontSize)
{
    if (mText.getFontSize() == inFontSize) {
        return;
    }
    
    KOMP_SET_DIRTY
    mText.setFontSize(inFontSize);
}


const AccidentalType&
DrawableAccidental::getAccidentalType() const
{
    return mAccidentalType;
}


void
DrawableAccidental::setAccidentalType(AccidentalType inAccidentalType)
{
    if (inAccidentalType == mAccidentalType) {
        return;
    }
    KOMP_SET_DIRTY
    mAccidentalType = inAccidentalType;
    if (!AccidentalInfo::isPitchAltering(inAccidentalType)) {
        clearState();
        return;
    }
    
    const auto& accidentalData = FontTheme::instance().AccidentalMap.at(getAccidentalType());
    mText.setText(DrawableChars{accidentalData.character});
}


unsigned short
DrawableAccidental::getCharacter() const
{
    if (mText.getText().size() != 1) {
        return (unsigned short){0};
    }
    
    return mText.getText().front().getChar();
}

void
DrawableAccidental::observedItemChanged(const Item& observedItem,
                                        const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

