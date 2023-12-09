// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Local
#include "DefaultMarkPlacement.h"

namespace komp
{

    DefaultMarkPlacement::~DefaultMarkPlacement() {}
    
    MarkPlacementUPtr
    DefaultMarkPlacement::clone() const
    {
        return std::make_unique<DefaultMarkPlacement>(*this);
    }
    
    
    void
    DefaultMarkPlacement::placeMark(Mark& outMark, PlacementType inPlacement)
    {
    
    }
    
    
    void
    DefaultMarkPlacement::placeMark(Mark& outMark)
    {
    
    }

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
