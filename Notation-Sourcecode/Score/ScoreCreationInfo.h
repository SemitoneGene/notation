// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Constants.h"
#include "StaffInfo.h"
#include "SystemBracket.h"
#include "TrackInfo.h"


namespace komp
{
    struct ScoreMetadata
    {
        std::string title;
        std::string subtitle;
        std::string composer;
        std::string lyricist;
        std::string arranger;
        std::string publisher;
        std::string copyright;
    };

    struct ScoreInfo
    {
        // Default inset for first system on page
        static constexpr double SYSTEM_INSET = 32.0;

        int measuresPerLine{1};
        int linesPerPage{1};
        int pages{1};
        TrackInfoList trackInfoList;
        SystemBracketInfoList systemBracketInfoList;
        double systemInset{SYSTEM_INSET};
        
        ScoreMetadata metadata;
        std::string filename;
        
        KeySignatureType keySignature{KeySignatureType::C};
        TimeSignatureParams timeSignature;
        
        bool insertInitialTempoMark{false};
        TempoValue initialTempoMark{static_cast<int>(DEFAULT_TEMPO), TemporalType::QUARTER};
                
        bool showInstrumentNames{true};
        bool showTempoMark{true};
        
        void deduceBrackets(const std::unordered_map<komp::SoundID,
                            komp::Instrument>& inInstruments);
    };

    inline static ScoreInfo defaultScoreInfo() {
        
        ScoreInfo info;
        info.metadata.title = "Untitled";
        info.measuresPerLine = 3;
        info.linesPerPage = 5;
        info.pages = 1;
        info.systemInset = ScoreInfo::SYSTEM_INSET;

        Instrument instrument;
        instrument.name = "Piano";
        instrument.abbreviation = "Pno.";
        
        TrackInfo track(instrument);
        track.setSoundID(SoundID::KEYBOARD_PIANO);
        track.addStaffInfo(ClefType::G);
        track.addStaffInfo(ClefType::F);
        info.trackInfoList.emplace_back(std::move(track));
        
        SystemBracketInfo bracket(SystemBracketType::BRACE, 0, 0, 0);
        info.systemBracketInfoList.emplace_back(std::move(bracket));

        return info;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
