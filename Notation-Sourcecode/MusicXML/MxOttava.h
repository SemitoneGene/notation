// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <map>

// Local
#include "MxTimeSpanner.h"
#include "Octave.h"

namespace komp
{
    class MxOttava;
    using MxOttavaPtr = std::unique_ptr<MxOttava>;
    
    struct MxOttavaParams
    {
        int numberLevel;
        TimePoint timePoint;
        OctaveType octaveType;
    };
    
    class MxOttava : public MxTimeSpanner
    {
    public:
        MxOttava(MxOttavaParams params);
        
        OctaveType getOctaveType() const;
        
    private:
        OctaveType mOctaveType;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
