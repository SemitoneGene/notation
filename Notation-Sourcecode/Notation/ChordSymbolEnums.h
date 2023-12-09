// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <map>

/// We are using the terminology "ChordSymbolChar" to mean an atomic piece of the
/// the composite ChordSymbol.  So the half-dimished sign is a type of char as is
/// the string "Maj"

namespace komp
{
constexpr const char* const CHORD_SYMBOL_STRING_MAJ_TEXT = "Maj";
constexpr const char* const CHORD_SYMBOL_STRING_MIN_TEXT = "min";
constexpr const char* const CHORD_SYMBOL_STRING_MAJ_LETTER = "M";
constexpr const char* const CHORD_SYMBOL_STRING_MIN_LETTER = "m";
constexpr const char* const CHORD_SYMBOL_STRING_SUS_TEXT = "sus";
constexpr const char* const CHORD_SYMBOL_STRING_AUG_TEXT = "aug";
constexpr const char* const CHORD_SYMBOL_STRING_DIM_TEXT = "dim";

#define KOMP_ENUM_CLASS_NAME ChordSymbolCharValue

#define KOMP_ENUM_MEMBERS( KOMP_ENUM_VALUE     ) \
KOMP_ENUM_VALUE( UNKNOWN_CHORD_CHAR      ) \
KOMP_ENUM_VALUE( ACCIDENTAL_DOUBLE_FLAT  ) \
KOMP_ENUM_VALUE( ACCIDENTAL_DOUBLE_SHARP ) \
KOMP_ENUM_VALUE( ACCIDENTAL_FLAT         ) \
KOMP_ENUM_VALUE( ACCIDENTAL_NATURAL      ) \
KOMP_ENUM_VALUE( ACCIDENTAL_SHARP        ) \
KOMP_ENUM_VALUE( NUMBER_11               ) \
KOMP_ENUM_VALUE( NUMBER_13               ) \
KOMP_ENUM_VALUE( NUMBER_2                ) \
KOMP_ENUM_VALUE( NUMBER_4                ) \
KOMP_ENUM_VALUE( NUMBER_5                ) \
KOMP_ENUM_VALUE( NUMBER_6                ) \
KOMP_ENUM_VALUE( NUMBER_7                ) \
KOMP_ENUM_VALUE( NUMBER_9                ) \
KOMP_ENUM_VALUE( PARENS_LEFT             ) \
KOMP_ENUM_VALUE( PARENS_RIGHT            ) \
KOMP_ENUM_VALUE( QUALITY_AUG_SYMBOL      ) \
KOMP_ENUM_VALUE( QUALITY_AUG_TEXT        ) \
KOMP_ENUM_VALUE( QUALITY_DIM_SYMBOL      ) \
KOMP_ENUM_VALUE( QUALITY_DIM_TEXT        ) \
KOMP_ENUM_VALUE( QUALITY_HALF_DIM_SYMBOL ) \
KOMP_ENUM_VALUE( QUALITY_LOWERCASE_M     ) \
KOMP_ENUM_VALUE( QUALITY_MAJ_TEXT        ) \
KOMP_ENUM_VALUE( QUALITY_MAJOR_SYMBOL    ) \
KOMP_ENUM_VALUE( QUALITY_MIN_TEXT        ) \
KOMP_ENUM_VALUE( QUALITY_MINOR_SYMBOL    ) \
KOMP_ENUM_VALUE( QUALITY_SUS_TEXT        ) \
KOMP_ENUM_VALUE( QUALITY_UPPERCASE_M     ) \
KOMP_ENUM_VALUE( ROOT_A                  ) \
KOMP_ENUM_VALUE( ROOT_B                  ) \
KOMP_ENUM_VALUE( ROOT_C                  ) \
KOMP_ENUM_VALUE( ROOT_D                  ) \
KOMP_ENUM_VALUE( ROOT_E                  ) \
KOMP_ENUM_VALUE( ROOT_F                  ) \
KOMP_ENUM_VALUE( ROOT_G                  ) \
KOMP_ENUM_VALUE( SLASH                   ) \

#include "kern/Enum.def"

struct ChordSymbolChar
{
    ChordSymbolCharValue chordSymbolCharValue;
    bool isSuperscript;
    
    
    ChordSymbolChar()
    : chordSymbolCharValue{ChordSymbolCharValue::ROOT_A}
    , isSuperscript{false}
    {
        
    }
    
    
    ChordSymbolChar(ChordSymbolCharValue inCharValue, bool inIsSuperscript)
    : chordSymbolCharValue{inCharValue}
    , isSuperscript{inIsSuperscript}
    {
        
    }
    
    
    ChordSymbolChar(const ChordSymbolChar& inOther)
    : chordSymbolCharValue(inOther.chordSymbolCharValue)
    , isSuperscript{inOther.isSuperscript}
    {
        
    }
    
    
    ChordSymbolChar(ChordSymbolChar&& inOther) noexcept
    : chordSymbolCharValue{std::move(inOther.chordSymbolCharValue)}
    , isSuperscript{std::move(inOther.isSuperscript)}
    {
        
    }
    
    
    ChordSymbolChar& operator=(const ChordSymbolChar& inOther)
    {
        this->chordSymbolCharValue = inOther.chordSymbolCharValue;
        this->isSuperscript = inOther.isSuperscript;
        return *this;
    }
    
    
    
    ChordSymbolChar& operator=(ChordSymbolChar&& inOther) noexcept
    {
        this->chordSymbolCharValue = std::move(inOther.chordSymbolCharValue);
        this->isSuperscript = std::move(inOther.isSuperscript);
        return *this;
    }
    
};

using ChordSymbolString = std::vector<ChordSymbolChar>;

/// Used to map the members of ChordSymbolCharValue to their
/// functionality in a string of chord symbol chars
enum class ChordSymbolCharType
{
    ROOT,
    ACCIDENTAL,
    QUALITY,
    EXTENSION,
    PARENS,
    SLASH,
};

/// Describes the quality of the third and fifth of a chord. For example
/// Major refers to a Major Triad, Diminished a Diminished Triad. The
/// exception is Half-Diminished which refers to the quality of the third
/// fifth *and seventh*.  This is the only case when the seventh is
/// is specified or implied by the ChordQuality enum.  In the case of a
/// sus4 chord the "third" is replaced by the suspended 4.
enum class ChordQuality
{
    UNKNOWN,         ///< The chord quality cannot be determined
    MAJOR,           ///< A Major Triad
    MINOR,           ///< A Minor Triad
    AUGMENTED,       ///< An Augmented Triad
    DIMINISHED,      ///< A Diminished Triad
    HALF_DIMINISHED, ///< A Half-Dimished Chord (which includes the b7)
    SUS4,            ///< A Triad where the 4th replaces the 3rd
};

enum class ChordQualityIndicator
{
    UNKNOWN,  ///< unknown
    NONE,     ///< ''
    SYMBOLIC, ///< 'o'. '+'
    TEXT,     ///< 'maj', 'min', 'dim', 'aug', 'sus'
    LETTER,   ///< 'm' or 'M'
};

/// indicates whether the numbers (like 7, 9, 11, 13) have an accidental
#define KOMP_ENUM_CLASS_NAME ChordExtensionAlter

#define KOMP_ENUM_MEMBERS( KOMP_ENUM_VALUE     ) \
KOMP_ENUM_VALUE( UNKNOWN                       ) \
KOMP_ENUM_VALUE( DOUBLE_FLAT                   ) \
KOMP_ENUM_VALUE( DOUBLE_SHARP                  ) \
KOMP_ENUM_VALUE( FLAT                          ) \
KOMP_ENUM_VALUE( NONE                          ) \
KOMP_ENUM_VALUE( SHARP                         ) \

#include "kern/Enum.def"

/// indicates whether the numbers (like 7, 9, 11, 13) have an accidental
#define KOMP_ENUM_CLASS_NAME ChordExtensionQuality

#define KOMP_ENUM_MEMBERS( KOMP_ENUM_VALUE     ) \
KOMP_ENUM_VALUE( UNKNOWN                       ) \
KOMP_ENUM_VALUE( AUGMENTED                     ) \
KOMP_ENUM_VALUE( DIMINISHED                    ) \
KOMP_ENUM_VALUE( DOUBLE_AUGMENTED              ) \
KOMP_ENUM_VALUE( DOUBLE_DIMINISHED             ) \
KOMP_ENUM_VALUE( MAJOR                         ) \
KOMP_ENUM_VALUE( MINOR                         ) \
KOMP_ENUM_VALUE( PERFECT                       ) \

#include "kern/Enum.def"

enum class ChordSymbolCharFont
{
    TEXT,
    SMUFL,
    BRAVURA_TEXT,
};


inline bool operator<(const ChordSymbolChar& inLeft, const ChordSymbolChar& inRight)
{
    const int leftChar = static_cast<int>(inLeft.chordSymbolCharValue);
    const int rightChar = static_cast<int>(inRight.chordSymbolCharValue);
    
    if (leftChar < rightChar) {
        return true;
    } else if (leftChar > rightChar) {
        return false;
    }
    
    const int leftIsSuper = static_cast<int>(inLeft.isSuperscript);
    const int rightIsSuper = static_cast<int>(inRight.isSuperscript);
    
    if (leftIsSuper < rightIsSuper) {
        return true;
    } else if (leftIsSuper > rightIsSuper) {
        return false;
    }
    
    return false;
}


inline bool operator>(const ChordSymbolChar& inLeft, const ChordSymbolChar& inRight)
{
    return inRight < inLeft;
}


inline bool operator==(const ChordSymbolChar& inLeft, const ChordSymbolChar& inRight)
{
    const bool isLessThan = (inLeft < inRight);
    const bool isGreaterThan = (inLeft > inRight);
    return (!isLessThan) && (!isGreaterThan);
}

inline bool operator!=(const ChordSymbolChar& inLeft, const ChordSymbolChar& inRight)
{
    return !(inLeft == inRight);
}


inline bool operator<=(const ChordSymbolChar& inLeft, const ChordSymbolChar& inRight)
{
    return (inLeft < inRight) || (inLeft == inRight);
}


inline bool operator>=(const ChordSymbolChar& inLeft, const ChordSymbolChar& inRight)
{
    return (inLeft > inRight) || (inLeft == inRight);
}

inline std::ostream& operator<<(std::ostream& os, const ChordSymbolChar& inValue)
{
    os << "{ \"chordSymbolCharValue\": \"" << ChordSymbolCharValueToString(inValue.chordSymbolCharValue) << "\", ";
    os << "\"isSuperscript\": \"" << std::boolalpha << inValue.isSuperscript << "\" }";
    return os;
}


inline bool operator<(const ChordSymbolString& inLeft, const ChordSymbolString& inRight)
{
    auto leftIter = inLeft.cbegin();
    const auto leftEnd = inLeft.cend();
    auto rightIter = inRight.cbegin();
    const auto rightEnd = inRight.cend();
    
    for( ; (leftIter != leftEnd) && (rightIter != rightEnd); ++leftIter, ++rightIter) {
        const auto& leftChar = *leftIter;
        const auto& rightChar = *rightIter;
        
        if (leftChar < rightChar) {
            return true;
        } else if (leftChar > rightChar) {
            return false;
        }
    }
    
    if (inLeft.size() < inRight.size()) {
        return true;
    } else if (inLeft.size() > inRight.size()) {
        return false;
    }
    
    // equal
    return false;
}


inline bool operator>(const ChordSymbolString& inLeft, const ChordSymbolString& inRight)
{
    return inRight < inLeft;
}


inline bool operator==(const ChordSymbolString& inLeft, const ChordSymbolString& inRight)
{
    const bool isLessThan = (inLeft < inRight);
    const bool isGreaterThan = (inLeft > inRight);
    return (!isLessThan) && (!isGreaterThan);
}

inline bool operator!=(const ChordSymbolString& inLeft, const ChordSymbolString& inRight)
{
    return !(inLeft == inRight);
}


inline bool operator<=(const ChordSymbolString& inLeft, const ChordSymbolString& inRight)
{
    return (inLeft < inRight) || (inLeft == inRight);
}


inline bool operator>=(const ChordSymbolString& inLeft, const ChordSymbolString& inRight)
{
    return (inLeft > inRight) || (inLeft == inRight);
}

inline std::ostream& operator<<(std::ostream& os, const ChordSymbolString& inValue)
{
    os << "[";
    
    bool isFirst = true;
    
    for (const auto& chordSymbolChar : inValue) {
        
        if (!isFirst)
        {
            os << ", ";
        }
        else
        {
            os << " ";
        }
        
        os << chordSymbolChar;
        
        isFirst = false;
    }
    
    os << "]";
    
    return os;
}

class ChordSymbolMaps
{
public:
    static ChordQuality getQuality(ChordSymbolCharValue inChar);
    static ChordQualityIndicator getQualityIndicator(ChordSymbolCharValue inChar);
    static int getInt(ChordExtensionAlter inAlter);
    static ChordExtensionAlter getChordExtensionAlter(int inAlterValue);
    static ChordSymbolCharValue getAlterCharValue(ChordExtensionAlter inAlter);
    
private:
    ChordSymbolMaps();
    static const ChordSymbolMaps& getInstance();
    std::map<ChordSymbolCharValue, ChordQuality> mQualityMap;
    std::map<ChordSymbolCharValue, ChordQualityIndicator> mQualityIndicatorMap;
};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
