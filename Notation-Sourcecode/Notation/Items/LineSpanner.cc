// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "LineSpanner.h"

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Measure.h"
#include "Score.h"
#include "StaffSystem.h"
#include "Staff.h"

namespace komp
{
LineSpanner::LineSpanner(const MusicLocation& inLocation,
                         const SpannerParams& inParams,
                         Style inStyle,
                         PlacementType inPlacement )
: Spanner(inLocation, inParams)
, mStyle(inStyle)
{
    switch (inPlacement)
    {
    case PlacementType::ABOVE_ITEM:
        setSlot(16);
        break;
        
    case PlacementType::BELOW_ITEM:
        setSlot(-2);
        break;
        
    default:
        setSlot(0);
    }
}

LineSpanner::LineSpanner(const MusicLocation& inLocation,
                         const SpannerParams& inParams,
                         Style inStyle)
: Spanner(inLocation, inParams)
, mStyle(inStyle)
{
    setSlot(16);
}


LineSpanner::~LineSpanner()
{
    
}

const ItemTraits&
LineSpanner::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::LineSpanner);
}

ItemUPtr
LineSpanner::getMemento() const
{
    return std::make_unique<LineSpanner>(*this);
}

void
LineSpanner::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const LineSpanner* downcastedP = dynamic_cast<const LineSpanner*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    this->operator=(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

Rect
LineSpanner::getFrame() const
{
    // Construct the frame
    Rect frameRect;
    const auto& loc = getLocation();
    frameRect.left = loc.getMeasure().getDataX() + getX();
    frameRect.top = loc.getStaff().slotToPixel(getSlot());
    frameRect.setHeight(Staff::getSpacing());
    const auto endPosition = loc.getMeasure().positionForTime(mEndTime - getTime());
    frameRect.setWidth(endPosition);
    
    return frameRect;
}

void
LineSpanner::createDrawable()
{
}

    //LOG_F(WARNING, "LineSpanner: Convert to drawable");
    
    //        const Rect frameRect = getFrame();
    //
    //        switch (mStyle)
    //        {
    //            case Style::DASHED:
    //            {
    //                inContext->drawDashedLine(frameRect.left,
    //                                          frameRect.top,
    //                                          frameRect.right,
    //                                          frameRect.top,
    //                                          4,
    //                                          8,
    //                                          0);
    //                break;
    //            }
    //
    //            case Style::SOLID:
    //            {
    //                inContext->drawLine(
    //                         frameRect.left,
    //                         frameRect.top,
    //                         frameRect.right,
    //                         frameRect.top);
    //                break;
    //            }
    //
    //            case Style::WAVY:
    //            {
    //                // Get the character to draw
    //                const unsigned short wiggle = FontTheme::instance().Glissando;
    //                const auto size = measureCharacter(wiggle);
    //
    //                // Determine the total number of characters to plot along line
    //                // TODO: Get value from metadata.json
    //                const auto adjustedWidth = size.width - 1.032 * 2.0;
    //                int count = floor((frameRect.getWidth() + adjustedWidth / 2) / adjustedWidth);
    //                if (count * adjustedWidth > frameRect.getWidth()) {
    //                    // Trim off any excess items
    //                    --count;
    //                }
    //
    //                // Set the start point
    //                auto posX = frameRect.left;
    //
    //                // Get the font information
    //                const auto fontSize = Staff::getFontSize();
    //
    //                // Draw the squiggle
    //                while (count--) {
    //                    inContext->drawNotation(fontSize, posX, frameRect.top, wiggle);
    //                    posX += adjustedWidth;
    //                }
    //
    //                break;
    //            }
    //        }
}

// namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
