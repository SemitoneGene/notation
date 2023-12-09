// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <iostream>
#include <memory>
#include <set>

// Local
#include "Draggable.h"
#include "Item.h"
#include "PlacementType.h"
#include "SpaceTimePoint.h"

namespace komp
{
    class KOMP_EXPORT Spatial : public Item
    {
    public:
        Spatial() = delete;
        Spatial(const MusicLocation& inLocation);

        virtual ~Spatial() = default;

        ///@{ Item //////////////////////////////////////////////////
        virtual const ItemTraits& getItemTraits() const override = 0;
        virtual ItemUPtr getMemento() const override = 0;
        virtual void restoreMemento(const Item& inItem) override = 0;
        virtual void update() override {};
        virtual void itemIDChanged() override;
        virtual void itemIDInvalidated() override;
        virtual void registerObserver(ItemObserver& inObserver) override final;
        virtual void unregisterObserver(ItemObserver& inObserver) override final;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override = 0;
        virtual bool getIsQuiescent() const override;
        virtual void setIsQuiescent(bool inValue) override;
        virtual Rect getFrame() const override = 0;
        virtual Region getRegion() const override = 0;
        virtual bool contains(const Point& inPoint, bool inPrecise) const override;
        virtual void inserted() override;
        virtual void removed()override;
        virtual void removeSelf() override;
        virtual const MusicLocation& getLocation() const override;
        virtual double getX() const override;
        virtual void setX(double inValue) override;
        virtual double getScoreX() const override;
        virtual int getSlot() const override;
        virtual void setSlot(int inValue) override;
        virtual double getScoreY() const override;
        virtual Point getDrawPoint() const override;
        virtual double getTime() const override;
        virtual int getRoundedTime() const override;
        virtual void setTime(double inValue) override;
        virtual double getSpaceSize() const override;
        virtual double getFontSize() const override;
        ///@} Item //////////////////////////////////////////////////
        
        ///@{ Draggable /////////////////////////////////////////////
        virtual void drag(const CurrentState& inState) override;
        ///@} Draggable /////////////////////////////////////////////
        
        ///@{ Spatial ///////////////////////////////////////////////
        double characterWidth(const unsigned short inChar) const;
        double characterHeight(const unsigned short inChar) const;
        komp::Size measureCharacter(const unsigned short inChar) const;
        SpatialPointIntf& getSpatialPoint();
        const SpatialPointIntf& getSpatialPoint() const;
        ///@} Spatial ///////////////////////////////////////////////

    protected:
        virtual void notifyObservers(const ItemChanges& inChanges) override final;

        MusicLocation mLocation;
        
    private:
        ///@{ Item
        virtual void createDrawable() override = 0;
        ///@}

        SpaceTimePoint mSpaceTimePoint;
        ItemObserverRefVec mObservers;
        bool mIsQuiescent{true};
        ItemChanges mDeferredChanges;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
