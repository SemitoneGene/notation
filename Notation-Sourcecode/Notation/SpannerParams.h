// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{
    struct SpannerParams
    {
        Measure* startMeasure;
        Measure* endMeasure;
        Track* track;
        int hand;
        int startTime;
        int endTime;
        
        SpannerParams()
        : startMeasure(nullptr)
        , endMeasure(nullptr)
        , track{}
        , hand(0)
        , startTime(0)
        , endTime(0) {}
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
