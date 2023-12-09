// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Draggable.h"

// Local
#include "CurrentState.h"

namespace komp
{
void
Draggable::prepareToDrag(const CurrentState& inState)
{
    mDragStart = inState.getPoint();
    mDragLast = inState.getPoint();
    beginDrag(inState);
    
    mDragOffset = calculateDragOffset();
}

void
Draggable::updateDrag(const CurrentState& inState)
{
    const auto& point = inState.getPoint();
    if (point == mDragLast) {
        // Bail out if we have no position change
        return;
    }
    
    Point dragPoint = point;
    dragPoint.x -= mDragOffset.x;
    dragPoint.y -= mDragOffset.y;
    
    // Determine if this is a horizontal or vertical drag
    mDragDelta.x = dragPoint.x - mDragLast.x;
    mDragDelta.y = dragPoint.y - mDragLast.y;
    
    drag(inState);
    
    mDragLast = point;
}

void
Draggable::completeDrag(const CurrentState& inState)
{
    endDrag(inState);
}

Point
Draggable::calculateDragOffset() const
{
    return Point(0, 0);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
