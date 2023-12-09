// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BarlineType.h"
#include "Drawable.h"
#include "DrawableComposite.h"
#include "DrawableEllipse.h"

namespace komp
{
    /// \brief  Draws a Barline
    ///
    /// \discussion Holds the atomic components of a barline
    /// and draws them into a context.
    ///
    class DrawableBarline : public Drawable
	{
	public:
        DrawableBarline() = delete;
        DrawableBarline(BarlineType inType, double inHeight);
        virtual ~DrawableBarline() = default;
        
        ///@{ Drawable
        virtual void removeFromParent() const override;
        virtual void restoreParent() const override;
        virtual Point getOffset() const override;
        virtual void setOffset(const Point& inPoint) override;
        virtual void setZOrder(int inOrder) override;
        virtual void setScale(double inScale) override;
        virtual void setRotation(double inDegrees) override;
        virtual Rect getFrame(const Point& inPoint) const override;
        virtual double getWidth() const override;
        virtual double getHeight() const override;
        virtual Region getRegion(const Point& inPoint) const override;
        virtual Point getAnchor(const Point& inPoint) const override;
        virtual DrawResult getMetrics(const Point& inPoint) const override;
        virtual void draw(const DrawParams& inDrawParams) const override;
        ///@}

        ///@{ DrawableBarline
        ///@}
        
        const BarlineType getType() const { return mBarline; }
        void setType(BarlineType inType);
        
    private:
        void init(double inHeight);
        std::unique_ptr<DrawableEllipse> createDot() const;

        DrawableComposite mDrawables;
        BarlineType mBarline;        
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
