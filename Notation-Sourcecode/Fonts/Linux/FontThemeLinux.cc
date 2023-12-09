// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FontThemeLinux.h"

// System
#include <fstream>
#include <iostream>

// Local
#include "Throw.h"

namespace
{
static const std::string BravuraPath = "bravura-1.18/";
static constexpr const double PIXELS_PER_EM = 4.0;
}

namespace komp
{

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

static unsigned short
utf8_to_codepoint(const std::string& utf8_str) {
    
    if (utf8_str.empty()) {
        KOMP_THROW("Input string is empty");
    }

    unsigned char lead = utf8_str[0];
    unsigned short codepoint = 0;

    if (lead < 0x80) {
        return lead;
    } else if ((lead >> 5) == 0x6) { // 110xxxxx: 2-byte sequence
        if (utf8_str.size() < 2) {
            KOMP_THROW("Incomplete UTF-8 sequence");
        }
        codepoint = ((lead & 0x1F) << 6) | (utf8_str[1] & 0x3F);
    } else if ((lead >> 4) == 0xE) { // 1110xxxx: 3-byte sequence
        if (utf8_str.size() < 3) {
            KOMP_THROW("Incomplete UTF-8 sequence");
        }
        codepoint = ((lead & 0x0F) << 12) | ((utf8_str[1] & 0x3F) << 6) | (utf8_str[2] & 0x3F);
    }

    if (codepoint > 0xFFFF) {
        KOMP_THROW("Codepoint is too large for unsigned short");
    }

    return codepoint;
}

static unsigned short
getCharacter(const nlohmann::json& inGlyphs, std::string_view inCharacter)
{
    const auto charString = std::string(inCharacter);
    const auto data = inGlyphs[charString];
    const std::string codepoint = data["codepoint"];
    const auto character = utf8_to_codepoint(codepoint);
    return character;
}

FontThemeLinux::FontThemeLinux() = default;

void
FontThemeLinux::configure(std::string_view inPath)
{
    const auto path = expandPath(std::string(inPath));

    // Load the glyphs
    const std::string glyphPath = std::string(path) + BravuraPath + "glyphnames.json";
    std::ifstream glyphStream(glyphPath);
    if (!glyphStream) {
        KOMP_THROW("Unable to load glyph info: " + glyphPath);
    }
    glyphStream >> mGlyphs;
    
    // Load the metadata
    const std::string metadataPath = std::string(path) + BravuraPath + "bravura_metadata.json";
    std::ifstream metadataStream(metadataPath);
    if (!metadataStream) {
        KOMP_THROW("Unable to load metadata info: " + metadataPath);
    }
    metadataStream >> mMetadata;
}

Rect
FontThemeLinux::getBoundingBox(std::string_view inName) const
{
    // Get the bounding box dictionary
    const auto boundingBoxes = mMetadata["glyphBBoxes"];

    // Get the bounding box dictionary
    const auto boundingBox = boundingBoxes[std::string(inName)];
    
    // Get the "bBoxNE" array
    const auto boundingBoxNE = boundingBox["bBoxNE"];
    
    // Get the "bBoxSW" array
    const auto boundingBoxSW = boundingBox["bBoxSW"];
    
    // Get the bounding box values as em values
    const double rightNumber = boundingBoxNE[0];
    const double topNumber = boundingBoxNE[1];
    const double leftNumber = boundingBoxSW[0];
    const double bottomNumber = boundingBoxSW[1];
    
    // Convert the em values to pixels
    const auto right = rightNumber * PIXELS_PER_EM;
    const auto top = topNumber * PIXELS_PER_EM;
    const auto left = leftNumber * PIXELS_PER_EM;
    const auto bottom = bottomNumber * PIXELS_PER_EM;
    
    return {top, left, bottom, right};
}

static Point
convertStemValue(const nlohmann::json& inArray){
    
    Point result;
    if (inArray.is_array() && inArray.size() == 2) {
        
        // Get the values as em values
        const auto xEms = inArray[0].get<double>();
        const auto yEms = inArray[1].get<double>();
        
        // Convert the em values to pixels
        const auto x = xEms * PIXELS_PER_EM;
        const auto y = yEms * PIXELS_PER_EM;
        
        result.x = x;
        result.y = y;
    }
    return result;
}

StemPlacement
FontThemeLinux::getStemPlacement(std::string_view inName) const
{
    StemPlacement result;
    
    const auto glyphsWithAnchors = mMetadata.find("glyphsWithAnchors");
    if (glyphsWithAnchors == mMetadata.end()) {
        return result;
    }
    
    const auto anchorData = glyphsWithAnchors->find(std::string(inName));
    if (anchorData == glyphsWithAnchors->end()) {
        return result;
    }
          
    auto stemArray = anchorData->find("stemUpNW");
    if (stemArray != anchorData->end()) {
        result.upNW = convertStemValue(*stemArray);
    }

    stemArray = anchorData->find("stemDownNW");
    if (stemArray != anchorData->end()) {
        result.downNW = convertStemValue(*stemArray);
    }

    stemArray = anchorData->find("stemUpSE");
    if (stemArray != anchorData->end()) {
        result.upSE = convertStemValue(*stemArray);
    }

    stemArray = anchorData->find("stemDownSE");
    if (stemArray != anchorData->end()) {
        result.downSE = convertStemValue(*stemArray);
    }
    
    return result;
}

void
FontThemeLinux::populateNameMap(phmap::flat_hash_map<unsigned short,
                                std::string>& ioMap) const
{
    for (auto iter = mGlyphs.cbegin(); iter != mGlyphs.cend(); ++iter) {
        const auto& data = *iter;
        const std::string codepoint = data["codepoint"];
        const auto character = utf8_to_codepoint(codepoint);
        ioMap[character] = iter.key();
    }
}

unsigned short
FontThemeLinux::getCodepoint(std::string_view inCodepoint) const
{
    return getCharacter(mGlyphs, inCodepoint);
}

unsigned short
FontThemeLinux::getDoubleWholeCodepoint(std::string_view inPrefix) const
{
    return getCharacter(mGlyphs, std::string(inPrefix) + "DoubleWhole");
}

unsigned short
FontThemeLinux::getWholeCodepoint(std::string_view inPrefix) const
{
    return getCharacter(mGlyphs, std::string(inPrefix) + "Whole");
}

unsigned short
FontThemeLinux::getHalfCodepoint(std::string_view inPrefix) const
{
    return getCharacter(mGlyphs, std::string(inPrefix) + "Half");
}

unsigned short
FontThemeLinux::getNoteVariant(std::string_view inPrefix) const
{
    const auto plainVariant = mGlyphs.contains(inPrefix);
    const unsigned short notehead = plainVariant ? getCharacter(mGlyphs, inPrefix) : getCharacter(mGlyphs,  std::string(inPrefix) + "Black");
    return notehead;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
