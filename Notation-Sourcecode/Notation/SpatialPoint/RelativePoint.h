// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "SpatialPointIntf.h"

namespace komp
{
    /// \brief  A spatial point defined relative to a spatial object
    ///
    /// \discussion Describes a point on the score in relation to a spatial object
    /// in the score. Provides scaling and absolute position pixel values.
    ///
    /// @pre the SpatialPtr must have a validScore, Measure, Track and Staff
    /// pointers. Constructor will throw if any of these are invalid (i.e. null).
    ///
    /// @warning A cautionary note about Y-direction values:'slots' increase in
    /// value as you move 'up'Y-Pixels originate at the top of the score and
    /// increase in value as you move 'down'. Thus, a positive slot distance of 1
    /// converts to, for example, a Y-Pixel distance of -6.0.
	class RelativePoint : public SpatialPointIntf
	{
	public:
        RelativePoint(Item& inItem,
                      double inRelativeX,
                      double inRelativeYSlot);

        RelativePoint(Item& inItem);

        virtual SpatialPointIntfUPtr clone() const override;
        virtual Measure& getMeasure() const override;
        virtual Staff& getStaff() const override;

        Item& getItem() const;
        
        /// get the height of a staff space in pixels
        virtual double getSpaceSize() const override;

        /// get the height of a slot in pixels
        virtual double getSlotSize() const override;

		/// set the point's Y-value, relative to the
		/// SpatialPtr's position, denomitated in slots
		virtual void setYSlots(double inValueSlots) override;
        
        /// adds the inValue to the current Y-slots value
        virtual void addYSlots(double inValueSlots) override;
        
        virtual void setSlot(double inSlot) override;
        
        /// converts the inValue (pixels) into slots
        /// and adds that to the YSlots value.  Note slots and
        /// pixels go in different directions so add positive
        /// y pixels will make the YSlots value smaller.
        virtual void addYPixels(double inValuePixels) override;
        
        /// set the point's Y-value in absolute
        /// score pixels. this will be converted and stored
        /// as a slot offset from the spatial ptr's slot
        virtual void setScoreY(double inScorePixels) override;
        
		/// set the point's X-value, relative to the
		/// SpatialPtr's position, denomitated in pixels
		virtual void setX(double inValuePixels) override;
        
        /// adds the inValue to the current x (pixels)
        virtual void addX(double inValuePixels) override;
        
        /// gets x, in pixels, relative to
        /// getSpatial()->getMeasure()->getDataX()
        virtual double getMeasureRelativeX() const override;
        
        /// sets x, in pixels, relative to the start of
        /// the measure's dataX
        virtual void setMeasureRelativeX(double inValue) override;
        
        /// set the point's X-value in absolute
        /// score pixels. this will be converted and stored
        /// as a pixel offset from the spatial ptr.
        virtual void setScoreX(double inScorePixels) override;
        
        /// set the point's Y-value, relative to the
		/// SpatialPtr's position, denomitated in slots
        virtual double getYSlots() const override;

        /// set the point's Y-value, relative to the
		/// SpatialPtr's position, denomitated in pixels
        virtual double getYPixels() const override;
        
        /// get the point's absolute Y-slot, i.e. returns
        /// the Spatial->getSlot() + getYSlots()
        virtual double getSlot() const override;

        /// set the point's X-value, relative to the
        /// SpatialPtr's position, denomitated in pixels
        virtual double getX() const override;

        /// get the absolute x in score pixels
        virtual double getScoreX() const override;
        
        /// get the absolute y in  score pixels
        virtual double getScoreY() const override;
                
        /// find out if Y is locked.  when Y is locked, positional
        /// Changes to the attached Spatial's Y-position will no
        /// longer affect this SpatialPoint's Y-position.
        virtual bool getIsYLocked() const override;

        /// lock Y.  when Y is locked, positional
        /// Changes to the attached Spatial's Y-position will no
        /// longer affect this SpatialPoint's Y-position.
        virtual void lockY() override;
        
        /// unlock Y.  when Y is locked, positional
        /// Changes to the attached Spatial's Y-position will no
        /// longer affect this SpatialPoint's Y-position.
        virtual void unlockY() override;
        
        /// find out if X is locked.  when X is locked, positional
        /// Changes to the attached Spatial's X-position will no
        /// longer affect this SpatialPoint's X-position.
        virtual bool getIsXLocked() const override;
        
        /// lock X.  when X is locked, positional
        /// Changes to the attached Spatial's X-position will no
        /// longer affect this SpatialPoint's X-position.
        virtual void lockX() override;
        
        /// unlock X.  when X is locked, positional
        /// Changes to the attached Spatial's X-position will no
        /// longer affect this SpatialPoint's X-position.
        virtual void unlockX() override;

        /// get the point's time position in the measure
        virtual double getTime() const override;
        
        /// set the point's time position in the measure
        virtual void setTime(double inValue) override;

        /// get the point's time positio in the measure, rounded to
        /// the nearest integer
        virtual int getRoundedTime() const override;

	private:
        Item* mItem;               ///< The Spatial item to which this SpatialPoint is tied
		double mRelativeX;         ///< When we are unlocked - this SpatialPoint's x position (in pixels) as an offset from its Spatial item
		double mRelativeYSlots;    ///< When we are unlocked - this SpatialPoint's y position (in slots) relative as an offset from its Spatial item
        double mLockedX;           ///< When we are locked - this SpatialPoint's x position (in score pixels)
        double mLockedYSlot;       ///< When we are locked - this SpatialPoint's y position (as a slot value) relative to the Staff
        bool mIsYLocked;           ///< Is the y-position locked?
        bool mIsXLocked;           ///< Is the x-position locked?
	};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
