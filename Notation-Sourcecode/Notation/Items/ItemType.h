// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{
    /// \brief  Various Sub-Types of the Item class
    ///
    /// \discussion Various Sub-Types of the Item class. The numeric values of
    /// these enum members is not significant. Please keep this enum sorted
    /// in alphabetical order.  Please insure that all *final* sub-classes
    /// of Item are represented here, and in the additional static maps found
    /// in this translation unit.  Do not assign constants to these, allow the
    /// compiler to assign the values.
    ///
    enum class ItemType
    {
        Beam,
        Chord,
        ChordSymbol,
        Clef,
        Data,
        Ending,
        KeySignature,
        LineSpanner,
        Mark,
        Marker,
        Note,
        Octave,
        RawAccidental,
        Rest,
        RehearsalMark,
        Slur,
        TempoMark,
        Text,
        Tie,
        TimeSignature,
        Tuplet,
        Wedge,
        NumberOfItemTypes, ///< This must always be the last enum member
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
