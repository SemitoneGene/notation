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
#include "TestFileRepository.h"
#include "Directories.h"

// Komp
#include "InstrumentConfig.h"

namespace
{
    inline komp::InstrumentConfig getConfig()
    {
        const auto dir = komp::getConfigDirectory();
        const std::string configFilePath = dir + std::string{"/instruments.json"};
        komp::InstrumentConfig cfg;
        cfg.loadFromFile(configFilePath.c_str());
        return cfg;
    }

    inline komp::ScoreUPtr getFunkyInstrumentsScore()
    {
        const auto cfg = getConfig();
        return komp::TestFileRepository::loadScore("FunkyInstruments.xml", cfg);
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( P1, Instrument )
    {
        const auto score = getFunkyInstrumentsScore();
        const auto& track = score->getTracks().at(0);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::WIND_REED_OBOE );
        CHECK_EQUAL( "Oboe d'Amore", instrument.name );
    }

    TEST( P2, Instrument )
    {
        const auto score = getFunkyInstrumentsScore();
        const auto& track = score->getTracks().at(1);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::BRASS_TRUMPET );
        CHECK_EQUAL( "Trumpet in D", instrument.name );
    }

    TEST( P3, Instrument )
    {
        const auto score = getFunkyInstrumentsScore();
        const auto& track = score->getTracks().at(2);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::METAL_BELLS_CHIMES );
        CHECK_EQUAL( "Bob", instrument.name );
    }

    TEST( P5, Instrument )
    {
        const auto score = getFunkyInstrumentsScore();
        const auto& track = score->getTracks().at(4);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::STRINGS_VIOLA );
        CHECK_EQUAL( "Viola d'Amore", instrument.name );
    }

    TEST( P6, Instrument )
    {
        const auto score = getFunkyInstrumentsScore();
        const auto& track = score->getTracks().at(5);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::KEYBOARD_PIANO );
        CHECK_EQUAL( "", instrument.name );
    }

    TEST( P1rt, Instrument )
    {
        const auto initial = getFunkyInstrumentsScore();
        const auto score = komp::roundTripScore(initial);
        const auto& track = score->getTracks().at(0);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::WIND_REED_OBOE );
        CHECK_EQUAL( "Oboe d'Amore", instrument.name );
    }

    TEST( P2rt, Instrument )
    {
        const auto initial = getFunkyInstrumentsScore();
        const auto score = komp::roundTripScore(initial);
        const auto& track = score->getTracks().at(1);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::BRASS_TRUMPET );
        CHECK_EQUAL( "Trumpet in D", instrument.name );
    }

    TEST( P3rt, Instrument )
    {
        const auto initial = getFunkyInstrumentsScore();
        const auto score = komp::roundTripScore(initial);
        const auto& track = score->getTracks().at(2);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::METAL_BELLS_CHIMES );
        CHECK_EQUAL( "Bob", instrument.name );
    }

    TEST( P5rt, Instrument )
    {
        const auto initial = getFunkyInstrumentsScore();
        const auto score = komp::roundTripScore(initial);
        const auto& track = score->getTracks().at(4);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::STRINGS_VIOLA );
        CHECK_EQUAL( "Viola d'Amore", instrument.name );
    }

    TEST( P6rt, Instrument )
    {
        const auto initial = getFunkyInstrumentsScore();
        const auto score = komp::roundTripScore(initial);
        const auto& track = score->getTracks().at(5);
        const auto instrument = track->getInstrument();
        CHECK( instrument.getSoundID() == komp::SoundID::KEYBOARD_PIANO );
        CHECK_EQUAL( "", instrument.name );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
