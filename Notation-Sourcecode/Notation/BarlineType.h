// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class BarlineType
    {
        UNKNOWN,             ///< A problem or uninitialized state
        LEFT_HEAVY_LIGHT,    ///< e.g. a forward facing repeat but without the dots, i.e. HEAVY-LIGHT
        LEFT_NONE,           ///< intentional absence of a barline
        LEFT_NORMAL,         ///< a normal barline
        LEFT_REPEAT,         ///< a forward-facing repeat mark, i.e. HEAVY-LIGHT with repeat dots
        LEFT_DOUBLE_LIGHT,   ///< a double bar where both barlines are LIGHT
        RIGHT_LIGHT_HEAVY,   ///< A 'FINAL' barline, i.e. LIGHT-HEAVY
        RIGHT_NONE,          ///< intentional absence of a barline
        RIGHT_NORMAL,        ///< a normal barline
        RIGHT_REPEAT,        ///< A rearward facing repeat, i.e. dots then LIGHT-HEAVY
        RIGHT_DOUBLE_LIGHT,  ///< a double barline where both lines are LIGHT
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
