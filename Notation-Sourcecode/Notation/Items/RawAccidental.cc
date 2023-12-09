// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "RawAccidental.h"

// Local
#include "DrawableContext.h"
#include "ItemImpl.h"
#include "Score.h"
#include "ScoreColors.h"

namespace komp
{
RawAccidental::RawAccidental(ItemRegistry& inRegistry,
                             const MusicLocation& inLocation,
                             AccidentalType inAccidentalType)
: mSpaceTimePoint{*this}
, mLocation{inLocation}
, mAccidentalType{inAccidentalType}
{
}


RawAccidental::~RawAccidental()
{
    
}


const ItemTraits&
RawAccidental::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::RawAccidental);
}


ItemUPtr
RawAccidental::getMemento() const
{
    return std::make_unique<RawAccidental>(*this);
}


void
RawAccidental::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const RawAccidental* downcastedP = dynamic_cast<const RawAccidental*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    this->operator=(*downcastedP);
    setIsQuiescent(wasQuiescent);
}


void
RawAccidental::itemIDChanged()
{
    mSpaceTimePoint.setItem(*this);
}

void
RawAccidental::itemIDInvalidated()
{
}

void
RawAccidental::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
RawAccidental::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
RawAccidental::notifyObservers(const ItemChanges& inChanges)
{
    ItemImpl::notifyObservers(*this, mObservers, std::move(inChanges));
}


void
RawAccidental::observedItemChanged(const Item& observedItem,
                                   const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        return;
    }
}


bool
RawAccidental::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
RawAccidental::setIsQuiescent(bool inValue)
{
    mIsQuiescent = inValue;
}


Rect
RawAccidental::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    return drawable->getFrame(getDrawPoint());
}


Region
RawAccidental::getRegion() const
{
    return ItemImpl::getRegion<DrawableAccidental>(*this);
}


Point
RawAccidental::getAnchor() const
{
    return getFrame().getCenter();
}


bool
RawAccidental::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}

void
RawAccidental::createDrawable()
{
    ItemImpl::createDrawable<DrawableAccidental>(*this);
    const auto drawable = ItemImpl::getDrawable<DrawableAccidental>(*this);
    drawable->setAccidentalType(mAccidentalType);
    drawable->setFontSize(getFontSize());
}

void
RawAccidental::inserted()
{
    update();
    notifyObservers({ItemChangeType::Inserted});
}


void
RawAccidental::removed()
{
    notifyObservers({ItemChangeType::Removed});
}


void
RawAccidental::removeSelf()
{
    ItemImpl::removeSelf(*this);
    notifyObservers({ItemChangeType::Removed});
}


const MusicLocation&
RawAccidental::getLocation() const
{
    return mLocation;
}


double
RawAccidental::getX() const
{
    return mSpaceTimePoint.getMeasureRelativeX();
}


void
RawAccidental::setX(double inValue)
{
    mSpaceTimePoint.setX(inValue);
}


double
RawAccidental::getScoreX() const
{
    return mSpaceTimePoint.getScoreX();
}


int
RawAccidental::getSlot() const
{
    return ItemImpl::getSlot(mSpaceTimePoint);
}


void
RawAccidental::setSlot(int inValue)
{
    ItemImpl::setSlot(*this, inValue, mSpaceTimePoint);
}


double
RawAccidental::getScoreY() const
{
    return mSpaceTimePoint.getScoreY();
}


Point
RawAccidental::getDrawPoint() const
{
    Point result{getScoreX(), getScoreY()};
    return result;
}


double
RawAccidental::getTime() const
{
    return mSpaceTimePoint.getTime();
}


int
RawAccidental::getRoundedTime() const
{
    return mSpaceTimePoint.getRoundedTime();
}


void
RawAccidental::setTime(double inValue)
{
    mSpaceTimePoint.setTime(inValue);
}


double
RawAccidental::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
RawAccidental::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}


AccidentalType
RawAccidental::getAccidentalType() const
{
    return mAccidentalType;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
