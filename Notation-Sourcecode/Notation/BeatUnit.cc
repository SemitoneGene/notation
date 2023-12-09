// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "BeatUnit.h"

// Local
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{
    BeatUnit::BeatUnit(TemporalType inTemporalType)
    : mTemporalType(inTemporalType)
    , mDots(0)
    , mBeamBreaks(TemporalValue::getNumFlags(inTemporalType))
    {
        // by default, set a beam break for the zeroeth (main)
        // beam, but do not break any of the other beams
        bool value = true;
        for (auto b = mBeamBreaks.begin(); b != mBeamBreaks.end(); ++b) {
            *b = value;
            value = false;
        }
    }
    
    
    BeatUnit::BeatUnit(TemporalType inTemporalType, int inDots)
    : mTemporalType(inTemporalType)
    , mDots(inDots)
    , mBeamBreaks(TemporalValue::getNumFlags(inTemporalType))
    {
        KOMP_ASSERT(inDots >= 0);
        
        // by default, set a beam break for the zeroeth (main)
        // beam, but do not break any of the other beams
        bool value = true;
        for (auto b = mBeamBreaks.begin(); b != mBeamBreaks.end(); ++b) {
            *b = value;
            value = false;
        }
    }
    
    
    TemporalType
    BeatUnit::getTemporalType() const
    {
        return mTemporalType;
    }
    
    
    const std::vector<bool>&
    BeatUnit::getBeamBreaks() const
    {
        return mBeamBreaks;
    }
    
    
    int
    BeatUnit::getCount() const
    {
        return static_cast<int>(mBeamBreaks.size());
    }
    
    
    bool
    BeatUnit::getBeamBreak(int inBeamLevel)
    {
        KOMP_ASSERT(inBeamLevel >= 0);
        KOMP_ASSERT(inBeamLevel < getCount());
        return mBeamBreaks.at(inBeamLevel);
    }
    
    
    void
    BeatUnit::setBeamBreak(int inBeamLevel, bool inValue)
    {
        KOMP_ASSERT(inBeamLevel >= 0);
        KOMP_ASSERT(inBeamLevel < getCount());
        mBeamBreaks.at(inBeamLevel) = inValue;
    }
    
    
    int
    BeatUnit::getDots() const
    {
        return mDots;
    }
    
    
    int
    BeatUnit::getClocks() const
    {
        return TemporalValue::getValue(mTemporalType, mDots);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
