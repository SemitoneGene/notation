// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// iOS
#import <CoreText/CoreText.h>

// Local frameworks
#include <parallel_hashmap/phmap.h>

// Local
#include "Geometry.h"

namespace
{
template <class K, class V>
using ParallelHashMap = phmap::parallel_node_hash_map<K, V>;
}


namespace
{
template <class K, class V>
using ParallelHashMap = phmap::parallel_node_hash_map<K, V>;
}

namespace komp
{
class Score;
class Staff;

template <class K, class V>
using ParallelHashMap = phmap::parallel_node_hash_map<K, V>;

}

namespace komp
{
using FontKey = std::pair<std::string, double>;
using FontCache = ParallelHashMap<FontKey, CTFontRef>;

using CharacterKey = std::pair<unsigned short, double>;
using CharacterFrameCache = ParallelHashMap<CharacterKey, CGRect>;
using CharacterPathCache = ParallelHashMap<CharacterKey, Region>;

class FontManagerMacOS
{
public:
    FontManagerMacOS();
    ~FontManagerMacOS();
    
    static double getFontAscent(std::string_view inFontName, double inSize);
    static double getFontDescent(std::string_view inFontName, double inSize);
    static double getFontLeading(std::string_view inFontName, double inSize);
    
    CGRect getCharacterFrame(std::string_view inFontName,
                             double inFontSize,
                             unsigned short inChar,
                             bool inAddToCache = true) const;
    
    const Region& getCharacterPath(std::string_view inName,
                                   double inSize,
                                   unsigned short inChar) const;
    
    void clearCache();
    
private:
    friend class FontManager;
    
    const CTFontRef getFont(std::string_view inName, double inSize) const;
    const CTFontRef addFont(std::string_view inName, double inSize) const;
    
    CGRect addCharacterFrame(std::string_view inFontName,
                             double inFontSize,
                             unsigned short inChar,
                             bool inAddToCache) const;
    
    mutable FontCache mFontCache;
    mutable CharacterFrameCache mCharacterFrameCache;
    mutable CharacterPathCache mCharacterPathCache;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
