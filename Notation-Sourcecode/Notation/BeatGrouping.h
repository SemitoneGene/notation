// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BeatUnit.h"

// System
#include <vector>

namespace komp
{
    /// \brief  represents how temporals should be grouped into beats
    ///
    /// \discussion can be used by time signature and beaming code to
    /// specify how the notes should be grouped to show the beats
    ///
    class BeatGrouping
    {
    public:
        BeatGrouping(const std::vector<BeatUnit>& inBeatUnits);

        /// The total number of clock ticks represented by
        /// adding up all of the individual beat units.
        /// this should match with time signature.
        int getTotalClocks() const;
        int getBeatUnitCount() const;
        const std::vector<BeatUnit>& getBeatUnits() const;

        /// Returns the clock times at which each separate beat occurrs
        std::vector<int> getBeatTimes() const;
        
        /// For a given time signature, returns a usable default beat grouping
        /// e.g. 4/4 will come back with 4 quarter-notes.  6/8 will give two
        /// dotted quarters
        static BeatGrouping getDefaultGroupingForTimeSignature(int inBeatsPerBar,
                                                               int inBeatUnit);
        
    private:
        std::vector<BeatUnit> mBeatUnits;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
