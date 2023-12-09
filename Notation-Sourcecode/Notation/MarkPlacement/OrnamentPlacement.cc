// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "OrnamentPlacement.h"

// Local
#include "Mark.h"
#include "MarkImpl.h"
#include "Temporal.h"
#include "Note.h"
#include "Rest.h"
#include "Throw.h"
#include "Staff.h"
#include "TupletIntf.h"
#include "BeamIntf.h"

namespace komp
{
    OrnamentPlacement::~OrnamentPlacement()
    {
    }
    
    MarkPlacementUPtr
    OrnamentPlacement::clone() const
    {
        return std::make_unique<OrnamentPlacement>(*this);
    }
    
    void
    OrnamentPlacement::placeMark(Mark& outMark, PlacementType inPlacement)
    {
        place(outMark, inPlacement);
    }
    
    void
    OrnamentPlacement::placeMark(Mark& outMark)
    {
        placeMark(outMark, outMark.getMarkData().getPlacementType());
    }
    
    void
    OrnamentPlacement::place(Mark& inMark, PlacementType inPlacement)
    {
        const auto avoidanceFrame = MarkImpl::getAvoidanceFrame(inMark, true);
        const auto markY = MarkImpl::getVerticalPlacement(inMark,
                                                          inPlacement,
                                                          avoidanceFrame);
        inMark.getRelativePoint().setScoreY(markY);

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
