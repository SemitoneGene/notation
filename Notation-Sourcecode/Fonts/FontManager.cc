// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FontManager.h"

// Local
#include "FontTheme.h"
#include "Forward.h"
#include "LockTypes.h"
#include "Score.h"
#include "Staff.h"

#ifdef KOMP_BUILD_APPLE
#include "FontManagerMacOS.h"
#elif KOMP_BUILD_LINUX
#include "FontManagerLinux.h"
#endif

//#define DEBUG_FONT_NAMES 1

namespace komp
{
// MARK: - FontManager

FontManager& FontManager::instance() {
    static FontManager instance;
    return instance;
}

FontManager::FontManager()
: mMusicFont("Bravura")
, mMusicTextFont("BravuraText")
, mTextFont("BodoniSvtyTwoITCTT-Book")
, mNotationSize(12.0)
{
}

void
FontManager::configure(std::string_view inPath)
{
#ifdef KOMP_BUILD_APPLE
    mFontManagerImpl = std::make_unique<FontManagerMacOS>();
#elif KOMP_BUILD_LINUX
    mFontManagerImpl = std::make_unique<FontManagerLinux>(inPath);
#endif
    auto& fontTheme = FontTheme::instance();
    fontTheme.configure(inPath);
}

const DrawableFont
FontManager::getFont(std::string_view inName, double inSize) const
{
    return mFontManagerImpl->getFont(inName, inSize);
}

void
FontManager::clearCache()
{
    mFontManagerImpl->clearCache();
}

/// \brief Get the ascent of the font
double
FontManager::getFontAscent(std::string_view inFontName, double inSize)
{
#ifdef KOMP_BUILD_APPLE
    return FontManagerMacOS::getFontAscent(inFontName, inSize);
#elif KOMP_BUILD_LINUX
    return 0;
#endif
}

/// \brief Get the descent of the font
double
FontManager::getFontDescent(std::string_view inFontName, double inSize)
{
#ifdef KOMP_BUILD_APPLE
    return FontManagerMacOS::getFontDescent(inFontName, inSize);
#elif KOMP_BUILD_LINUX
    return 0;
#endif
}

/// \brief Get the leading of the font
double
FontManager::getFontLeading(std::string_view inFontName, double inSize)
{
#ifdef KOMP_BUILD_APPLE
    return FontManagerMacOS::getFontLeading(inFontName, inSize);
#elif KOMP_BUILD_LINUX
    return 0;
#endif
}

/// \brief Retrieve the character size for the named font
/// \param inFontName The font name
/// \param inFontSize The font size
/// \param inChar The character
/// \param inAddToCache If true, add the frame to the cache
/// \result The character frame
komp::Rect
FontManager::getCharacterFrame(std::string_view inFontName,
                               double inFontSize,
                               unsigned short inChar,
                               bool inAddToCache) const
{
    return mFontManagerImpl->getCharacterFrame(inFontName,
                                               inFontSize,
                                               inChar,
                                               inAddToCache);
}

komp::Rect
FontManager::addCharacterFrame(std::string_view inFontName,
                               double inFontSize,
                               unsigned short inChar,
                               bool inAddToCache) const
{
    return mFontManagerImpl->addCharacterFrame(inFontName,
                                               inFontSize,
                                               inChar,
                                               inAddToCache);
}

const Region&
FontManager::getCharacterPath(std::string_view inName,
                              double inSize,
                              unsigned short inChar) const
{
    return mFontManagerImpl->getCharacterPath(inName,
                                              inSize,
                                              inChar);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
