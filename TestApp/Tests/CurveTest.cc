// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// System
#include <sstream>

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test Support
#include "CurveScores.h"
#include "TestHelpers.h"
#include "TestFileRepository.h"

// Komp
#include "Tie.h"
#include "FileMediatorFactory.h"

namespace
{
    bool contains(const std::string& inString, const std::string& inThingToFind)
    {
        return inString.find(inThingToFind) != std::string::npos;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( TestSetup, Curve)
    {
        const auto& score = getCurveScore();
        const auto notes = getItems<Note>(*score.get(), 0);
        const auto ties = getItems<Tie>(*score.get(), 0);
        CHECK ( notes.size() >= 2 );
    }

    TEST( MxSave_TiedStart, Curve)
    {
        const auto& score = getCurveScore();
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        const auto xml = oss.str();
        CHECK( contains(xml, "<tied type=\"start\"") );
        const auto notes = getItems<Note>(*score.get(), 0);
    }

    TEST( MxSave_TiedStop, Curve)
    {
        const auto& score = getCurveScore();
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        const auto xml = oss.str();
        CHECK( contains(xml, "<tied type=\"stop\"") );
        const auto notes = getItems<Note>(*score.get(), 0);
    }

    TEST( MxSave_TieStart, Curve)
    {
        const auto& score = getCurveScore();
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        const auto xml = oss.str();
        CHECK( contains(xml, "<tie type=\"start\"") );
        const auto notes = getItems<Note>(*score.get(), 0);
    }

    TEST( MxSave_TieStop, Curve)
    {
        const auto& score = getCurveScore();
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        const auto xml = oss.str();
        CHECK( contains(xml, "<tie type=\"stop\"") );
        const auto notes = getItems<Note>(*score.get(), 0);
    }

    TEST( MxLoad_Ties_00_00, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 0;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedEndTemoral = temporals.at(1);
        CHECK( &expectedEndTemoral == &curve.getEndTemporal() );
    }

    TEST( MxLoad_Ties_00_01, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 1;
        const int expectedTerminusIndex = 0;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxLoad_Ties_00_02, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 2;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_00_03, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 3;
        const int expectedTerminusIndex = 6;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::A );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxLoad_Ties_00_04, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 4;
        const int expectedTerminusIndex = 7;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxLoad_Ties_00_05, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 2;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_00_06, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 6;
        const int expectedTerminusIndex = 3;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::A );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxLoad_Ties_00_07, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 7;
        const int expectedTerminusIndex = 4;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxLoad_Ties_01_00, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 0;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_01, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 1;
        const int expectedTerminusIndex = 4;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::B );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxLoad_Ties_01_02, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 2;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::D );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_03, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 3;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_04, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 4;
        const int expectedTerminusIndex = 1;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::B );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxLoad_Ties_01_05, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 5;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::D );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_06, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 3;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_07, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 7;
        const int expectedTerminusIndex = 11;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxLoad_Ties_01_08, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 8;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::A );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_09, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 9;
        const int expectedTerminusIndex = 13;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::C );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxLoad_Ties_01_10, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 10;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_11, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 11;
        const int expectedTerminusIndex = 7;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxLoad_Ties_01_12, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 12;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::G );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxLoad_Ties_01_13, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 13;
        const int expectedTerminusIndex = 9;

        const auto score = TestFileRepository::loadScore(fileName);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::C );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxRoundtrip_Ties_00_00, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 0;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const auto xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedEndTemoral = temporals.at(1);
        CHECK( &expectedEndTemoral == &curve.getEndTemporal() );
    }

    TEST( MxRoundtrip_Ties_00_01, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 1;
        const int expectedTerminusIndex = 0;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxRoundtrip_Ties_00_02, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 2;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_00_03, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 3;
        const int expectedTerminusIndex = 6;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::A );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxRoundtrip_Ties_00_04, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 4;
        const int expectedTerminusIndex = 7;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxRoundtrip_Ties_00_05, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 2;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_00_06, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 6;
        const int expectedTerminusIndex = 3;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::A );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxRoundtrip_Ties_00_07, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 0;
        const int temporalIndex = 7;
        const int expectedTerminusIndex = 4;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxRoundtrip_Ties_01_00, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 0;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_01, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 1;
        const int expectedTerminusIndex = 4;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::B );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxRoundtrip_Ties_01_02, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 2;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::D );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_03, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 3;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_04, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 4;
        const int expectedTerminusIndex = 1;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::B );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxRoundtrip_Ties_01_05, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 5;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::D );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_06, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 3;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_07, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 7;
        const int expectedTerminusIndex = 11;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxRoundtrip_Ties_01_08, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 8;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::A );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_09, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 9;
        const int expectedTerminusIndex = 13;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::C );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 1, outgoingCurves.size() );
        const Curve& curve = outgoingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getEndTemporal() );
    }

    TEST( MxRoundtrip_Ties_01_10, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 10;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsChord() );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_11, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 11;
        const int expectedTerminusIndex = 7;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::F );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

    TEST( MxRoundtrip_Ties_01_12, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 12;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::G );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 0, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
    }

    TEST( MxRoundtrip_Ties_01_13, Curve)
    {
        const std::string fileName = "k005b_Spanners_Ties.xml";
        const int measureIndex = 1;
        const int temporalIndex = 13;
        const int expectedTerminusIndex = 9;

        const auto original = TestFileRepository::loadScore(fileName);
        std::stringstream ss;
        auto fm = FileMediatorFactory::create(FileMediatorType::MusicXML);
        
        FileSaveParams fparams{*original.get()};
        fm->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::istringstream iss{xml};
        const auto score = fm->loadStream(iss);
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsNote() );
        CHECK( temporal.getPitch().step == PitchStep::C );
        const auto& attachments = temporal.getAttachments();
        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();
        CHECK_EQUAL( 1, incomingCurves.size() );
        CHECK_EQUAL( 0, outgoingCurves.size() );
        const Curve& curve = incomingCurves.at(0);
        CHECK( curve.getItemTraits().getItemType() == ItemType::Tie );
        const Temporal& expectedTerminus = temporals.at(expectedTerminusIndex);
        CHECK( expectedTerminus.getIsNote() );
        CHECK( &expectedTerminus == &curve.getBeginTemporal() );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
