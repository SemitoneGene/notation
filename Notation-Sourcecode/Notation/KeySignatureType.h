// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

namespace komp
{
enum class ClefType;

enum class TranspositionType
{
    NONE,
    UP,
    DOWN,
};

struct KeyInfo
{
    int YPosAcc[7];
    int NumSpaces;
    int DeltaPitch;
    int ScalePos[12];
    int PitchAcc[12];
};

static const KeyInfo KeyInfoTable[15] =
{
    // C
    0,  0,  0,  0,  0,  0,  0,
    0,
    0,
    0,  0,  1,  2,  2,  3,  3,  4,  4,  5,  6,  6,
    0,  1,  0, -1,  0,  0,  1,  0,  1,  0, -1,  0,
    
    // F
    0,  0,  0,  0,  0,  0, -1,
    1,
    5,
    0,  0,  1,  2,  2,  3,  3,  4,  5,  5,  6,  6,
    0,  1,  0, -1,  0,  0,  1,  0, -1,  0,  0,  3,
    
    // Bb
    0,  0, -1,  0,  0,  0, -1,
    2,
    10,
    0,  1,  1,  2,  2,  3,  3,  4,  5,  5,  6,  6,
    0, -1,  0,  0,  3,  0,  1,  0, -1,  0,  0,  3,
    
    // Eb
    0,  0, -1,  0,  0, -1, -1,
    3,
    3,
    0,  1,  1,  2,  2,  3,  4,  4,  5,  5,  6,  6,
    0, -1,  0,  0,  3,  0, -1,  0,  0,  3,  0,  3,
    
    // Ab
    0, -1, -1,  0,  0, -1, -1,
    4,
    8,
    0,  1,  1,  2,  2,  3,  4,  4,  5,  5,  6,  7,
    0,  0,  3,  0,  3,  0, -1,  0,  0,  3,  0, -1,
    
    // Db
    0, -1, -1,  0, -1, -1, -1,
    5,
    1,
    0,  1,  1,  2,  3,  3,  4,  4,  5,  5,  6,  7,
    0,  0,  3,  0, -1,  0,  0,  3,  0,  3,  0, -1,
    
    // Gb
    -1, -1, -1, 0, -1, -1, -1,
    6,
    6,
    0,  1,  1,  2,  3,  3,  4,  4,  5,  6,  6,  7,
    3,  0,  3,  0, -1,  0,  0,  3,  0, -2,  0,  0,
    
    // Cb
    -1, -1, -1, -1, -1, -1, -1,
    7,
    11,
    0,  1,  2,  2,  3,  3,  4,  4,  5,  6,  6,  7,
    3,  0, -2,  0,  0,  3,  0,  3,  0, -2,  0,  0,
    
    // G
    0,  0,  0,  1,  0,  0,  0,
    1,
    7,
    0,  0,  1,  1,  2,  3,  3,  4,  4,  5,  6,  6,
    0,  1,  0,  1,  0,  3,  0,  0,  1,  0, -1,  0,
    
    // D
    1,  0,  0,  1,  0,  0,  0,
    2,
    2,
    0,  0,  1,  1,  2,  3,  3,  4,  4,  5,  5,  6,
    3,  0,  0,  1,  0,  3,  0,  0,  1,  0,  1,  0,
    
    // A
    1,  0,  0,  1,  1,  0,  0,
    3,
    9,
    0,  0,  1,  1,  2,  2,  3,  4,  4,  5,  5,  6,
    3,  0,  0,  1,  0,  1,  0,  3,  0,  0,  1,  0,
    
    // E
    1,  1,  0,  1,  1,  0,  0,
    4,
    4,
    -1,  0,  1,  1,  2,  2,  3,  4,  4,  5,  5,  6,
    1,  0,  3,  0,  0,  1,  0,  3,  0,  0,  1,  0,
    
    // B
    1,  1,  0,  1,  1,  1,  0,
    5,
    11,
    -1,  0,  1,  1,  2,  2,  3,  3,  4,  5,  5,  6,
    1,  0,  3,  0,  0,  1,  0,  2,  0,  3,  0,  0,
    
    // F#
    1,  1,  1,  1,  1,  1,  0,
    6,
    6,
    -1,  0,  0,  1,  2,  2,  3,  3,  4,  5,  5,  6,
    1,  0,  2,  0,  3,  0,  0,  2,  0,  3,  0,  0,
    
    // C#
    1,  1,  1,  1,  1,  1,  1,
    7,
    1,
    -1,  0,  0,  1,  2,  2,  3,  3,  4,  4,  5,  6,
    1,  0,  2,  0,  3,  0,  0,  2,  0,  2,  0,  3
};


/// The names of Major keys for the purpose of
/// determining how many sharps or flats will
/// be in the key signature. Use the relative
/// major value in the case where you are
/// thinking in terms of a different mode.
/// in legacy code, integers were used to
/// represent key signature values. you should
/// use the KeySignatureValue class to do the
/// conversion to legacy int values. the values
/// of the enum members are randomized here
/// to force us not to rely on them
///
enum class KeySignatureType
{
    C_FLAT  = 61, ///< 7 Flats
    G_FLAT  = 54, ///< 6 Flats
    D_FLAT  = 34, ///< 5 Flats
    A_FLAT  = 33, ///< 4 Flats
    E_FLAT  = 41, ///< 3 Flats
    B_FLAT  = 19, ///< 2 Flats
    F       = 56, ///< 1 Flat
    C       = 37, ///< No Flats or Sharps
    G       = 84, ///< 1 Sharp
    D       = 29, ///< 2 Sharps
    A       = 99, ///< 3 Sharps
    E       = 88, ///< 4 Sharps
    B       = 22, ///< 5 Sharps
    F_SHARP = 77, ///< 6 Sharps
    C_SHARP = 16, ///< 7 Sharps
};

/// the legacy int representation of key signatures defined as:
/// 0 is C Major
/// 1-7 is the number of flats
/// 8-14 is the number of sharps (after subtracting 7)
using KeyInt = int;

class KeySignatureValue
{
public:
    KeySignatureValue();
    KeySignatureValue(KeySignatureType inValue);
    
    KeySignatureType getValue() const;
    void setValue(KeySignatureType inValue);
    
    /// Converts the key signature type to its legacy int value
    /// Legacy Int | Key        | Accidentals
    /// 0               | C Major  | None
    /// 1               | F             | 1 Flat
    /// 2               | Bb          | 2 Flats
    /// 3               | Eb          | 3 Flats
    /// 4               | Ab          | 4 Flats
    /// 5               | Db          | 5 Flats
    /// 6               | Gb          | 6 Flats
    /// 7               | Cb          | 7 Flats
    /// 8               | G            | 1 Sharp
    /// 9               | D            | 2 Sharps
    /// 10              | A           | 3 Sharps
    /// 11              | E           | 4 Sharps
    /// 12              | B           | 5 Sharps
    /// 13              | F#         | 6 Sharps
    /// 14              | C#         | 7 Sharps
    KeyInt getInt() const;
    
    /// Takes a legacy key signature int value, converts it to
    /// KeySignatureType, and sets this objects value
    void setInt(KeyInt inValue);
    
    /// Returns the number of accidentals with direction
    /// for example -2 for B_FLAT, 0 for C, 7 for C_SHARP
    int getAccidentals() const;
    
    /// Sets the value of this object based on the number
    /// and direction of accidentals desired. for example
    /// use -3 for E_FLAT, 0 for C, 6 for F_SHARP
    void setAccidentals(int inValue);
    
    /// for the given clef, returns the slots where accidentals
    /// should be drawn.
    std::vector<int> calculateSlots(ClefType inClef) const;
    
private:
    KeySignatureType mValue;
};

bool operator==(const KeySignatureValue& l, const KeySignatureValue& r);
bool operator!=(const KeySignatureValue& l, const KeySignatureValue& r);

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
