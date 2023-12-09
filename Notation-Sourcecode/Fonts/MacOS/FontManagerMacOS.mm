// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FontManagerMacOS.h"

// System
#include <shared_mutex>

// MacOs
#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif

// Local
#include "Forward.h"
#include "Score.h"
#include "Staff.h"
#if !TARGET_OS_IPHONE
#include "XUIBezierPathAdditions.h"
#endif

//#define DEBUG_FONT_NAMES 1

namespace komp
{
// MARK: - FontManagerMacOS

FontManagerMacOS::FontManagerMacOS()
{
#if DEBUG_FONT_NAMES
    // Dump all system font names for debugging
    NSArray* fontFamilies = [UIFont familyNames];
    for (int i = 0; i < [fontFamilies count]; i++) {
        NSString* fontFamily = [fontFamilies objectAtIndex:i];
        NSArray* fontNames = [UIFont fontNamesForFamilyName:[fontFamilies objectAtIndex:i]];
        NSLog (@"%@: %@", fontFamily, fontNames);
    }
#endif
    
    // Pre-fill the cached paths and frame
}

FontManagerMacOS::~FontManagerMacOS()
{
    clearCache();
}

void
FontManagerMacOS::clearCache()
{
    for (auto font : mFontCache) {
        ::CFRelease(font.second);
    }
    
    mFontCache.clear();
    mCharacterFrameCache.clear();
    mCharacterPathCache.clear();
}

/// \brief Get the ascent of the font
double
FontManagerMacOS::getFontAscent(std::string_view inFontName, double inSize)
{
    const auto fontName = [[NSString alloc] initWithCString:std::string(inFontName).c_str() encoding:NSMacOSRomanStringEncoding];
    const KompFont* theFont = [KompFont fontWithName:fontName size:inSize];
    assert(theFont != nil);
    return [theFont ascender];
}

/// \brief Get the descent of the font
double
FontManagerMacOS::getFontDescent(std::string_view inFontName, double inSize)
{
    const auto fontName = [[NSString alloc] initWithCString:std::string(inFontName).c_str() encoding:NSMacOSRomanStringEncoding];
    const KompFont* theFont = [KompFont fontWithName:fontName size:inSize];
    assert(theFont != nil);
    return [theFont descender];
}

/// \brief Get the leading of the font
double
FontManagerMacOS::getFontLeading(std::string_view inFontName, double inSize)
{
    const auto fontName = [[NSString alloc] initWithCString:std::string(inFontName).c_str() encoding:NSMacOSRomanStringEncoding];
    const KompFont* theFont = [KompFont fontWithName:fontName size:inSize];
    assert(theFont != nil);
    return [theFont leading];
}

/// \brief Retrieve the named font for the size from the font cache
/// \param inName The font name
/// \param inSize The font size
/// \result The font
const CTFontRef
FontManagerMacOS::getFont(std::string_view inName, double inSize) const
{
    CTFontRef result = NULL;
    
    const auto fontKey = FontKey(inName, floor(inSize));
    
    const auto iter = mFontCache.find(fontKey);
    if (iter == mFontCache.end()) {
        result = addFont(inName, inSize);
    } else {
        result = iter->second;
    }
    
    return result;
}

const CTFontRef
FontManagerMacOS::addFont(std::string_view inName, double inSize) const
{
    NSString* const fontName = [[NSString alloc] initWithCString:std::string(inName).c_str()
                                                        encoding:NSMacOSRomanStringEncoding];
    const auto font = CTFontCreateWithName((__bridge CFStringRef)fontName,
                                           inSize,
                                           NULL);
    const auto fontKey = FontKey(inName, floor(inSize));
    mFontCache[fontKey] = font;
    
    return font;
}

/// \brief Retrieve the character size for the named font
/// \param inFontName The font name
/// \param inFontSize The font size
/// \param inChar The character
/// \param inAddToCache If true, add the frame to the cache
/// \result The character frame
CGRect
FontManagerMacOS::getCharacterFrame(std::string_view inFontName,
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

CGRect
FontManagerMacOS::addCharacterFrame(std::string_view inFontName,
                                    double inFontSize,
                                    unsigned short inChar,
                                    bool inAddToCache) const
{
    // Get the cached font
    const auto font = getFont(inFontName, inFontSize);
    
    // Get the characters from the string
    const NSUInteger length = 1;
    unsigned short characters[length];
    characters[0] = inChar;
    
    // Allocate glyphs and bounding box arrays for holding the result
    // NOTE: We are assuming that each character is one glyph
    CGGlyph glyphs[length];
    ::CTFontGetGlyphsForCharacters(font, characters, glyphs, length);
    
    const auto glyphPath = ::CTFontCreatePathForGlyph(font, glyphs[0], NULL);
    const auto glyphFrame = ::CGPathGetBoundingBox(glyphPath);
    if (inAddToCache) {
        const auto characterKey = CharacterKey(inChar, floor(inFontSize));
        
        // Store the mirrored path to avoid transforming it every time it is drawn
        KompBezierPath* path = [KompBezierPath bezierPathWithCGPath:glyphPath];
        const auto scale = CGAffineTransformMakeScale(1.0f, -1.0f);
        [path applyTransform:scale];
        
        // Add the transformed path to the cache
        mCharacterPathCache[characterKey] = CGPathRetain(path.CGPath);
        mCharacterFrameCache[characterKey] = glyphFrame;
    }
    CGPathRelease(glyphPath);
    
    return glyphFrame;
}

const Region&
FontManagerMacOS::getCharacterPath(std::string_view inName,
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
