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
    class DrawableRectangle : public Drawable
    {
    public:
        DrawableRectangle(bool inFill = true, bool inStroke = false);
        DrawableRectangle(const Rect& inBounds, bool inFill = true, bool inOutline = false);
        virtual ~DrawableRectangle() = default;

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

        void setBounds(const Rect& inBounds);
        void setDrawStroke(bool inValue) { mStroke = inValue; }
        void setStrokeThickness(double inValue) { mStrokeThickness = inValue; }
        void setDrawFill(bool inValue) { mFill = inValue; }

    private:
        Point mOffset{0.0, 0.0};
        int mZOrder{0};
        double mScale{1.0};
        bool mFill;
        bool mStroke;
        double mStrokeThickness{0.5};
        Rect mBounds;
        DrawResultCache mCache;
        mutable std::unique_ptr<DrawableLayer> mLayer;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

