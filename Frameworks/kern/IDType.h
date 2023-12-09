// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <cstdint>

namespace kern
{
    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    class IDType
    {
    public:
        IDType()
        : value{NULL_VALUE}
        {

        }

        ID_TYPE value;
    };

    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    inline bool operator<(const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inLeft,
                          const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inRight)
    {
        return inLeft.value < inRight.value;
    }

    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    inline bool operator>(const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inLeft,
                          const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inRight)
    {
        return inLeft.value > inRight.value;
    }

    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    inline bool operator==(const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inLeft,
                           const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inRight)
    {
        return inLeft.value == inRight.value;
    }

    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    inline bool operator!=(const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inLeft,
                           const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inRight)
    {
        return inLeft.value != inRight.value;
    }

    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    inline bool operator<=(const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inLeft,
                           const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inRight)
    {
        return inLeft.value <= inRight.value;
    }

    template<typename SENTINAL, typename ID_TYPE, ID_TYPE NULL_VALUE>
    inline bool operator>=(const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inLeft,
                           const IDType<SENTINAL, ID_TYPE, NULL_VALUE>& inRight)
    {
        return inLeft.value >= inRight.value;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
