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
#include "BasicScore.h"
#include "CurveScores.h"
#include "TestNoteFactory.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"
#include "TupletScores.h"

// Komp
#include "Staff.h"
#include "KeySignatureType.h"
#include "DrawableStem.h"

namespace
{
    inline komp::ScoreUPtr createSomeNotes(komp::KeySignatureType inKeySignatureType)
    {
        using namespace komp;
        auto score = komp::getBasicScore(1);
        const auto& track = score->getTracks().front();
        KeySignatureValue keyValue{inKeySignatureType};
        auto& measure = score->getMeasure(0);
        
        measure->insertKeySignature(*track.get(),
                                    0, // hand
                                    keyValue);

        TestNoteFactory noteFactory{*score.get()};
        noteFactory.pitch.step = PitchStep::F;
        
        {
            Note& note = noteFactory.insertNote();
            noteFactory.advancePosition();
            note.setAccidental(AccidentalType::SHARP);
            note.deducePitch();
        }
        
        {
            Note& note = noteFactory.insertNote();
            noteFactory.advancePosition();
            note.setAccidental(AccidentalType::NATURAL);
            note.deducePitch();
        }
        
        {
            Note& note = noteFactory.insertNote();
            noteFactory.advancePosition();
            note.setAccidental(AccidentalType::FLAT);
            note.deducePitch();
        }
        
        {
            Note& note = noteFactory.insertNote();
            noteFactory.advancePosition();
            note.setAccidental(AccidentalType::NATURAL);
            note.deducePitch();
        }
        
        score->layout();
        return score;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST(PitchTest00, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::C);
        auto notes = getItems<Note>(*score.get(), 0);
        Note& note = notes.at(0);
        auto pitch = note.getPitch().getMidiNote();
        const uint8_t expected = 66; // F#4 MIDI value is 66.
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }
    
    TEST(PitchTest01, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::C);
        auto notes = getItems<Note>(*score.get(), 0);
        Note& note = notes.at(1);
        auto pitch = note.getPitch().getMidiNote();
        uint8_t expected = 65; // F4 MIDI value is 65.
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }
    
    TEST(PitchTest02, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::C);
        auto notes = getItems<Note>(*score.get(), 0);
        Note& note = notes.at(2);
        auto pitch = note.getPitch().getMidiNote();
        uint8_t expected = 64; // Fb4 MIDI value is 64.
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }
    
    TEST(PitchTest03, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::C);
        auto notes = getItems<Note>(*score.get(), 0);
        Note& note = notes.at(3);
        auto pitch = note.getPitch().getMidiNote();
        uint8_t expected = 65;
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }
    
    TEST(PitchTest04, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::B_FLAT);
        auto notes = getItems<Note>(*score.get(), 0);
        Note& note = notes.at(1);
        note.setSlot(6);
        note.setAccidental(AccidentalType::NONE);
        note.deducePitch();
        auto pitch = note.getPitch().getMidiNote();
        uint8_t expected = 70;
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }
    
    TEST(PitchTest05, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::A);
        auto notes = getItems<Note>(*score.get(), 0);
        Note& note = notes.at(1);
        note.setSlot(7);
        note.setAccidental(AccidentalType::FLAT);
        note.deducePitch();
        auto pitch = note.getPitch().getMidiNote();
        uint8_t expected = 71;
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }
    
    TEST(PitchTest06, Note)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::A);
        auto notes = getItems<Note>(*score.get(), 0);

        for (int i = 0; i < 4; ++i) {
            Note& n = notes.at(i);
            n.setSlot(7);
            n.setAccidental(AccidentalType::NONE);
            n.deducePitch();
        }
        
        Note& note = notes.at(2);
        note.setAccidental(AccidentalType::DOUBLE_FLAT);
        note.deducePitch();
        
        note = notes.at(3);
        note.deducePitch();
        auto pitch = note.getPitch().getMidiNote();
        uint8_t expected = 73; // C#5
        CHECK_EQUAL(static_cast<int>(expected), static_cast<int>(pitch));
    }

    TEST(WholeStem, Note)
    {
        using namespace komp;
        auto score = getBasicScore(1, 1);
        TestNoteFactory f{*score.get()};
        f.temporalType = TemporalType::WHOLE;
        Note& note = f.insertNote();
        CHECK(!note.getStemPolicy().getHasStem());
    }

    TEST(WholeStem2, Note)
    {
        using namespace komp;
        auto score = getBasicScore(1, 1);
        TestNoteFactory f{*score.get()};
        f.temporalType = TemporalType::QUARTER;
        Note& note = f.insertNote();
        note.setDuration(note.getDuration().setTemporalType(TemporalType::WHOLE));
        CHECK(!note.getStemPolicy().getHasStem());
    }

    TEST(QuarterStem, Note)
    {
        using namespace komp;
        auto score = getBasicScore(1, 1);
        TestNoteFactory f{*score.get()};
        f.temporalType = TemporalType::WHOLE;
        Note& note = f.insertNote();
        note.setDuration(note.getDuration().setTemporalType(TemporalType::QUARTER));
        CHECK(note.getStemPolicy().getHasStem());
    }

    TEST(EighthStem, Note)
    {
        using namespace komp;
        const auto score = TestFileRepository::loadScore("k008a_Beaming.xml");
        const auto notes = getItems<Note>(*score.get(), 0);
        const Note& note = notes.at(0);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == StemDirection::DOWN);
        CHECK(stemPolicy.policy == Policy::AUTOMATIC);
    }


    TEST(SingleStemA, Note)
    {
        using namespace komp;
        const int measureIndex = 0;
        const int noteIndex = 0;
        const auto score = TestFileRepository::loadScore("k008c_Notes_Single_Stems_and_Flags.xml");
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == StemDirection::UP);
        CHECK(stemPolicy.policy == Policy::AUTOMATIC);
        const auto stemFrame = note.getFrames().stem;
        CHECK( stemFrame.top < stemFrame.bottom );
        CHECK( stemFrame.left < stemFrame.right );
        const auto& staff = note.getLocation().getStaff();
        const auto staffTopY = staff.getScoreY();
        CHECK( stemFrame.top < staffTopY );
        const auto staffBottomY = staff.getScoreBottom();
        CHECK( stemFrame.bottom < staffBottomY );
        const auto staffMiddleY = staff.getScoreMiddle();
        CHECK( stemFrame.bottom > staffMiddleY );
        const auto noteFrame = note.getFrame();
        CHECK_DOUBLES_EQUAL( noteFrame.top, stemFrame.top, 0.00001 );
    }


    TEST(SingleStemB, Note)
    {
        using namespace komp;
        const int measureIndex = 0;
        const int noteIndex = 1;
        const auto score = TestFileRepository::loadScore("k008c_Notes_Single_Stems_and_Flags.xml");
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == StemDirection::DOWN);
        CHECK(stemPolicy.policy == Policy::AUTOMATIC);
        const auto stemFrame = note.getFrames().stem;
        CHECK( stemFrame.top < stemFrame.bottom );
        CHECK( stemFrame.left < stemFrame.right );
        const auto& staff = note.getLocation().getStaff();
        const auto staffTopY = staff.getScoreY();
        CHECK( stemFrame.top < staffTopY );
        const auto staffBottomY = staff.getScoreBottom();
        CHECK( stemFrame.bottom < staffBottomY );
        const auto staffMiddleY = staff.getScoreMiddle();
        CHECK_DOUBLES_EQUAL( staffMiddleY, stemFrame.bottom, 0.5);
        const auto noteFrame = note.getFrame();
        CHECK_DOUBLES_EQUAL( noteFrame.bottom, stemFrame.bottom, 0.00001 );
    }


    TEST(FlagY_01_01, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 1;
        const int noteIndex = 1;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 5;
        const int expectedFlagEndSlot = 12;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0 );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_01_02, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 1;
        const int noteIndex = 2;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 9;
        const int expectedFlagEndSlot = 2;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_01_04, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 1;
        const int noteIndex = 4;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = -3;
        const int expectedFlagEndSlot = 6;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 0.01 );
    }


    TEST(FlagY_02_00, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 0;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 12;
        const int expectedFlagEndSlot = 5;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_02_02, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 2;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 3;
        const int expectedFlagEndSlot = 10;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 0.01 );
    }


    TEST(FlagY_02_04, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 4;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = -8;
        const int expectedFlagEndSlot = 6;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 0.01 );
    }


    TEST(FlagY_02_07, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 7;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 20;
        const int expectedFlagEndSlot = 6;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 0.01 );
    }


    TEST(FlagY_02_08, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 8;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = -8;
        const int expectedFlagEndSlot = 7;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 0.01 );
    }


    TEST(FlagY_02_09, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 9;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 20;
        const int expectedFlagEndSlot = 5;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 1.0 );
    }


    TEST(FlagY_02_10, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 10;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 7;
        const int expectedFlagEndSlot = -1;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_02_11, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 11;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 5;
        const int expectedFlagEndSlot = 14;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, 3.0 );
    }


    TEST(FlagY_02_12, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 12;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = -6;
        const int expectedFlagEndSlot = 9;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_02_14, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 14;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 2;
        const int expectedFlagEndSlot = 12;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_02_15, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 15;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 10;
        const int expectedFlagEndSlot = 0;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto quarterSlot = slotSize / 4.0;
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, quarterSlot );
    }


    TEST(FlagY_02_16, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 16;
        const auto expectedStemDirection = StemDirection::UP;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = -7;
        const int expectedFlagEndSlot = 10;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, slotSize );
    }


    TEST(FlagY_02_17, Note)
    {
        // values
        using namespace komp;
        const std::string testFile{"k008c_Notes_Single_Stems_and_Flags.xml"};
        const int measureIndex = 2;
        const int noteIndex = 17;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;
        const int expectedStemAttachSlot = 13;
        const int expectedFlagEndSlot = 2;

        // test code
        komp::Rect stemFrame;
        komp::Rect flagFrame;
        const bool isUp = (expectedStemDirection == StemDirection::UP);
        double* stemAttach = isUp ? &stemFrame.bottom : &stemFrame.top;
        double* flagEnd = isUp ? &flagFrame.top : &flagFrame.bottom;
        const auto score = TestFileRepository::loadScore(testFile);
        const auto notes = getItems<Note>(*score.get(), measureIndex);
        const Note& note = notes.at(noteIndex);
        const auto stemPolicy = note.getStemPolicy();
        CHECK(stemPolicy.direction == expectedStemDirection);
        CHECK(stemPolicy.policy == expectedStemPolicy);
        stemFrame = note.getFrames().stem;
        flagFrame = note.getFrames().flags;
        const auto& staff = note.getLocation().getStaff();
        const auto slotSize = Staff::getSlotSize();
        const auto expectedStemAttach = staff.slotToPixel(expectedStemAttachSlot);
        const auto actualStemAttach = *stemAttach;
        const auto expectedFlagEnd = staff.slotToPixel(expectedFlagEndSlot);
        const auto actualFlagEnd = *flagEnd;

        // assertions
        CHECK_DOUBLES_EQUAL( expectedStemAttach, actualStemAttach, slotSize / 2.0  );
        CHECK_DOUBLES_EQUAL( expectedFlagEnd, actualFlagEnd, slotSize );
    }


    TEST(StemDirection00, Note)
    {
        // values
        using namespace komp;
        const int measureIndex = 0;
        const int noteIndex = 0;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;

        auto score = getCurveScore();
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(noteIndex);
        CHECK( temporal.getIsNote() );
        const auto stemPolicy = temporal.getStemPolicy();
        const auto actualStemDirection = stemPolicy.direction;
        const auto actualStemPolicy = stemPolicy.policy;
        CHECK( expectedStemDirection == actualStemDirection );
        CHECK( expectedStemPolicy == actualStemPolicy );
    }


    TEST(StemDirection01, Note)
    {
        // values
        using namespace komp;
        const int measureIndex = 0;
        const int noteIndex = 1;
        const auto expectedStemDirection = StemDirection::DOWN;
        const Policy expectedStemPolicy = Policy::AUTOMATIC;

        const auto& score = getCurveScore();
        const auto temporals = getItems<Temporal>(*score.get(), measureIndex);
        const Temporal& temporal = temporals.at(noteIndex);
        CHECK( temporal.getIsNote() );
        const auto stemPolicy = temporal.getStemPolicy();
        const auto actualStemDirection = stemPolicy.direction;
        const auto actualStemPolicy = stemPolicy.policy;
        CHECK( expectedStemDirection == actualStemDirection );
        CHECK( expectedStemPolicy == actualStemPolicy );
    }
    
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
