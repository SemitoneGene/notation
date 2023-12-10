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
#include "TestHelpers.h"
#include "ChordSymbolScores.h"
#include "Directories.h"

// Komp
#include "ChordSymbol.h"
#include "ChordSymbolValue.h"
#include "kern/Time.h"

// Mx
#include "mx/api/DocumentManager.h"

#include <fstream>

namespace
{
    const bool doWriteFiles = false;
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    // this test is broken because I couldn't figure out how to handle
    // loading chords that have an x-position that is not in-line with
    // the chord's time position.  there is a TODO about this in one of
    // the MxReader classes
    
//    TEST( MusicXML_PositionTest, ChordSymbol)
//    {
//        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
//        const auto& expectedValue = collection.at(5);
//        const auto score = getBasicScore(1, 2);
//        ChordSymbolParams params;
//        params.track = score->getTracks().at(1);
//        params.value = expectedValue;
//        params.isXDifferentFromTime = true;
//        params.measureRelativeX = 123.0;
//        params.isSlotSpecified = true;
//        params.slot = -15;
//        score->layout();
//        const auto& measure = score->getMeasure(0);
//        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
//        cmd->execute();
//        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
//        const ChordSymbol& expected = expectedVec.front();
//        const auto xml = saveScore(score);
//        std::cout << xml << std::endl;
//        const auto reloadedScore = readScore(xml);
//        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
//        const ChordSymbol& actual = actualVec.front();
//        CHECK_DOUBLES_EQUAL(123.0, actual.getX(), 0.01);
//        CHECK_DOUBLES_EQUAL(-15, actual.getSlot(), 0.01);
//    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_513, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(513);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }

        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_941, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(941);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }

        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        ChordSymbolSemantics expectedSemantics;
        expectedSemantics.setHasRoot(true);
        expectedSemantics.setQuality(ChordQuality::MAJOR);
        expectedSemantics.setQualityIndicator(ChordQualityIndicator::NONE);
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK_EQUAL( expectedSemantics, actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_570, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(570);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_252, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(252);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_952, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(952);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_1, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(1);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_0, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(0);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_121, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(121);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_345, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(345);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_874, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(874);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_246, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(246);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_68, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(68);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_706, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(706);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_6, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(6);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_418, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(418);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_17, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(17);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_963, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(963);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_488, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(488);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_156, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(156);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_816, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(816);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_470, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(470);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_313, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(313);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_671, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(671);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_968, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(968);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_803, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(803);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_13, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(13);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_719, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(719);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_371, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(371);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_197, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(197);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_607, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(607);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_872, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(872);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_793, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(793);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_971, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(971);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_588, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(588);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_396, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(396);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto measure = score->getMeasure(0).get();
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_694, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(694);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_534, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(534);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_78, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(78);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_935, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(935);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_21, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(21);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_407, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(407);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_645, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(645);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_582, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(582);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_358, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(358);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getRandomlyGenerated_264, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(264);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }

        
        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }
    
    
    TEST( MusicXML_ImportTest_getRandomlyGenerated_929, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto& expectedValue = collection.at(929);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);
        
        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }

        
        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }

    TEST( MusicXML_ImportTest_getLogicallyGenerated_898, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(898);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_70, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(70);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_88, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(88);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_461, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(461);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_648, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(648);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_555, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(555);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_158, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(158);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_333, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(333);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_307, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(307);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_893, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(893);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_729, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(729);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_162, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(162);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_778, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(778);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_777, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(777);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_661, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(661);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_15, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(15);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_936, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(936);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_716, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(716);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_291, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(291);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_471, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(471);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_996, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(996);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_241, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(241);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_459, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(459);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_548, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(548);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_44, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(44);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_336, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(336);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_111, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(111);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_115, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(115);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_506, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(506);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_211, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(211);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_33, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(33);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_446, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(446);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_224, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(224);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_538, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(538);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_766, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(766);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_831, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(831);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_645, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(645);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_39, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(39);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_945, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(945);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_318, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(318);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_763, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(763);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_698, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(698);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_473, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(473);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_534, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(534);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_784, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(784);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_360, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(360);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_439, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(439);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_738, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(738);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_380, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(380);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_246, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(246);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_297, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(297);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_642, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(642);
        const auto score = getBasicScore(1, 2);
        ChordSymbolParams params(*score->getTracks().at(1).get());
        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_552, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(552);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_783, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(783);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_71, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(71);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_655, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(655);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_249, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(249);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_625, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(625);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_822, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(822);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_575, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(575);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_296, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(296);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_101, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(101);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_508, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(508);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_72, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(72);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_705, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(705);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_995, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(995);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_239, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(239);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_972, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(972);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_347, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(347);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_501, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(501);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_973, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(973);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_615, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(615);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_163, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(163);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_40, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(40);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_367, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(367);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_606, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(606);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_449, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(449);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_847, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(847);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_480, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(480);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_468, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(468);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_401, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(401);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_140, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(140);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_743, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(743);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_127, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(127);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_78, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(78);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_832, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(832);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_684, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(684);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_971, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(971);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_474, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(474);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_515, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(515);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_48, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(48);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_878, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(878);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_286, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(286);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_359, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(359);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_316, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(316);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_476, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(476);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_431, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(431);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_649, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(649);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }


        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }


    TEST( MusicXML_ImportTest_getLogicallyGenerated_872, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(872);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);

        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }

        
        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }
    
    
    TEST( MusicXML_ImportTest_getLogicallyGenerated_874, ChordSymbol)
    {
        const auto& collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto& expectedValue = collection.at(874);
        const auto score = getBasicScore(1, 2);
ChordSymbolParams params(*score->getTracks().at(1).get());        params.value = expectedValue;
        const auto& measure = score->getMeasure(0);
        const auto cmd = score->getCommandFactory().insertChordSymbolCommand(*measure, params);
        cmd->execute();
        const auto expectedVec = getItems<ChordSymbol>(*score.get(), 0);
        const ChordSymbol& expected = expectedVec.front();
        const auto xml = saveScore(score);
        
        // remove the Komp Json to make it like an import from a different application
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{xml};
        const auto docId = docMgr.createFromStream(iss);
        auto scoreData = docMgr.getData(docId);
        docMgr.destroyDocument(docId);
        auto& mxChordSymbol = scoreData.parts.at(1).measures.at(0).staves.at(0).directions.at(0).chords.at(0);
        mxChordSymbol.miscData.clear();
        scoreData.encoding = mx::api::EncodingData{};
        const auto secondDocId = docMgr.createFromScore(scoreData);
        std::ostringstream oss;
        docMgr.writeToStream(secondDocId, oss);
        const auto alteredXml = oss.str();

        if (doWriteFiles) {
            std::stringstream fname;
            fname << getTestOutputDirectory() << "/" << this->getName() << ".xml";
            std::cout << "trying " << fname.str() << std::endl;
            std::ofstream ofile{ fname.str().c_str() };
            KOMP_ASSERT(ofile.is_open());
            ofile << alteredXml;
            ofile.close();
        }

        
        const auto reloadedScore = readScore(alteredXml);
        const auto actualVec = getItems<ChordSymbol>(*reloadedScore.get(), 0);
        const ChordSymbol& actual = actualVec.front();
        const auto expectedSemantics = expected.getChordSymbolValue().getSemantics();
        const auto actualSemantics = actual.getChordSymbolValue().getSemantics();
        CHECK( expectedSemantics == actualSemantics );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
