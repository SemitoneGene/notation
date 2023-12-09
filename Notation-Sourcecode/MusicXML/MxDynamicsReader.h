// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MxScoreReader.h"
#include "Temporal.h"


namespace komp
{
    class MxDynamicsReader
    {
    public:
        
        // Use for notations dynamics
        MxDynamicsReader(
            MxScoreReader& mxScore,
            mx::api::MarkData inMarkData,
            MxLocation inMxLocation);

        // Use for directions dynamics
        MxDynamicsReader(
            MxScoreReader& mxScore,
            mx::api::MarkData inMarkData,
            MxLocation inMxLocation,
            mx::api::Placement inDirectionPlacement);

        void insertDynamic(Temporal& inTemporal);
 
        /// Find the nearest temporal and attach a dynamic to it.
        void insertDynamicOnNearestTemporalIfPossible();
 
    private:
        MxScoreReader& mMxScoreReader;
        mx::api::MarkData mMarkData;
        const MxLocation mMxLocation;
        const mx::api::Placement mDirectionPlacement;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
