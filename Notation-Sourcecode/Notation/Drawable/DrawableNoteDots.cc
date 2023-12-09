// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableNoteDots.h"

// System
#include <cmath>

// Local
#include "Throw.h"
#include "TemporalValue.h"
#include "FontTheme.h"
#include "Temporal.h"
#include "TemporalConstants.h"
#include "DrawableText.h"
#include "Staff.h"

namespace komp
{

DrawableNoteDots::~DrawableNoteDots() = default;

DrawableNoteDots::DrawableNoteDots()
: mComposite{}
, mSpacesBeforeFirstDot{SPACES_BEFORE_FIRST_DOT}
, mSpacesBetweenDots{SPACES_BETWEEN_DOTS}
, mCache{}
{
    
}

void
DrawableNoteDots::removeFromParent() const
{
    mComposite.removeFromParent();
}

void
DrawableNoteDots::restoreParent() const
{
    mComposite.restoreParent();
}

Point
DrawableNoteDots::getOffset() const
{
    return mComposite.getOffset();
}


void
DrawableNoteDots::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mComposite.setOffset(inOffset);
}

void
DrawableNoteDots::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mComposite.setZOrder(inOrder);
}

void
DrawableNoteDots::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mComposite.setScale(inScale);
}

void
DrawableNoteDots::setRotation(double inDegrees)
{
}

Rect
DrawableNoteDots::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}

double
DrawableNoteDots::getWidth() const
{
    return getFrame(Point{}).getHeight();
}


double
DrawableNoteDots::getHeight() const
{
    return getFrame(Point{}).getWidth();
}


Region
DrawableNoteDots::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}

Point
DrawableNoteDots::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableNoteDots::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        const auto result = mComposite.getMetrics(inPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableNoteDots::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mComposite.draw(inDrawParams);
}

bool
DrawableNoteDots::clearDots()
{
    if (mComposite.getDrawableCount() == 0
        && mComposite.getOffset() == POINT_ZERO) {
        return false;
    }
    KOMP_SET_DIRTY
    mComposite.clearDrawables();
    mComposite.setOffset(Point{0.0, 0.0});
    return true;
}

void
DrawableNoteDots::setSpacesBeforeFirstDot(double inValue)
{
    KOMP_SET_DIRTY
    mSpacesBeforeFirstDot = inValue;
}


void
DrawableNoteDots::setSpacesBetweenDots(double inValue)
{
    KOMP_SET_DIRTY
    mSpacesBetweenDots = inValue;
}


std::unique_ptr<DrawableText>
DrawableNoteDots::makeDot(const Temporal& inTemporal)
{
    auto dot = std::make_unique<DrawableText>();
    const auto uchar = FontTheme::instance().MarkInfo.at(MarkType::DOT).getCharacter();
    dot->setText(DrawableChars{uchar});
    dot->setFontSize(inTemporal.getFontSize());
    return dot;
}

void
DrawableNoteDots::update(const Temporal& inTemporal)
{
    const auto dots = inTemporal.getDuration().getDots();
    if (mComposite.getDrawableCount() == dots) {
        // I'm guessing we don't need to update anything
        return;
    }
    
    KOMP_SET_DIRTY
    clearDots();
    
    if (dots < 1) {
        return;
    }
    
    const auto slot = inTemporal.getSlot();
    const int staffLines = inTemporal.getLocation().getStaff().getStaffLineCount();
    const bool isOddNumStaffLines = (staffLines % 2) != 0;
    const bool spacesAreOdd = isOddNumStaffLines;
    const bool slotIsOdd = (slot % 2) != 0;
    const bool noteheadIsOnSpace = spacesAreOdd && slotIsOdd;
    
    auto dot = std::make_unique<DrawableText>();
    const auto uchar = FontTheme::instance().MarkInfo.at(MarkType::DOT).getCharacter();
    dot->setText(DrawableChars{uchar});
    dot->setFontSize(inTemporal.getFontSize());
    const auto dotFrame = dot->getFrame(Point{});
    
    // seems like it should be necessary to adjust this per the
    // dot's center, but looks better without doing so
    //const auto dotCenter = dotFrame.getCenter();
    //const auto dotYCenter = dotCenter.y;
    double dotY = 0.0;//(-1 * dotYCenter);
    
    const auto spaceSize = inTemporal.getSpaceSize();
    const auto slotSize = spaceSize / 2.0;
    
    if (!noteheadIsOnSpace) {
        dotY -= slotSize;
    }
    
    const auto firstSpacePixels = spaceSize * mSpacesBeforeFirstDot;
    const auto firstDotX = inTemporal.getFrames().head.getWidth() + firstSpacePixels;
    
    setOffset(Point{firstDotX, dotY});
    
    const auto spaceBetween = mSpacesBetweenDots * spaceSize;
    const auto stride = spaceBetween + dotFrame.getWidth();
    
    for (int d = 0; d < dots; ++d) {
        auto dot = makeDot(inTemporal);
        if (d != 0) {
            dot->setOffset(Point{stride, 0.0});
        }
        mComposite.addDrawable(std::move(dot));
    }
}


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

