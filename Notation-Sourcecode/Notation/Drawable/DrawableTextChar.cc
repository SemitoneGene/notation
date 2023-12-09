// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableTextChar.h"

namespace komp
{
    DrawableTextCharOffsets::DrawableTextCharOffsets()
    : xOffset{0.0}
    , yOffset{0.0}
    , adjustFrameTop{0.0}
    , adjustFrameBottom{0.0}
    , adjustFrameLeft{0.0}
    , adjustFrameRight{0.0}
    , leftMargin{0.0}
    , rightMargin{0.0}
    {

    }


	DrawableTextChar::DrawableTextChar(unsigned short inChar)
    : DrawableTextChar{inChar, DrawableTextCharOffsets{}}
	{

    }


    DrawableTextChar::DrawableTextChar(unsigned short inChar,
                                       const DrawableTextCharOffsets& inOffsets)
    : mUniChar{inChar}
    , mXOffset{inOffsets.xOffset}
    , mYOffset{inOffsets.yOffset}
    , mAdjustFrameTop{inOffsets.adjustFrameTop}
    , mAdjustFrameBottom{inOffsets.adjustFrameBottom}
    , mAdjustFrameLeft{inOffsets.adjustFrameLeft}
    , mAdjustFrameRight{inOffsets.adjustFrameRight}
    , mLeftMargin{inOffsets.leftMargin}
    , mRightMargin{inOffsets.rightMargin}
    {

    }


	unsigned short
	DrawableTextChar::getChar() const
	{
        return mUniChar;
	}


    double
    DrawableTextChar::getXOffset() const
    {
        return mXOffset;
    }


    double
    DrawableTextChar::getYOffset() const
    {
        return mYOffset;
    }


    double
    DrawableTextChar::getAdjustFrameTop() const
    {
        return mAdjustFrameTop;
    }


    double
    DrawableTextChar::getAdjustFrameBottom() const
    {
        return mAdjustFrameBottom;
    }


    double
    DrawableTextChar::getAdjustFrameLeft() const
    {
        return mAdjustFrameLeft;
    }


    double
    DrawableTextChar::getAdjustFrameRight() const
    {
        return mAdjustFrameRight;
    }


    double
    DrawableTextChar::getLeftMargin() const
    {
        return mLeftMargin;
    }


    double
    DrawableTextChar::getRightMargin() const
    {
        return mRightMargin;
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
