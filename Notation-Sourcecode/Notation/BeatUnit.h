// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "TemporalType.h"

// System
#include <vector>

namespace komp
{
    class BeatUnit
    {
    public:
        BeatUnit(TemporalType inTemporalType);
        BeatUnit(TemporalType inTemporalType, int inDots);
        TemporalType getTemporalType() const;
        const std::vector<bool>& getBeamBreaks() const;
        int getCount() const;
        bool getBeamBreak(int inBeamLevel);
        void setBeamBreak(int inBeamLevel, bool inValue);
        int getDots() const;
        int getClocks() const;
        
    private:
        TemporalType mTemporalType;
        int mDots;
        std::vector<bool> mBeamBreaks;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
