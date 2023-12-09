// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "PlacementType.h"

namespace komp
{
	/// strategy interface for placing marks
    class MarkPlacement
    {
    public:
        virtual ~MarkPlacement() {}
        
        virtual MarkPlacementUPtr clone() const = 0;

        /// adjust the mark's placement
        virtual void placeMark(Mark& outMark, PlacementType inPlacement) = 0;
        
        /// default PlacementType
        virtual void placeMark(Mark& outMark) = 0;
    };
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
