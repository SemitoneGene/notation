// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "EngraverMacOS.h"

// MacOs
#import <CoreText/CoreText.h>
#import <Foundation/Foundation.h>

// Local
#include "FontManager.h"
#include "Text.h"

namespace komp
{
/// \brief Measure the text.
/// \param inText The string to measure
/// \result The width and height of the character.
komp::Size
EngraverMacOS::MeasureText(std::string_view inFontName,
                           const double inFontSize,
                           const char* inText)
{
    assert(inText != NULL);
    
    NSString* const fontName = [[NSString alloc] initWithCString:std::string(inFontName).c_str()
                                                        encoding:NSMacOSRomanStringEncoding];
    
    NSString* const theString = [[NSString alloc] initWithCString:inText
                                                         encoding:NSMacOSRomanStringEncoding];
        
    CTFontRef ctFont = CTFontCreateWithName((__bridge CFStringRef)fontName,
                                            inFontSize,
                                            NULL);
    NSAttributedString* attributed = [[NSAttributedString alloc] initWithString:theString
                                                                     attributes:[NSDictionary dictionaryWithObject:(__bridge id)ctFont
                                                                                                            forKey:(__bridge NSString*)kCTFontAttributeName]];
    CFRelease(ctFont);
    
    CGMutablePathRef letters = CGPathCreateMutable();
    CTLineRef line = CTLineCreateWithAttributedString((__bridge CFAttributedStringRef)attributed);
    CFArrayRef runArray = CTLineGetGlyphRuns(line);
    
    for (CFIndex runIndex = 0;
         runIndex < CFArrayGetCount(runArray);
         runIndex++) {
        
        CTRunRef run = (CTRunRef)CFArrayGetValueAtIndex(runArray, runIndex);
        CTFontRef runFont = (CTFontRef)CFDictionaryGetValue(CTRunGetAttributes(run),
                                                            kCTFontAttributeName);
        
        for (CFIndex runGlyphIndex = 0;
             runGlyphIndex < CTRunGetGlyphCount(run);
             runGlyphIndex++) {
            
            CFRange thisGlyphRange = CFRangeMake(runGlyphIndex, 1);
            CGGlyph glyph;
            CGPoint position;
            CTRunGetGlyphs(run, thisGlyphRange, &glyph);
            CTRunGetPositions(run, thisGlyphRange, &position);
            
            CGPathRef letter = CTFontCreatePathForGlyph(runFont, glyph, NULL);
            CGAffineTransform t = CGAffineTransformMakeTranslation(position.x, position.y);
            CGPathAddPath(letters, &t, letter);
            CGPathRelease(letter);
        }
    }
    
    const auto boundingBox = CGPathGetBoundingBox(letters);
    CGPathRelease(letters);
    CFRelease(line);
    
    const auto x = boundingBox.origin.x;
    const auto y = boundingBox.origin.y;

    const auto width = ceil(boundingBox.size.width - x);
    const auto height = ceil(boundingBox.size.height - y);
    
    return komp::Size{width, height};
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
