// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>

// Mx
#include "mx/api/MarkData.h"

namespace komp
{
    enum class MarkType;
    
    struct MxMark
    {
        const mx::api::MarkType markType;
        const mx::api::Placement placement;
        const komp::MarkType kompMark;
        
        MxMark(mx::api::MarkType inMarkType,
               mx::api::Placement inPlacement,
               komp::MarkType inKompMark);
    };
    
    bool operator==(const MxMark& lhs, const MxMark& rhs);
    bool operator!=(const MxMark& lhs, const MxMark& rhs);
    bool operator<(const MxMark& lhs, const MxMark& rhs);
    bool operator>(const MxMark& lhs, const MxMark& rhs);
    bool operator<=(const MxMark& lhs, const MxMark& rhs);
    bool operator>=(const MxMark& lhs, const MxMark& rhs);
    
    using MxMarkSet = std::set<MxMark>;

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
