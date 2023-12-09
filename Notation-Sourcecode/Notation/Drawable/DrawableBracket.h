// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "DrawableComposite.h"
#include "Forward.h"
#include "SystemBracketInfo.h"

namespace komp
{
    /// \brief  Draws a Bracket
    ///
    /// \discussion Holds the atomic components of a bracket
    /// and draws them into a context.
    ///
    class DrawableBracket : public Drawable
	{
	public:
        DrawableBracket() = delete;
        DrawableBracket(SystemBracketType inType,
                        int inLevel,
                        Staff& inStart,
                        Staff& inStop);
        virtual ~DrawableBracket() = default;
        
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

        ///@{ DrawableBracket
        ///@}
        
        const SystemBracketType getType() const { return mType; }
        void setType(SystemBracketType inType);
        
    private:
        void init(Staff& inStart, Staff& inStop);
        
        void addBracket(double inHeight);
        void addBrace(double inHeight);
        void addLine(double inHeight);
        void addSquare(double inHeight);

        DrawableComposite mDrawables;
        SystemBracketType mType;
        int mLevel;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
