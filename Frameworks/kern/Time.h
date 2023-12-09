// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <chrono>
#include <limits>

namespace kern
{
    using Nanoseconds = std::chrono::nanoseconds;
    using SystemClock = std::chrono::system_clock;
    using Timestamp = uint64_t;

    constexpr const Timestamp TIMESTAMP_NULL = std::numeric_limits<Timestamp>::max();

    class Time
    {
    public:

        // returns the System Time in Nanoseconds since Epoch
        static inline Timestamp getTimestamp()
        {
            const auto now = SystemClock::now();
            const auto dur = now.time_since_epoch();
            const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(dur);
            const auto count = nanoseconds.count();
            const auto result = static_cast<Timestamp>(count);
            return result;
        }

    private:

    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

