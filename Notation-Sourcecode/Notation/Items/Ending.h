// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableEnding.h"
#include "EndingParams.h"
#include "Forward.h"
#include "Spatial.h"

namespace komp
{
    class KOMP_EXPORT Ending : public Spatial
    {
    public:
        Ending(const MusicLocation& inLocation,
               const EndingParams& inParams);
        Ending(const Ending& inObject);
        
        Ending& operator=(const Ending& other);
        
        virtual ~Ending() = default;

        virtual const ItemTraits& getItemTraits() const override;
        virtual void update() override final;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override {}

        virtual Rect getFrame() const override;
        virtual Region getRegion() const override;
        inline virtual Point getAnchor() const override { return POINT_ZERO; }
        
        virtual double getScoreX() const override;

        ///@{ Draggable /////////////////////////////////////////////
        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        ///@} Draggable /////////////////////////////////////////////
        
        int duration() const;
        
        const std::vector<EndingElement>& getElements() const { return  mElements; }
        
    private:
        friend class ItemRegistry;
        
        Ending() = delete;
        
        ///@{ Item
        virtual void createDrawable() override;
        ///@}

        void copyState(const Ending& other);
        void configure(const EndingParams& inParams);
        
        DrawableEnding mDrawable;
        
        std::vector<EndingElement> mElements;
        
        mutable Rect mHandleLeft;
        mutable Rect mHandleRight;
        
        enum class DragLocation
        {
            LEFT,
            RIGHT,
            NONE
        };        
        DragLocation mDragLocation;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
