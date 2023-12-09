// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "SpaceTimePoint.h"

// Local
#include "Measure.h"
#include "Track.h"
#include "StaffSystem.h"
#include "Staff.h"
#include "Score.h"

namespace komp
{
    SpaceTimePoint::SpaceTimePoint(Item& inItem)
    : mX{0.0}
    , mSlot{0.0}
    , mTime{0.0}
    , mItem{inItem}
    {
    }

        
    SpatialPointIntfUPtr
    SpaceTimePoint::clone() const
    {
        return std::make_unique<SpaceTimePoint>(*this);
    }
    
    
    void
    SpaceTimePoint::setItem(Item& inItem)
    {
        mItem = inItem;
    }
    
        
    Item&
    SpaceTimePoint::getItem() const
    {
        return mItem.get();
    }


    Measure&
    SpaceTimePoint::getMeasure() const
    {
        const auto& item = getItem();
        const auto& loc = item.getLocation();
        auto& measure = loc.getMeasure();
        return measure;
    }

    
    Staff&
    SpaceTimePoint::getStaff() const
    {
        const auto& item = getItem();
        const auto& loc = item.getLocation();
        const auto& measure = loc.getMeasure();
        const auto& system = measure.getStaffSystem();
        KOMP_ASSERT(system);
        auto& track = loc.getTrack();
        auto& staff = system->getStaff(track, loc.getHand());
        return staff;
    }

    
    double
    SpaceTimePoint::getSpaceSize() const
    {
        return Staff::getSpacing();
    }

    
    double
    SpaceTimePoint::getSlotSize() const
    {
        return Staff::getSlotSize();
    }

    
    void
    SpaceTimePoint::setYSlots(double inValueSlots)
    {
        const double newValue = std::max(std::min(256.0, inValueSlots), -256.0);
        mSlot = newValue;
    }

    
    void
    SpaceTimePoint::addYSlots(double inValueSlots)
    {
        double newValue = mSlot + inValueSlots;
        newValue = std::max(std::min(256.0, newValue), -256.0);
        mSlot = newValue;
    }

    
    void
    SpaceTimePoint::addYPixels(double inValuePixels)
    {
        const double slotSize = getSlotSize();
        KOMP_ASSERT(slotSize != 0.0);
        const double valueInSlots = inValuePixels / slotSize;
        addYSlots(valueInSlots);
    }
    
    
    void
    SpaceTimePoint::setSlot(double inSlot)
    {
        const auto newSlot = std::min(std::max(inSlot, -256.0), 256.0);
        mSlot = newSlot;
    }

    
    void
    SpaceTimePoint::setScoreY(double inScorePixels)
    {
        const auto slotValue = getStaff().pixelToSlot(inScorePixels);
        setYSlots(slotValue);
    }

    
    void
    SpaceTimePoint::setX(double inValuePixels)
    {
        mX = inValuePixels;
    }

    
    void
    SpaceTimePoint::addX(double inValuePixels)
    {
        mX += inValuePixels;
    }

    
    double
    SpaceTimePoint::getMeasureRelativeX() const
    {
        return mX;
    }

    
    void
    SpaceTimePoint::setMeasureRelativeX(double inValue)
    {
        setX(inValue);
    }

    
    void
    SpaceTimePoint::setScoreX(double inScorePixels)
    {
        auto measureRelativeX = inScorePixels - getMeasure().getDataX();
        setMeasureRelativeX(measureRelativeX);
    }

    
    double
    SpaceTimePoint::getYSlots() const
    {
        return mSlot;
    }

    
    double
    SpaceTimePoint::getYPixels() const
    {
        return getStaff().slotToPixel(mSlot);
    }

    
    double
    SpaceTimePoint::getSlot() const
    {
        return mSlot;
    }

    
    double
    SpaceTimePoint::getX() const
    {
        return mX;
    }

    
    double
    SpaceTimePoint::getScoreX() const
    {
        return mX + getMeasure().getDataX();
    }

    
    double
    SpaceTimePoint::getScoreY() const
    {
        return getYPixels();
    }

    bool
    SpaceTimePoint::getIsYLocked() const
    {
        return false;
    }

    
    void
    SpaceTimePoint::lockY()
    {
        // ?
    }

    
    void
    SpaceTimePoint::unlockY()
    {
        // Y
    }

    
    bool
    SpaceTimePoint::getIsXLocked() const
    {
        return false;
    }

    
    void
    SpaceTimePoint::lockX()
    {
        // ?
    }

    
    void
    SpaceTimePoint::unlockX()
    {
        // ?
    }

    
    double
    SpaceTimePoint::getTime() const
    {
        return mTime;
    }

    
    void
    SpaceTimePoint::setTime(double inValue)
    {
        mTime = inValue;
    }

    
    int
    SpaceTimePoint::getRoundedTime() const
    {
        return static_cast<int>(std::ceil(getTime() - 0.5));
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
