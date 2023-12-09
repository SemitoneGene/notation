// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Geometry.h"

#ifdef KOMP_BUILD_APPLE
// MacOs
#import <CoreText/CoreText.h>
#endif

// Forward declarations
namespace komp
{
class FontManagerLinux;
class FontManagerMacOS;
class Score;
class Staff;

#ifdef KOMP_BUILD_APPLE
using DrawableFont = CTFontRef;
#elif KOMP_BUILD_LINUX
using DrawableFont = QFont;
#endif

}

namespace komp
{
class FontManager
{
public:
    static FontManager& instance();
    
    FontManager();
    ~FontManager() = default;
    
    void configure(std::string_view inPath = "");
    
    const DrawableFont getFont(std::string_view inName, double inSize) const;

    std::string_view getNotationFontName() const;
    std::string_view getNotationTextFontName() const;
    std::string_view getTextFontName() const;
    
    static double getFontAscent(std::string_view inFontName, double inSize);
    static double getFontDescent(std::string_view inFontName, double inSize);
    static double getFontLeading(std::string_view inFontName, double inSize);
    
    komp::Rect getCharacterFrame(std::string_view inFontName,
                                 double inFontSize,
                                 unsigned short inChar,
                                 bool inAddToCache = true) const;
    
    const Region& getCharacterPath(std::string_view inName,
                                   double inSize,
                                   unsigned short inChar) const;
    
    void clearCache();
    
    
private:
    komp::Rect addCharacterFrame(std::string_view inFontName,
                                 double inFontSize,
                                 unsigned short inChar,
                                 bool inAddToCache) const;
    
    std::string mMusicFont;
    std::string mMusicTextFont;
    std::string mTextFont;
    double mNotationSize;
    
#ifdef KOMP_BUILD_APPLE
    std::unique_ptr<FontManagerMacOS> mFontManagerImpl;
#elif KOMP_BUILD_LINUX
    std::unique_ptr<FontManagerLinux> mFontManagerImpl;
#endif
};

inline std::string_view FontManager::getNotationFontName() const { return mMusicFont; }
inline std::string_view FontManager::getNotationTextFontName() const { return mMusicTextFont; }
inline std::string_view FontManager::getTextFontName() const { return mTextFont; }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
