// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxOttava.h"

// Local
#include "Temporal.h"

namespace komp
{

    MxOttava::MxOttava(MxOttavaParams params)
    : MxTimeSpanner(params.numberLevel, params.timePoint)
    , mOctaveType(params.octaveType)
    {
        
    }
    
    OctaveType
    MxOttava::getOctaveType() const
    {
        return mOctaveType;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
