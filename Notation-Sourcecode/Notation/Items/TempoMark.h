// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Spatial.h"
#include "TempoValue.h"

// Forward declarations
namespace komp
{
    enum class TemporalType;
}

namespace komp
{
    class KOMP_EXPORT TempoMark : public Spatial
    {
    public:
        virtual ~TempoMark() {}
        
        virtual const ItemTraits& getItemTraits() const override;
        virtual void update() override final;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override {}
        
        virtual void inserted() override;
        virtual void removed()override;

        virtual Rect getFrame() const override;
        inline virtual Region getRegion() const override;
        inline virtual Point getAnchor() const override { return POINT_ZERO; }

        virtual Point getDrawPoint() const override final;
        virtual double getScoreX() const override;

        ///@{ Draggable
        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        ///@}

        TempoValue getTempoValue() const;
        void setTempoValue(const TempoValue& inValue);
        
        static constexpr const int DefaultSlot{16};

    private:

        friend class ItemRegistry;
        
        TempoMark() = delete;
        
        TempoMark(const MusicLocation& inLocation, TempoValue inTempoValue);

        ///@{ Item
        virtual void createDrawable() override;
        ///@}
        
        TempoValue mTempoValue;
        komp::Point mDragOffset;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
