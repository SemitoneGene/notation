// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "ChordSymbolEnums.h"
#include "Drawable.h"
#include "DrawableText.h"

namespace komp
{
    struct DrawableChordAtomParams
    {
        DrawableChars text;
        ChordSymbolChar chordSymbolChar;
        ChordSymbolCharFont chordSymbolCharFont;
        bool isShortened; ///< e.g. M instead of Maj, or m instead of min
        ChordSymbolCharType chordSymbolCharType;
        std::string asciiText;
        double normalFontSize;
        std::string fontName;
        double xNormalOffset;
        double yNormalOffset;
        double xSuperscriptOffset;
        double ySuperscriptOffset;
        double superscriptFontSize;


        DrawableChordAtomParams()
        : text{}
        , chordSymbolChar{}
        , chordSymbolCharFont{ChordSymbolCharFont::BRAVURA_TEXT}
        , asciiText{}
        , normalFontSize{1.0}
        , xNormalOffset{0.0}
        , yNormalOffset{0.0}
        , xSuperscriptOffset{0.0}
        , ySuperscriptOffset{0.0}
        , superscriptFontSize{1.0}
        {

        }
    };

    class DrawableChordAtom;
    using DrawableChordAtomUPtr = std::unique_ptr<DrawableChordAtom>;

    /// \brief  Draws a Chord Symbol.
    ///
    /// \discussion Holds the atomic components of a chord symbol
    /// and draws them into a context.
    ///
    class DrawableChordAtom : public Drawable
    {
    public:
        DrawableChordAtom(const DrawableChordAtomParams& inParams);
        virtual ~DrawableChordAtom() = default;

    public:
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

        ///@{ DrawableChordAtom
        ChordSymbolChar getChordSymbolChar() const;
        ChordSymbolCharType getChordSymbolCharType() const;
        std::string_view getAsciiText() const;
        double getLeftMargin() const;
        double getRightMargin() const;
        const DrawableText& getDrawableText() const;
        const DrawableChordAtomParams& getParams() const { return mParams; }
        ///@}

    private:
        DrawableText mText;
        DrawableChordAtomParams mParams;
        ChordSymbolChar mChordSymbolChar;
        ChordSymbolCharType mChordSymbolCharType;
        std::string mAsciiText;

    private:

    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
