// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "CommandIntf.h"

// Local
#include "Score.h"

namespace komp
{
    CommandIntf::CommandIntf(Score& inScore)
    : mScore{inScore}
    {
    }

    CommandResult
    CommandIntf::execute()
    {
        const auto result = executeImpl();
        mScore.setDirty(true);
        return result;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
