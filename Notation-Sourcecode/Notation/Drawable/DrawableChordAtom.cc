// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableChordAtom.h"

namespace komp
{
DrawableChordAtom::DrawableChordAtom(const DrawableChordAtomParams& inParams)
: mParams{inParams}
, mChordSymbolChar{inParams.chordSymbolChar}
, mChordSymbolCharType{inParams.chordSymbolCharType}
, mAsciiText{inParams.asciiText}
{
    mText.setFontName(mParams.fontName);
    mText.setText(mParams.text);
    
    if (mParams.chordSymbolChar.isSuperscript) {
        const Point p{mParams.xSuperscriptOffset, mParams.ySuperscriptOffset};
        mText.setOffset(p);
        mText.setFontName(mParams.fontName);
        mText.setFontSize(mParams.superscriptFontSize);
    } else {
        const Point p{mParams.xNormalOffset, mParams.yNormalOffset};
        mText.setOffset(p);
        mText.setFontName(mParams.fontName);
        mText.setFontSize(mParams.normalFontSize);
    }
}

// MARK: - Drawable Interface

void
DrawableChordAtom::removeFromParent() const
{
    mText.removeFromParent();
}

void
DrawableChordAtom::restoreParent() const
{
    mText.restoreParent();
}

Point
DrawableChordAtom::getOffset() const
{
    return mText.getOffset();
}

void
DrawableChordAtom::setOffset(const Point& inPoint)
{
    mText.setOffset(inPoint);
}

void
DrawableChordAtom::setZOrder(int inOrder)
{
    mText.setZOrder(inOrder);
}

void
DrawableChordAtom::setScale(double inScale)
{
    mText.setScale(inScale);
}

void
DrawableChordAtom::setRotation(double inDegrees)
{
}

Rect
DrawableChordAtom::getFrame(const Point& inPoint) const
{
    return mText.getFrame(inPoint);
}

double
DrawableChordAtom::getWidth() const
{
    return mText.getWidth();
}


double
DrawableChordAtom::getHeight() const
{
    return mText.getHeight();
}


Region
DrawableChordAtom::getRegion(const Point& inPoint) const
{
    return mText.getRegion(inPoint);
}


Point
DrawableChordAtom::getAnchor(const Point& inPoint) const
{
    return mText.getAnchor(inPoint);
}


DrawResult
DrawableChordAtom::getMetrics(const Point& inPoint) const
{
    return mText.getMetrics(inPoint);
}


void
DrawableChordAtom::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mText.draw(inDrawParams);
}


double
DrawableChordAtom::getLeftMargin() const
{
    const auto& chars = mText.getText();
    
    if (chars.empty()) {
        return 0.0;
    }
    
    const auto result = chars.front().getLeftMargin();
    return result;
}


double
DrawableChordAtom::getRightMargin() const
{
    const auto& chars = mText.getText();
    
    if (chars.empty()) {
        return 0.0;
    }
    
    const auto result = chars.back().getRightMargin();
    return result;
}


// MARK: - DrawableChordAtom

ChordSymbolChar
DrawableChordAtom::getChordSymbolChar() const
{
    return mChordSymbolChar;
}


ChordSymbolCharType
DrawableChordAtom::getChordSymbolCharType() const
{
    return mChordSymbolCharType;
}


std::string_view
DrawableChordAtom::getAsciiText() const
{
    return mAsciiText;
}


const DrawableText&
DrawableChordAtom::getDrawableText() const
{
    return mText;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
