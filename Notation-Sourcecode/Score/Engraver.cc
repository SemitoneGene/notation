// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Engraver.h"

// Local
#include "DrawableContext.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Note.h"
#include "Preferences.h"
#include "Score.h"
#include "Staff.h"
#include "Text.h"

#ifdef KOMP_BUILD_APPLE
#include "EngraverMacOS.h"
#elif KOMP_BUILD_LINUX
#include "EngraverLinux.h"
#endif

namespace komp
{    
static const double SCORE_INCH = 25.4;

Engraver& Engraver::instance() {
    static Engraver instance;
    return instance;
}

double
Engraver::scaleFactor() const
{
    return mMillimeters / (mTenths * SCORE_INCH) * PIXELS_PER_INCH;
}

double
Engraver::stemLineWidth() const
{
    return scaledValue(mStemLineWidth);
}

double
Engraver::beamLineWidth() const
{
    return scaledValue(mBeamLineWidth);
}

double
Engraver::staffLineWidth() const
{
    return scaledValue(mStaffLineWidth);
}

double
Engraver::lightBarlineWidth() const
{
    return scaledValue(mLightBarlineWidth);
}

double
Engraver::heavyBarlineWidth() const
{
    return scaledValue(mHeavyBarlineWidth);
}

double
Engraver::endingLineWidth() const
{
    return scaledValue(mEndingLineWidth);
}

double
Engraver::legerLineWidth() const
{
    return scaledValue(mLegerLineWidth);
}

double
Engraver::wedgeLineWidth() const
{
    return scaledValue(mWedgeLineWidth);
}

double
Engraver::enclosureLineWidth() const
{
    return scaledValue(mEnclosureLineWidth);
}

double
Engraver::tupletBracketLineWidth() const
{
    return scaledValue(mTupletBracketLineWidth);
}

double
Engraver::graceNoteSize() const
{
    return scaledValue(mGraceNoteSize);
}

double
Engraver::cueNoteSize() const
{
    return scaledValue(mCueNoteSize);
}

double
Engraver::hyphenDistance() const
{
    return scaledValue(mHyphenDistance);
}

double
Engraver::beamDistance() const
{
    return scaledValue(mBeamDistance);
}

double
Engraver::graceNoteScale() const
{
    return mGraceNoteScale;
}

double
Engraver::scaledValue(double inValue) const
{
    return inValue * scaleFactor();
}


} // namespace


/// \brief Measure the notation character
/// \param inFontSize The font size
/// \param inChar The unicode character code to measure.
/// \result The width and height of the character.
komp::Size
MeasureCharacter(const double inFontSize, unsigned short inChar)
{
    const auto& fontManager = komp::FontManager::instance();
    const auto& fontName = fontManager.getNotationFontName();
    const auto& frame = fontManager.getCharacterFrame(fontName, inFontSize, inChar);
    const auto size = komp::Size(frame.getWidth(), frame.getHeight());
    return size;
}

/// \brief Get the notation character frame
/// \param inFontSize The font size
/// \param inChar The unicode character code to measure.
/// \result The frame of the character
komp::Rect
CharacterFrame(const double inFontSize, unsigned short inChar)
{
    const auto& fontManager = komp::FontManager::instance();
    const auto& fontName = fontManager.getNotationFontName();
    const auto& frame = fontManager.getCharacterFrame(fontName, inFontSize, inChar);
    return frame;
}

/// \brief Measure the width of the notation character
/// \param inFontSize The font size
/// \param inChar The unicode character code to measure.
/// \result The width of the character.
double
CharacterWidth(const double inFontSize, unsigned short inChar)
{
    const auto stringSize = MeasureCharacter(inFontSize, inChar);
    return stringSize.width;
}

/// \brief Measure the text.
/// \param inText The string to measure
/// \result The width and height of the character.
komp::Size
MeasureText(std::string_view inFontName,
            const double inFontSize,
            const char* inText)
{
    assert(inText != NULL);
#ifdef KOMP_BUILD_APPLE
    return komp::EngraverMacOS::MeasureText(inFontName,
                                            inFontSize,
                                            inText);
#elif KOMP_BUILD_LINUX
    return komp::EngraverLinux::MeasureText(inFontName,
                                            inFontSize,
                                            inText);
#endif
}


/// \brief Measure the width of the text.
double
TextWidth(std::string_view inFontName,
          const double inFontSize,
          const char* inText)
{
    assert(inText != NULL);
#ifdef KOMP_BUILD_APPLE
    const auto stringSize = komp::EngraverMacOS::MeasureText(inFontName,
                                                             inFontSize,
                                                             inText);
    return stringSize.width;
#elif KOMP_BUILD_LINUX
    const auto stringSize = komp::EngraverLinux::MeasureText(inFontName,
                                                             inFontSize,
                                                             inText);
    return stringSize.width;
#endif
}


/// \brief Measure the height of the text
/// \result The width of the text
double
TextHeight(std::string_view inFontName,
           const double inFontSize,
           const char* inText)
{
    assert(inText != NULL);
#ifdef KOMP_BUILD_APPLE
    const auto stringSize = komp::EngraverMacOS::MeasureText(inFontName,
                                                             inFontSize,
                                                             inText);
    return stringSize.height;
#elif KOMP_BUILD_LINUX
    const auto stringSize = komp::EngraverLinux::MeasureText(inFontName,
                                                             inFontSize,
                                                             inText);
    return stringSize.height;
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
