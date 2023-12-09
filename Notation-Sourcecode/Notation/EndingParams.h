// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{
    enum class EndingType
    {
        BEGIN,
        CONTINUATION,
        END
    };

    enum class EndingStatus
    {
        DEFAULT,
        HIDDEN,
        VISIBLE
    };
    
    struct EndingElement
    {
        Measure* measure;
        EndingType type;
        EndingStatus status;
        int ending;

        EndingElement()
        : type(EndingType::BEGIN)
        , status(EndingStatus::DEFAULT)
        , ending(1)
        {
        }
    };
        
    struct EndingParams
    {
        Measure* measure;
        Track* track;
        std::vector<EndingElement> elements;
        
        EndingParams()
        {}
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
