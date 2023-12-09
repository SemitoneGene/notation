// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "SpatialPointIntf.h"

namespace komp
{
    /// \brief  describes a spatial point relative to a measure and staff
    ///
    /// \discussion stores both time and x separately (e.g. for Temporals).
    /// All values are stored relative to the staff and measure, but the
    /// staff and measure values are not stored themselves.  Instead the
    /// SpaceTimePoint stores an ItemID and queries the ItemRegistry.
    /// Operations will throw if the ItemID is invalid or if the Item is
    /// not inserted in a measure (i.e. has a null measure).
    ///
	class SpaceTimePoint : public SpatialPointIntf
	{
	public:
        SpaceTimePoint() = delete;
        
        SpaceTimePoint(Item& inItem);

        virtual ~SpaceTimePoint() = default;
        
        /// \brief  Set the Item
        void setItem(Item& inItem);
                
        /// \brief Gets the Item. throws if the Item is not set.
        Item& getItem() const;
        
        ///@{ SpatialPointIntf ///////////////////////////////////////////////
        virtual SpatialPointIntfUPtr clone() const override;
        virtual Measure& getMeasure() const override;
        virtual Staff& getStaff() const override;
        virtual double getSpaceSize() const override;
        virtual double getSlotSize() const override;
		virtual void setYSlots(double inValueSlots) override;
        virtual void addYSlots(double inValueSlots) override;
        virtual void addYPixels(double inValuePixels) override;
        virtual void setSlot(double inSlot) override;
        virtual void setScoreY(double inScorePixels) override;
		virtual void setX(double inValuePixels) override;
        virtual void addX(double inValuePixels) override;
        virtual double getMeasureRelativeX() const override;
        virtual void setMeasureRelativeX(double inValue) override;
        virtual void setScoreX(double inScorePixels) override;
        virtual double getYSlots() const override;
        virtual double getYPixels() const override;
        virtual double getSlot() const override;
        virtual double getX() const override;
        virtual double getScoreX() const override;
        virtual double getScoreY() const override;
        virtual bool getIsYLocked() const override;
        virtual void lockY() override;
        virtual void unlockY() override;
        virtual bool getIsXLocked() const override;
        virtual void lockX() override;
        virtual void unlockX() override;
        virtual double getTime() const override;
        virtual void setTime(double inValue) override;
        virtual int getRoundedTime() const override;
        ///@} SpatialPointIntf ///////////////////////////////////////////////

    private:
        ItemRef mItem;
        double mX;
        double mSlot;
        double mTime;
	};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
