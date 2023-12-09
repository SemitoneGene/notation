// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ArticulationPlacement.h"

// Local
#include "BeamIntf.h"
#include "Constants.h"
#include "Mark.h"
#include "MarkImpl.h"
#include "Note.h"
#include "Rest.h"
#include "Temporal.h"
#include "Throw.h"
#include "TupletIntf.h"

namespace komp
{
    ArticulationPlacement::ArticulationPlacement()
    : mGapSpaces(DEFAULT_GAP_SPACES)
    {

    }

    ArticulationPlacement::ArticulationPlacement(double inGapSpaces)
    : mGapSpaces(inGapSpaces)
    {

    }

    ArticulationPlacement::~ArticulationPlacement()
    {
    	
    }
    
    
    MarkPlacementUPtr
    ArticulationPlacement::clone() const
    {
        return std::make_unique<ArticulationPlacement>(*this);
    }

    
    void
    ArticulationPlacement::placeMark(Mark& inMark,
                                     PlacementType inPlacement)
    {
        if (inPlacement == PlacementType::FREEFORM) {
            return;
        } else if (inPlacement == PlacementType::ABOVE_STAFF
                || inPlacement == PlacementType::BELOW_STAFF) {
            // TODO: handle this case
            return;
        }
        
        place(inMark, inPlacement);
    }

    
    void
    ArticulationPlacement::placeMark(Mark& outMark)
    {
        placeMark(outMark, outMark.getMarkData().getPlacementType());
    }
    
    
    void
    ArticulationPlacement::place(Mark& inMark, PlacementType inPlacement)
    {
        auto& item = inMark.getRelativePoint().getItem();
        KOMP_ASSERT(item.is<Temporal>());
        const auto& temporal = item.as<Temporal>();
        const auto temporalFrames = temporal.getFrames();
        
        // Match the mark's x-center with the notehead's x-center
        const auto markFrame = inMark.getFrame();
        const auto markCenterX = markFrame.getCenter().x;
        const auto noteheadCenterX = temporalFrames.head.getCenter().x;
        const auto deltaX = noteheadCenterX - markCenterX;
        const auto markOriginalX = inMark.getX();
        const auto markNewX = markOriginalX + deltaX;
        inMark.setX(markNewX);

        // Find the current y-value of the mark that is relevant for moving
        // the mark relative to the avoidance frame;
        const auto isAbove = getIsAbove(temporal);
        const auto markCurrentY = isAbove ? markFrame.bottom : markFrame.top;
        const auto yDirection = isAbove ? -1.0 : 1.0;
        const auto avoidanceFrame = MarkImpl::getAvoidanceFrame(inMark, false);
        const auto avoidanceY = isAbove ? avoidanceFrame.top : avoidanceFrame.bottom;
        const auto yDistance = std::abs(avoidanceY - markCurrentY);
        const auto spaceSize = inMark.getSpaceSize();
        const auto gapPixels = mGapSpaces * spaceSize;
        const auto yDelta = (yDistance + gapPixels) * yDirection;
        const auto markNewY = markCurrentY + yDelta;
        inMark.getRelativePoint().setScoreY(markNewY);
        inMark.update();
    }

    bool
    ArticulationPlacement::getIsAbove(const Temporal& inTemporal) const
    {
        const auto stemPolicy = inTemporal.getStemPolicy();
        if (stemPolicy.getHasStem()) {
            return stemPolicy.getIsStemDown() || !stemPolicy.getHasStem();
        }
        
        return inTemporal.getSlot() >= MIDDLE_SLOT;
    }

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
