// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMediatorIntf.h"

// System
#include <memory>

namespace komp
{
    enum class FileMediatorType
    {
        MusicXML,
    };

    class FileMediatorFactory
    {
    public:
        static FileMediatorIntfUPtr create(FileMediatorType fileMediatorType);
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
