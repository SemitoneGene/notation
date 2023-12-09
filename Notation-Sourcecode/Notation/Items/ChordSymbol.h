// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "ChordSymbolParams.h"
#include "Item.h"

namespace komp
{
    class KOMP_EXPORT ChordSymbol : public Item
    {
    public:

        ///@{ Item
        virtual ~ChordSymbol() = default;
        ChordSymbol(const ChordSymbol& other);
        ChordSymbol(ChordSymbol&& other);
        ChordSymbol& operator=(const ChordSymbol& other);
        ChordSymbol& operator=(ChordSymbol&& other);
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override final;
        virtual void restoreMemento(const Item& inItem) override final;
        virtual void update() override final;
        virtual void itemIDInvalidated() override final;
        virtual void registerObserver(ItemObserver& inObserver) override final;
        virtual void unregisterObserver(ItemObserver& inObserver) override final;
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

        /// Drag affects both X and Time
        virtual void drag(const CurrentState& inState) override final;
        ///@}

        ///@{ ChordSymbol
        const ChordSymbolValue& getChordSymbolValue() const;
        void setChordSymbolValue(ChordSymbolValue inValue);
        void setXFromTime();
        void setTimeFromX();
        bool getIsTimeLocked() const;
        ///@}

    private:
        friend class ItemRegistry;

        ///@{ Item
        virtual void notifyObservers(const ItemChanges& inChanges) override final;
        virtual void itemIDChanged() override final;
        virtual void createDrawable() override;
        ///@}

        /// private constructor available only to the ItemRegistry
        ChordSymbol(const MusicLocation& inLocation,
                    const ChordSymbolParams& inChordSymbolParams);

    private:
        static constexpr const double DEFAULT_CHORD_SYMBOL_SLOT = 17.0;

        int mHand;
        int mVoice;
        int mTime;
        double mSlot{DEFAULT_CHORD_SYMBOL_SLOT};
        double mX{NAN};
        bool mDoResetXUponMeasureInsert{false};
        ItemObserverRefVec mObservers;
        bool mIsQuiescent{true};
        ChordSymbolValue mChordSymbolValue;
        bool mIsXTimeLocked{true};
        MusicLocation mLocation;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

