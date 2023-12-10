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
#include "TestNoteFactory.h"
#include "TestHelpers.h"
#include "TestFileRepository.h"

// Komp
#include "KeySignature.h"


namespace
{
    inline komp::ScoreUPtr
    createSomeNotes(komp::KeySignatureType inKeySignatureType)
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
        Note& note = noteFactory.insertNote();
        noteFactory.advancePosition();
        note.setAccidental(AccidentalType::SHARP);
        note.deducePitch();
        
        note = noteFactory.insertNote();
        noteFactory.advancePosition();
        note.setAccidental(AccidentalType::NONE);
        note.deducePitch();
        
        
        note = noteFactory.insertNote();
        noteFactory.advancePosition();
        note.setAccidental(AccidentalType::FLAT);
        note.deducePitch();
        
        note = noteFactory.insertNote();
        noteFactory.advancePosition();
        note.setAccidental(AccidentalType::NATURAL);
        note.deducePitch();
        
        score->layout();
        return score;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS   

    TEST(KeyTest, KeySignature)
    {
        using namespace komp;
        auto score = createSomeNotes(KeySignatureType::D);
        
        const auto& measure = score->getMeasure(0);
        const auto& keys = measure->items<KeySignature>();
        CHECK( keys.size() == 1 );
        const KeySignature& keySignature = keys.front();
        CHECK( keySignature.getKey().getValue() == KeySignatureType::D );
    }
    
    TEST(Slots_C_FLAT_G, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::G;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {6, 9, 5, 8, 4, 7, 3};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_FLAT_G_LINE1, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::G_LINE1;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {4, 7, 3, 6, 2, 5, 1};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_FLAT_C_LINE1, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE1;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {8, 4, 7, 3, 6, 2, 5};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_FLAT_C_LINE2, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE2;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {10, 6, 9, 5, 8, 4, 7};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_FLAT_C_LINE3, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {5, 8, 4, 7, 3, 6, 2};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_FLAT_C_LINE4, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE4;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {7, 10, 6, 9, 5, 8, 4};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_FLAT_C_LINE5, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE5;
        KeySignatureValue key{KeySignatureType::C_FLAT};
        const std::vector<int> expected = {9, 5, 8, 4, 7, 3, 6};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_G, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::G;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {10, 7, 11, 8, 5, 9, 6};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_G_LINE1, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::G_LINE1;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {8, 5, 9, 6, 3, 7, 4};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_C_LINE1, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE1;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {5, 2, 6, 3, 7, 4, 8};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_C_LINE2, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE2;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {7, 4, 8, 5, 9, 6, 10};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_C_LINE3, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {9, 6, 10, 7, 4, 8, 5};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_C_LINE4, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE4;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {4, 8, 5, 9, 6, 10, 7};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_C_LINE5, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::C_LINE5;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {6, 3, 7, 4, 8, 5, 9};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_F, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::F;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {8, 5, 9, 6, 3, 7, 4};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;
    
    TEST(Slots_C_SHARP_F_LINE3, KeySignatureValue)
    {
        using namespace komp;
        const auto clef = ClefType::F_LINE3;
        KeySignatureValue key{KeySignatureType::C_SHARP};
        const std::vector<int> expected = {6, 3, 7, 4, 8, 5, 9};
        const auto actual = key.calculateSlots(clef);
        CHECK_EQUAL( expected.size(), actual.size() );
        for (size_t i = 0; i < expected.size(); ++i) {
            CHECK_EQUAL( expected.at(i), actual.at(i) );
        }
    }
    T_END;

    TEST(MxRoundTrip, KeySignatureValue)
    {
        const auto originalScore = TestFileRepository::loadScore("k017a_KeySignatures.xml");
        const auto resultScore = komp::roundTripScore(originalScore);
        int measureIndex = 0;
        const int numMeasures = resultScore->getMeasureCount();
        int expectedFifthsValue = -7;

        for ( ; measureIndex < numMeasures; ++measureIndex, ++expectedFifthsValue) {
            const auto& measure = resultScore->getMeasure(measureIndex);
            for (const auto& track : resultScore->getTracks()) {
                for (int h = 0; h < track->getHands(); ++h) {
                    const auto& keys = measure->items<KeySignature>(*track.get(), h);
                    KOMP_ASSERT(keys.size() == 1);
                    const KeySignature& keySignature = keys.front();
                    const auto fifths = keySignature.getKey().getAccidentals();
                    CHECK_EQUAL( expectedFifthsValue, fifths );
                }
            }
        }
    }
    T_END;

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
