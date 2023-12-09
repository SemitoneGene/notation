// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BreathPlacement.h"

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
    BreathPlacement::~BreathPlacement()
    {
    }

    MarkPlacementUPtr
    BreathPlacement::clone() const
    {
        return std::make_unique<BreathPlacement>(*this);
    }

    void
    BreathPlacement::placeMark(Mark& inMark, PlacementType inPlacement)
    {
        place(inMark, inPlacement);
    }

    void
    BreathPlacement::placeMark(Mark& outMark)
    {
        placeMark(outMark, outMark.getMarkData().getPlacementType());
    }

    void
    BreathPlacement::place(Mark& inMark, PlacementType inPlacement)
    {
        const auto avoidanceFrame = MarkImpl::getAvoidanceFrame(inMark, true);
        const auto markY = MarkImpl::getVerticalPlacement(inMark,
                                                          inPlacement,
                                                          avoidanceFrame);
        inMark.getRelativePoint().setScoreY(markY);

        // Place it to the right of the note
        auto& relativeItem = inMark.getRelativePoint().getItem();
        KOMP_ASSERT(relativeItem.is<Temporal>());
        const auto& temporal = relativeItem.as<Temporal>();
        const auto temporalRightX = temporal.getFrame().right;
        const auto markLeftX = inMark.getFrame().left;
        const auto deltaX = (temporalRightX - markLeftX) + 3;
        const auto newX = inMark.getScoreX() + deltaX;
        inMark.getRelativePoint().setScoreX(newX);

        // Determine flippage
        inMark.update();
    }

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
