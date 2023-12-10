// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Duration.h"
#include "Measure.h"
#include "Staff.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"
#include "Score.h"
#include "Clef.h"
#include "TestHelpers.h"

namespace
{

}

namespace komp
{
#define X_KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( ClefScrambulationBug1, Clef )
    {
        ScoreInfo sinfo{};
        sinfo.measuresPerLine = 4;
        sinfo.linesPerPage = 1;
        TrackInfo tinfo;
        tinfo = TrackInfo{};
        tinfo.name = "Track 0";
        tinfo.staves.push_back(ClefType::C_LINE1);
        tinfo.staves.push_back(ClefType::C_LINE2);
        sinfo.trackInfoList.push_back(tinfo);
        tinfo = TrackInfo{};
        tinfo.name = "Track 1";
        tinfo.staves.clear();
        tinfo.staves.push_back(ClefType::C);
        tinfo.staves.push_back(ClefType::C_LINE4);
        sinfo.trackInfoList.push_back(tinfo);
        tinfo = TrackInfo{};
        tinfo.name = "Track 2";
        tinfo.staves.clear();
        tinfo.staves.push_back(ClefType::C_LINE5);
        tinfo.staves.push_back(ClefType::F);
        sinfo.trackInfoList.push_back(tinfo);
        auto score = Score::create(sinfo);

        auto track = score->getTracks().at(0);
        auto staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C_LINE1 );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::C_LINE2 );

        track = score->getTracks().at(1);
        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::C_LINE4 );

        track = score->getTracks().at(2);
        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C_LINE5 );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::F );
    }
    T_END;

    TEST( ClefScrambulationBug2, Clef )
    {
        ScoreInfo sinfo{};
        sinfo.measuresPerLine = 4;
        sinfo.linesPerPage = 1;
        TrackInfo tinfo;
        tinfo = TrackInfo{};
        tinfo.name = "Track 0";
        tinfo.staves.push_back(ClefType::C_LINE1);
        tinfo.staves.push_back(ClefType::C_LINE2);
        sinfo.trackInfoList.push_back(tinfo);
        tinfo = TrackInfo{};
        tinfo.name = "Track 1";
        tinfo.staves.clear();
        tinfo.staves.push_back(ClefType::C);
        tinfo.staves.push_back(ClefType::C_LINE4);
        sinfo.trackInfoList.push_back(tinfo);
        tinfo = TrackInfo{};
        tinfo.name = "Track 2";
        tinfo.staves.clear();
        tinfo.staves.push_back(ClefType::C_LINE5);
        tinfo.staves.push_back(ClefType::F);
        sinfo.trackInfoList.push_back(tinfo);
        auto score = Score::create(sinfo);

        auto newinfo = score->getScoreInfo();
        TrackInfo newTrack;
        newTrack.staves.push_back( ClefType::F_15MA );
        newinfo.trackInfoList.push_back(newTrack);
        score->initWithScoreInfo(newinfo);

        auto track = score->getTracks().at(0);
        auto staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C_LINE1 );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::C_LINE2 );

        track = score->getTracks().at(1);
        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::C_LINE4 );

        track = score->getTracks().at(2);
        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C_LINE5 );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::F );
    }
    T_END;

    TEST( ClefScrambulationBug3, Clef )
    {
        ScoreInfo sinfo{};
        sinfo.measuresPerLine = 4;
        sinfo.linesPerPage = 1;
        TrackInfo tinfo;
        tinfo = TrackInfo{};
        tinfo.name = "Track 0";
        tinfo.staves.push_back(ClefType::C_LINE1);
        tinfo.staves.push_back(ClefType::C_LINE2);
        sinfo.trackInfoList.push_back(tinfo);
        tinfo = TrackInfo{};
        tinfo.name = "Track 1";
        tinfo.staves.clear();
        tinfo.staves.push_back(ClefType::C);
        tinfo.staves.push_back(ClefType::C_LINE4);
        sinfo.trackInfoList.push_back(tinfo);
        tinfo = TrackInfo{};
        tinfo.name = "Track 2";
        tinfo.staves.clear();
        tinfo.staves.push_back(ClefType::C_LINE5);
        tinfo.staves.push_back(ClefType::F);
        sinfo.trackInfoList.push_back(tinfo);
        const auto original = Score::create(sinfo);

        const auto score = komp::roundTripScore(original.get());

        auto track = score->getTracks().at(0);
        auto staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C_LINE1 );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::C_LINE2 );

        track = score->getTracks().at(1);
        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::C_LINE4 );

        track = score->getTracks().at(2);
        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 0);
        CHECK( staff->getClef()->type() == ClefType::C_LINE5 );

        staff = score->getStaffSystems().at(0)->getStaff(track.get(), 1);
        CHECK( staff->getClef()->type() == ClefType::F );
    }
    T_END;


#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
