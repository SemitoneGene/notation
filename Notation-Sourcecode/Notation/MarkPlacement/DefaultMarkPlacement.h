// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MarkPlacement.h"

namespace komp
{
    /// strategy interface for placing marks
    class DefaultMarkPlacement : public MarkPlacement
    {
    public:
        virtual ~DefaultMarkPlacement();
        
        virtual inline MarkPlacementUPtr clone() const override;
        
        /// adjust the mark's placement
        virtual inline void placeMark(Mark& outMark, PlacementType inPlacement) override;
        
        /// default PlacementType
        virtual inline void placeMark(Mark& outMark) override;
    };
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
