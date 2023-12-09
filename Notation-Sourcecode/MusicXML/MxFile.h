// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <istream>
#include <ostream>
#include <string>

// Local Frameworks
#include "mx/api/ScoreData.h"

// Local
#include "FileMessage.h"
#include "Instrument.h"
#include "InstrumentConfig.h"

namespace komp
{
    class MxFile
    {
    public:
        MxFile(std::string_view inputFilePath);
        MxFile(std::istream& inputDataStream);
        MxFile(const mx::api::ScoreData& inputScoreData);
        ~MxFile();
        MxFile(const MxFile&) = default;
        MxFile(MxFile&&) = default;
        MxFile& operator=(const MxFile&) = default;
        MxFile& operator=(MxFile&&) = default;

        mx::api::ScoreData convertToScoreData() const;
        mx::api::ScoreData convertToScoreData(const InstrumentConfig& inConfig) const;
        void saveFile(std::string_view filePath);
        void saveStream(std::ostream& dataStream);
        FileMessages getFileMessages() const;

    private:
        int mDocumentId;
        FileMessages mFileMessages;

    private:
        void deduceInstruments(const InstrumentConfig& inConfig, mx::api::ScoreData& ioScoreData) const;
        void deduceInstrument(const InstrumentConfig& inConfig, mx::api::PartData& ioPartData) const;
        bool findExactMatch(const InstrumentConfig& inConfig, const komp::SoundID inSoundID, Instrument& outInstrument) const;
        void populateVirtualInstrument(const Instrument& inInstrument, mx::api::PartData& ioPartData) const;
        Instrument fuzzySoundIDMatch(const InstrumentConfig& inConfig, const SoundID inSoundID) const;
        Instrument fuzzyNameMatch(const InstrumentConfig& inConfig, std::string_view inName) const;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
