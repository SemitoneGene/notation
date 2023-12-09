// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxClef.h"
#include "MxConverter.h"

// komp
#include "Clef.h"
#include "Measure.h"
#include "MxNoteReader.h"
#include "Score.h"

namespace komp
{
    MxClef::MxClef(const mx::api::ClefData& inClefData,
                   const MxLocation& inMxLocation)
    : mClefData{inClefData}
    , mMxLocation{inMxLocation}
    {
        KOMP_ASSERT(inClefData.tickTimePosition == inMxLocation.mxTickPosition);
    }
    
    void
    MxClef::insertClef(Score& outScore, FileMessages& outMessages)
    {
        const auto& measure = outScore.getMeasure(mMxLocation.getKompMeasureIndex());
        const auto kompTickPosition = mMxLocation.getKompTickPosition();
        auto clefType = MxConverter::convertClefType(mClefData);
        
        if (clefType == ClefType::UNKNOWN) {
            std::stringstream clefNotFound;
            clefNotFound << "Unknown clef type '" << mClefData.toString()
            << "'";
            KOMP_FILE_WARN(clefNotFound.str());
        }
        
        const auto& tracks = outScore.getTracks();
        measure->insertClef(clefType,
                            *tracks.at(mMxLocation.partIndex).get(),
                            mMxLocation.handIndex,
                            kompTickPosition);                           
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
