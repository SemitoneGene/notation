// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "TemporalType.h"

namespace komp
{
    /// The DurationCount struct encapsulates a count and a duration type.
    /// For example 4 Quarter Notes, 3 Dotted Eighth Notes, etc.
    class DurationCount
    {
    public:
        DurationCount(int inCount,
                      TemporalType inTemporalType,
                      int inDots);

        int getCount() const;
        TemporalType getTemporalType() const;
        int getDots() const;
        double getClocks() const;
        int getRoundedClocks() const;
        
        /// Returns true if this DurationCount and the other DurationCount have
        /// exactly the same values
        bool equals(const DurationCount& other) const;
        
    private:
        int mCount;
        TemporalType mTemporalType;
        int mDots;
    };

    inline bool operator==(const DurationCount& inLeft, const DurationCount& inRight)
    {
        if (inLeft.getCount() != inRight.getCount()) {
            return false;
        }

        if (inLeft.getTemporalType() != inRight.getTemporalType()) {
            return false;
        }

        if (inLeft.getDots() != inRight.getDots()) {
            return false;
        }

        return true;
    }

    inline bool operator!=(const DurationCount& inLeft, const DurationCount& inRight)
    {
        return !(inLeft == inRight);
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
