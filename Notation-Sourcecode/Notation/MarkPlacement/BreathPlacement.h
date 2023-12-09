// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MarkPlacement.h"

namespace komp
{
    class BreathPlacement : public MarkPlacement
    {
    public:
        virtual ~BreathPlacement();
        virtual MarkPlacementUPtr clone() const override;
        virtual void placeMark(Mark& inMark, PlacementType inPlacement) override;
        virtual void placeMark(Mark& inMark) override;

    private:
        void place(Mark& mark, PlacementType placement);
    };

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
