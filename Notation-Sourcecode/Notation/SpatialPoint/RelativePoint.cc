// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "RelativePoint.h"

// Local
#include "Item.h"
#include "Staff.h"
#include "Score.h"
#include "Throw.h"
#include "Track.h"

namespace komp
{
    RelativePoint::RelativePoint(Item& inItem,
                                 double inRelativeX,
                                 double inRelativeYSlot)
    : mItem(&inItem)
	, mRelativeX(inRelativeX)
	, mRelativeYSlots(inRelativeYSlot)
    , mLockedX(std::numeric_limits<double>::infinity())
    , mLockedYSlot(std::numeric_limits<double>::infinity())
    , mIsYLocked(false)
    , mIsXLocked(false)
	{
	}
    

	RelativePoint::RelativePoint(Item& inItem)
    : mItem(&inItem)
	, mRelativeX(0.0)
    , mRelativeYSlots(0.0)
    , mLockedX(std::numeric_limits<double>::infinity())
    , mLockedYSlot(std::numeric_limits<double>::infinity())
    , mIsYLocked(false)
    , mIsXLocked(false)
	{
    }
    
    
    SpatialPointIntfUPtr
    RelativePoint::clone() const
    {
        return std::make_unique<RelativePoint>(*this);
    }
    
    
    Item&
	RelativePoint::getItem() const
	{
        KOMP_ASSERT(mItem);
		return *mItem;
	}


    Measure&
    RelativePoint::getMeasure() const
    {
        return getItem().getLocation().getMeasure();
    }


    Staff&
    RelativePoint::getStaff() const
    {
        return getItem().getLocation().getStaff();
    }
    
    
    double
    RelativePoint::getSpaceSize() const
    {
        return Staff::getSpacing();
    }
    
    
    double
    RelativePoint::getSlotSize() const
    {
        return Staff::getSlotSize();
    }


	void
	RelativePoint::setYSlots(double inValueSlots)
	{
        if (getIsYLocked()) {
            const auto spatialSlot = static_cast<double>(getItem().getSlot());
            mLockedYSlot = spatialSlot - inValueSlots;
        } else {
            mRelativeYSlots = inValueSlots;
        }
	}
    
    
    void
    RelativePoint::addYSlots(double inValueSlots)
    {
        if (getIsYLocked()) {
            mLockedYSlot += inValueSlots;
        } else {
            mRelativeYSlots += inValueSlots;
        }
    }

    
    void
    RelativePoint::addYPixels(double inValuePixels)
    {
        const auto slotConversion = inValuePixels / (-1 * getSlotSize());
        addYSlots(slotConversion);
    }
    
    
    void
    RelativePoint::setSlot(double inSlot)
    {
        auto adjustmentAmount = inSlot - getSlot();
        addYSlots(adjustmentAmount);
    }
    
    
    void
    RelativePoint::setScoreY(double inScorePixels)
    {
        const auto spatialItemYPosition = getItem().getScoreY();
        const auto yDelta = spatialItemYPosition - inScorePixels;
        const auto ySlotDelta = yDelta / Staff::getSlotSize();
        setYSlots(ySlotDelta);
    }
    
    
    void
    RelativePoint::setScoreX(double inScorePixels)
    {
        if (getIsXLocked()) {
            mLockedX = inScorePixels;
        } else {
            mRelativeX = inScorePixels - getItem().getScoreX();
        }
    }
    
	
    void
	RelativePoint::setX(double inValuePixels)
	{
        if (getIsXLocked()) {
            mLockedX = getItem().getScoreX() + inValuePixels;
        } else {
            mRelativeX = inValuePixels;
        }
	}
    
    
    void
    RelativePoint::addX(double inValuePixels)
    {
        if (getIsXLocked()) {
            mLockedX += inValuePixels;
        } else {
            mRelativeX += inValuePixels;
        }
    }
    
    
    double
    RelativePoint::getMeasureRelativeX() const
    {
        const double scoreX = getScoreX();
        const auto& measure = getItem().getLocation().getMeasure();
        const double measureDataX = measure.getDataX();
        return scoreX - measureDataX;
    }
    
    
    void
    RelativePoint::setMeasureRelativeX(double inValue)
    {
        const double measureDataX = getMeasure().getDataX();
        const double scoreX = measureDataX + inValue;
        setScoreX(scoreX);
    }
    
    
    double
	RelativePoint::getYSlots() const
	{
        if (getIsYLocked()) {
            const auto spatialSlot = static_cast<double>(getItem().getSlot());
            return mLockedYSlot - spatialSlot;
        } else {
            return mRelativeYSlots;
        }
	}

	
    double
	RelativePoint::getYPixels() const
	{
        // slots go 'up' pixels go 'down' so flip the sign
        return -1 * (getYSlots() * getSlotSize());
	}
    
    
    double
    RelativePoint::getSlot() const
    {
        if (getIsYLocked()) {
            return mLockedYSlot;
        } else {
            const double spatialSlot = static_cast<double>(getItem().getSlot());
            return spatialSlot + getYSlots();
        }
    }
    
    
    double
    RelativePoint::getX() const
    {
        return mRelativeX;
    }

    
    double
	RelativePoint::getScoreX() const
	{
        if (getIsXLocked()) {
            return mLockedX;
        } else {
            return getItem().getScoreX() + mRelativeX;
        }
	}

	
    double
	RelativePoint::getScoreY() const
	{
        const auto spatialPtrPosition = getItem().getScoreY();
        const auto yOffsetPixels = getYPixels();
        const auto result = spatialPtrPosition + yOffsetPixels;
        return result;
	}

    bool
    RelativePoint::getIsYLocked() const
    {
        return mIsYLocked;
    }
    
    
    void
    RelativePoint::lockY()
    {
        if (getIsYLocked()) {
            return;
        }
        mLockedYSlot = getSlot();
        mIsYLocked = true;
        mRelativeYSlots = std::numeric_limits<double>::infinity();
    }
    
    void
    RelativePoint::unlockY()
    {
        if (!getIsYLocked()) {
            return;
        }
        mRelativeYSlots = getYSlots();
        mIsYLocked = false;
        mLockedYSlot = std::numeric_limits<double>::infinity();
    }
    
    bool
    RelativePoint::getIsXLocked() const
    {
        return mIsXLocked;
    }
    
    
    void
    RelativePoint::lockX()
    {
        if (getIsXLocked()) {
            return;
        }
        mLockedX = getScoreX();
        mIsXLocked = true;
        mRelativeYSlots = std::numeric_limits<double>::infinity();
    }
    
    
    void
    RelativePoint::unlockX()
    {
        if (!getIsXLocked()) {
            return;
        }
        mRelativeX = getScoreX() - getItem().getScoreX();
        mIsXLocked = false;
        mLockedX = std::numeric_limits<double>::infinity();
    }


    double
    RelativePoint::getTime() const
    {
        const auto& measure = getMeasure();
        return measure.time(getStaff().getTrack(),
                            getMeasureRelativeX());
    }
    

    void
    RelativePoint::setTime(double inValue)
    {
        const auto& measure = getMeasure();
        auto measureRelativeX = measure.positionForTime(inValue);
        setMeasureRelativeX(measureRelativeX);
    }


    int
    RelativePoint::getRoundedTime() const
    {
        return static_cast<int>(std::ceil(getTime() - 0.5));
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
