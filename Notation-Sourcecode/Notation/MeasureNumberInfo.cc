// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MeasureNumberInfo.h"

// Local
#include "Measure.h"

namespace komp
{
    MeasureNumberInfo::MeasureNumberInfo()
    : Type(MeasureNumberType::NONE)
    , Box(false)
    , Increment(1)
    , Location(MeasureNumberLocation::ABOVE)
    , Offset(0)
    {
    }
    
    MeasureNumberInfo::MeasureNumberInfo(const MeasureNumberInfo& inInfo)
    : Box(inInfo.Box)
    , Increment(inInfo.Increment)
    , Type(inInfo.Type)
    , Location(inInfo.Location)
    , Offset(inInfo.Offset)
    {
    }

} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

