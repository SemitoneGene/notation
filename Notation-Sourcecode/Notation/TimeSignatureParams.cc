// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TimeSignatureParams.h"

// System
#include <array>

// Local
#include "Throw.h"

namespace komp
{
    static constexpr const int DEFAULT_BEATS = 4;
    static constexpr const int DEFAULT_DENOMINATOR = 4;

    TimeSignatureParams::TimeSignatureParams()
    : type(TimeSignatureType::NUMERIC)
    , beatsPerBar(DEFAULT_BEATS)
    , beatUnit(DEFAULT_DENOMINATOR)
    , clocks(0)
    , beatGrouping(BeatGrouping::getDefaultGroupingForTimeSignature(DEFAULT_BEATS,
                                                                     DEFAULT_DENOMINATOR))
    {
        KOMP_ASSERT(TimeSignatureParams::isBeatsPerBarValid(beatsPerBar));
        KOMP_ASSERT(TimeSignatureParams::isBeatUnitValid(beatUnit));
    }

    int
    TimeSignatureParams::slot() const
    {
        int slot = 0;
        
        switch (type)
        {
            case TimeSignatureType::NUMERIC:
            {
                slot = 8;
                break;
            }
                                    
            case TimeSignatureType::COMMON:
            case TimeSignatureType::CUT:
            {
                slot = 6;
                break;
            }
                
            default:
                break;
        }
        
        return slot;
    }
    
    bool
    TimeSignatureParams::isBeatUnitValid(int inBeatUnit)
    {
        static const std::array<int, 7> VALID_DENOMINATORS = {
            1, 2, 4, 8, 16, 32, 64
        };
        for (const auto& i : VALID_DENOMINATORS) {
            if (i == inBeatUnit) {
                return true;
            }
        }
        return false;
    }

    bool
    TimeSignatureParams::isBeatsPerBarValid(int inBeatsPerBar)
    {
        if (inBeatsPerBar < 1) {
            return false;
        }
        
        if (inBeatsPerBar > 99) {
            return false;
        }
        
        return true;
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
