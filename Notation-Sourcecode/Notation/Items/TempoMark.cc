// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TempoMark.h"

// Local
#include "DrawableContext.h"
#include "DrawableTempoMark.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"
#include "Temporal.h"
#include "TemporalConstants.h"

namespace komp
{
TempoMark::TempoMark(const MusicLocation& inLocation,
                     TempoValue inTempoValue)
: Spatial(inLocation)
, mTempoValue(inTempoValue)
{
}

const ItemTraits&
TempoMark::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::TempoMark);
}

ItemUPtr
TempoMark::getMemento() const
{
    return std::make_unique<TempoMark>(*this);
}

void
TempoMark::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const TempoMark* downcastedP = dynamic_cast<const TempoMark*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    update();
    setIsQuiescent(wasQuiescent);
}

void
TempoMark::update()
{
    notifyObservers({ItemChangeType::Time, ItemChangeType::Updated});
}

void
TempoMark::createDrawable()
{
    ItemImpl::createDrawable<DrawableTempoMark>(*this);
}

void
TempoMark::inserted()
{
    setX(0);
    setSlot(DefaultSlot);
    update();
}

void
TempoMark::removed()
{
    notifyObservers({ItemChangeType::Removed});
}

Rect
TempoMark::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    const auto frame = drawable->getFrame(getDrawPoint());
    return frame;
}

Region
TempoMark::getRegion() const
{
    return ItemImpl::getRegion<DrawableTempoMark>(*this);
}

double
TempoMark::getScoreX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto dataX = measure.getDataX();
    const auto tempoX = getX();
    const auto scoreX = dataX + tempoX;
    // TODO: How can we get the actual width?
    const auto meterWidth = measure.getMeterVisible() ? 8.0 : 0.0;
    return scoreX - meterWidth;
}

Point
TempoMark::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}

void
TempoMark::beginDrag(const CurrentState& inState)
{
#if TEMPO_DRAG
    const auto frame = getFrame();
    mDragOffset = inPoint;
    mDragOffset.x -= frame.left;
    mDragOffset.y -= frame.top;
#endif
}

void
TempoMark::drag(const CurrentState& inState)
{
#if TEMPO_DRAG
    auto dragPoint = inPoint;
    dragPoint.x -= mDragOffset.x;
    dragPoint.y += mDragOffset.y;
    Spatial::drag(dragPoint);
#endif
}

void
TempoMark::endDrag(const CurrentState& inState)
{
}

TempoValue
TempoMark::getTempoValue() const
{
    return mTempoValue;
}

void
TempoMark::setTempoValue(const TempoValue& inValue)
{
    mTempoValue = inValue;
    update();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
