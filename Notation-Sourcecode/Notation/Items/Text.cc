// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Text.h"

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"

namespace komp
{

// MARK: - Text

Text::Text(const MusicLocation& inLocation,
           std::string_view inText,
           PlacementType inPlacement)
: Spatial(inLocation)
, mText{inText}
, mPlacement{inPlacement == PlacementType::AUTOMATIC ? PlacementType::ABOVE_STAFF : inPlacement}
{
}

Text::Text(const Text& other)
: Spatial(other)
, mText{other.mText}
, mPlacement{other.mPlacement}
{
    switch (mPlacement)
    {
        
    default:
        break;
    }
}

Text&
Text::operator=(const Text& other)
{
    copyState(other);
    return *this;
}

Text::Text(Text&& other) = default;


Text&
Text::operator=(Text&& other) = default;


const ItemTraits&
Text::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Text);
}

void
Text::copyState(const Text& other)
{
    Spatial::operator=(other);
}

ItemUPtr
Text::getMemento() const
{
    return std::make_unique<Text>(*this);
}

void
Text::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Text* downcastedP = dynamic_cast<const Text*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

void
Text::update()
{
    calculateAboveBelow();
}

void
Text::observedItemChanged(const Item& observedItem,
                          const ItemChanges& inChanges)
{
}

Rect
Text::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    return drawable->getFrame(getDrawPoint());
}

Region
Text::getRegion() const
{
    return ItemImpl::getRegion<DrawableString>(*this);
}

void
Text::inserted()
{
    Spatial::inserted();
    notifyObservers({ItemChangeType::Inserted});
}

void
Text::removed()
{
    Spatial::removed();
    notifyObservers({ItemChangeType::Removed});
}

double
Text::getX() const
{
    return Spatial::getX();
}

void
Text::setX(double inValue)
{
    Spatial::setX(inValue);
}


double
Text::getScoreX() const
{
    return mX;
}

double
Text::getScoreY() const
{
    return mY;
}

void
Text::beginDrag(const CurrentState& inState)
{
}

void
Text::drag(const CurrentState& inState)
{
}

void
Text::endDrag(const CurrentState& inState)
{
}

void
Text::calculateAboveBelow()
{
    const auto& location = getLocation();
    const auto& staff = location.getStaff();
    
    const auto isAbove = mPlacement == PlacementType::ABOVE_ITEM
    || mPlacement == PlacementType::ABOVE_STAFF;
    
    double staffY = 0.0;
    
    if (isAbove) {
        const auto baselinedSlot = staff.getTopLineSlot();
        const auto baselinePixel = staff.slotToPixel(baselinedSlot);
        staffY = baselinePixel;
    } else {
        const auto baselinedSlot = staff.getBottomLineSlot();
        const auto baselinePixel = staff.slotToPixel(baselinedSlot);
        staffY = baselinePixel;
    }
    
    const auto& measure = location.getMeasure();
    const auto measureLeft = measure.getX();
    const auto measureRight = measure.getRight();
    
    const auto& font = Preferences::instance().TextFont;
    const auto textSize = MeasureText(font.Font,
                                      font.Size,
                                      mText.c_str());
    const auto halfTextWidth = textSize.width / 2.0;
    const auto halfTextHeight = textSize.height / 2.0;
    
    const auto spaceSize = Staff::getSpacing() * 1.25;
    
    const auto time = location.getTime();
    const auto timeX = measure.positionForTime(time);
    const auto textX = measureLeft + timeX + halfTextWidth;
    const auto textRight = textX + halfTextWidth;
    
    mX = textX;
    
    // Ensure the text is within the owning measure
    if (textRight > measureRight) {
        mX -= textRight - measureRight;
    }
    
    switch (mPlacement)
    {
    case PlacementType::ABOVE_STAFF:
    default:
        {
            mY = staffY - halfTextHeight - spaceSize;
            break;
        }
        
    case PlacementType::BELOW_STAFF:
        {
            mY = staffY + halfTextHeight + spaceSize;
            break;
        }
    }
}

void
Text::createDrawable()
{
    ItemImpl::createDrawable<DrawableString>(*this);
    const auto drawable = ItemImpl::getDrawable<DrawableString>(*this);
    drawable->setString(mText);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

