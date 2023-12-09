// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "PlacementType.h"

namespace komp
{    
    struct CurveParams
    {
        /// the temporal item to which the start of the curve is attached
        Temporal& beginTemporal;
        
        /// the temporal item to which the end of the curve is attached
        Temporal& endTemporal;

        /// ABOVE_ITEM means a slur should be drawn above the notes or
        /// a tie should be drawn with an 'overhand' curve direction.
        /// BELOW_ITEM means a slur should be drawn below the notes or
        /// a tie should be drawn with an 'underhand' curve direction.
        /// The only acceptable values are AUTOMATIC, ABOVE_ITEM and
        /// BELOW_ITEM.  Any other PlacementType values will default
        /// to AUTOMATIC
        PlacementType placementType;
        
        CurveParams() = delete;
        
        CurveParams(Temporal& inBegin,
                    Temporal& inEnd,
                    PlacementType inPlacement = PlacementType::AUTOMATIC)
        : beginTemporal{inBegin}
        , endTemporal{inEnd}
        , placementType(inPlacement)
        {
        }
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
