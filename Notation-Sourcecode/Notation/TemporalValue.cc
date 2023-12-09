// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TemporalValue.h"

// Local
#include "Forward.h"

namespace komp
{
    double
    TemporalValue::getValue(TemporalType temporalType, int dots)
    {
        double value = TemporalValue::getValue(temporalType);
        double currentDotValue = value / 2.0;
        for (int i = 1; i <= dots; ++i) {
            value += currentDotValue;
            currentDotValue /= 2.0;
        }
        return value;
    }

    
    const std::map<int, TemporalType>
    TemporalValue::getClocksMap()
    {
        std::map<int, TemporalType> result;
        result.emplace(roundToInt(TemporalValue::WHOLE), TemporalType::WHOLE);
        result.emplace(roundToInt(TemporalValue::HALF), TemporalType::HALF);
        result.emplace(roundToInt(TemporalValue::QUARTER), TemporalType::QUARTER);
        result.emplace(roundToInt(TemporalValue::EIGHTH), TemporalType::EIGHTH);
        result.emplace(roundToInt(TemporalValue::SIXTEENTH), TemporalType::SIXTEENTH);
        result.emplace(roundToInt(TemporalValue::THIRTY_SECOND), TemporalType::THIRTY_SECOND);
        result.emplace(roundToInt(TemporalValue::SIXTY_FOURTH), TemporalType::SIXTY_FOURTH);
        result.emplace(roundToInt(TemporalValue::ONE_TWENTY_EIGHTH), TemporalType::ONE_TWENTY_EIGHTH);
        result.emplace(roundToInt(TemporalValue::FIVE_TWELFTH), TemporalType::FIVE_TWELFTH);
        result.emplace(roundToInt(TemporalValue::TEN_TWENTY_FOURTH), TemporalType::TEN_TWENTY_FOURTH);
        return result;
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
