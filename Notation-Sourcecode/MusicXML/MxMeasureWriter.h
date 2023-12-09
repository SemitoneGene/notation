// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MxScoreWriter.h"
#include "MxLocation.h"

// Mx
#include "mx/api/MeasureData.h"

namespace komp
{
    class Measure;
    
    class MxMeasureWriter
    {
    public:
        MxMeasureWriter(Measure& inMeasure,
                        MxLocation inMxLocation,
                        MxScoreWriter& inMxScoreWriter);
        
        mx::api::MeasureData getMeasureData();
        
    private:
        Measure& mMeasure;
        MxLocation mMxLocation;
        MxScoreWriter& mMxScoreWriter;
        mx::api::MeasureData mOutMeasureData;
        
    private:
        void setTimeSignature();
        mx::api::TimeSignatureData parseTimeSignatureDataFromKompMeasure(Measure& measure) const;
        ItemRefVec getItems(Track& inTrack, int handIndex) const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
