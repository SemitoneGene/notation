// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <sstream>

// Komp
#include "Clef.h"
#include "Measure.h"
#include "Score.h"
#include "Track.h"
#include "TrackInfo.h"

namespace komp
{
    inline komp::ScoreUPtr getBasicScore(int inNumMeasures, int inNumInstruments)
    {
        using namespace komp;
        komp::TrackInfoList tracks;

        komp::Instrument instrument;

        if (inNumInstruments <= 0) {
            instrument.name = "PartName";
            instrument.startingClefs.push_back(komp::ClefType::G);
            komp::TrackInfo voiceTrackInfo{instrument};
            tracks.push_back(voiceTrackInfo);
        } else {
            instrument.name = "Vocals";
            for (int i = 0; i < inNumInstruments; ++i) {
                komp::TrackInfo voiceTrackInfo(instrument);
                std::stringstream ss;
                ss << "P";
                if (inNumInstruments > 0) {
                    ss << (i + 1);
                }
                voiceTrackInfo.setName(ss.str());
                voiceTrackInfo.addStaffInfo(komp::ClefType::G);
                tracks.push_back(voiceTrackInfo);
            }
        }

        komp::ScoreInfo scoreInfo;
        scoreInfo.trackInfoList = std::move(tracks);
        scoreInfo.pages = 1;
        scoreInfo.measuresPerLine = inNumMeasures;
        scoreInfo.linesPerPage = 1;

        auto score = Score::create(scoreInfo);
        MeasureNumberInfo measureNumberInfo;
        measureNumberInfo.Type = MeasureNumberType::EVERY;
        score->setMeasureNumberInfo(measureNumberInfo);
        return score;
    }

    inline komp::ScoreUPtr getBasicScore(int inNumMeasures)
    {
        using namespace komp;
        komp::TrackInfoList tracks;
        
        komp::Instrument instrument;
        instrument.name = "Vocals";
        komp::TrackInfo voiceTrackInfo(instrument);
        voiceTrackInfo.setName("PartName");
        voiceTrackInfo.addStaffInfo(komp::ClefType::G);
        tracks.push_back(voiceTrackInfo);
        
        komp::ScoreInfo scoreInfo;
        scoreInfo.trackInfoList = std::move(tracks);
        scoreInfo.pages = 1;
        scoreInfo.measuresPerLine = inNumMeasures;
        scoreInfo.linesPerPage = 1;
        
        auto score = Score::create(scoreInfo);
        MeasureNumberInfo measureNumberInfo;
        measureNumberInfo.Type = MeasureNumberType::EVERY;
        score->setMeasureNumberInfo(measureNumberInfo);
        return score;
    }
    
    inline komp::ScoreUPtr getBasicScore()
    {
        return getBasicScore(3);
    }

    inline komp::ScoreUPtr getPianoScore(int inNumMeasures)
    {
        using namespace komp;
        komp::TrackInfoList tracks;

        komp::Instrument instrument;
        instrument.name = "Piano";
        komp::TrackInfo voiceTrackInfo(instrument);
        voiceTrackInfo.setName("Piano");
        voiceTrackInfo.addStaffInfo(komp::ClefType::G);
        voiceTrackInfo.addStaffInfo(komp::ClefType::F);
        tracks.push_back(voiceTrackInfo);

        komp::ScoreInfo scoreInfo;
        scoreInfo.trackInfoList = std::move(tracks);
        scoreInfo.pages = 1;
        scoreInfo.measuresPerLine = inNumMeasures;
        scoreInfo.linesPerPage = 1;

        auto score = Score::create(scoreInfo);
        MeasureNumberInfo measureNumberInfo;
        measureNumberInfo.Type = MeasureNumberType::EVERY;
        score->setMeasureNumberInfo(measureNumberInfo);
        return score;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
