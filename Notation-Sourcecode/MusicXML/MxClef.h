// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "MxLocation.h"
#include "Throw.h"

// Mx
#include "mx/api/ClefData.h"

namespace komp
{
    class Score;
    
    class MxClef
    {
    public:
        MxClef(const mx::api::ClefData& inClefData,
               const MxLocation& inMxLocation);

        // pre-condition: Score.mCurrentState must be set
        void insertClef(Score& outScore, FileMessages& outMessages);

    private:
        const mx::api::ClefData mClefData;
        const MxLocation mMxLocation;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
