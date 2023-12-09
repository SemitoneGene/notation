// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeatGrouping.h"

// Local
#include "TemporalValue.h"
#include "Throw.h"
#include "TimeSignatureParams.h"

namespace komp
{
    BeatGrouping::BeatGrouping(const std::vector<BeatUnit>& inBeatUnits)
    : mBeatUnits(inBeatUnits)
    {
    }
    
    
    int
    BeatGrouping::getTotalClocks() const
    {
        int value = 0;
        for (const auto& bu : mBeatUnits) {
            value += bu.getClocks();
        }
        return value;
    }
    
    
    int
    BeatGrouping::getBeatUnitCount() const
    {
        return static_cast<int>(mBeatUnits.size());
    }
    
    
    const std::vector<BeatUnit>&
    BeatGrouping::getBeatUnits() const
    {
        return mBeatUnits;
    }
    
    
    std::vector<int>
    BeatGrouping::getBeatTimes() const
    {
        std::vector<int> beatTimes;
        int currentClockLocation = 0;
        
        for (const auto& beatUnit : getBeatUnits()) {
            beatTimes.emplace_back(currentClockLocation);
            currentClockLocation += beatUnit.getClocks();
        }
        return beatTimes;
    }
    
    
    BeatGrouping
    BeatGrouping::getDefaultGroupingForTimeSignature(int inBeatsPerBar, int inBeatUnit)
    {
        KOMP_ASSERT(TimeSignatureParams::isBeatsPerBarValid(inBeatsPerBar));
        KOMP_ASSERT(TimeSignatureParams::isBeatUnitValid(inBeatUnit));
        
        // common time signatures
        // TODO: is there an algorithm here?

        const bool isOdd = inBeatsPerBar > 1 && ((inBeatsPerBar % 2) != 0);
        const bool isTriple = inBeatsPerBar > 1 && ((inBeatsPerBar % 3) == 0);

        TemporalType temporalType = TemporalType::WHOLE;
        switch (inBeatUnit) {
            case 1: {
                temporalType = TemporalType::WHOLE;
                break;
            }
            case 2: {
                temporalType = TemporalType::HALF;
                break;
            }
            case 4: {
                temporalType = TemporalType::QUARTER;
                break;
            }
            case 8: {
                // note the difference here is intentional
                temporalType = TemporalType::QUARTER;
                break;
            }
            case 16: {
                temporalType = TemporalType::EIGHTH;
                break;
            }
            case 32: {
                temporalType = TemporalType::SIXTEENTH;
                break;
            }
            case 64: {
                temporalType = TemporalType::THIRTY_SECOND;
                break;
            }
            default: {
                temporalType = TemporalType::QUARTER;
                break;
            }
        }

        if (inBeatsPerBar == 1) {
            const auto t = TemporalValue::getTypeByTimeSignatureDenominator(inBeatUnit);
            std::vector<BeatUnit> beatUnits{static_cast<size_t>(inBeatsPerBar), BeatUnit{t}};
            return BeatGrouping{std::move(beatUnits)};
        }
        
        if (inBeatUnit <= 4) {
            std::vector<BeatUnit> beatUnits{static_cast<size_t>(inBeatsPerBar), BeatUnit{temporalType}};
            return BeatGrouping{std::move(beatUnits)};
        }
        
        if (isTriple) {
            int numBeats = inBeatsPerBar / 3;
            std::vector<BeatUnit> beatUnits{static_cast<size_t>(numBeats), BeatUnit{temporalType, 1}};
            return BeatGrouping{std::move(beatUnits)};
        }
        
        BeatUnit beatUnit(temporalType);
        int numBeats = inBeatsPerBar / 2;
        std::vector<BeatUnit> beatUnits;
        
        for (int i = 0; i < numBeats; ++i) {
            bool isLast = ( i == numBeats - 1);
            if (isLast && isOdd) {
                beatUnit = BeatUnit(temporalType, 1);
            }
            
            beatUnits.push_back(beatUnit);
        }
        
        return BeatGrouping{std::move(beatUnits)};
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
