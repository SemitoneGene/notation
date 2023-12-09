// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Drawable.h"
#include "DrawableComposite.h"
#include "DrawableRectangle.h"
#include "Forward.h"
#include "WedgeType.h"
#include "ItemObserver.h"

namespace komp
{
    class DrawableWedge : public Drawable, public ItemObserver
    {
    public:
        DrawableWedge();
        virtual ~DrawableWedge() = default;
        
        ///@{ Drawable Interface
        virtual void removeFromParent() const override final;
        virtual void restoreParent() const override final;
        virtual Point getOffset() const override final;
        virtual void setOffset(const Point& inOffset) override final;
        virtual void setZOrder(int inOrder) override final;
        virtual void setScale(double inScale) override final;
        virtual void setRotation(double inDegrees) override final;
        virtual Rect getFrame(const Point& inPoint) const override final;
        virtual double getWidth() const override final;
        virtual double getHeight() const override final;
        virtual Region getRegion(const Point& inPoint) const override final;
        virtual Point getAnchor(const Point& inPoint) const override final;
        virtual DrawResult getMetrics(const Point& inPoint) const override final;
        virtual void draw(const DrawParams& inDrawParams) const override final;
        ///@}
        
        ///@{ ItemObserver
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override;
        ///@}
        
    private:
        void configure(WedgeType inType, Rect inFrame, double inThickness);

        DrawableComposite mDrawables;
        DrawableRectangle mOutline{false, true};
        WedgeType mType;
        Rect mFrame;
        double mThickness;
    };
    
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
