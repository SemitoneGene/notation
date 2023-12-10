// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <sstream>

// Local
#include "Score.h"
#include "FileMediatorFactory.h"

namespace komp
{
    /// saves the inScore to MusicXML, then loads it
    /// back from the MusicXML and returns the result
    inline komp::ScoreUPtr roundTrip(const komp::ScoreUPtr& inScore)
    {
        using namespace komp;
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*inScore.get()};
        fileMediator->saveStream(fparams, ss);
        const std::string xml{ss.str()};
        std::istringstream iss{xml};
        return fileMediator->loadStream(iss);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
