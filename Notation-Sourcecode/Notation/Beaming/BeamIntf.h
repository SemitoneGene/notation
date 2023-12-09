// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include "Forward.h"
#include "BeamedNoteIntf.h"
#include "BeamPlacement.h"
#include "Spatial.h"
#include "StemPolicy.h"

namespace komp
{
    /// the length of a fractional beam in score pixels
    constexpr const double FRACTIONAL_BEAM_LENGTH = 4.5;
    
    /// \brief  nope
    ///
    /// \discussion nope
    ///
	class BeamIntf : public Item
	{
	public:
        virtual ~BeamIntf() = default;

        ///@{ Item
        virtual const ItemTraits& getItemTraits() const override = 0;
        virtual ItemUPtr getMemento() const override = 0;
        virtual void restoreMemento(const Item& inItem) override = 0;
        virtual void update() override = 0;
        virtual void itemIDChanged() override = 0;
        virtual void itemIDInvalidated() override = 0;
        virtual void registerObserver(ItemObserver& inObserver) override = 0;
        virtual void unregisterObserver(ItemObserver& inObserver) override = 0;
        virtual void notifyObservers(const ItemChanges& inChanges) override = 0;
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override = 0;
        virtual bool getIsQuiescent() const override = 0;
        virtual void setIsQuiescent(bool inValue) override = 0;
        virtual Rect getFrame() const override = 0;
        virtual Region getRegion() const override = 0;
        virtual Point getAnchor() const override = 0;
        virtual bool contains(const Point& inPoint, bool inPrecise) const override = 0;
        virtual void inserted() override = 0;
        virtual void removed() override = 0;
        virtual void removeSelf() override = 0;
        virtual const MusicLocation& getLocation() const override = 0;
        virtual double getX() const override = 0;
        virtual void setX(double inValue) override = 0;
        virtual double getScoreX() const override = 0;
        virtual int getSlot() const override = 0;
        virtual void setSlot(int inValue) override = 0;
        virtual double getScoreY() const override = 0;
        virtual Point getDrawPoint() const override = 0;
        virtual double getTime() const override = 0;
        virtual int getRoundedTime() const override = 0;
        virtual void setTime(double inValue) override = 0;
        virtual double getSpaceSize() const override = 0;
        virtual double getFontSize() const override = 0;
        virtual void notifyMeasureXChanged() override = 0;
        virtual void notifyStaffYChanged() override = 0;
        ///@}

        /// get a frame that surropunds only the beam lines and not the stems
        virtual Rect getBeamLinesFrame() const = 0;

        /// Recalculate the beam's location and appearance before drawing
        virtual void updateBeam() = 0;
        
        /// get the notes that are part of this beam
        virtual const BeamedNoteIntfUPtrVec& getBeamedNotes() const = 0;

        /// get the notes that are part of this beam
        virtual TemporalRefVec getNotes() const = 0;

        /// get the stem policy
        virtual StemPolicy getStemPolicy() const = 0;

        /// set the stem policy
        virtual void setStemPolicy(StemPolicy inPolicy) = 0;

        /// returns true if the slope of the beam is flat
        virtual bool getIsFlat() const = 0;

        /// gets the Score Y intercept point at the top of the
        /// main beam line for the given Score X position.
        virtual double getYIntercept(double inScoreX) const = 0;

        /// gets the thickness of the given OldBeamLine
        virtual double getBeamLineThickness(int inOldBeamLineIndex) const = 0;

        /// gets the frame of the stem for the given temporal
        virtual Rect getStemFrame(const Temporal& inTemporal) const = 0;

        // returns true if the passed temporal is contained in the beam
        virtual bool getIsMember(const Temporal& inTemporal) const = 0;

        // returns true if the passed temporal is the first in the beam
        virtual bool getIsFirst(const Temporal& inTemporal) const = 0;

        // returns true if the passed temporal is the last in the beam
        virtual bool getIsLast(const Temporal& inTemporal) const = 0;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
