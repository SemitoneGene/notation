// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BeatGrouping.h"

namespace komp
{
    enum class TimeSignatureType
    {
        COMMON, ///< Symbol 'C' for Common Time
        CUT,    ///< Symbol C with vertical line for Cut Time
        NUMERIC ///< Normal Time Signature with two numbers, e.g. 3/4
    };

    /// \brief  TimeSignatureParams encapsulates all of the information required
    /// for a TimeSignature object to construct itself
    ///
    /// \discussion TimeSignatureParams encapsulates all of the information
    /// required for a TimeSignature object to construct itself.
    struct TimeSignatureParams
    {
        TimeSignatureType type;
        int beatsPerBar;
        int beatUnit;
        int clocks;
        BeatGrouping beatGrouping;

        TimeSignatureParams();

        int slot() const;

        static bool isBeatsPerBarValid(int inBeatsPerBar);
        static bool isBeatUnitValid(int inBeatUnit);
        
        bool operator == (const TimeSignatureParams& other)
        {
            return type == other.type
                && beatsPerBar == other.beatsPerBar
                && beatUnit == other.beatUnit
                && clocks == other.clocks;
        }
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
