// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>
#include <vector>

namespace komp
{
    /// \brief  a class to list instruments in a certain order and gives
    /// that ordering a name.
    ///
    class InstrumentOrder
    {

    public:
        std::string instrumentOrderName;
        std::vector<std::string> instrumentFamilyNames;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
