// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TupletRatio.h"

// System
#include "math.h"

// Local
#include "Throw.h"

namespace komp
{
    TupletRatio::TupletRatio()
    : mActual{3, TemporalType::EIGHTH, 0}
    , mNormal{2, TemporalType::EIGHTH, 0}
    {

    }


    TupletRatio::TupletRatio(DurationCount inActualNotes, DurationCount inNormalNotes)
    : mActual{std::move(inActualNotes)}
    , mNormal{std::move(inNormalNotes)}
    {
        
    }
    
    
    DurationCount
    TupletRatio::getActual() const
    {
        return mActual;
    }
    
    
    DurationCount
    TupletRatio::getNormal() const
    {
        return mNormal;
    }
    
    
    double
    TupletRatio::getModifier() const
    {
        const auto numerator = mNormal.getClocks();
        const auto denominator = mActual.getClocks();
        KOMP_ASSERT(denominator != 0.0);
        const auto result = numerator / denominator;
        return result;
    }
    
    
    int
    TupletRatio::getRoundedModifier() const
    {
        return static_cast<int>(ceil(getModifier() - 0.5));
    }


    bool
    TupletRatio::getIsNormalRatio() const
    {
        if (getActual().getTemporalType() != getNormal().getTemporalType()) {
            return false;
        }

        if (getActual().getDots() > 0) {
            return false;
        }

        if (getNormal().getDots() > 0) {
            return false;
        }

        const int actual = getActual().getCount();
        const int normal = getNormal().getCount();

        if (actual == 2 && normal == 3) {
            return true;
        } else if (actual == 3 && normal == 2) {
            return true;
//        } else if (actual == 4 && normal == 3) {
//            return true;
        } else if (actual == 5 && normal == 4) {
            return true;
        } else if (actual == 6 && normal == 4) {
            return true;
        } else if (actual == 7 && normal == 4) {
            return true;
        }

        return false;
    }
    
    
    bool
    TupletRatio::equals(const komp::TupletRatio& other) const
    {
        if (!mActual.equals(other.mActual)) {
            return false;
        }
        
        if (!mNormal.equals(other.mNormal)) {
            return false;
        }
        
        return true;
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
