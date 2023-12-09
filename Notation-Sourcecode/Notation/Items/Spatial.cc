// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Spatial.h"

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "Staff.h"
#include "Track.h"

namespace komp
{
Spatial::Spatial(const MusicLocation& inLocation)
: mSpaceTimePoint{*this}
, mLocation(inLocation)
{
}

void
Spatial::itemIDChanged()
{
    mSpaceTimePoint.setItem(*this);
}

void
Spatial::itemIDInvalidated()
{
}

void
Spatial::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}

void
Spatial::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}

void
Spatial::notifyObservers(const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        mDeferredChanges.insert(inChanges.begin(), inChanges.end());
        return;
    }
    
    ItemImpl::notifyObservers(*this, mObservers, std::move(inChanges));
}


bool
Spatial::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
Spatial::setIsQuiescent(bool inValue)
{
    if (mIsQuiescent == inValue) {
        return;
    }
    
    mIsQuiescent = inValue;
    
    if (mDeferredChanges.empty()) {
        return;
    }
    
    if (!mIsQuiescent) {
        notifyObservers(mDeferredChanges);
        mDeferredChanges.clear();
    }
}

void
Spatial::inserted()
{
}

void
Spatial::removed()
{
}

void
Spatial::removeSelf()
{
    ItemImpl::removeSelf(*this);
}

const MusicLocation&
Spatial::getLocation() const
{
    return mLocation;
}

/// \brief Check if the point is contained by the item
/// \param inPoint The point to check for containment
/// \param inPrecise If true, use the region if available
/// \result true if the item contains the point
bool
Spatial::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}

double
Spatial::getX() const
{
    return getSpatialPoint().getMeasureRelativeX();
}

void
Spatial::setX(double inValue)
{
    ItemImpl::setX(*this, inValue, mSpaceTimePoint);
}

double
Spatial::getScoreX() const
{
    return getSpatialPoint().getScoreX();
}

int
Spatial::getSlot() const
{
    return ItemImpl::getSlot(mSpaceTimePoint);
}

void
Spatial::setSlot(int inValue)
{
    ItemImpl::setSlot(*this, inValue, mSpaceTimePoint);
}

double
Spatial::getScoreY() const
{
    const auto& theStaff = getLocation().getStaff();
    return theStaff.slotToPixel(getSlot());
}

Point
Spatial::getDrawPoint() const
{
    return ItemImpl::getDrawPoint(*this, mSpaceTimePoint);
}


double
Spatial::getTime() const
{
    return getSpatialPoint().getTime();
}

int
Spatial::getRoundedTime() const
{
    return ItemImpl::getRoundedTime(*this);
}

void
Spatial::setTime(double inValue)
{
    ItemImpl::setTime(*this, mSpaceTimePoint, inValue);
}

double
Spatial::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
Spatial::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}


void
Spatial::drag(const CurrentState& inState)
{
    // Use the time of a note near this symbol so octavas, pedals
    // and dynamics will affect the expected note. Octave ends come after the
    // corresponding octave start.
    const auto& point = inState.getPoint();
    auto& stateMeasure = inState.getMeasure();
    const auto time = stateMeasure.absoluteTime(point.x, false);
    
    auto& measure = mLocation.getMeasure();
    if (measure.getIndex() != stateMeasure.getIndex()) {
        // Move the item to a new measure
        measure.removeItem(*this);
        stateMeasure.insertItem(*this, time);
    }
    
    // Note: Use the time stamp that Measure::insertItem put in
    setTime(time);
    setX(inState.getX());
    setSlot(inState.getSlot());
}

/// \brief Get the character width using the current staff settings
/// \param inChar The character to measure
/// \result The width
double
Spatial::characterWidth(const unsigned short inChar) const
{
    const auto size = measureCharacter(inChar);
    return size.width;
}

/// \brief Get the character height using the current staff settings
/// \param inChar The character to measure
/// \result The height
double
Spatial::characterHeight(const unsigned short inChar) const
{
    const auto size = measureCharacter(inChar);
    return size.height;
}

/// \brief Measure the character using the current staff settings
/// \param inChar The character to measure
/// \result The measured size
komp::Size
Spatial::measureCharacter(const unsigned short inChar) const
{
    // Get the font information
    const auto fontSize = Staff::getFontSize();
    return MeasureCharacter(fontSize, inChar);
}

SpatialPointIntf&
Spatial::getSpatialPoint()
{
    return mSpaceTimePoint;
}

const SpatialPointIntf&
Spatial::getSpatialPoint() const
{
    return mSpaceTimePoint;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

