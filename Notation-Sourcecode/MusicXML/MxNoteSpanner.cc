// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxNoteSpanner.h"

// Local
#include "Temporal.h"
#include "Throw.h"

namespace komp
{

    MxNoteSpanner::MxNoteSpanner(int inNumberLevel, Temporal& inStartTemporal)
    : mNumberLevel(inNumberLevel)
    , mStart(&inStartTemporal)
    , mStop(nullptr)
    , mPlacementType(PlacementType::AUTOMATIC)
    {
    }
    
    Temporal&
    MxNoteSpanner::getStart() const
    {
        return *mStart;
    }
    
    Temporal&
    MxNoteSpanner::getStop() const
    {
        KOMP_ASSERT(mStop);
        return *mStop;
    }
    
    void
    MxNoteSpanner::setStop(Temporal& inTemporal)
    {
        mStop = &inTemporal;
    }

    int
    MxNoteSpanner::getNumberLevel() const
    {
        return mNumberLevel;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
