// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeamStrokeParams.h"

namespace komp
{
    BeamStrokeParams::BeamStrokeParams(Measure& inMeasure,
                                       Track& inTrack,
                                       komp::Temporal& inStart,
                                       komp::Temporal& inEnd,
                                       const TemporalRefVec& inTemporals)
    : isSameTemporal{false}
    , isSameMeasure{false}
    , isSameTrack{false}
    , isSameHand{false}
    , isSameVoice{false}
    , isLocationBeamable{false}
    , isLocationFlaggable{false}
    , numBeams{0}
    , numRests{0}
    , numUnbeamableDurations{0}
    , numUnbeamedTemporals{0}
    , isContinuousBeam{false}
    , isMultiTemporalType{false}
    , isInitialBeam{false}
    , isFractionalBeam{false}
    , isAdditionalBeam{false}
    , isConnectionBeam{false}
    , isHalvingBeam{false}
    , isExistingStemDirectionDeterministic{false}
    , stemDirection{StemDirection::NONE}
    , measure{inMeasure}
    , track{inTrack}
    , start{inStart}
    , end{inEnd}
    , temporals{}
    {
        for (const auto& temporal : inTemporals) {
            temporals.emplace_back(temporal);
        }
    }


    Temporal&
    BeamStrokeParams::getStart() const
    {
        return start;
    }


    Temporal&
    BeamStrokeParams::getEnd() const
    {
        return end;
    }


    const TemporalRefVec&
    BeamStrokeParams::getTemporals() const
    {
        return temporals;
    }


    Measure&
    BeamStrokeParams::getMeasure() const
    {
        return measure;
    }


    Track&
    BeamStrokeParams::getTrack() const
    {
        return track;
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
