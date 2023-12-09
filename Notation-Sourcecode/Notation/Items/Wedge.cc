// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Wedge.h"

// Local
#include "DrawableColor.h"
#include "DrawableContext.h"
#include "DrawableWedge.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "StaffSystem.h"
#include "Staff.h"

namespace komp
{

static const auto SymbolSnap = 2.0;

static const std::vector<WedgeType> sWedgeRanges =
{
    WedgeType::CRESCENDO,
    WedgeType::DIMINUENDO
};

static const std::set<WedgeType> sWedges(std::cbegin(sWedgeRanges),
                                         std::cend(sWedgeRanges));
struct WedgeRange
{
    static size_t
    getIndex(WedgeType inMark) {
        return std::distance(std::begin(sWedgeRanges),
                             std::find(std::begin(sWedgeRanges),
                                       std::end(sWedgeRanges),
                                       inMark));
    }
};


Wedge::Wedge(const MusicLocation& inLocation,
             const WedgeParams& inWedgeParams)
: Spanner(inLocation, inWedgeParams.spannerParams)
, mType(inWedgeParams.wedgeType)
, mHeight(1)
{
    switch (inWedgeParams.placementType)
    {
    case PlacementType::ABOVE_ITEM:
    case PlacementType::ABOVE_STAFF:
        setSlot(16);
        break;
        
    case PlacementType::BELOW_ITEM:
    case PlacementType::BELOW_STAFF:
        setSlot(-2);
        break;
        
    default:
        setSlot(0);
    }
}

Wedge::Wedge(const Wedge& inItem)
: Spanner(inItem)
, mType(inItem.mType)
, mHeight(inItem.mHeight)
{
    setTime(inItem.getTime());
}

Wedge::~Wedge()
{
}

Wedge&
Wedge::operator=(const Wedge& other)
{
    if (this != &other) {
        Spanner::operator=(other);
        mType = other.mType;
        mHeight = other.mHeight;
    }
    return *this;
}

const ItemTraits&
Wedge::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Wedge);
}

void
Wedge::copyState(const Wedge& other)
{
    if (this != &other) {
        Spanner::operator=(other);
        mType = other.mType;
        mHeight = other.mHeight;
    }
}

ItemUPtr
Wedge::getMemento() const
{
    return std::make_unique<Wedge>(*this);
}

void
Wedge::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Wedge* downcastedP = dynamic_cast<const Wedge*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

void
Wedge::update()
{
    if (getIsQuiescent()) {
        return;
    }
    notifyObservers({ItemChangeType::Updated});
}

Rect
Wedge::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    return drawable->getFrame(getDrawPoint());
    //
    //        // TODO: This will not work when we support multiple systems per page
    //        const auto& staff = getStaff();
    //        const auto left = getMeasure()->getDataX() + getX();
    //        const auto top = staff->slotToPixel(getSlot());
    //        const auto height = mHeight * Staff::getSpacing();
    //        const auto bottom = top + height;
    //        const auto& endMeasure = mEndMeasure;
    //        const auto right = endMeasure->getDataX() + endMeasure->positionForTime(mEndTime);
    //        const Rect frameRect(top, left, bottom, right);
    //        return frameRect;
}

Region
Wedge::getRegion() const
{
    return ItemImpl::getRegion<DrawableWedge>(*this);
}

void
Wedge::createDrawable()
{
    ItemImpl::createDrawable<DrawableWedge>(*this);
}

void
Wedge::inserted()
{
    
    Spatial::inserted();
    notifyObservers({ItemChangeType::Inserted});
}

void
Wedge::removed()
{
    Spatial::removed();
    notifyObservers({ItemChangeType::Removed});
}

WedgeType
Wedge::getWedgeType() const
{
    return mType;
}

Rect
Wedge::getPositions() const
{
    // Get the items used to draw with
    const auto& loc = getLocation();
    const auto& measure = loc.getMeasure();
    const auto& staff = loc.getStaff();
    
    // Get the start position
    const auto left = measure.getDataX() + getX();
    const auto top = staff.slotToPixel(getSlot());
    const auto height = mHeight * Staff::getSpacing();
    const auto bottom = top + height;
    
    // Get the end position
    const auto dataX = mEndMeasure->getDataX();
    const auto right = dataX + getEndX();
    
    return Rect{top, left, bottom, right};
}

// MARK: - Draggable

void
Wedge::beginDrag(const CurrentState& inState)
{
    mDragStartType = mType;
}

void
Wedge::drag(const CurrentState& inState)
{
    const auto& delta = dragDelta();
    if (delta.x == 0 && delta.y == 0) {
        return;
    }
    
    if (!mDragAxisLock) {
        if (abs(delta.x) > abs(delta.y)) {
            mDragAxisX = true;
        } else {
            mDragAxisY = true;
        }
        mDragAxisLock = true;
    }
    
    if (mDragAxisX) {
        
    } else {
        
        const auto startIndex = static_cast<int>(WedgeRange::getIndex(mDragStartType));
        const auto totalDynamics = static_cast<int>(sWedges.size() - 1);
        
        const auto dragDistance = dragStart().y - dragLast().y;
        const auto step = static_cast<int>(floor(dragDistance / SymbolSnap));
        const auto newIndex = std::clamp(startIndex + step, 0, totalDynamics);
        const auto newWedge = sWedgeRanges.at(newIndex);
        
        if (newWedge != mType) {
            mType = newWedge;
            update();
            ItemImpl::setNeedsDisplay(*this);
        }
    }
    
    //        // TODO: Fix this for multiple lines
    //        const auto& delta = dragDelta();
    //        if (delta.x == 0 && delta.y == 0) {
    //            // Nothing to do
    //            return;
    //        }
    //
    //        // Get the measures
    //        const auto measure = getMeasure();
    //        const auto dragMeasure = inState.getMeasure();
    //
    //        switch (mDragLocation)
    //        {
    //            case DragLocation::LEFT:
    //            {
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
    //                break;
    //            }
    //
    //            case DragLocation::RIGHT:
    //            {
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
    //                break;
    //            }
    //
    //            case DragLocation::CENTER:
    //            {
    //                // Restrict the dragging to slot only
    //                setSlot(state.getSlot());
    //                break;
    //            }
    //        }
}

void
Wedge::endDrag(const CurrentState& inState)
{
    mDragAxisLock = false;
    mDragAxisX = false;
    mDragAxisY = false;
    mDragStartType = WedgeType::UNKNOWN;
}
}

// namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
