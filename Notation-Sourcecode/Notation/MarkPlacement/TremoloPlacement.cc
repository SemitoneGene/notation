// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TremoloPlacement.h"

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

MarkPlacementUPtr
TremoloPlacement::clone() const
{
    return std::make_unique<TremoloPlacement>(*this);
}

void
TremoloPlacement::placeMark(Mark& outMark, PlacementType inPlacement)
{
    place(outMark, inPlacement);
}

void
TremoloPlacement::placeMark(Mark& outMark)
{
    placeMark(outMark, outMark.getMarkData().getPlacementType());
}

void
TremoloPlacement::place(Mark& inMark, PlacementType inPlacement)
{
    auto& relativeItem = inMark.getRelativePoint().getItem();
    KOMP_ASSERT(relativeItem.is<Temporal>());
    Temporal* temporal = &relativeItem.as<Temporal>();
    if (temporal->getIsChordMember()) {
        temporal = temporal->getChord();
    }
    
    const auto temporalFrames = temporal->getFrames();
    const auto stemPolicy = temporal->getStemPolicy();
    const auto markCurrentFrame = inMark.getFrame();
    const auto hasStem = stemPolicy.getHasStem();
    
    const auto xCenter = hasStem ? temporalFrames.stem.getCenter().x
    : temporalFrames.head.getCenter().x;
    
    const auto yCenter = hasStem ? temporalFrames.stem.getCenter().y
    : temporalFrames.head.getCenter().y;
    
    double slopY = 0.0;
    
    switch (inMark.getMarkType())
    {
    case MarkType::TREMOLO_ONE:
        {
            if (!hasStem) {
                slopY = -5.75;
            }
            break;
        }
        
    case MarkType::TREMOLO_TWO:
        {
            if (hasStem) {
                slopY = 0.875 * (stemPolicy.getIsStemUp() ? -1.0 : 1.0);
            } else {
                slopY = -7.5;
            }
            break;
        }
        
    case MarkType::TREMOLO_THREE:
        {
            if (hasStem) {
                slopY = 1.75 * (stemPolicy.getIsStemUp() ? -1.0 : 1.0);
            } else {
                slopY = -9.0;
            }
            break;
        }
        
    case MarkType::TREMOLO_FOUR:
        {
            if (!hasStem) {
                slopY = -10.5;
            }
            break;
        }
        
    case MarkType::TREMOLO_FIVE:
        {
            break;
        }
        
    default:
        break;
        
    }
    
    const auto yTarget = yCenter + slopY;
    
    const auto markCurrentXCenter = markCurrentFrame.getCenter().x;
    const auto markCurrenyYCenter = markCurrentFrame.getCenter().y;
    
    const auto deltaX = xCenter - markCurrentXCenter;
    const auto deltaY = yTarget - markCurrenyYCenter;
    
    const auto markCurrentX = inMark.getX();
    const auto markCurrentY = inMark.getScoreY();
    
    const auto markNewX = markCurrentX + deltaX;
    const auto markNewY = markCurrentY + deltaY;
    
    inMark.setX(markNewX);
    inMark.getRelativePoint().setScoreY(markNewY);
}

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
