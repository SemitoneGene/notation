// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FontManagerLinux.h"

// System
#include <cassert>
#include <string>
#include <unistd.h>

// Qt
#include <QtGui/QFontMetrics>

// Local
#include "Score.h"

namespace komp
{

static const std::string BodoniPath = "bodoni/";
static const std::string BravuraPath = "bravura-1.18/";


static std::string 
expandPath(const std::string& inPath) 
{
    if (inPath.empty()) {
        return inPath;
    }

    if (inPath[0] == '~') {
        const char* home = getenv("HOME");
        assert(home != nullptr && "Expected HOME environment variable to be set");
        return std::string(home) + inPath.substr(1);
    } else {
        return inPath;
    }
}


#pragma mark - FontManagerLinux

FontManagerLinux::FontManagerLinux(std::string_view inPath)
{
    mPath = expandPath(std::string(inPath));

    auto fontPath = mPath + BravuraPath + "Bravura.otf";
    auto fontId = QFontDatabase::addApplicationFont(fontPath.c_str());
    if (fontId == -1) {
        KOMP_THROW("Unable to load Bravura: " + fontPath);
    }
    mFontIdCache["Bravura"] = fontId;
    
    fontPath = mPath + BodoniPath + "BodoniBookRegular.otf";
    fontId = QFontDatabase::addApplicationFont(fontPath.c_str());
    if (fontId == -1) {
        KOMP_THROW("Unable to load Bodoni: " + fontPath);
    }
    mFontIdCache["BodoniSvtyTwoITCTT-Book"] = fontId;

    fontPath = mPath + BodoniPath + "BodoniBookItalic.otf";
    fontId = QFontDatabase::addApplicationFont(fontPath.c_str());
    if (fontId == -1) {
        KOMP_THROW("Unable to load Bodoni Italic: " + fontPath);
    }
    mFontIdCache["BodoniSvtyTwoITCTT-BookIta"] = fontId;

    fontPath = mPath + BodoniPath + "BodoniBold.otf";
    fontId = QFontDatabase::addApplicationFont(fontPath.c_str());
    if (fontId == -1) {
        KOMP_THROW("Unable to load Bodoni Bold: " + fontPath);
    }
    mFontIdCache["BodoniSvtyTwoITCTT-Bold"] = fontId;
}

void
FontManagerLinux::clearCache()
{
    mFontCache.clear();
    mCharacterFrameCache.clear();
    mCharacterPathCache.clear();
}

//------------------------------------------------------------------------
// FontManagerLinux::getFontAscent
//
/// Get the ascent of the font
//------------------------------------------------------------------------
double
FontManagerLinux::getFontAscent(std::string_view inFontName, double inSize)
{
    return 0;
}

//------------------------------------------------------------------------
// FontManagerLinux::getFontDescent
//
/// Get the descent of the font
//------------------------------------------------------------------------
double
FontManagerLinux::getFontDescent(std::string_view inFontName, double inSize)
{
    return 0;
}

//------------------------------------------------------------------------
// FontManagerLinux::getFontLeading
//
/// Get the leading of the font
//------------------------------------------------------------------------
double
FontManagerLinux::getFontLeading(std::string_view inFontName, double inSize)
{
    return 0;
}

//------------------------------------------------------------------------
// FontManagerLinux::getFont
//
/// Retrieve the named font for the size from the font cache
//
/// \param inName The font name
/// \param inSize The font size
/// \result The font
//------------------------------------------------------------------------
const QFont&
FontManagerLinux::getFont(std::string_view inName, double inSize) const
{
    const auto fontKey = FontKey(inName, floor(inSize));
    const auto iter = mFontCache.find(fontKey);
    if (iter == mFontCache.end()) {
        return addFont(inName, inSize);
    } else {
        return iter->second;
    }
}

const QFont&
FontManagerLinux::addFont(std::string_view inName, double inSize) const
{
    const auto key = std::string(inName);
    const auto iter = mFontIdCache.find(key);

    const auto fontId = iter->second;
    const auto loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    const auto fontKey = FontKey(inName, floor(inSize));
    mFontCache[fontKey] = QFont(loadedFontFamilies.at(0), inSize);
    return mFontCache.find(fontKey)->second;
}

//------------------------------------------------------------------------
// FontManagerLinux::getCharacterFrame
//
/// Retrieve the character size for the named font
//
/// \param inFontName The font name
/// \param inFontSize The font size
/// \param inChar The character
/// \param inAddToCache If true, add the frame to the cache
/// \result The character frame
//------------------------------------------------------------------------
komp::Rect
FontManagerLinux::getCharacterFrame(std::string_view inFontName,
                                    double inFontSize,
                                    unsigned short inChar,
                                    bool inAddToCache) const
{
    komp::Rect result;
    
    const auto characterKey = CharacterKey(inChar, floor(inFontSize));
    
    const auto iter = mCharacterFrameCache.find(characterKey);
    if (iter == mCharacterFrameCache.end()) {
        result = addCharacterFrame(inFontName,
                                   inFontSize,
                                   inChar,
                                   inAddToCache);
    } else {
        result = iter->second;
    }
    
#ifdef DEBUG
    if (!std::isfinite(result.top) ||
        !std::isfinite(result.bottom) ||
        !std::isfinite(result.left) ||
        !std::isfinite(result.right)) {
        KOMP_THROW("bad rect");
    }
#endif
    return result;
}

komp::Rect
FontManagerLinux::addCharacterFrame(std::string_view inFontName,
                                    double inFontSize,
                                    unsigned short inChar,
                                    bool inAddToCache) const
{
    // Get the cached font
    const auto font = getFont(inFontName, inFontSize);
    
    const QFontMetrics metrics(font);
    const auto glyphFrame = QRectF(metrics.boundingRect(inChar));
    
    if (inAddToCache) {
        QPainterPath path;
        path.addText({0, 0}, font, QString(inChar));
        const auto characterKey = CharacterKey(inChar, floor(inFontSize));
        mCharacterPathCache[characterKey] = path;
        mCharacterFrameCache[characterKey] = glyphFrame;
    }
    
    return glyphFrame;
}

const Region&
FontManagerLinux::getCharacterPath(std::string_view inName,
                                   double inSize,
                                   unsigned short inChar) const
{
    const auto characterKey = CharacterKey(inChar, floor(inSize));
    
    const auto iter = mCharacterPathCache.find(characterKey);
    if (iter == mCharacterPathCache.end()) {
        
        // Add the path to the cache
        getCharacterFrame(inName, inSize, inChar);
        return getCharacterPath(inName, inSize, inChar);
        
    } else {
        return iter->second;
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
