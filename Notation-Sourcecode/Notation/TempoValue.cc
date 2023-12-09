// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TempoValue.h"

// Local
#include "Throw.h"
#include "TemporalValue.h"

namespace komp
{
    TempoValue::TempoValue(double inBpm)
    : mBpm(inBpm)
    , mBeatType(DEFAULT_BEAT_TYPE)
    , mBeatTypeDots(DEFAULT_NUM_DOTS)
    {
        validate();
    }
    

    TempoValue::TempoValue(
        double inBpm,
        TemporalType inBeatType)
    : mBpm(inBpm)
    , mBeatType(inBeatType)
    , mBeatTypeDots(DEFAULT_NUM_DOTS)
    {
        validate();
    }
    

    TempoValue::TempoValue(
        double inBpm,
        TemporalType inBeatType,
        int inBeatTypeDots)
    : mBpm(inBpm)
    , mBeatType(inBeatType)
    , mBeatTypeDots(inBeatTypeDots)
    {
        validate();
    }

    bool operator==(const TempoValue& l, const TempoValue& r)
    {
        return l.getBpm() == r.getBpm()
            && l.getBeatType() == r.getBeatType()
            && l.getBeatTypeDots() == r.getBeatTypeDots();
    }
    
    bool operator!=(const TempoValue& l, const TempoValue& r)
    {
        return !(l == r);
    }


    double
    TempoValue::getBpm() const
    {
        return mBpm;
    }


    double
    TempoValue::getQpm() const
    {
        const double beatTypeClocks = static_cast<double>(TemporalValue::getValue(getBeatType(), getBeatTypeDots()));
        const double ratio = beatTypeClocks / static_cast<double>(TemporalValue::getValue(TemporalType::QUARTER));
        return ratio * mBpm;
    }


    TemporalType
    TempoValue::getBeatType() const
    {
        return mBeatType;
    }


    int
    TempoValue::getBeatTypeDots() const
    {
        return mBeatTypeDots;
    }
    
    
    int
    TempoValue::getRoundedBpm() const
    {
        return static_cast<int>(std::ceil(getBpm() - 0.5));
    }
    

    void
    TempoValue::validate() const
    {
        KOMP_ASSERT(getBpm() >= TempoValue::MIN_BPM);
        KOMP_ASSERT(getBpm() <= TempoValue::MAX_BPM);
        KOMP_ASSERT(getBeatTypeDots() >= 0);
        KOMP_ASSERT(getBeatTypeDots() <= TempoValue::MAX_DOTS);
    }
        
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
