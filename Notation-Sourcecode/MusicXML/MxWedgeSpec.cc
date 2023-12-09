// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxWedgeSpec.h"

// Local
#include "Score.h"
#include "Measure.h"

namespace komp
{
    std::atomic<int> MxWedgeSpec::mCounter{100000};

    MxWedgeSpec::MxWedgeSpec(Score& inScore)
    : mCreationOrder{mCounter++}
    , score{inScore}
    , track{}
    , hand{0}
    , startMeasure{}
    , startTime{0}
    , endMeasure{}
    , endTime{0}
    , wedgeType{WedgeType::CRESCENDO}
    , placementType{PlacementType::BELOW_STAFF}
    , mxFirstNote{}
    , mxWedgeStart{}
    , startLocation{MxLocationParameters{}}
    , mxWedgeStop{}
    , stopLocation{MxLocationParameters{}}
    {

    }


    int
    MxWedgeSpec::getCreationOrder() const
    {
        return mCounter;
    }


    void
    MxWedgeSpec::setMxWedgeStart(mx::api::WedgeStart inWedgeStart,
                                 MxLocation inMxLocation)
    {
        const auto& tracks = score.getTracks();
        track = tracks.at(inMxLocation.partIndex).get();
        hand = inMxLocation.handIndex;
        startMeasure = score.getMeasure(inMxLocation.getKompMeasureIndex()).get();
        startTime = inMxLocation.getKompTickPosition();
        wedgeType = WedgeType::CRESCENDO;

        if (inWedgeStart.wedgeType == mx::api::WedgeType::diminuendo) {
            wedgeType = WedgeType::DIMINUENDO;
        }
        
        mxWedgeStart = std::move(inWedgeStart);
        startLocation = std::move(inMxLocation);
    }


    void
    MxWedgeSpec::setMxWedgeStop(mx::api::WedgeStop inWedgeStop,
                                MxLocation inMxLocation)
    {
        endMeasure = score.getMeasure(inMxLocation.getKompMeasureIndex()).get();
        endTime = inMxLocation.getKompTickPosition();
        mxWedgeStop = std::move(inWedgeStop);
        stopLocation = std::move(inMxLocation);
    }

    WedgeParams
    MxWedgeSpec::getWedgeParams() const
    {
        SpannerParams sp;
        sp.track = track;
        sp.startMeasure = startMeasure;
        sp.startTime = startTime;
        sp.endMeasure = endMeasure;
        sp.endTime = endTime;
        WedgeParams result;
        result.spannerParams = sp;
        result.wedgeType = wedgeType;
        result.placementType = placementType;
        return result;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
