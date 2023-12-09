// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "AttachedItem.h"

// Local
#include "Measure.h"

namespace komp
{

    AttachedItem::AttachedItem(const MusicLocation& inLocation,
                               const RelativePoint& inRelativePoint)
    : Spatial(inLocation)
    , mRelativePoint(std::move(inRelativePoint))
    {

    }


    AttachedItem::AttachedItem(const AttachedItem& other)
    : Spatial(other)
    , mRelativePoint(other.getRelativePoint())
    {

    }


    AttachedItem&
    AttachedItem::operator=(const AttachedItem& other)
    {
        Spatial::operator=(other);
        mRelativePoint.operator=(other.getRelativePoint());
        return *this;
    }


    AttachedItem::AttachedItem(AttachedItem&& other) = default;


    AttachedItem&
    AttachedItem::operator=(AttachedItem&& other) = default;

    
    void
    AttachedItem::inserted()
    {
        Spatial::inserted();
    }


    void
    AttachedItem::removed()
    {
        // TODO: remove myself from mRelativePointer::mSpatial?
        Spatial::removed();
    }


    double
    AttachedItem::getX() const
    {
        const auto& spatial = mRelativePoint.getItem();
        const auto& measure = spatial.getLocation().getMeasure();
        const double dataX = measure.getDataX();
        const double myScoreX = getScoreX();
        return myScoreX - dataX;
    }


    void
    AttachedItem::setX(double inValue)
    {
        const auto& spatial = mRelativePoint.getItem();
        const auto& measure = spatial.getLocation().getMeasure();
        const double dataX = measure.getDataX();
        const double newScoreX = dataX + inValue;
        mRelativePoint.setScoreX(newScoreX);
        Spatial::setX(inValue);
    }


    double
    AttachedItem::getScoreX() const
    {
        return mRelativePoint.getScoreX();
    }


    double
    AttachedItem::getScoreY() const
    {
        return mRelativePoint.getScoreY();
    }


    int
    AttachedItem::getSlot() const
    {
        // TODO: add a function for this on the RelativePoint class and use it here
        const auto& spatial = mRelativePoint.getItem();
        const int spatialSlot = spatial.getSlot();
        const int myRoundedSlotOffset = static_cast<int>(std::ceil(mRelativePoint.getYSlots() - 0.5));
        const int result = spatialSlot + myRoundedSlotOffset;
        return result;
    }


    void
    AttachedItem::setSlot(int inValue)
    {
        // TODO: add a function for this on the RelativePoint class and use it here
        const auto& spatial = mRelativePoint.getItem();
        const double spatialSlot = spatial.getSlot();
        const double newSlotOffset = static_cast<double>(inValue) - spatialSlot;
        mRelativePoint.setYSlots(newSlotOffset);
        Spatial::setSlot(static_cast<int>(std::ceil(newSlotOffset - 0.5)));
    }


    const RelativePoint&
    AttachedItem::getRelativePoint() const
    {
        return mRelativePoint;
    }
    
    
    RelativePoint&
    AttachedItem::getRelativePoint()
    {
        return mRelativePoint;
    }
    
    double
    AttachedItem::getTime() const
    {
        const auto& spatial = mRelativePoint.getItem();
        return spatial.getTime();
    }
    
    
    int
    AttachedItem::getRoundedTime() const
    {
        const auto& spatial = mRelativePoint.getItem();
        return spatial.getRoundedTime();
    }
    
    
    void
    AttachedItem::setTime(double inValue)
    {
        // nothing happens because mRelativePoint determines this data
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

