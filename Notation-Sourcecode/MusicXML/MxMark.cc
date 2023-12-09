// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxMark.h"

namespace komp
{
    MxMark::MxMark(mx::api::MarkType inMarkType,
                   mx::api::Placement inPlacement,
                   komp::MarkType inKompMark)
    : markType{inMarkType}
    , placement{inPlacement}
    , kompMark{inKompMark}
    {
        
    }
    
    bool operator==(const MxMark& lhs, const MxMark& rhs)
    {
        return lhs.markType == rhs.markType && lhs.placement == rhs.placement && lhs.kompMark == rhs.kompMark;
    }
    
    bool operator!=(const MxMark& lhs, const MxMark& rhs)
    {
        return !(lhs == rhs);
    }
    
    bool operator<(const MxMark& lhs, const MxMark& rhs)
    {
        if (lhs.markType > rhs.markType) {
            return false;
        }
        else if (lhs.markType < rhs.markType) {
            return true;
        }
        else if (lhs.placement > rhs.placement) {
            return false;
        }
        else if (lhs.placement < rhs.placement) {
            return true;
        }
        else if (lhs.kompMark > rhs.kompMark) {
            return false;
        }
        else if (lhs.kompMark < rhs.kompMark) {
            return true;
        }
        else return false;
    }
    
    bool operator>(const MxMark& lhs, const MxMark& rhs)
    {
        return rhs < lhs;
    }
    
    bool operator<=(const MxMark& lhs, const MxMark& rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }
    
    bool operator>=(const MxMark& lhs, const MxMark& rhs)
    {
        return (lhs > rhs) || (lhs == rhs);
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
