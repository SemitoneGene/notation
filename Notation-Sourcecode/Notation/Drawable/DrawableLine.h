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
    class DrawableLine : public Drawable
    {
    public:
        DrawableLine();
        virtual ~DrawableLine() = default;

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
        
        void clearState();

        /// set length in pixels
        void setLength(double inValue);

        /// get length in pixels
        double getLength() const;

        /// set thickness in pixels
        void setThickness(double inValue);

        /// get thickness in pixels
        double getThickness() const;

        /// set the angle of the line in degrees.
        ///   0 degrees - the line will be drawn horizontally from left to right
        ///  45 degrees - the line will be drawn sloping downward left to right
        ///  90 degrees - the line will be drawn vertically from top to bottom
        /// 135 degrees - the line will be drawn sloping downward from right to left
        /// 180 degrees - the line will be drawn horizontally from right to left
        /// 225 degrees - the line will be drawn sloping upwards from right to left
        /// 270 degrees - the line will be drawn vertically from bottom to top
        /// 315 degrees - the line will be drawn sloping upwards from left to right
        /// Any value outside the range of 0 - 360 will be stored as its 0 - 360 equivalent
        void setDegrees(double inValue);

        /// get the line's direction in degrees. see setDiration for details.
        double getDegrees() const;
        
        /// same as degrees, but converted to radians
        double getRadians() const;
        
    private:
        Point mOffset;
        int mZOrder;
        double mScale{1.0};
        double mLength;
        double mThickness;
        double mDegrees;
        DrawResultCache mCache;
        mutable std::unique_ptr<DrawableLayer> mLayer;
    };
    
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

