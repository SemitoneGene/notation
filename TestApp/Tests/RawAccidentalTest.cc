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
#include "TestHelpers.h"

// Komp
#include "Staff.h"
#include "RawAccidental.h"
#include "FontTheme.h"
#include "FontManager.h"

#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

namespace
{
    struct TestState
    {
        komp::Score& score;
        komp::Measure& measure;
        komp::Staff* staff;
        double measureDataX;
        double scoreX;
        double scoreY;
        double measureRelativeX;
        int slot;
        komp::Point insertionPoint;
        komp::MusicLocation musicLocation;
        komp::AccidentalType accidentalType;
        komp::RawAccidental& rawAccidental;
        komp::Rect rawAccidentalFrame;
        unsigned short character;
        komp::Rect characterFrame;
        
        TestState(komp::Score& inScore,
                  komp::Measure& inMeasure,
                  komp::MusicLocation& inLocation,
                  komp::RawAccidental& inAccidental)
        : score(inScore)
        , measure(inMeasure)
        , musicLocation(inLocation)
        , rawAccidental(inAccidental)
        {
        }
        TestState() = delete;
    };

    inline const TestState setupTest(double inMeasureRelativeX,
                                     int inSlot,
                                     komp::AccidentalType inAccidentalType)
    {
        using namespace komp;
        
        const auto testScore = getBasicScore(1, 1);
        const auto& measure = testScore->getMeasure(0);
        const auto& staff = measure->getStaffSystem()->getStaves().at(0);
        const auto dataX = measure->getDataX();
        const auto scoreX = inMeasureRelativeX + dataX;
        const auto scoreY = staff->slotToPixel(inSlot);
        const auto insertionPoint = komp::Point{inMeasureRelativeX, dataX};
        auto loc = MusicLocation(testScore->getCurrentState(insertionPoint));
        auto& registry = testScore->getItemRegistry();
        auto& accidental = registry.createItem<RawAccidental>(registry,
                                                              loc,
                                                              inAccidentalType);
        TestState ts(*testScore.get(),
                     *measure.get(),
                     loc,
                     accidental);
        ts.measureRelativeX = inMeasureRelativeX;
        ts.slot = inSlot;
        ts.accidentalType = inAccidentalType;
        ts.measureDataX = dataX;
        ts.scoreX = scoreX;
        ts.staff = staff.get();
        ts.scoreY = scoreY;
        ts.insertionPoint.setX(scoreX);
        ts.insertionPoint.setY(scoreY);
        ts.rawAccidental.setIsQuiescent(false);
        ts.measure.insertItem(ts.rawAccidental);
        ts.rawAccidental.setX(ts.measureRelativeX);
        ts.rawAccidentalFrame = ts.rawAccidental.getFrame();
        auto& fontTheme = FontTheme::instance();
        ts.character = fontTheme.AccidentalMap.at(inAccidentalType).character;
        auto& fontManager = FontManager::instance();
        ts.characterFrame = fontManager.getCharacterFrame(fontManager.getNotationFontName(),
                                                          ts.rawAccidental.getFontSize(),
                                                          ts.character);
        return ts;
    }
}

namespace komp
{
    TEST( select, RawAccidental )
    {
        const int slot = 5;
        const double measureRelativeX = 60.0;
        const auto accidentalType = AccidentalType::FLAT;

        const auto t = setupTest(measureRelativeX, slot, accidentalType);

        CHECK( !t.rawAccidentalFrame.getIsEmpty() );
        CHECK_DOUBLES_EQUAL(t.scoreX, t.rawAccidentalFrame.left, 0.01);
        CHECK_DOUBLES_EQUAL(t.characterFrame.getWidth(), t.rawAccidentalFrame.getWidth(), 0.01);
        CHECK_DOUBLES_EQUAL(t.characterFrame.getHeight(), t.rawAccidentalFrame.getHeight(), 0.01);

    }
    T_END;
}
#endif

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
