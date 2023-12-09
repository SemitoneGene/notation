// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TechniquePlacement.h"

// Local
#include "Mark.h"
#include "Temporal.h"
#include "Note.h"
#include "Rest.h"
#include "Throw.h"
#include "Staff.h"
#include "TupletIntf.h"
#include "BeamIntf.h"

namespace komp
{
TechniquePlacement::~TechniquePlacement()
{
}

MarkPlacementUPtr
TechniquePlacement::clone() const
{
    return std::make_unique<TechniquePlacement>(*this);
}

void
TechniquePlacement::placeMark(Mark& outMark, PlacementType inPlacement)
{
    place(outMark, inPlacement);
}

void
TechniquePlacement::placeMark(Mark& outMark)
{
    placeMark(outMark, outMark.getMarkData().getPlacementType());
}

void
TechniquePlacement::place(Mark& inMark, PlacementType inPlacement)
{
    auto& relativeItem = inMark.getRelativePoint().getItem();
    KOMP_ASSERT(relativeItem.is<Temporal>());
    auto& temporal = relativeItem.as<Temporal>();
    const auto temporalFrame = temporal.getFrame();
    
    Point center;
    const auto& headFrame = temporal.getFrames().getCompleteHeadFrame();
    if (headFrame.getIsEmpty()) {
        center = temporal.getFrame().getCenter();
    } else {
        center = headFrame.getCenter();
    }
    const auto& markCenter = inMark.getFrame().getCenter();
    
    const auto& data = inMark.getMarkData();

    // Set vertical position
    const auto deltaY = center.y - markCenter.y;
    const auto newY = inMark.getScoreY() + deltaY + data.getOffsetY();
    inMark.getRelativePoint().setScoreY(newY);
        
    // Set horizontal position
    auto newX = inMark.getScoreX();
    const auto offsetX = data.getOffsetX();
    if (offsetX < 0) {
        newX = temporalFrame.left + offsetX;
    } else if (offsetX > 0) {
        newX = temporalFrame.right + offsetX;
    }
    inMark.getRelativePoint().setScoreX(newX);
    
    inMark.update();
}

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
