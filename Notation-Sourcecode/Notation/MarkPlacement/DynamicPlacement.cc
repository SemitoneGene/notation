// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DynamicPlacement.h"

// Local
#include "BeamIntf.h"
#include "Mark.h"
#include "MarkImpl.h"
#include "Temporal.h"
#include "Note.h"
#include "Rest.h"
#include "Staff.h"
#include "Throw.h"
#include "TupletIntf.h"

namespace komp
{
    MarkPlacementUPtr
    DynamicPlacement::clone() const
    {
        return std::make_unique<DynamicPlacement>(*this);
    }

    void
    DynamicPlacement::placeMark(Mark& outMark, PlacementType inPlacement)
    {
        if (inPlacement == PlacementType::FREEFORM) {
            return;
        }
        
        place(outMark, inPlacement);
    }

    void
    DynamicPlacement::placeMark(Mark& outMark)
    {
        placeMark(outMark, outMark.getMarkData().getPlacementType());
    }
    
    void
    DynamicPlacement::place(Mark& inMark, PlacementType inPlacement)
    {
        const auto avoidanceFrame = MarkImpl::getAvoidanceFrame(inMark, true);

        const auto finalY = MarkImpl::getNearestBaselinePosition(inMark,
                                                                 inPlacement,
                                                                 avoidanceFrame);
        inMark.getRelativePoint().setScoreY(finalY);
        
        // Center horizontally
        const auto centerX = MarkImpl::getHorizonalCenter(inMark);
        inMark.getRelativePoint().setScoreX(centerX);
        
        // Determine flippage
        inMark.update();
    }
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
