// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Marker.h"

// Local
#include "DrawableContext.h"
#include "DrawableText.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"

namespace komp
{
Marker::Marker(const MusicLocation& inLocation,
               const MarkerParams& inParams)
: Spatial(inLocation)
, mParams{inParams}
{
}

Marker::Marker(const Marker& inObject)
: Spatial(inObject)
{
    copyState(inObject);
}

Marker&
Marker::operator=(const Marker& other)
{
    copyState(other);
    return *this;
}

void
Marker::copyState(const Marker& other)
{
    if (this != &other) {
        Spatial::operator=(other);
    }
}

ItemUPtr
Marker::getMemento() const
{
    return std::make_unique<Marker>(*this);
}

void
Marker::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Marker* downcastedP = dynamic_cast<const Marker*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

void
Marker::configure(const MarkerParams& inParams)
{
    update();
}

void
Marker::update()
{
    if (getIsQuiescent()) {
        return;
    }
    
    const auto& theme = FontTheme::instance();
    const auto fontSize = Staff::getFontSize() * 0.65;
    const auto symbol = theme.MarkerMap.at(mParams.type);
    const auto drawable = ItemImpl::getDrawable<DrawableText>(*this);
    drawable->setText(DrawableChars{symbol});
    drawable->setFontSize(fontSize);
}

const ItemTraits&
Marker::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Marker);
}

Rect
Marker::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    const auto frame = drawable->getFrame(getDrawPoint());
    return frame;
}

Region
Marker::getRegion() const
{
    return ItemImpl::getRegion<DrawableText>(*this);
}

double
Marker::getScoreX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto drawX = measure.getX();
    const auto drawable = ItemImpl::getDrawable(*this);
    const auto halfWidth = drawable->getWidth() / 2.0;
    return drawX - halfWidth;
}

double
Marker::getScoreY() const
{
    const auto& staff = getLocation().getStaff();
    const auto y = staff.slotToPixel(14);
    return y;
}

Point
Marker::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}


// MARK: - Drawing

void
Marker::createDrawable()
{
}

// MARK: - Draggable

void
Marker::beginDrag(const CurrentState& inState)
{
    //Spatial::beginDrag(inState);
}

void
Marker::drag(const CurrentState& inState)
{
    //Spatial::drag(inState);
}

void
Marker::endDrag(const CurrentState& inState)
{
    //Spatial::endDrag(inState);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
