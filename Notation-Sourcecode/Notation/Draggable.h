// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"

namespace komp
{
    class Draggable
    {
    public:
        Draggable() = default;
        virtual ~Draggable() = default;
        
        void prepareToDrag(const CurrentState& inState);
        void updateDrag(const CurrentState& inState);
        void completeDrag(const CurrentState& inState);

        const Point& dragStart() const;
        const Point& dragDelta() const;
        const Point& dragLast() const;
        const Point& dragOffset() const;

    protected:
        virtual Point calculateDragOffset() const;

    private:
        virtual void beginDrag(const CurrentState& inState) {}
        virtual void drag(const CurrentState& inState) {}
        virtual void endDrag(const CurrentState& inState) {}
        
        Point mDragStart{POINT_ZERO};
        Point mDragDelta{POINT_ZERO};
        Point mDragOffset{POINT_ZERO};
        Point mDragLast{POINT_ZERO};
    };
    
    inline const Point& Draggable::dragStart() const { return mDragStart; }
    inline const Point& Draggable::dragDelta() const { return mDragDelta; }
    inline const Point& Draggable::dragLast() const { return mDragLast; }
    inline const Point& Draggable::dragOffset() const { return mDragOffset; }


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.



