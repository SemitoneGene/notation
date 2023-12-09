// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{

class ScoreObserver
{
public:
    virtual ~ScoreObserver() = default;
    
    virtual void loaded(const Score& inScore) = 0;
    
    virtual void measureAdded(Measure& inMeasure) = 0;
    virtual void measureRemoved(Measure& inMeasure) = 0;
    
    virtual void trackAdded(Track& inTrack) = 0;
    virtual void trackRemoved(Track& inTrack) = 0;
};

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
