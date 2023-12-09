// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "AttachedItem.h"
#include "DrawableText.h"
#include "Forward.h"

namespace komp
{
    class KOMP_EXPORT Lyric : public AttachedItem
    {
    public:
        virtual ~Lyric();
        Lyric(const Lyric& other);
        Lyric& operator=(const Lyric& other);
        Lyric(Lyric&& other);
        Lyric& operator=(Lyric&& other);
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void update() override;
        virtual void observedItemChanged(Item& observedItem,
                                         ItemChanges inChanges) override;
        virtual Rect getFrame() const override;
        inline virtual Point getAnchor() const override { return POINT_ZERO; }
        virtual Region getRegion() const override;
        virtual void inserted() override;
        virtual void removed() override;
        virtual double getX() const override;
        virtual void setX(double inValue) override;
        virtual double getScoreX() const override;
        virtual double getScoreY() const override;
        virtual int getSlot() const override;
        virtual void setSlot(int inValue) override;
        virtual void beginDrag(const CurrentState& inState) override;
        virtual void drag(const CurrentState& inState) override;
        virtual void endDrag(const CurrentState& inState) override;
        
        ///@{ Lyric
        bool getStartsMelisma() const;
        void setMelismaStart(bool inValue);
        
        bool getEndsMelisma() const;
        void setMelismaEnd(bool inValue);
        
        bool hasRightTie() const;
        ///@}
        
    private:
        
        ///@{ Item
        virtual void draw(const DrawableContext& inContext) override;
        ///@}

        /// operator=() implementation
        void copyState(const Lyric& other);
        
        friend class ItemRegistry;
        Lyric(RelativePoint inRelativePoint);
        
        DrawableText mDrawable;
        std::string mLineOne;
        std::string mLineTwo;
        
        enum {
            MELISMA_START,
            MELISMA_END,
            LYRIC_TIED_RIGHT,
            TOTAL_FLAGS
        };
        std::bitset<TOTAL_FLAGS> mLyricFlags;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
