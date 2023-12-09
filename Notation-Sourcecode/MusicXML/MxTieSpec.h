// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{
    
    class MxTieSpec
    {
    public:
        MxTieSpec();
        
        int numberLevel;
        Measure* firstMeasure;
        Track* track;
        int hand;
        int voice;
        mx::api::NoteData mxFirstNote;
        mx::api::NoteData mxLastNote;
        Temporal* firstTemporal;
        Temporal* lastTemporal;

    private:

    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
