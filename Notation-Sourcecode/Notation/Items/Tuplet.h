// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "AboveBelow.h"
#include "Policy.h"
#include "RelativePoint.h"
#include "TupletIntf.h"
#include "TupletParams.h"
#include "UpDown.h"

namespace komp
{
    class KOMP_EXPORT Tuplet : public TupletIntf
    {
    public:
        
        ///@{ Item
        virtual ~Tuplet();
        Tuplet(const Tuplet& other) = default;
        Tuplet& operator=(const Tuplet& other) = default;
        Tuplet(Tuplet&& other) = delete;
        Tuplet& operator=(Tuplet&& other) = delete;
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
        virtual const MusicLocation& getLocation() const override final;
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
        
        ///@{ Tuplet
        virtual const TemporalRefVec& getTemporals() const override final;
        virtual bool isMember(const Temporal& inTemporal) const override final;
        virtual Temporal* getFirst() const override final;
        virtual Temporal* getLast() const override final;
        virtual const TupletRatio& getTupletRatio() const override final;
        virtual void updateTuplet() override final;
        virtual void unsetTemporals() override final;
        ///@}

        static constexpr const double DEFAULT_HOOK_LENGTH_SPACES = 0.5;
        static constexpr const double DEFAULT_THICKNESS_SPACES = 0.125;
        static constexpr const double DEFAULT_NUMBER_MARGIN_SIZE_SPACES = 0.25;
        
    private:
        ///@{ Item
        virtual void itemIDChanged() override final;
        virtual void notifyObservers(const ItemChanges& inChanges) override final;
        ///@}
        
        friend class ItemRegistry;
        
        /// private constructor available only to the ItemRegistry
        Tuplet(const MusicLocation& inLocation,
               const TupletParams& inTupletParams);
        
        /// throws if the tuplet is in an invalid state
        void validate() const;
        
        /// sorts the temporals into time order
        void sort(TemporalRefVec& outTemporals) const;
        
        /// updates the temporals duration values to account for this tuplet
        void setTemporals();

        /// sets the appearance attributes of the tuplet
        /// returns a Rect which surrounds the temporals
        Rect analyzeAndSetAttributes();

        /// creates the drawable objects
        void createDrawableObjects(const Rect& inAvoidanceFrame);

        /// creates the tuplet number
        void createTupletNumber(const Rect& inAvoidanceFrame);

        /// creates the bracket
        void createTupletBracket(const Rect& inAvoidanceFrame);

        /// if the tuplet's frame intersects with that of the staff, move it
        void moveOutsideStaff();
        
        virtual void createDrawable() override;
        
    private:
        TemporalRefVec mTemporals;
        TupletRatio mTupletRatio;
        bool mAreTemporalsSet{false};
        ItemObserverRefVec mObservers;
        Policy mAboveBelowPolicy{Policy::AUTOMATIC};
        AboveBelow mAboveBelow{AboveBelow::ABOVE};
        Policy mNumberTypePolicy{Policy::AUTOMATIC};
        TupletNumberType mNumberType{TupletNumberType::NUMBER};
        Policy mHasBracketPolicy{Policy::AUTOMATIC};
        bool mHasBracket{false};
        Policy mHookDirectionPolicy{Policy::AUTOMATIC};
        UpDown mHookDirection{UpDown::DOWN};
        Policy mHookLengthPolicy{Policy::AUTOMATIC};
        double mHookLength{2.5};
        Policy mThicknessPolicy{Policy::AUTOMATIC};
        double mThickness{DEFAULT_THICKNESS_SPACES};
        bool mIsBeamedTuplet{false};
        double mYBoundary{0.0};
        bool mIsQuiescent{true};
        MusicLocation mLocation;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

