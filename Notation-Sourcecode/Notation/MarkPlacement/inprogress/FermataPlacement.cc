// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FermataPlacement.h"

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
    FermataPlacement::~FermataPlacement()
    {
    }
    
    MarkPlacementUPtr
    FermataPlacement::clone() const
    {
        return std::make_unique<FermataPlacement>(*this);
    }

    void
    FermataPlacement::placeMark(Mark& outMark, PlacementType inPlacement)
    {
        place(outMark, inPlacement);
    }

    void
    FermataPlacement::placeMark(Mark& outMark)
    {
        placeMark(outMark, outMark.getMarkData().getPlacementType());
    }
    
    void
    FermataPlacement::place(Mark& inMark, PlacementType inPlacement)
    {
        auto& relativeItem = inMark.getRelativePoint().getItem();
        KOMP_ASSERT(relativeItem.is<Temporal>());
        
        const auto& temporal = relativeItem.as<Temporal>();
        const auto temporalFrame = temporal.getFrame();
        const auto& staff = temporal.getLocation().getStaff();
        const auto staffTopY = staff.getScoreY();
        const auto staffBottomY = staff.getScoreBottom();

        auto avoidanceFrame = temporalFrame;
        avoidanceFrame.top = std::min(avoidanceFrame.top, staffTopY);
        avoidanceFrame.bottom = std::max(avoidanceFrame.bottom, staffBottomY);

        if (!temporal.getTuplets().empty()) {
            const TupletIntf& tuplet = temporal.getTuplets().front();
            const auto tupletFrame = tuplet.getFrame();
            avoidanceFrame.top = std::min(avoidanceFrame.top, tupletFrame.top);
            avoidanceFrame.bottom = std::max(avoidanceFrame.bottom, tupletFrame.bottom);
        }

        const auto beam = temporal.getBeam();
        if (beam) {
            const auto beamFrame = beam->getFrame();
            avoidanceFrame.top = std::min(avoidanceFrame.top, beamFrame.top);
            avoidanceFrame.bottom = std::max(avoidanceFrame.bottom, beamFrame.bottom);
        }

        const auto& marks = temporal.getAttachments().getMarks();
        for (const Mark& mark : marks) {
            if (&mark != &inMark) {
                avoidanceFrame.initializeOrUnion(mark.getFrame());
            }
        }

        auto placement = inPlacement;
        if (placement == PlacementType::AUTOMATIC) {
            const auto& stemPolicy = temporal.getStemPolicy();
            const bool hasStem = stemPolicy.getHasStem();
            const bool isBelowStaff = temporal.getSlot() < staff.getBottomLineSlot();
            const bool isBelowMiddle = temporal.getSlot() < MIDDLE_SLOT;

            if (isBelowStaff) {
                placement = PlacementType::BELOW_STAFF;
            } else if (!hasStem && isBelowMiddle) {
                placement = PlacementType::BELOW_STAFF;
            } else {
                placement = PlacementType::ABOVE_STAFF;
            }
        }

        double boundingY = 0.0;
        double direction = 1.0;
        double importantFrameY = 0.0;

        const auto markFrame = inMark.getFrame();

        const auto isAbove = inPlacement == PlacementType::ABOVE_ITEM
                          || inPlacement == PlacementType::ABOVE_STAFF;
        if (isAbove) {
            boundingY = avoidanceFrame.top;
            importantFrameY = markFrame.bottom;
            direction = -1.0;
                        
        } else {
            boundingY = avoidanceFrame.bottom;
            importantFrameY = markFrame.top;
            direction = 1.0;
        }
        
        auto deltaY = std::abs(importantFrameY - boundingY);
        const auto spaceSize = Staff::getSpacing();
        deltaY *= direction;
        const auto newY = inMark.getScoreY() + deltaY;
        
        if (isAbove) {

            // If the 'baseline' is further away then use the baseline
            const auto baselinedSlot = staff.getTopLineSlot();
            const auto baselinePixel = staff.slotToPixel(baselinedSlot);
            const auto finalY = (baselinePixel < newY) ? baselinePixel : newY;
            inMark.getRelativePoint().setScoreY(finalY);

        } else {
            
            // If the 'baseline' is further away then use the baseline
            const auto baselinedSlot = staff.getBaselineSlotBelow();
            const auto baselinePixel = staff.slotToPixel(baselinedSlot);
            const auto finalY = (baselinePixel > newY) ? baselinePixel : newY;
            inMark.getRelativePoint().setScoreY(finalY);
        }

        // Center horizontally
        const auto temporalCenterX = temporal.getFrame().getCenter().x;
        const auto markCenterX = inMark.getFrame().getCenter().x;
        const auto deltaX = temporalCenterX - markCenterX;
        const auto newX = inMark.getScoreX() + deltaX;
        inMark.getRelativePoint().setScoreX(newX);

        // Determine flippage
        inMark.update();
    }

    bool
    FermataPlacement::calculateIsAbove(Mark& mark, Item& item, PlacementType placement)
    {
        if (placement == PlacementType::ABOVE_ITEM) {
            return true;
        } else if (placement == PlacementType::BELOW_ITEM) {
            return false;
        }
        
        if (item.is<Note>()) {
            const auto& note = item.as<Note>();
            return !(note.getStemPolicy().getIsStemUp());
        }
        
        return mark.getMarkData().getPlacementType() == PlacementType::ABOVE_ITEM;
    }
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
