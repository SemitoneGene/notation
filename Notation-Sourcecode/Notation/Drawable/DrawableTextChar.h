// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

// Local
#include "Forward.h"
#include "Drawable.h"
#include "DrawResultCache.h"

namespace komp
{
    struct DrawableTextCharOffsets
    {
    public:
        DrawableTextCharOffsets();
        double xOffset;
        double yOffset;
        double adjustFrameTop;
        double adjustFrameBottom;
        double adjustFrameLeft;
        double adjustFrameRight;
        double leftMargin;
        double rightMargin;
    };

    /// \brief  Holds a unsigned short character and spacing margins.
    ///
    /// \discussion Holds a unsigned short character and spacing margins.
    ///
	class DrawableTextChar
	{
	public:
		DrawableTextChar(unsigned short inChar);
        DrawableTextChar(unsigned short inChar,
                         const DrawableTextCharOffsets& inOffsets);
        
    public:
    	unsigned short getChar() const;
        double getXOffset() const;
        double getYOffset() const;
        double getAdjustFrameTop() const;
        double getAdjustFrameBottom() const;
        double getAdjustFrameLeft() const;
        double getAdjustFrameRight() const;
        double getLeftMargin() const;
        double getRightMargin() const;

        inline bool equals(const DrawableTextChar& other) const {
        	return mUniChar == other.mUniChar
                && areFloatsEqual(mXOffset, other.mXOffset)
                && areFloatsEqual(mYOffset, other.mYOffset)
                && areFloatsEqual(mAdjustFrameTop, other.mAdjustFrameTop)
                && areFloatsEqual(mAdjustFrameBottom, other.mAdjustFrameBottom)
                && areFloatsEqual(mAdjustFrameLeft, other.mAdjustFrameLeft)
                && areFloatsEqual(mAdjustFrameRight, other.mAdjustFrameRight)
                && areFloatsEqual(mLeftMargin, other.mLeftMargin)
                && areFloatsEqual(mRightMargin, other.mRightMargin);
        }

    private:
        unsigned short mUniChar;
        double mXOffset;
        double mYOffset;
        double mAdjustFrameTop;
        double mAdjustFrameBottom;
        double mAdjustFrameLeft;
        double mAdjustFrameRight;
        double mLeftMargin;
        double mRightMargin;
	};

    using DrawableChars = std::vector<DrawableTextChar>;

	inline bool operator==(const DrawableTextChar& a,
                           const DrawableTextChar& b) {
		return a.equals(b);
	}

    inline bool operator==(const DrawableChars& a,
                           const DrawableChars& b) {
		return areVectorsEqual(a, b);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
