// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MarkPlacement.h"

namespace komp
{
    class ExpressionPlacement : public MarkPlacement
    {
    public:
        virtual ~ExpressionPlacement();
        virtual MarkPlacementUPtr clone() const override;
        virtual void placeMark(Mark& ioMark, PlacementType inPlacement) override;
        virtual void placeMark(Mark& ioMark) override;
        
    private:
        void place(Mark& mark, Item& inItem, PlacementType placement);
        bool calculateIsAbove(Mark& mark, Item& inItem, PlacementType placement);
    };
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
