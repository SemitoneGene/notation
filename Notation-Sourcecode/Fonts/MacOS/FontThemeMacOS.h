// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// MacOs
#include <CoreFoundation/CFDictionary.h>

// Local frameworks
#include <parallel_hashmap/phmap.h>

// Local
#include "Forward.h"
#include "MarkData.h"
#include "StemType.h"

// System
#include <map>
#include <string>
#include <vector>

namespace komp
{
    class FontThemeMacOS
    {
    public:
        FontThemeMacOS();
        
    private:
        friend class FontTheme;
        
        void configure(std::string_view inPath);
        
        void populateNameMap(phmap::flat_hash_map<unsigned short,
                             std::string>& ioMap) const;

        Rect getBoundingBox(std::string_view inName) const;
        StemPlacement getStemPlacement(std::string_view inName) const;

        unsigned short getCodepoint(std::string_view inCodepoint) const;
        unsigned short getDoubleWholeCodepoint(std::string_view inPrefix) const;
        unsigned short getWholeCodepoint(std::string_view inPrefix) const;
        unsigned short getHalfCodepoint(std::string_view inPrefix) const;
        unsigned short getNoteVariant(std::string_view inPrefix) const;
        
        CFDictionaryRef mGlyphs;
        CFDictionaryRef mMetadata;
    };
    
} //namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
