// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordSymbol.h"

// Local
#include "DrawableContext.h"
#include "DrawableChordString.h"
#include "FontManager.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Score.h"
#include "ScoreColors.h"

namespace komp
{
ChordSymbol::ChordSymbol(const MusicLocation& inLocation,
                         const ChordSymbolParams& inChordSymbolParams)
: mHand{inChordSymbolParams.hand}
, mVoice{inChordSymbolParams.voice}
, mTime{inChordSymbolParams.time}
, mChordSymbolValue(inChordSymbolParams.value)
, mLocation{inLocation}
{
    update();
    
    if (inChordSymbolParams.isXDifferentFromTime) {
        mX = inChordSymbolParams.measureRelativeX;
        mIsXTimeLocked = false;
    }
    
    if (inChordSymbolParams.isSlotSpecified) {
        mSlot = inChordSymbolParams.slot;
    }
}

ChordSymbol::ChordSymbol(const ChordSymbol& other) = default;
ChordSymbol::ChordSymbol(ChordSymbol&& other) = default;
ChordSymbol& ChordSymbol::operator=(const ChordSymbol& other) = default;
ChordSymbol& ChordSymbol::operator=(ChordSymbol&& other) = default;

const ItemTraits&
ChordSymbol::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::ChordSymbol);
}


ItemUPtr
ChordSymbol::getMemento() const
{
    return std::make_unique<ChordSymbol>(*this);
}


void
ChordSymbol::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const ChordSymbol* downcastedP = dynamic_cast<const ChordSymbol*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    update();
    setIsQuiescent(wasQuiescent);
}


void
ChordSymbol::update()
{
    if (getIsQuiescent()) {
        return;
    }
    
    notifyObservers({ItemChangeType::Updated});
}


void
ChordSymbol::itemIDChanged()
{
}


void
ChordSymbol::itemIDInvalidated()
{
}

void
ChordSymbol::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
ChordSymbol::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
ChordSymbol::observedItemChanged(const Item& observedItem, const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        return;
    }
}


bool
ChordSymbol::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
ChordSymbol::setIsQuiescent(bool inValue)
{
    mIsQuiescent = inValue;
}


Rect
ChordSymbol::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    const auto frame = drawable->getFrame(getDrawPoint());
    return frame;
}


Region
ChordSymbol::getRegion() const
{
    return ItemImpl::getRegion<DrawableChordString>(*this);
}


Point
ChordSymbol::getAnchor() const
{
    return getFrame().getCenter();
}


bool
ChordSymbol::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}


void
ChordSymbol::createDrawable()
{
    ItemImpl::createDrawable<DrawableChordString>(*this);
}

void
ChordSymbol::inserted()
{
    if (mIsXTimeLocked) {
        setXFromTime();
    } else {
        const auto& measure = getLocation().getMeasure();
        const auto positionForTime = measure.positionForTime(getRoundedTime());
        if (std::abs(positionForTime - mX) < 0.5) {
            setXFromTime();
        }
    }
}


void
ChordSymbol::removed()
{
    notifyObservers({ItemChangeType::Removed});
}


void
ChordSymbol::removeSelf()
{
    ItemImpl::removeSelf(*this);
    notifyObservers({ItemChangeType::Removed});
}

const MusicLocation&
ChordSymbol::getLocation() const
{
    return mLocation;
}

double
ChordSymbol::getX() const
{
    if (mIsXTimeLocked) {
        const auto& measure = getLocation().getMeasure();
        const auto positionX = measure.positionForTime(getRoundedTime());
        return positionX;
    }
    
    return mX;
}


void
ChordSymbol::setX(double inValue)
{
    mX = inValue;
    
    const auto& measure = getLocation().getMeasure();
    const auto positionForTime = measure.positionForTime(getRoundedTime());
    
    if (std::abs(positionForTime - mX) < 0.5) {
        mIsXTimeLocked = true;
        mX = NAN;
    } else {
        mIsXTimeLocked = false;
    }
}


double
ChordSymbol::getScoreX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto measureDataX = measure.getDataX();
    const auto myX = getX();
    const auto scoreX = measureDataX + myX;
    return scoreX;
}


int
ChordSymbol::getSlot() const
{
    return roundToInt(mSlot);
}


void
ChordSymbol::setSlot(int inValue)
{
    mSlot = inValue;
}


double
ChordSymbol::getScoreY() const
{
    const auto& staff = getLocation().getStaff();
    const auto y = staff.slotToPixel(getSlot());
    return y;
}


Point
ChordSymbol::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}


double
ChordSymbol::getTime() const
{
    return static_cast<double>(mTime);
}


int
ChordSymbol::getRoundedTime() const
{
    return mTime;
}


void
ChordSymbol::setTime(double inValue)
{
    mTime = komp::roundToInt(inValue);
    
    if (mIsXTimeLocked) {
        setXFromTime();
    }
    
    notifyObservers({ItemChangeType::Time});
    notifyObservers({ItemChangeType::X});
}


double
ChordSymbol::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
ChordSymbol::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}

const ChordSymbolValue&
ChordSymbol::getChordSymbolValue() const
{
    return mChordSymbolValue;
}


void
ChordSymbol::setChordSymbolValue(ChordSymbolValue inValue)
{
    mChordSymbolValue = std::move(inValue);
    update();
}


void
ChordSymbol::setXFromTime()
{
    mX = NAN;
    mIsXTimeLocked = true;
}


void
ChordSymbol::setTimeFromX()
{
    const auto& measure = getLocation().getMeasure();
    const auto time = measure.time(mLocation.getTrack(),
                                   getX());
    setTime(time);
}

bool
ChordSymbol::getIsTimeLocked() const
{
    if (mIsXTimeLocked) {
        return true;
    } else if (mX == NAN) {
        return true;
    }
    return false;
}

// MARK: - Dragging

void
ChordSymbol::drag(const CurrentState& inState)
{
    // Dragging is only allowed in the y-direction, to move the
    // ChordSymbol in the x-direction, either change the time, or setX
    const auto& delta = dragDelta();
    if (delta.y == 0) {
        return;
    }
    
    setSlot(inState.getSlot());
    
    if (mIsXTimeLocked) {
        setXFromTime();
    }
}

// MARK: - Observers

void
ChordSymbol::notifyObservers(const ItemChanges& inChanges)
{
    ItemImpl::notifyObservers(*this, mObservers, std::move(inChanges));
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
