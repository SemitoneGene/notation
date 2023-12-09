// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{
    class MarkImpl
    {
    public:

        /// returns a rect that includes all the things that the inMark
        /// needs to avoid, for example noteheads, other marks attached
        /// to the same note, tuplet numbers and brackets, etc.
        static Rect getAvoidanceFrame(const Mark& inMark, bool inAvoidStaff);
        
        static double getHorizonalCenter(const Mark& inMark);
        static double getVerticalPlacement(const Mark& inMark,
                                           PlacementType inPlacement,
                                           const Rect& inAvoidanceFrame);

        static std::pair<double, double> getStaffTopBottom(const Mark& inMark);
        
        static double getNearestBaselinePosition(const Mark& inMark,
                                                 PlacementType inPlacement,
                                                 const Rect& inAvoidanceFrame);
        
        static void placeMarks(const Temporal& inTemporal);
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
