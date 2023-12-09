// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ExpressionPlacement.h"

// Local
#include "Mark.h"
#include "Temporal.h"
#include "Note.h"
#include "Rest.h"
#include "Throw.h"

namespace komp
{
    ExpressionPlacement::~ExpressionPlacement()
    {
    }
    
    MarkPlacementUPtr
    ExpressionPlacement::clone() const
    {
        return std::make_unique<ExpressionPlacement>(*this);
    }
    
    void
    ExpressionPlacement::placeMark(Mark& outMark, PlacementType inPlacement)
    {
        if (inPlacement == PlacementType::FREEFORM) {
            return;
        } else if (inPlacement == PlacementType::ABOVE_STAFF
                || inPlacement == PlacementType::BELOW_STAFF) {
            // TODO: handle this case
            return;
        }
        
        auto& attachedItem = outMark.getRelativePoint().getItem();
        place(outMark, attachedItem, inPlacement);
    }
    
    void
    ExpressionPlacement::placeMark(Mark& outMark)
    {
        placeMark(outMark, outMark.getMarkData().getPlacementType());
    }
    
    void
    ExpressionPlacement::place(Mark& ioMark,
                               Item& inItem,
                               PlacementType inPlacement)
    {
        // Find the mark width
        const auto markRect = ioMark.getFrame();
        const auto markWidth = markRect.getWidth();
        KOMP_ASSERT(markWidth > 0);
        
        // Find the center of the temporal in x-space
        const auto itemRect = inItem.getFrame();
        const auto itemWidth = itemRect.getWidth();
        KOMP_ASSERT(itemWidth > 0);
        
        // Find the difference in widths
        const auto widthDiff = itemWidth - markWidth;
        const auto halfWidthDiff = widthDiff / 2.0;
        ioMark.getRelativePoint().setX(halfWidthDiff);
        
        const auto isAbove = calculateIsAbove(ioMark, inItem, inPlacement);
        
        // Find the mark height
        const auto markHeight = markRect.bottom - markRect.top;
        KOMP_ASSERT(markHeight > 0);
        const auto markHalfHeight = markHeight / 2.0;
        
        // Find the rect of the item including any existing marks
        auto itemTallRect = inItem.getFrame();
        if (inItem.is<Temporal>()) {
            const auto& temporal = inItem.as<Temporal>();
            for (Mark& mark : temporal.getAttachments().getMarks()) {
                if (&ioMark != &mark) {
                    itemTallRect.unionRect(mark.getFrame());
                }
            }
        }
        
        // Find the point at which the mark's rect should stop
        double markY = 0.0;
        const double marginSizePixels = 1.5;
        
        if (isAbove) {
            markY = itemTallRect.top;
            markY += markHalfHeight;
            markY -= markHeight;
            markY -= marginSizePixels;
            ioMark.getRelativePoint().setScoreY(markY);
        } else {
            markY = itemTallRect.bottom;
            markY += markHalfHeight;
            markY += marginSizePixels;
            ioMark.getRelativePoint().setScoreY(markY);
        }
    }
    
    bool
    ExpressionPlacement::calculateIsAbove(Mark& mark,
                                          Item& inItem,
                                          PlacementType placement)
    {
        if (placement == PlacementType::ABOVE_ITEM) {
            return true;
        } else if (placement == PlacementType::BELOW_ITEM) {
            return false;
        }
        
        if (inItem.getItemTraits().getItemType() == ItemType::Note) {
            const auto& note = inItem.as<Note>();
            return !(note.getStemPolicy().getIsStemUp());
        }
        
        return mark.getMarkData().getPlacementType() == PlacementType::ABOVE_ITEM;
    }
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
