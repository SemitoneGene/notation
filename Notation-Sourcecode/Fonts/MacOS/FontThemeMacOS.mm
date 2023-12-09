// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FontThemeMacOS.h"

// MacOs
#include <Foundation/Foundation.h>

// Local Frameworks
#include <nlohmann/json.hpp>

// System
#include <fstream>
#include <string>

namespace komp
{
    // Pixels per Em
    static constexpr const double PIXELS_PER_EM = 4.0;

    // Static dictionary so glyphs are kept in scope
    static NSDictionary* sGlyphs;
    
    /// \brief Helper function to get unicode codepoint from glyph dictionary
    static unsigned short
    getCharacter(CFDictionaryRef inDictionary, NSString* inCharacter)
    {
        NSDictionary* glyphs = (__bridge NSDictionary*)inDictionary;
        NSDictionary* info = [glyphs objectForKey:inCharacter];
        NSString* codepoint = [info objectForKey:@"codepoint"];
        const unsigned short character = [codepoint characterAtIndex:0];
        return character;
    }
    
    static unsigned short
    getCharacter(CFDictionaryRef inDictionary, std::string_view inCharacter)
    {
        NSString* inValue = [NSString stringWithUTF8String:std::string(inCharacter).c_str()];
        return getCharacter(inDictionary, inValue);
    }
    
    /// \discussion This class contains all the values for the character for a specific
    /// font.  It then creates tables of characters for access by the
    /// engraver.
    FontThemeMacOS::FontThemeMacOS()
    {
    }
    
    void
    FontThemeMacOS::configure(std::string_view inPath)
    {
        // Load the glyphs
        NSString* const glyphFilePath = [[NSBundle mainBundle] pathForResource:@"glyphnames" ofType:@"json"];
        NSData* glyphData = [NSData dataWithContentsOfFile:glyphFilePath];
        sGlyphs = [NSJSONSerialization JSONObjectWithData:glyphData options:kNilOptions error:nil];
        mGlyphs = (__bridge CFDictionaryRef)sGlyphs;
        
        // Load the metadata
        NSString* const metadataFilePath = [[NSBundle mainBundle] pathForResource:@"bravura_metadata" ofType:@"json"];
        NSData* metadatData = [NSData dataWithContentsOfFile:metadataFilePath];
        NSDictionary* metadata = [NSJSONSerialization JSONObjectWithData:metadatData options:kNilOptions error:nil];
        mMetadata = (__bridge CFDictionaryRef)metadata;
    }
    
    Rect
    FontThemeMacOS::getBoundingBox(std::string_view inName) const
    {
        // Get the bounding box dictionary
        NSDictionary* metadata = (__bridge NSDictionary*)mMetadata;
        NSDictionary* boundingBoxes = [metadata objectForKey:@"glyphBBoxes"];
        
        // Get the bounding box for the black notehead
        NSString* nameString = [NSString stringWithUTF8String:std::string(inName).c_str()];
        NSDictionary* boundingBox = [boundingBoxes objectForKey:nameString];
        
        // Get the "bBoxNE" array
        NSArray* boundingBoxNE = [boundingBox objectForKey:@"bBoxNE"];
        
        // Get the "bBoxSW" array
        NSArray* boundingBoxSW = [boundingBox objectForKey:@"bBoxSW"];
        
        // Get the bounding box values as em values
        NSNumber* rightNumber = [boundingBoxNE objectAtIndex:0];
        NSNumber* topNumber = [boundingBoxNE objectAtIndex:1];
        NSNumber* leftNumber = [boundingBoxSW objectAtIndex:0];
        NSNumber* bottomNumber = [boundingBoxSW objectAtIndex:1];
        
        // Convert the em values to pixels
        const auto right = rightNumber.doubleValue * PIXELS_PER_EM;
        const auto top = topNumber.doubleValue * PIXELS_PER_EM;
        const auto left = leftNumber.doubleValue * PIXELS_PER_EM;
        const auto bottom = bottomNumber.doubleValue * PIXELS_PER_EM;
        
        return Rect{top, left, bottom, right};
    }
    
    static Point
    convertStemValue(NSArray* inArray)
    {
        Point result;
        if (inArray) {
            
            // Get the values as em values
            NSNumber* xNumber = [inArray objectAtIndex:0];
            NSNumber* yNumber = [inArray objectAtIndex:1];
            
            // Convert the em values to pixels
            const auto x = xNumber.doubleValue * PIXELS_PER_EM;
            const auto y = yNumber.doubleValue * PIXELS_PER_EM;
            
            result.x = x;
            result.y = y;
        }
        return result;
    }
    
    StemPlacement
    FontThemeMacOS::getStemPlacement(std::string_view inName) const
    {
        StemPlacement result;
        
        // Get the bounding box dictionary
        NSDictionary* metadata = (__bridge NSDictionary*)mMetadata;
        NSDictionary* glyphsWithAnchors = [metadata objectForKey:@"glyphsWithAnchors"];
        
        // Get the glyph anchor data for the note
        NSString* nameString = [NSString stringWithUTF8String:std::string(inName).c_str()];
        NSDictionary* anchorData = [glyphsWithAnchors objectForKey:nameString];
        if (anchorData) {

            NSArray* stemArray = [anchorData objectForKey:@"stemUpNW"];
            result.upNW = convertStemValue(stemArray);

            stemArray = [anchorData objectForKey:@"stemDownNW"];
            result.downNW = convertStemValue(stemArray);

            stemArray = [anchorData objectForKey:@"stemUpSE"];
            result.upSE = convertStemValue(stemArray);

            stemArray = [anchorData objectForKey:@"stemDownSE"];
            result.downSE = convertStemValue(stemArray);
        }
        
        return result;
    }
    
    void
    FontThemeMacOS::populateNameMap(phmap::flat_hash_map<unsigned short,
                                    std::string>& ioMap) const
    {
        NSDictionary* glyphs = (__bridge NSDictionary*)mGlyphs;
        for (NSString* const key : glyphs) {
            NSDictionary* info = [glyphs objectForKey:key];
            NSString* codepoint = [info objectForKey:@"codepoint"];
            const unsigned short character = [codepoint characterAtIndex:0];
            ioMap[character] = key.UTF8String;
        }
    }
    
    unsigned short
    FontThemeMacOS::getCodepoint(std::string_view inCodepoint) const
    {
        return getCharacter(mGlyphs, inCodepoint);
    }

    unsigned short
    FontThemeMacOS::getDoubleWholeCodepoint(std::string_view inPrefix) const
    {
        NSString* prefix = [NSString stringWithUTF8String:std::string(inPrefix).c_str()];
        return getCharacter(mGlyphs, [NSString stringWithFormat:@"%@DoubleWhole", prefix]);
    }
    
    unsigned short
    FontThemeMacOS::getWholeCodepoint(std::string_view inPrefix) const
    {
        NSString* prefix = [NSString stringWithUTF8String:std::string(inPrefix).c_str()];
        return getCharacter(mGlyphs, [NSString stringWithFormat:@"%@Whole", prefix]);
    }
    
    unsigned short
    FontThemeMacOS::getHalfCodepoint(std::string_view inPrefix) const
    {
        NSString* prefix = [NSString stringWithUTF8String:std::string(inPrefix).c_str()];
        return getCharacter(mGlyphs, [NSString stringWithFormat:@"%@Half", prefix]);
    }
    
    unsigned short
    FontThemeMacOS::getNoteVariant(std::string_view inPrefix) const
    {
        NSString* prefix = [NSString stringWithUTF8String:std::string(inPrefix).c_str()];
        NSDictionary* glyphs = (__bridge NSDictionary*)mGlyphs;
        const bool plainVariant = [glyphs objectForKey:prefix] != nil;
        const unsigned short notehead = plainVariant ? getCharacter(mGlyphs, prefix) : getCharacter(mGlyphs, [NSString stringWithFormat:@"%@Black", prefix]);
        return notehead;
    }
  
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
