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
#include "TestSupport/TestFileRepository.h"

// Komp
#include "BeamSaveScores.h"
#include "BeamSpecIntf.h"
#include "BeamedNoteIntf.h"
#include "FileMediatorFactory.h"

// Mx
#include "mx/api/DocumentManager.h"

namespace
{

}


namespace komp
{
 #define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( S00_N00_FF, BeamSave )
    {
        const int noteIndex = 0;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 0;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::begin == mxNote.beams.at(0) );
    }
    
    TEST( S00_N01_FF, BeamSave )
    {
        const int noteIndex = 1;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 0;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::extend == mxNote.beams.at(0) );
    }
    
    TEST( S00_N02_FF, BeamSave )
    {
        const int noteIndex = 2;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 0;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::end == mxNote.beams.at(0) );
    }
    
    TEST( S00_N03_FF, BeamSave )
    {
        const int noteIndex = 3;
        const int numNotesInPreviousBeams = 3;
        const int beamIndex = 1;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::begin == mxNote.beams.at(0) );
    }
    
    TEST( S00_N04_FF, BeamSave )
    {
        const int noteIndex = 4;
        const int numNotesInPreviousBeams = 3;
        const int beamIndex = 0;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::extend == mxNote.beams.at(0) );
    }
    
    TEST( S00_N05_FF, BeamSave )
    {
        const int noteIndex = 5;
        const int numNotesInPreviousBeams = 3;
        const int beamIndex = 1;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::end == mxNote.beams.at(0) );
    }
    
    TEST( S00_N06_FF, BeamSave )
    {
        const int noteIndex = 6;
        const int numNotesInPreviousBeams = 6;
        const int beamIndex = 2;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::begin == mxNote.beams.at(0) );
    }
    
    TEST( S00_N07_FF, BeamSave )
    {
        const int noteIndex = 7;
        const int numNotesInPreviousBeams = 6;
        const int beamIndex = 2;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::end == mxNote.beams.at(0) );
    }
    
    TEST( S00_N08_FF, BeamSave )
    {
        const int noteIndex = 0;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 3;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::begin == mxNote.beams.at(0) );
    }
    
    TEST( S00_N09_FF, BeamSave )
    {
        const int noteIndex = 1;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 3;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::extend == mxNote.beams.at(0) );
    }
    
    TEST( S00_N10_FF, BeamSave )
    {
        const int noteIndex = 2;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 3;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::extend == mxNote.beams.at(0) );
    }
    
    TEST( S00_N11_FF, BeamSave )
    {
        const int noteIndex = 3;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 3;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::extend == mxNote.beams.at(0) );
    }
    
    TEST( S00_N12_FF, BeamSave )
    {
        const int noteIndex = 4;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 3;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::extend == mxNote.beams.at(0) );
    }
    
    TEST( S00_N13_FF, BeamSave )
    {
        const int noteIndex = 5;
        const int numNotesInPreviousBeams = 0;
        const int beamIndex = 3;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::end == mxNote.beams.at(0) );
    }
    
    TEST( S00_N14_FF, BeamSave )
    {
        const int noteIndex = 6;
        const int numNotesInPreviousBeams = 6;
        const int beamIndex = 4;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::begin == mxNote.beams.at(0) );
    }
    
    TEST( S00_N15_FF, BeamSave )
    {
        const int noteIndex = 7;
        const int numNotesInPreviousBeams = 6;
        const int beamIndex = 4;
        const int beamedNoteIndex = noteIndex  - numNotesInPreviousBeams;
        const int measureIndex = 1;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        auto score = makeBeamSaveScore0(beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        
        FileSaveParams fparams{*score.get()};
        auto result = fileMediator->saveStream(fparams, ss);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(0);
        CHECK( isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::end == mxNote.beams.at(0) );
    }
#endif
    
#if 0
// TODO: make tests like this for story https://www.pivotaltracker.com/story/show/134632987
    TEST( S01_N00_FF, BeamSave )
    {
        const int noteIndex = 0;
        const int beamIndex = 0;
        const int beamedNoteIndex = 0;
        const int measureIndex = 0;
        
        using namespace komp;
        BeamSpecIntfUPtrVec beamSpecs{};
        makeBeamSaveScore1(score, beamSpecs);
        auto notes = score->getMeasure(measureIndex)->items<Note>(*score->getTracks().at(0).get());
        auto note = notes.at(noteIndex);
        const auto& beamSpec = beamSpecs.at(beamIndex);
        StemPolicy p;
        const auto isCalculateBeamLinesSuccessful = beamSpec->calculateBeamLines(p);
        CHECK( isCalculateBeamLinesSuccessful );
        
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        auto result = fileMediator->saveStream(score, ss);
        
        const bool isBeam1Broken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(1);
        const bool isBeamBroken = beamSpec->getBeamedNote(beamedNoteIndex).getIsBroken(2);
        CHECK( !isBeam1Broken );
        CHECK( !isBeamBroken );
        
        auto& docMgr = mx::api::DocumentManager::getInstance();
        std::istringstream iss{ss.str()};
        auto docId = docMgr.createFromStream(iss);
        const auto scoreData = docMgr.getData(docId);
        const auto& mxNotes = scoreData.parts.at(0).measures.at(measureIndex).staves.at(0).voices.at(0).notes;
        const auto& mxNote = mxNotes.at(noteIndex);
        CHECK( mx::api::Beam::begin == mxNote.beams.at(0) );
        CHECK( mx::api::Beam::begin == mxNote.beams.at(1) );
        CHECK( mx::api::Beam::begin == mxNote.beams.at(2) );
    }
#endif
}


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
