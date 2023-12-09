// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Drawable.h"
#include "DrawResultCache.h"

namespace komp
{
    class DrawableEllipse : public Drawable
    {
    public:
        DrawableEllipse();
        DrawableEllipse(double inWidth, double inHeight);
        virtual ~DrawableEllipse() = default;

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

    private:
        Point mOffset;
        int mZOrder;
        double mScale{1.0};
        double mWidth;
        double mHeight;
        DrawResultCache mCache;
        mutable std::unique_ptr<DrawableLayer> mLayer;
    };
    
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

