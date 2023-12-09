// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxFile.h"
#include "mx/api/DocumentManager.h"

// Local
#include "kern/StringDistance.h"
#include "MxConverter.h"

namespace komp
{
    MxFile::MxFile(std::string_view inputFilePath)
    : mDocumentId{-1}
    , mFileMessages{}
    {
        mDocumentId = mx::api::DocumentManager::getInstance().createFromFile(std::string(inputFilePath));
        // TODO: retrieve mx parsing messages and put them into mFileMessages
    }

    MxFile::MxFile(std::istream& inputDataStream)
    : mDocumentId{-1}
    , mFileMessages{}
    {
        mDocumentId = mx::api::DocumentManager::getInstance().createFromStream(inputDataStream);
        // TODO: retrieve mx parsing messages and put them into mFileMessages
    }

    MxFile::MxFile(const mx::api::ScoreData& inputScoreData)
    : mDocumentId{-1}
    , mFileMessages{}
    {
        mDocumentId = mx::api::DocumentManager::getInstance().createFromScore(inputScoreData);
        // TODO: retrieve mx parsing messages and put them into mFileMessages
    }

    MxFile::~MxFile()
    {
        mx::api::DocumentManager::getInstance().destroyDocument(mDocumentId);
    }

    mx::api::ScoreData
    MxFile::convertToScoreData() const
    {
        return mx::api::DocumentManager::getInstance().getData(mDocumentId);
    }


    mx::api::ScoreData
    MxFile::convertToScoreData(const InstrumentConfig& inConfig) const
    {
        auto scoreData = convertToScoreData();
        deduceInstruments(inConfig, scoreData);
        return scoreData;
    }


    void
    MxFile::saveFile(std::string_view filePath)
    {
        mx::api::DocumentManager::getInstance().writeToFile(mDocumentId,
                                                            std::string(filePath));
    }

    void
    MxFile::saveStream(std::ostream& dataStream)
    {
        mx::api::DocumentManager::getInstance().writeToStream(mDocumentId, dataStream);
    }

    FileMessages
    MxFile::getFileMessages() const
    {
        // TODO: give real data here
        return FileMessages{};
    }


    void
    MxFile::deduceInstruments(const InstrumentConfig& inConfig,
                              mx::api::ScoreData& ioScoreData) const
    {
        if (inConfig.instruments.empty()) {
            return;
        }

        for (auto& part : ioScoreData.parts) {
            deduceInstrument(inConfig, part);
        }
    }


    void
    MxFile::deduceInstrument(const InstrumentConfig& inConfig,
                             mx::api::PartData& ioPartData) const
    {
        // if an exact SoundID match can be found, then use it to populate virtual instrument
        Instrument instr{};

        const auto isSoundID = ioPartData.instrumentData.soundID != mx::api::SoundID::unspecified &&
                               ioPartData.instrumentData.soundID != mx::api::SoundID::errorBadString;

        const auto isName = !ioPartData.name.empty();

        if (isSoundID) {
            const auto inSoundID = MxConverter::convertSoundID(ioPartData.instrumentData.soundID);
            if (!findExactMatch(inConfig, inSoundID, instr)) {
                instr = fuzzySoundIDMatch(inConfig, inSoundID);
            }
        } else if (isName) {
            instr = fuzzyNameMatch(inConfig, ioPartData.name);
        } else {
            findExactMatch(inConfig, SoundID::KEYBOARD_PIANO, instr);
        }

        populateVirtualInstrument(instr, ioPartData);
    }


    bool
    MxFile::findExactMatch(const InstrumentConfig& inConfig,
                           const komp::SoundID inSoundID,
                           Instrument& outInstrument) const
    {
        const auto iter = std::find_if(inConfig.instruments.begin(),
                                       inConfig.instruments.end(),
                                       [inSoundID](const auto& value) {
            return value.second.getSoundID() == inSoundID;
        });
        
        if (iter != std::end(inConfig.instruments)) {
            outInstrument = iter->second;
            return true;
        }

        return false;
    }


    void
    MxFile::populateVirtualInstrument(const Instrument& inInstrument, mx::api::PartData& ioPartData) const
    {
        ioPartData.instrumentData.soundID = MxConverter::convertSoundID(inInstrument.getSoundID());
        //ioPartData.instrumentData.midiData.virtualLibrary = inInstrument.getPatchID();
        //ioPartData.instrumentData.midiData.virtualName = inInstrument.getPatchID();
        ioPartData.instrumentData.name = inInstrument.name;
        ioPartData.instrumentData.abbreviation = "";
        //ioPartData.instrumentData.midiData.device = inInstrument.getPatchID();
        ioPartData.instrumentData.midiData.bank = -1;
        ioPartData.instrumentData.midiData.channel = -1;
        ioPartData.instrumentData.midiData.unpitched = -1;
        ioPartData.instrumentData.midiData.volume = -1.0;
        ioPartData.instrumentData.midiData.isPanSpecified = false;
        ioPartData.instrumentData.midiData.pan = 0.0;
        ioPartData.instrumentData.midiData.isElevationSpecified = false;
        ioPartData.instrumentData.midiData.elevation = 0.0;
    }


    Instrument
    MxFile::fuzzySoundIDMatch(const InstrumentConfig& inConfig, const SoundID inSoundID) const
    {
        const auto inString = SoundIDToString(inSoundID);
        using Distance = std::pair<double, Instrument>;
        std::vector<Distance> distances{};

        for (const auto& [key, instr] : inConfig.instruments) {
            const std::string instrStr = SoundIDToString(instr.getSoundID());
            const double distance = kern::getDistance(inString, instrStr);
            const auto pair = std::make_pair(distance, instr);
            distances.push_back(pair);
        }

        auto sortCompare = [&](const Distance& inLeft, const Distance& inRight) {
            return inLeft.first < inRight.first;
        };

        std::ranges::sort(distances, sortCompare);
        KOMP_ASSERT(!distances.empty());
        return distances.front().second;
    }


    Instrument
    MxFile::fuzzyNameMatch(const InstrumentConfig& inConfig,
                           std::string_view inName) const
    {
        using Distance = std::pair<double, Instrument>;
        std::vector<Distance> distances{};

        for (const auto& [key, instr] : inConfig.instruments) {
            const auto distance = kern::getDistance(inName, instr.name);
            const auto pair = std::make_pair(distance, instr);
            distances.push_back(pair);
        }

        auto sortCompare = [&](const Distance& inLeft, const Distance& inRight) {
            return inLeft.first < inRight.first;
        };

        std::ranges::sort(distances, sortCompare);
        KOMP_ASSERT(!distances.empty());
        return distances.front().second;
    }


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
