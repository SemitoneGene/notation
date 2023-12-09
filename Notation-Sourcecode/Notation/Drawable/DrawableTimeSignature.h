// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableComposite.h"
#include "DrawableRectangle.h"
#include "Forward.h"

namespace komp
{
    enum class TimeSignatureType;

    /// \brief  Draws a Time Signature
    ///
    /// \discussion Holds the atomic components of a time signature
    /// and draws them into a context.
    ///
    class DrawableTimeSignature : public Drawable
	{
	public:
        DrawableTimeSignature();
        virtual ~DrawableTimeSignature() = default;
        
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

        ///@{ DrawableTimeSignature
        void update(Staff* inStaff,
                    TimeSignatureType inType,
                    int inBeatsPerBar,
                    int inBeatUnit);
        ///@}
        
    private:
        void init();
        Size measureCharacter(unsigned short inChar) const;

        DrawableComposite mDrawables;
        Staff* mStaff;
        TimeSignatureType mType;
        int mBeatsPerBar;
        int mBeatUnit;
        DrawableRectangle mOutline;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
