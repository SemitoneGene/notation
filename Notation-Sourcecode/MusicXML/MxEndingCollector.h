// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>
#include <list>

// Local
#include "Ending.h"
#include "Forward.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{
    struct MxEndingSpec
    {
        Measure& measure;
        Track& track;
        mx::api::EndingType type;
        int number;
        
        MxEndingSpec() = delete;
        
        MxEndingSpec(Measure& inMeasure,
                     Track& inTrack,
                     mx::api::EndingType inType,
                     int inNumber)
        : measure{inMeasure}
        , track{inTrack}
        , type{inType}
        , number{inNumber}
        {
        }
    };
    
    class MxEndingCollector
    {
    public:
        MxEndingCollector();

        void addEnding(Measure& inMeasure,
                       Track& inTrack,
                       mx::api::EndingType inType,
                       int inNumber);

        void insertEndings(ItemRegistry& inRegistry) const;

    private:
        void insertEnding(ItemRegistry& inRegistry,
                          const komp::EndingParams& inParams) const;

        std::vector<MxEndingSpec> mEndingSpecs;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
