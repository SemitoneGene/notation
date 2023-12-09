// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxFileMediator.h"

// Local
#include "MxFile.h"
#include "MxScoreReader.h"
#include "MxScoreWriter.h"
#include "Preferences.h"
#include "Score.h"
#include "StaffSystem.h"
#include "Throw.h"
#include "Track.h"

// System
#include <algorithm>
#include <numeric>

namespace
{
    komp::ScoreUPtr
    importScoreData(const mx::api::ScoreData& inData)
    {
        LOG_F(3, "loading");
        using namespace komp;
        MxScoreReader mxScoreReader(inData);
        return mxScoreReader.read();
    }

    mx::api::ScoreData
    exportScoreData(const komp::FileSaveParams& inParams, komp::FileMessages& outFileMessages)
    {
        LOG_F(3, "saving");
        komp::MxScoreWriter mxScoreWriter{ inParams, outFileMessages };
        return mxScoreWriter.getScoreData();
    }
}

namespace komp
{
    komp::ScoreUPtr
    MxFileMediator::loadFile(std::string_view inPath)
    {
        MxFile mxFile{inPath};
        const auto scoreData = mxFile.convertToScoreData();
        KOMP_ASSERT(!scoreData.parts.empty());
        return importScoreData(scoreData);
    }


    komp::ScoreUPtr
    MxFileMediator::loadFile(std::string_view inPath, const InstrumentConfig& inConfig)
    {
        MxFile mxFile{inPath};
        const auto scoreData = mxFile.convertToScoreData(inConfig);
        KOMP_ASSERT(!scoreData.parts.empty());
        auto importedData = importScoreData(scoreData);
        
        // Update tracks with instrument info
        for (auto& track : importedData->getTracks()) {
            auto instrument = track->getInstrument();
            const auto soundID = instrument.getSoundID();
            const auto& updated = inConfig.instruments.at(soundID);
            instrument.concertClef = updated.concertClef;
            instrument.transposingClef = updated.transposingClef;
            instrument.transposition = updated.transposition;
            track->setInstrument(instrument);
        }
        
        return importedData;
    }


    komp::ScoreUPtr
    MxFileMediator::loadStream(std::istream& inDataStream)
    {
        MxFile mxFile{inDataStream};
        auto scoreData = mxFile.convertToScoreData();
        return importScoreData(scoreData);
    }


    komp::ScoreUPtr
    MxFileMediator::loadStream(std::istream& inDataStream,
                               const InstrumentConfig& inConfig)
    {
        MxFile mxFile{inDataStream};
        auto scoreData = mxFile.convertToScoreData(inConfig);
        return importScoreData(scoreData);
    }


    FileSaveResult
    MxFileMediator::saveFile(const FileSaveParams& inParams,
                             std::string_view filePath)
    {
        auto messages = FileMessages{};
        auto scoreData = exportScoreData(inParams, messages);
        MxFile file{scoreData};
        file.saveFile(filePath);
        return FileSaveResult{std::move(messages), FileSaveStatus::Suceess};
    }


    FileSaveResult
    MxFileMediator::saveStream(const FileSaveParams& inParams,
                               std::ostream& outDataStream)
    {
        auto messages = FileMessages{};
        auto scoreData = exportScoreData(inParams, messages);
        MxFile file{scoreData};
        file.saveStream(outDataStream);
        return FileSaveResult{std::move(messages), FileSaveStatus::Suceess};
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
