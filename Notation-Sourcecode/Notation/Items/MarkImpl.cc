// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MarkImpl.h"

// Local
#include "Chord.h"
#include "ItemImpl.h"
#include "Mark.h"
#include "Staff.h"
#include "Temporal.h"
#include "Throw.h"
#include "TupletIntf.h"
#include "BeamIntf.h"

namespace komp
{
Rect
MarkImpl::getAvoidanceFrame(const Mark& inMark, bool inAvoidStaff)
{
    auto& item = inMark.getRelativePoint().getItem();
    KOMP_ASSERT(item.is<Temporal>());
    const auto& temporal = item.as<Temporal>();
    const auto temporalFrames = temporal.getFrames();
    
    Rect avoidanceFrame = temporalFrames.head;
    
    if (temporal.getIsChordMember()) {
        const auto chord = temporal.getChord();
        KOMP_ASSERT(chord);
        const auto chordFrames = chord->getFrames();
        avoidanceFrame.initializeOrUnion(chordFrames.head);
    }
    
    const auto& tuplets = temporal.getTuplets();
    if (!tuplets.empty()) {
        const TupletIntf& tuplet = tuplets.front();
        const auto tupletFrame = tuplet.getFrame();
        avoidanceFrame.initializeOrUnion(tupletFrame);
    }
    
    const auto beam = temporal.getBeam();
    if (beam) {
        const auto beamFrame = beam->getFrame();
        avoidanceFrame.top = std::min(avoidanceFrame.top, beamFrame.top);
        avoidanceFrame.bottom = std::max(avoidanceFrame.bottom, beamFrame.bottom);
    }
    
    const auto& allMarks = temporal.getAttachments().getMarks();
    for (const Mark& m : allMarks) {
        if (&m != &inMark) {
            avoidanceFrame.initializeOrUnion(m.getFrame());
        }
    }
    
    if (inAvoidStaff) {
        const auto staffTopBottom = MarkImpl::getStaffTopBottom(inMark);
        const auto staffTopY = staffTopBottom.first;
        const auto staffBottomY = staffTopBottom.second;
        
        avoidanceFrame.top = std::min(avoidanceFrame.top, staffTopY);
        avoidanceFrame.bottom = std::max(avoidanceFrame.bottom, staffBottomY);
    }
    
    return avoidanceFrame;
}

double
MarkImpl::getHorizonalCenter(const Mark& inMark)
{
    auto& item = inMark.getRelativePoint().getItem();
    KOMP_ASSERT(item.is<Temporal>());
    const auto& temporal = item.as<Temporal>();
    const auto temporalCenterX = temporal.getFrame().getCenter().x;
    const auto markCenterX = inMark.getFrame().getCenter().x;
    const auto deltaX = temporalCenterX - markCenterX;
    const auto center = inMark.getScoreX() + deltaX;
    return center;
}

std::pair<double, double>
MarkImpl::getStaffTopBottom(const Mark& inMark)
{
    auto& relativeItem = inMark.getRelativePoint().getItem();
    KOMP_ASSERT(relativeItem.is<Temporal>());
    const auto& temporal = relativeItem.as<Temporal>();
    const auto& staff = temporal.getLocation().getStaff();
    const auto staffTopY = staff.getScoreY();
    const auto staffBottomY = staff.getScoreBottom();
    return {staffTopY, staffBottomY};
}
    
double
MarkImpl::getVerticalPlacement(const Mark& inMark,
                               PlacementType inPlacement,
                               const Rect& inAvoidanceFrame)
{
    double boundingY = 0.0;
    double direction = 1.0;
    double importantFrameY = 0.0;

    const auto markFrame = inMark.getFrame();
    
    const auto isAbove = inPlacement == PlacementType::ABOVE_ITEM
                      || inPlacement == PlacementType::ABOVE_STAFF;
    if (isAbove) {
        boundingY = inAvoidanceFrame.top;
        importantFrameY = markFrame.bottom;
        direction = -1.0;
                    
    } else {
        boundingY = inAvoidanceFrame.bottom;
        importantFrameY = markFrame.top;
        direction = 1.0;
    }

    auto deltaY = std::abs(importantFrameY - boundingY);
    const auto spaceSize = Staff::getSpacing();
    deltaY += (spaceSize * 1.25);
    deltaY *= direction;
    const auto result = inMark.getScoreY() + deltaY;
    
    return result;
}

double
MarkImpl::getNearestBaselinePosition(const Mark& inMark,
                                     PlacementType inPlacement,
                                     const Rect& inAvoidanceFrame)
{
    auto& relativeItem = inMark.getRelativePoint().getItem();
    KOMP_ASSERT(relativeItem.is<Temporal>());
    const auto& temporal = relativeItem.as<Temporal>();

    if (inPlacement == PlacementType::AUTOMATIC) {
        const auto temporalY = temporal.getScoreY();
        const auto markY = inMark.getScoreY();
        inPlacement = markY > temporalY ? PlacementType::ABOVE_STAFF
                                        : PlacementType::BELOW_STAFF;
    }
    
    const auto positionY = MarkImpl::getVerticalPlacement(inMark,
                                                          inPlacement,
                                                          inAvoidanceFrame);

    const auto& staff = temporal.getLocation().getStaff();
    const auto isAbove = inPlacement == PlacementType::ABOVE_ITEM
                      || inPlacement == PlacementType::ABOVE_STAFF;

    double result = 0.0;

    if (isAbove) {
        const auto baselinedSlot = staff.getTopLineSlot();
        const auto baselinePixel = staff.slotToPixel(baselinedSlot);
        result = (baselinePixel < positionY) ? baselinePixel : positionY;

    } else {
        const auto baselinedSlot = staff.getBottomLineSlot();
        const auto baselinePixel = staff.slotToPixel(baselinedSlot);
        result = (baselinePixel > positionY) ? baselinePixel : positionY;
    }
    
    return result;
}

void
MarkImpl::placeMarks(const Temporal& inTemporal)
{
    const auto& marks = inTemporal.getAttachments().getMarks();

    // Reset to initial position before placement. We do this first to avoid
    // cascading offset errors when calculating avoidance rects.
    // TODO: Mark-specific vertical offset is ignored.
    const auto itemY = inTemporal.getScoreY();
    for (Mark& mark : marks) {
        mark.getRelativePoint().setScoreY(itemY);
    }
    
    for (Mark& mark : marks) {
        const auto& markData = mark.getMarkData();
        const auto& markPlacement = markData.getMarkPlacement();
        const auto placementType = markData.getPlacementType();
        markPlacement->placeMark(mark, placementType);
        ItemImpl::setNeedsDisplay(mark);
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
