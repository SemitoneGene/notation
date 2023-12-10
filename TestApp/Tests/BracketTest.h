// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

//// Test Framework
//#include "Cpul/cpulTestHarness.h"
//#include "TestControl.h"

// Komp
#include "TupletRatio.h"
#include "Tuplet.h"
#include "BeamFactory.h"
#include "Duration.h"
#include "Measure.h"
#include "Note.h"
#include "FileMediatorFactory.h"
#include "MxTupletSpec.h"
#include "Mark.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace komp
{

    inline ScoreUPtr getBracketScore()
    {
        ScoreUPtrVec result;

        ScoreInfo info;

        komp::Instrument instrument;
        instrument.name = "Piano";
        komp::TrackInfo track(instrument);
        track.setName("track0");
        StaffInfo trackStaff1;
        trackStaff1.clefType = ClefType::G;
        track.addStaffInfo(trackStaff1);
        StaffInfo trackStaff2;
        trackStaff2.clefType = ClefType::F;
        track.addStaffInfo(trackStaff2);
        info.trackInfoList.push_back(track);
        SystemBracketInfo bracket1{SystemBracketType::BRACE, 0, 0, 0};
        info.systemBracketInfoList.push_back(bracket1);

        track.clearStaffInfoList();
        track.setName("track1");
        track.addStaffInfo(trackStaff1);
        info.trackInfoList.push_back(track);

        track.setName("track2");
        info.trackInfoList.push_back(track);

        track.setName("track3");
        info.trackInfoList.push_back(track);

        SystemBracketInfo bracket2{SystemBracketType::BRACKET, 0, 1, 3};
        info.systemBracketInfoList.push_back(bracket2);

        info.measuresPerLine = 4;
        info.linesPerPage = 1;
        info.pages = 1;

        auto score = Score::create(info);
        score->layout();
        return score;
    };

    inline komp::ScoreUPtrVec getBracketScores()
    {
        komp::ScoreUPtrVec outScores;
        outScores.emplace_back(getBracketScore());
        return outScores;
    }

};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

