// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "TemporalType.h"

namespace komp
{
    class TempoValue
    {
    public:
        static constexpr const TemporalType DEFAULT_BEAT_TYPE = TemporalType::QUARTER;
        static constexpr const int DEFAULT_NUM_DOTS = 0;
        static constexpr const double MIN_BPM = 1.0;
        static constexpr const double MAX_BPM = 1000.0;
        static constexpr const int MAX_DOTS = 2;

        explicit TempoValue(double inBpm);
        
        explicit TempoValue(
            double inBpm,
            TemporalType inBeatType);
        
        explicit TempoValue(
            double inBpm,
            TemporalType inBeatType,
            int inBeatTypeDots);
        
        // Beats per Minute
        double getBpm() const;
        
        // Quarter Notes per Minute
        double getQpm() const;

        TemporalType getBeatType() const;
        int getBeatTypeDots() const;
        
        int getRoundedBpm() const;
    
    private:
        void validate() const;

    private:
        double mBpm;
        TemporalType mBeatType;
        int mBeatTypeDots;
    };
    
    bool operator==(const TempoValue& l, const TempoValue& r);
    bool operator!=(const TempoValue& l, const TempoValue& r);
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
