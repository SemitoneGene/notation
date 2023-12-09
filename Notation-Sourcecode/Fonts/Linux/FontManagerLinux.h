// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Qt
#include <QtGui/QFontDatabase>

// Local frameworks
#include <parallel_hashmap/phmap.h>

// Local
#include "Geometry.h"


namespace komp
{
class Score;
class Staff;

template <class K, class V>
using ParallelHashMap = phmap::parallel_node_hash_map<K, V>;
}

namespace komp
{
using FontIdCache = ParallelHashMap<std::string, int>;
using FontKey = std::pair<std::string, double>;
using FontCache = ParallelHashMap<FontKey, QFont>;

using CharacterKey = std::pair<unsigned short, double>;
using CharacterFrameCache = ParallelHashMap<CharacterKey, Rect>;
using CharacterPathCache = ParallelHashMap<CharacterKey, Region>;

class FontManagerLinux
{
public:
    FontManagerLinux(std::string_view inPath);
    ~FontManagerLinux() = default;
    
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
    friend class FontManager;
    
    const QFont& getFont(std::string_view inName, double inSize) const;
    const QFont& addFont(std::string_view inName, double inSize) const;
    
    komp::Rect addCharacterFrame(std::string_view inFontName,
                                 double inFontSize,
                                 unsigned short inChar,
                                 bool inAddToCache) const;
    
    std::string mPath;
    
    mutable FontIdCache mFontIdCache;
    mutable FontCache mFontCache;
    mutable CharacterFrameCache mCharacterFrameCache;
    mutable CharacterPathCache mCharacterPathCache;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
