// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "RehearsalMark.h"

// Local
#include "DrawableComposite.h"
#include "DrawableContext.h"
#include "DrawableString.h"
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
RehearsalMark::RehearsalMark(const MusicLocation& inLocation,
                             const RehearsalParams& inParams)
: Spatial(inLocation)
, mParams{inParams}
{
}

RehearsalMark::RehearsalMark(const RehearsalMark& inObject)
: Spatial(inObject)
{
    copyState(inObject);
}

RehearsalMark&
RehearsalMark::operator=(const RehearsalMark& other)
{
    copyState(other);
    return *this;
}

void
RehearsalMark::copyState(const RehearsalMark& other)
{
    if (this != &other) {
        Spatial::operator=(other);
    }
}

ItemUPtr
RehearsalMark::getMemento() const
{
    return std::make_unique<RehearsalMark>(*this);
}

void
RehearsalMark::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const RehearsalMark* downcastedP = dynamic_cast<const RehearsalMark*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    
    this->operator=(*downcastedP);
    update();

    setIsQuiescent(wasQuiescent);
}

void
RehearsalMark::configure(const RehearsalParams& inParams)
{
    update();
}

static constexpr const double MarkPad = 5.0;

void
RehearsalMark::update()
{
    if (getIsQuiescent()) {
        return;
    }
    
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    drawable->clearDrawables();

    const auto& font = Preferences::instance().RehearsalMarksFont;
    
    auto text = std::make_unique<DrawableString>();
    text->setFontName(font.Font);
    text->setFontSize(font.Size);
    text->setString(mParams.text);
    const auto textWidth = text->getWidth();
    const auto textHeight = text->getHeight();
    drawable->addDrawable(std::move(text));
    
    auto rectangle = std::make_unique<DrawableRectangle>();
    const auto width = std::max(textWidth, textHeight) + MarkPad;
    const auto height = textHeight + MarkPad;
    
    rectangle->setBounds({0, 0, height, width});
    rectangle->setDrawFill(false);
    rectangle->setDrawStroke(true);
    rectangle->setStrokeThickness(0.5);
    rectangle->setOffset({-(width / 2.0), -(height / 2.0)});
    drawable->addDrawable(std::move(rectangle));
    
    notifyObservers({ItemChangeType::Updated});
}

const ItemTraits&
RehearsalMark::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::RehearsalMark);
}

Rect
RehearsalMark::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    const auto frame = drawable->getFrame(getDrawPoint());
    return frame;
}

Region
RehearsalMark::getRegion() const
{
    return ItemImpl::getRegion<DrawableComposite>(*this);
}

double
RehearsalMark::getScoreX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto drawX = measure.getX();
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    const auto halfWidth = drawable->getWidth() / 2.0;
    return drawX - halfWidth;
}

double
RehearsalMark::getScoreY() const
{
    const auto& staff = getLocation().getStaff();
    const auto y = staff.slotToPixel(18);
    return y;
}

Point
RehearsalMark::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}


// MARK: - Drawing

void
RehearsalMark::createDrawable()
{
    ItemImpl::createDrawable<DrawableComposite>(*this);

}

void
RehearsalMark::inserted()
{
    update();
    notifyObservers({ItemChangeType::Inserted});
}

void
RehearsalMark::removed()
{
    notifyObservers({ItemChangeType::Removed});
}

// MARK: - Draggable

void
RehearsalMark::beginDrag(const CurrentState& inState)
{
}

void
RehearsalMark::drag(const CurrentState& inState)
{
}

void
RehearsalMark::endDrag(const CurrentState& inState)
{
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
