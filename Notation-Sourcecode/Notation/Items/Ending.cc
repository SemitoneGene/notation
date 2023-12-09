// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Ending.h"

// Local
#include "DrawableEnding.h"
#include "EndingParams.h"
#include "Engraver.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"

namespace komp
{

Ending::Ending(const MusicLocation& inLocation,
               const EndingParams& inParams)
: Spatial(inLocation)
, mElements(inParams.elements)
, mDragLocation(DragLocation::NONE)
{
    configure(inParams);
}

Ending::Ending(const Ending& inObject)
: Spatial(inObject)
, mElements(inObject.mElements)
{
    copyState(inObject);
}

Ending&
Ending::operator=(const Ending& other)
{
    copyState(other);
    return *this;
}

void
Ending::copyState(const Ending& other)
{
    if (this != &other) {
        Spatial::operator=(other);
        mElements = other.mElements;
    }
}

ItemUPtr
Ending::getMemento() const
{
    return std::make_unique<Ending>(*this);
}

void
Ending::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Ending* downcastedP = dynamic_cast<const Ending*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

void
Ending::configure(const EndingParams& inParams)
{
    mElements = inParams.elements;
    update();
}

void
Ending::update()
{
    if (getIsQuiescent()) {
        return;
    }
    
    ItemImpl::setNeedsDisplay(*this);
    notifyObservers({ItemChangeType::Updated});
}

const ItemTraits&
Ending::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Ending);
}

Rect
Ending::getFrame() const
{
    return mDrawable.getFrame(getDrawPoint());
}

Region
Ending::getRegion() const
{
    return ItemImpl::getRegion<DrawableEnding>(*this);
}

double
Ending::getScoreX() const
{
    // Endings always originate at the left barline of the measure
    const auto& point = getSpatialPoint();
    const auto measureX = point.getMeasure().getX();
    return point.getX() + measureX;
}

// MARK: - Drawing

void
Ending::createDrawable()
{
    ItemImpl::createDrawable<DrawableEnding>(*this);
}

// MARK: - Draggable

void
Ending::beginDrag(const CurrentState& inState)
{
    const auto& point = inState.getPoint();
    if (mHandleLeft.contains(point)) {
        mDragLocation = DragLocation::LEFT;
    } else if (mHandleRight.contains(point)) {
        mDragLocation = DragLocation::RIGHT;
    } else {
        mDragLocation = DragLocation::NONE;
    }
}

void
Ending::drag(const CurrentState& inState)
{
    // TODO: Fix this for multiple lines
    const auto& delta = dragDelta();
    if (delta.x == 0 && delta.y == 0) {
        // Nothing to do
        return;
    }
    
    // Get the measures
    //        const auto measure = getMeasure();
    //        const auto dragMeasure = state.getMeasure();
    
    switch (mDragLocation)
    {
    case DragLocation::LEFT:
        {
            //                // Get the positions in global coordinates
            //                const auto& endMeasure = mEndMeasure;
            //                const auto dataX = endMeasure->getDataX();
            //                const auto endX = endMeasure->positionForTime(mEndTime);
            //                const auto leftGap = endMeasure->getLeftGap();
            //                const auto right = (dataX + endX) - leftGap;
            //
            //                // Set the new left-most position
            //                const auto newX = std::max(0.0, inPoint.x - dragMeasure->getDataX());
            //                const auto left = measure->getDataX() + newX;
            //                if (left < right - MIN_WIDTH) {
            //                    setX(newX);
            //                    if (measure != dragMeasure) {
            //                        // Move the item to a new measure
            //                        measure->removeItem(*this);
            //                        const int time = dragMeasure->absoluteTime(inPoint.x, false);
            //                        dragMeasure->insertItem(*this, time);
            //                    }
            //                }
            break;
        }
        
    case DragLocation::RIGHT:
        {
            //                // Get the positions in global coordinates
            //                const auto dataX = measure->getDataX();
            //                const auto left = dataX + getX();
            //
            //                if (inPoint.x > left + MIN_WIDTH) {
            //                    // Set the new right-most position
            //                    setEndMeasure(dragMeasure);
            //                    const auto dragDataX = dragMeasure->getDataX();
            //                    const auto right = inPoint.x - dragDataX;
            //                    const int time = dragMeasure->time(state.getTrack(), right);
            //                    setEndTime(time);
            //                }
            break;
        }
        
    case DragLocation::NONE:
        {
            break;
        }
    }
}

void
Ending::endDrag(const CurrentState& inState)
{
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
