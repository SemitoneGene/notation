// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "ClefType.h"
#include "Staff.h"

namespace komp
{
    struct StaffInfo
    {
    	ClefType clefType;
    	StaffType staffType;

    	StaffInfo()
        : clefType{ClefType::G}
        , staffType{StaffType::NOTATION}
    	{}

        StaffInfo(ClefType inClefType)
        : clefType{inClefType}
        , staffType{StaffType::NOTATION}
        {}
        
        StaffInfo(ClefType inClefType,
                  StaffType inStaffType)
        : clefType{inClefType}
        , staffType{inStaffType}
        {}
    };
    
    using StaffInfoList = std::vector<StaffInfo>;
}
