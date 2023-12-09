// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{

    KOMP_FORWARD_DECLARE(SpatialPointIntf);

    /// \brief  describes a spatial point in the score
    ///
    /// \discussion The purpose of SpatialPointIntf is to make the anchoring
    /// of a SpatialPoint transparent to Score Items.  For example, when 
    /// we specify a point in the score we may want that point to be
    /// relative to a Staff and Measure (in pixels and slots), or relative to
    /// a Staff and Measure (in Time and slots), or relative to another
    /// Item, or relative to the Score's spatial plane.  The details of this
    /// anchoring will be implemented by subclasses of this interface so that
    /// Score Items need not know the details of their anchoring.
    ///
	class SpatialPointIntf
	{
	public:
        virtual ~SpatialPointIntf() {}

        virtual SpatialPointIntfUPtr clone() const = 0;
        virtual Measure& getMeasure() const = 0;
        virtual Staff& getStaff() const = 0;
        
        /// get the height of a staff space, in pixels,
        /// at this SpatialPoint's location.  If this object is
        /// not associated with a specific Staff, then the
        /// default SpaceSize for the Score is returned.
        virtual double getSpaceSize() const = 0;

        /// get the height of a slot, in pixels, at this
        /// SpatialPoint's location. If this object is not
        /// associated with a specific Staff, then the default
        /// SpaceSize for the Score is returned.
        virtual double getSlotSize() const = 0;

		/// set the point's Y-value, relative to the SpatialPtr's
        /// anchor, which is implementation-defined. For example
        /// this could be relative to the staff, or to an Item
		virtual void setYSlots(double inValueSlots) = 0;
        
        /// adds the inValue to the current Y-slots value
        virtual void addYSlots(double inValueSlots) = 0;
        
        /// converts the inValue (pixels) into slots
        /// and adds that to the Y-slots value. Y-slots and
        /// Y-pixels represent the same thing (the relative
        /// Y-position) but in different measurement units.
        virtual void addYPixels(double inValuePixels) = 0;
        
        /// sets the slot with respect to the staff,
        /// irrespective of the anchor point
        virtual void setSlot(double inSlot) = 0;
        
        /// set the point's Y-value in absolute
        /// score pixels. this will be converted and stored
        /// as a slot offset relative to this object's anchor
        virtual void setScoreY(double inScorePixels) = 0;
        
		/// set the point's X-value, relative to the this
        /// objects anchor. for example it could be relative
        /// to the start of a measure's dataX, it could be
        /// relative to an Item, or it could be an absolute
        /// Score location. X will always be denominate in
        /// score pixels.
		virtual void setX(double inValuePixels) = 0;
        
        /// adds the inValue to the current x (pixels)
        virtual void addX(double inValuePixels) = 0;
        
        /// gets x, in pixels, relative to the start of
        /// the measure's dataX
        virtual double getMeasureRelativeX() const = 0;
        
        /// sets x, in pixels, relative to the start of
        /// the measure's dataX
        virtual void setMeasureRelativeX(double inValue) = 0;
        
        /// set the point's X-value in absolute score pixels.
        virtual void setScoreX(double inScorePixels) = 0;
        
        /// Get the point's Y-value, relative to the
		/// the anchor, denomitated in slots
        virtual double getYSlots() const = 0;

        /// Get the point's Y-value, relative to the,
		/// anchor, denomitated in pixels
        virtual double getYPixels() const = 0;
        
        /// get the point's absolute Y-slot, i.e. returns
        /// the slot value relative to the staff.
        virtual double getSlot() const = 0;

        /// set the point's X-value, relative to the anchor,
        /// denomitated in pixels
        virtual double getX() const = 0;

        /// get the absolute x, in score space, in pixels
        virtual double getScoreX() const = 0;
        
        /// get the absolute y, in score space, in pixels
        virtual double getScoreY() const = 0;
        
        /// is Y Locked.  when Y is locked, positional changes to the
        /// attached anchor will no longer affect the SpatialPoint's
        /// Y-position. For example, if the SpatialPoint is defined
        /// relative to an Item, and if Y is locked, then Y movements
        /// of the attached Item will not affect the SpatialPoint's Y.
        virtual bool getIsYLocked() const = 0;

        /// lock Y.  when Y is locked, positional changes to the
        /// attached anchor will no longer affect the SpatialPoint's
        /// Y-position. For example, if the SpatialPoint is defined
        /// relative to an Item, and if Y is locked, then Y movements
        /// of the attached Item will not affect the SpatialPoint's Y.
        virtual void lockY() = 0;
        
        /// unlock Y.  when Y is locked, positional changes to the
        /// attached anchor will no longer affect the SpatialPoint's
        /// Y-position. For example, if the SpatialPoint is defined
        /// relative to an Item, and if Y is locked, then Y movements
        /// of the attached Item will not affect the SpatialPoint's Y.
        virtual void unlockY() = 0;
        
        /// is X Locked.  when X is locked, positional changes to the
        /// attached anchor will no longer affect the SpatialPoint's
        /// X-position. For example, if the SpatialPoint is defined
        /// relative to an Item, and if X is locked, then X movements
        /// of the attached Item will not affect the SpatialPoint's X.
        virtual bool getIsXLocked() const = 0;
        
        /// lock X.  when X is locked, positional changes to the
        /// attached anchor will no longer affect the SpatialPoint's
        /// X-position. For example, if the SpatialPoint is defined
        /// relative to an Item, and if X is locked, then X movements
        /// of the attached Item will not affect the SpatialPoint's X.
        virtual void lockX() = 0;
        
        /// unlock X.  when X is locked, positional changes to the
        /// attached anchor will no longer affect the SpatialPoint's
        /// X-position. For example, if the SpatialPoint is defined
        /// relative to an Item, and if X is locked, then X movements
        /// of the attached Item will not affect the SpatialPoint's X.
        virtual void unlockX() = 0;

        /// get the point's time position in the measure
        virtual double getTime() const = 0;
        
        /// set the point's time position in the measure
        virtual void setTime(double inValue) = 0;

        /// get the point's time positio in the measure, rounded to
        /// the nearest integer
        virtual int getRoundedTime() const = 0;
	};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
