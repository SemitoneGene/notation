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
#include "ChordSymbolRepository.h"
#include "MxChordConverter.h"

// Komp
#include "ChordSymbolValue.h"


namespace
{

}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS


    TEST( equals, ChordSymbolValue)
    {
        const auto& randomValues = ChordSymbolRepository::getRandomlyGenerated();
        const auto expected = randomValues.at(100);
        const auto actual = randomValues.at(100);
        CHECK( expected == actual );
    }

    TEST( notEqual, ChordSymbolValue)
    {
        const auto& randomValues = ChordSymbolRepository::getRandomlyGenerated();
        const auto expected = randomValues.at(101);
        auto str = expected.getString();
        str.emplace_back(ChordSymbolCharValue::ROOT_B, false);
        const ChordSymbolValue actual{str};
        CHECK( expected != actual );
        CHECK( expected < actual );
        CHECK( actual > expected );
    }

    TEST( jsonRoundTrip, ChordSymbolValue)
    {
        const auto& randomValues = ChordSymbolRepository::getRandomlyGenerated();

        for (const auto& randomValue : randomValues) {
            const auto json = randomValue.getJson();
            ChordSymbolValue deserialized{};
            deserialized.parseJson(json);

            CHECK_EQUAL(randomValue, deserialized);
        }
    }

    TEST( semantics001, ChordSymbolValue)
    {
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::E, 4, 0};
        const auto expectedQuality = ChordQuality::MAJOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::NONE;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected9{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::E, 4, 0};

        const auto logical = ChordSymbolRepository::getLogicallyGenerated();
        const auto value = logical.at(0);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }

    TEST( semantics002, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 234;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::G, 4, 0};
        const auto expectedQuality = ChordQuality::DIMINISHED;
        const auto expectedQualityIndicator = ChordQualityIndicator::SYMBOLIC;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::DIMINISHED, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{true, ChordExtensionQuality::DIMINISHED, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected9{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected11{true, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected13{true, ChordExtensionQuality::DOUBLE_AUGMENTED, ChordExtensionAlter::DOUBLE_SHARP};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::E, 4, 0};

        const auto logical = ChordSymbolRepository::getLogicallyGenerated();
        const auto value = logical.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }

    TEST( slashRoot, ChordSymbolValue)
    {
        ChordSymbolString str;
        str.emplace_back(ChordSymbolCharValue::ROOT_E, false);
        str.emplace_back(ChordSymbolCharValue::ACCIDENTAL_SHARP, true);
        str.emplace_back(ChordSymbolCharValue::NUMBER_7, false);
        str.emplace_back(ChordSymbolCharValue::ACCIDENTAL_FLAT, false);
        str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
        str.emplace_back(ChordSymbolCharValue::SLASH, false);
        str.emplace_back(ChordSymbolCharValue::ROOT_F, false);
        str.emplace_back(ChordSymbolCharValue::ACCIDENTAL_NATURAL, false);
        ChordSymbolValue value{str};
        const auto hasRoot = value.getSemantics().getHasRoot();
        CHECK(hasRoot);
    }


    TEST( semantics003, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 309;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::E, 4, 1};
        const auto expectedQuality = ChordQuality::MAJOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::NONE;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{true, ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected9{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{true, ChordExtensionQuality::MINOR, ChordExtensionAlter::FLAT};

        const bool expectedHasBass = true;
        const Pitch expectedBass{PitchStep::F, 4, 0};

        const auto logical = ChordSymbolRepository::getLogicallyGenerated();
        const auto value = logical.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }

    TEST( semantics004, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 760;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::C, 4, 1};
        const auto expectedQuality = ChordQuality::MAJOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::NONE;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{true, ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected9{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{true, ChordExtensionQuality::DIMINISHED, ChordExtensionAlter::DOUBLE_FLAT};

        const bool expectedHasBass = true;
        const Pitch expectedBass{PitchStep::F, 4, -2};

        const auto logical = ChordSymbolRepository::getLogicallyGenerated();
        const auto value = logical.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }


    TEST( semantics005, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 214;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::C, 4, 0};
        const auto expectedQuality = ChordQuality::MINOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::SYMBOLIC;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{true, ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected9{true, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{true, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::C, 4, 0};

        const auto logical = ChordSymbolRepository::getLogicallyGenerated();
        const auto value = logical.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }


    TEST( semantics006, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 999;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::G, 4, 0};
        const auto expectedQuality = ChordQuality::MAJOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::LETTER;

        const ChordSymbolExtension expected2{true, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{false, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected9{true, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::C, 4, 0};

        const auto collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto value = collection.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }


    TEST( semantics007, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 888;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::G, 4, 0};
        const auto expectedQuality = ChordQuality::AUGMENTED;
        const auto expectedQualityIndicator = ChordQualityIndicator::SYMBOLIC;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::AUGMENTED, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{false, ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected9{true, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::C, 4, 0};

        const auto collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto value = collection.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }


    TEST( semantics008, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 777;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::F, 4, 0};
        const auto expectedQuality = ChordQuality::MAJOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::NONE;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{true, ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected7{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected9{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::C, 4, 0};

        const auto collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto value = collection.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }


    TEST( semantics009, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 666;
        const bool expectedHasRoot = true;
        const Pitch expectedRoot{PitchStep::C, 4, 0};
        const auto expectedQuality = ChordQuality::MAJOR;
        const auto expectedQualityIndicator = ChordQualityIndicator::NONE;

        const ChordSymbolExtension expected2{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected4{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected5{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE};
        const ChordSymbolExtension expected6{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected7{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected9{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected11{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};
        const ChordSymbolExtension expected13{false, ChordExtensionQuality::UNKNOWN, ChordExtensionAlter::UNKNOWN};

        const bool expectedHasBass = false;
        const Pitch expectedBass{PitchStep::C, 4, 0};

        const auto collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto value = collection.at(chordSymbolIndex);
        const auto semantics = value.getSemantics();
        const auto actualHasRoot = semantics.getHasRoot();
        const auto actualRoot = semantics.getRoot();
        const auto actualQuality = semantics.getQuality();
        const auto actualQualityIndicator = semantics.getQualityIndicator();
        const auto actual2 = semantics.get2();
        const auto actual4 = semantics.get4();
        const auto actual5 = semantics.get5();
        const auto actual6 = semantics.get6();
        const auto actual7 = semantics.get7();
        const auto actual9 = semantics.get9();
        const auto actual11 = semantics.get11();
        const auto actual13 = semantics.get13();
        const auto actualHasBass = semantics.getHasBass();

        CHECK_EQUAL( expectedHasRoot, actualHasRoot );

        if (expectedHasRoot) {
            CHECK_EQUAL( expectedRoot, actualRoot );
        }

        CHECK( expectedQuality == actualQuality );
        CHECK( expectedQualityIndicator == actualQualityIndicator );
        CHECK_EQUAL( expected2, actual2 );
        CHECK_EQUAL( expected4, actual4 );
        CHECK_EQUAL( expected5, actual5 );
        CHECK_EQUAL( expected6, actual6 );
        CHECK_EQUAL( expected7, actual7 );
        CHECK_EQUAL( expected9, actual9 );
        CHECK_EQUAL( expected11, actual11 );
        CHECK_EQUAL( expected13, actual13 );

        CHECK_EQUAL( expectedHasBass, actualHasBass );

        if (expectedHasBass) {
            CHECK_EQUAL( expectedBass, semantics.getBass() );
        }
    }


    TEST( convertToMx001, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 554;
        const auto collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto kompValue = collection.at(chordSymbolIndex);
        const auto mxChord = MxChordConverter::convert(kompValue);
        CHECK( mxChord.root == mx::api::Step::d );
        CHECK_EQUAL( 0, mxChord.rootAlter );
        CHECK( mx::api::ChordKind::major == mxChord.chordKind );
        CHECK( mxChord.useSymbols == mx::api::Bool::unspecified );
        CHECK( mxChord.bass == mx::api::Step::unspecified );
        CHECK_EQUAL( 0, mxChord.bassAlter );
        CHECK_EQUAL( 0, mxChord.extensions.size() );
        CHECK_EQUAL( 1, mxChord.miscData.size() );
        const auto expectedMiscData = kompValue.getJson();
        const auto actualMiscData = mxChord.miscData.front().data;
        CHECK_EQUAL( expectedMiscData, actualMiscData );
        const std::string expectedMiscName = "komp-chord-string";
        const auto actualMiscName = mxChord.miscData.front().name;
        CHECK_EQUAL( expectedMiscName, actualMiscName );
    }


    TEST( convertToMx002, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 513;
        const auto collection = ChordSymbolRepository::getRandomlyGenerated();
        const auto kompValue = collection.at(chordSymbolIndex);
        const auto mxChord = MxChordConverter::convert(kompValue);
        CHECK( mxChord.root == mx::api::Step::e );
        CHECK( mx::api::ChordKind::major == mxChord.chordKind );
        CHECK_EQUAL( 0, mxChord.rootAlter );
        CHECK( mxChord.useSymbols == mx::api::Bool::unspecified );
        CHECK( mxChord.bass == mx::api::Step::unspecified );
        CHECK_EQUAL( 0, mxChord.bassAlter );
        CHECK_EQUAL( 0, mxChord.extensions.size() );
        CHECK_EQUAL( 1, mxChord.miscData.size() );
        const auto expectedMiscData = kompValue.getJson();
        const auto actualMiscData = mxChord.miscData.front().data;
        CHECK_EQUAL( expectedMiscData, actualMiscData );
        const std::string expectedMiscName = "komp-chord-string";
        const auto actualMiscName = mxChord.miscData.front().name;
        CHECK_EQUAL( expectedMiscName, actualMiscName );
    }


    TEST( convertToMx003, ChordSymbolValue)
    {
        const size_t chordSymbolIndex = 512;
        const auto collection = ChordSymbolRepository::getLogicallyGenerated();
        const auto kompValue = collection.at(chordSymbolIndex);
        const auto mxChord = MxChordConverter::convert(kompValue);
        CHECK( mxChord.root == mx::api::Step::d );
        CHECK( mx::api::ChordKind::diminishedSeventh == mxChord.chordKind );
        CHECK_EQUAL( 0, mxChord.rootAlter );
        CHECK( mxChord.useSymbols == mx::api::Bool::yes );
        CHECK( mxChord.bass == mx::api::Step::unspecified );
        CHECK_EQUAL( 0, mxChord.bassAlter );

        CHECK_EQUAL( 2, mxChord.extensions.size() );
        auto extension = mxChord.extensions.cbegin();
        const auto extEnd = mxChord.extensions.cend();

        CHECK( extension != extEnd );
        CHECK( extension->extensionNumber == mx::api::ExtensionNumber::eleventh );
        CHECK( extension->extensionAlter == mx::api::ExtensionAlter::none );
        CHECK( extension->extensionType == mx::api::ExtensionType::add );
        CHECK( extension->printObject == mx::api::Bool::yes );

        ++extension;
        CHECK( extension != extEnd );
        CHECK( extension->extensionNumber == mx::api::ExtensionNumber::thirteenth );
        CHECK( extension->extensionAlter == mx::api::ExtensionAlter::none );
        CHECK( extension->extensionType == mx::api::ExtensionType::add );
        CHECK( extension->printObject == mx::api::Bool::yes );

        CHECK_EQUAL( 1, mxChord.miscData.size() );
        const auto expectedMiscData = kompValue.getJson();
        const auto actualMiscData = mxChord.miscData.front().data;
        CHECK_EQUAL( expectedMiscData, actualMiscData );
        const std::string expectedMiscName = "komp-chord-string";
        const auto actualMiscName = mxChord.miscData.front().name;
        CHECK_EQUAL( expectedMiscName, actualMiscName );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
