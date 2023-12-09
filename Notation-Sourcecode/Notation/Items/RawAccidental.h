// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableAccidental.h"
#include "Forward.h"
#include "Item.h"
#include "SpaceTimePoint.h"

namespace komp
{
    class KOMP_EXPORT RawAccidental : public Item
    {
    public:

        ///@{ Item
        virtual ~RawAccidental();
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override final;
        virtual void restoreMemento(const Item& inItem) override final;
        virtual void update() override final {};
        virtual void itemIDInvalidated() override final;
        virtual void registerObserver(ItemObserver& inObserver) override final;
        virtual void unregisterObserver(ItemObserver& inObserver) override final;
        virtual void notifyObservers(const ItemChanges& inChanges) override final;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override final;
        virtual bool getIsQuiescent() const override final;
        virtual void setIsQuiescent(bool inValue) override final;
        virtual Rect getFrame() const override final;
        virtual Region getRegion() const override final;
        virtual Point getAnchor() const override final;
        virtual bool contains(const Point& inPoint, bool inPrecise) const override final;
        virtual void inserted() override final;
        virtual void removed() override final;
        virtual void removeSelf() override final;
        virtual const MusicLocation& getLocation() const override;
        virtual double getX() const override final;
        virtual void setX(double inValue) override final;
        virtual double getScoreX() const override final;
        virtual int getSlot() const override final;
        virtual void setSlot(int inValue) override final;
        virtual double getScoreY() const override final;
        virtual Point getDrawPoint() const override final;
        virtual double getTime() const override final;
        virtual int getRoundedTime() const override final;
        virtual void setTime(double inValue) override final;
        virtual double getSpaceSize() const override final;
        virtual double getFontSize() const override final;
        ///@}

        ///@{ RawAccidental
        AccidentalType getAccidentalType() const;
        ///@}

    private:
        friend class ItemRegistry;

        ///@{ Item
        virtual void itemIDChanged() override final;
        virtual void createDrawable() override;
        ///@}

        /// private constructor available only to the ItemRegistry
        RawAccidental(ItemRegistry& inRegistry,
                      const MusicLocation& inLocation,
                      AccidentalType inAccidentalType);

    private:
        SpaceTimePoint mSpaceTimePoint;
        ItemObserverRefVec mObservers;
        bool mIsQuiescent{true};
        MusicLocation mLocation;
        AccidentalType mAccidentalType;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

