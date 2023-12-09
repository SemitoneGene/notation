// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Item.h"
#include "TupletRatio.h"
#include "TupletParams.h"

namespace komp
{
    enum class TupletNumberType
    {
        NONE,
        NUMBER,
        RATIO
    };

    class TupletIntf : public Item
    {
    public:
        ///@{ Item
        virtual ~TupletIntf() = default;
        virtual const ItemTraits& getItemTraits() const override = 0;
        virtual ItemUPtr getMemento() const override = 0;
        virtual void restoreMemento(const Item& inItem) override = 0;
        virtual void itemIDChanged() override = 0;
        virtual void itemIDInvalidated() override = 0;
        virtual void registerObserver(ItemObserver& inObserver) override = 0;
        virtual void unregisterObserver(ItemObserver& inObserver) override = 0;
        virtual bool getIsQuiescent() const override = 0;
        virtual void setIsQuiescent(bool inValue) override = 0;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override = 0;
        virtual Rect getFrame() const override = 0;
        virtual Region getRegion() const override = 0;
        virtual Point getAnchor() const override = 0;
        virtual bool contains(const Point& inPoint, bool inPrecise) const override = 0;
        virtual void inserted() override = 0;
        virtual void removed()override = 0;
        virtual void removeSelf() override = 0;
        virtual const MusicLocation& getLocation() const override = 0;
        virtual double getX() const override = 0;
        virtual void setX(double inValue) override = 0;
        virtual double getScoreX() const override = 0;
        virtual int getSlot() const override = 0;
        virtual void setSlot(int inValue) override = 0;
        virtual double getScoreY() const override = 0;
        virtual double getTime() const override = 0;
        virtual int getRoundedTime() const override = 0;
        virtual void setTime(double inValue) override = 0;
        ///@}
        
        ///@{ Tuplet
        
        /// Returns the temporals that are affected by this tuplet.
        /// Will return a Note if the Note is not Chorded.  If a
        /// Note is Chorded then the Chord will be returned and its
        /// members will not.
        virtual const TemporalRefVec& getTemporals() const = 0;
        
        /// Returns true if the inTemporal is affected by this
        /// tuplet, otherwise returns false.
        virtual bool isMember(const Temporal& inTemporal) const = 0;
        
        /// Returns the first temporal in this tuplets group.
        virtual Temporal* getFirst() const = 0;
        
        /// Returns the last temporal in this tuplets group.
        virtual Temporal* getLast() const = 0;
        
        /// get this tuplets duration modifying ratio
        virtual const TupletRatio& getTupletRatio() const = 0;

        /// the tuplet will update its appearance and position
        virtual void updateTuplet() = 0;

        /// tells all of the tuplet's temporals to remove this tuplet
        virtual void unsetTemporals() = 0;
        
        ///@}        
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

