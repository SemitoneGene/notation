// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// System
#include <cmath>
#include <sstream>
#include <iomanip>

// Komp
#include "Throw.h"

namespace komp
{
    template<typename T>
    T roundPlaces( T inputNumber, int decimalPlaces)
    {
        KOMP_ASSERT(decimalPlaces >= 0);
        auto magicMultiplier = std::pow(10, decimalPlaces);
        const T altered = std::ceil((inputNumber * magicMultiplier) - static_cast<T>(0.5));
        const T theNumber = altered / magicMultiplier;
        std::stringstream ss;
        ss << std::setprecision(decimalPlaces) << theNumber;
        T output;
        ss >> output;
        return output;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
