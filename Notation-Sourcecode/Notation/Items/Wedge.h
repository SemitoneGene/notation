// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Spanner.h"
#include "WedgeType.h"

namespace komp
{
    struct WedgeParams
    {
        SpannerParams spannerParams;
        WedgeType wedgeType;
        PlacementType placementType;

        WedgeParams()
        : spannerParams{}
        , wedgeType{WedgeType::CRESCENDO}
        , placementType{PlacementType::BELOW_ITEM}
        {

        }

        WedgeParams(const SpannerParams& inParams,
                    WedgeType inWedgeType,
                    PlacementType inPlacementType)
        : spannerParams{inParams}
        , wedgeType{inWedgeType}
        , placementType{inPlacementType}
        {
        }
    };

    class KOMP_EXPORT Wedge : public Spanner
    {
    public:
        Wedge(const Wedge& inItem);
        
        virtual ~Wedge();
        
        virtual Wedge& operator=(const Wedge& other);
        
        virtual const ItemTraits& getItemTraits() const override;
        virtual void update() override final;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override {}
        
        virtual Rect getFrame() const override;
        virtual Region getRegion() const override;
        inline virtual Point getAnchor() const override { return POINT_ZERO; }
        
        virtual void inserted() override;
        virtual void removed() override;

        WedgeType getWedgeType() const;
        
        ///@{ Draggable /////////////////////////////////////////////
        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        ///@} Draggable /////////////////////////////////////////////

        Rect getPositions() const;
        
    private:
        friend class ItemRegistry;
        
        Wedge() = delete;
        
        Wedge(const MusicLocation& inLocation,
              const WedgeParams& inWedgeParams);
        
        ///@{ Item
        virtual void createDrawable() override;
        ///@}

        /// operator=() implementation
        void copyState(const Wedge& other);
        
        // Height is in staff units
        int mHeight;
        WedgeType mType;
        
        // Used to calculate offset into drag mark maps
        WedgeType mDragStartType;
        
        // Indicates dragging has been locked to an axis
        bool mDragAxisLock{false};
        
        // Dragging is locked to the X-axis
        bool mDragAxisX{false};
        
        // Dragging is locked to the Y-axis
        bool mDragAxisY{false};

    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
