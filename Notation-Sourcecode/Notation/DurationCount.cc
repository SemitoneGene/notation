// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DurationCount.h"

// System
#include <cmath>

// Local
#include "TemporalValue.h"
#include "Throw.h"
#include "Constants.h"

namespace komp
{

    DurationCount::DurationCount(int inCount,
                                 TemporalType inTemporalType,
                                 int inDots)
    : mCount{inCount}
    , mTemporalType{inTemporalType}
    , mDots{inDots}
    {
        KOMP_ASSERT(mCount >= 1);
        KOMP_ASSERT(mDots >= 0);
        KOMP_ASSERT(mCount <= MAX_TUPLET_COUNT);
        KOMP_ASSERT(mDots <= MAX_DOTS);
    }
    
    
    int
    DurationCount::getCount() const
    {
        return mCount;
    }
    
    
    TemporalType
    DurationCount::getTemporalType() const
    {
        return mTemporalType;
    }
    
    
    int
    DurationCount::getDots() const
    {
        return mDots;
    }
    
    
    double
    DurationCount::getClocks() const
    {
        const auto baseValue = TemporalValue::getValue(getTemporalType(), getDots());
        const auto result = static_cast<double>(getCount()) * baseValue;
        return result;
    }
    
    
    int
    DurationCount::getRoundedClocks() const
    {
        return static_cast<int>(std::ceil(getClocks() - 0.5));
    }
    
    
    bool
    DurationCount::equals(const DurationCount& other) const
    {
        if (this->mCount != other.mCount) {
            return false;
        }
        
        if (this->mTemporalType != other.mTemporalType) {
            return false;
        }
        
        if (this->mDots != other.mDots) {
            return false;
        }
        
        return true;
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
