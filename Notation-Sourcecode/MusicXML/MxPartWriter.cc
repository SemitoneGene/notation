// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "MxPartWriter.h"
#include "MxMeasureWriter.h"

// Komp
#include "Score.h"
#include "Track.h"

namespace komp
{
    MxPartWriter::MxPartWriter(MxScoreWriter& mxScoreWriter, MxLocation inMxLocation)
    : mMxScoreWriter(mxScoreWriter)
    , mMxLocation(inMxLocation)
    , mOutPartData()
    {
        KOMP_ASSERT(mMxLocation.partIndex >= 0);
        KOMP_ASSERT(mMxLocation.partIndex < static_cast<int>(mMxScoreWriter.getScore().getTracks().size()));
    }
    
    mx::api::PartData MxPartWriter::getPartData()
    {
        mOutPartData = mx::api::PartData{};
        writePartHeader();
        mMxLocation.mxMeasureIndex = 0;
        for (int m = 0; m < mMxLocation.getTotalMxMeasures(); ++m) {
            
            const int kompMeasureIndex = mMxLocation.getKompMeasureIndex();
            auto measureData = mx::api::MeasureData{};

            if (kompMeasureIndex >= 0) {
                const auto& kompMeasure = mMxScoreWriter.getScore().getMeasure(kompMeasureIndex);
                MxMeasureWriter measureWriter{*kompMeasure.get(), mMxLocation, mMxScoreWriter};
                measureData = measureWriter.getMeasureData();
            }
            mOutPartData.measures.emplace_back(std::move(measureData));
            ++mMxLocation.mxMeasureIndex;
        }
        
        
        mx::api::PartData temp = std::move(mOutPartData);
        mOutPartData = mx::api::PartData{};
        return temp;
    }
    
    void MxPartWriter::writePartHeader()
    {
        const auto& track = mMxScoreWriter.getScore().getTracks().at(mMxLocation.partIndex);
        const auto& instrument = track->getInstrument();
        mOutPartData.name = instrument.name;
        mOutPartData.displayName = instrument.name;
        mOutPartData.abbreviation = instrument.abbreviation;
        mOutPartData.displayAbbreviation = instrument.abbreviation;

        mOutPartData.uniqueId = std::string("P") + std::to_string(mMxLocation.partIndex + 1);
        mOutPartData.instrumentData.name = instrument.name;
        mOutPartData.instrumentData.midiData.program = 0;

//        const auto patch = instrument.getPatchID();
//        mOutPartData.instrumentData.midiData.name = patch;
//        mOutPartData.instrumentData.midiData.virtualName = patch;
//        mOutPartData.instrumentData.midiData.virtualLibrary = patch;
//        mOutPartData.instrumentData.uniqueId = std::string("I") + std::to_string(mMxLocation.partIndex + 1);
        const komp::SoundID trackSoundID = track->getInstrument().getSoundID();
        const mx::api::SoundID mxSoundID = MxConverter::convertSoundID(trackSoundID);
        mOutPartData.instrumentData.soundID = mxSoundID;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
