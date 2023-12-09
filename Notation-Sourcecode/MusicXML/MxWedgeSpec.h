// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "WedgeType.h"
#include "PlacementType.h"
#include "MxLocation.h"
#include "Wedge.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{
    class MxWedgeSpec
    {
    public:
        
        MxWedgeSpec() = delete;
        MxWedgeSpec(Score& inScore);

        Score& score;
        Track* track;
        int hand;
        Measure* startMeasure;
        int startTime;
        Measure* endMeasure;
        int endTime;
        WedgeType wedgeType;
        PlacementType placementType;
        mx::api::NoteData mxFirstNote;
        mx::api::WedgeStart mxWedgeStart;
        MxLocation startLocation;
        mx::api::WedgeStop mxWedgeStop;
        MxLocation stopLocation;

        int getCreationOrder() const;

        void setMxWedgeStart(mx::api::WedgeStart inWedgeStart,
                             MxLocation inMxLocation);

        void setMxWedgeStop(mx::api::WedgeStop inWedgeStop,
                            MxLocation inMxLocation);

        WedgeParams getWedgeParams() const;

    private:
        int mCreationOrder;
        static std::atomic<int> mCounter;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
