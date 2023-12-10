// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

// Komp
#include "BeamFactory.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( Test001, BeamFactory )
    {
        using namespace komp;
        auto score = getBasicScore(2);
        BeamFactory beamFactory;
        
        for (auto iter = score->getMeasuresBegin(); iter != score->getMeasuresEnd(); ++iter) {
            const auto& m = *iter;
            m->setMeter(5, 8);
        }
        
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.temporalType = TemporalType::EIGHTH;
        TemporalRefVec temporals;
        
        for (int i = 0; i < 10; ++i) {
            temporals.emplace_back(noteFactory.insertNote());
            noteFactory.advancePosition();
        }
        auto beamSpecs = beamFactory.groupBeats(temporals);
        CHECK_EQUAL(4, beamSpecs.size());

        for (int i = 0; i < 4; ++i) {
            if (i % 2 == 0) {
                CHECK_EQUAL(2, beamSpecs.at(i)->getBeamedNoteCount());
            } else {
                CHECK_EQUAL(3, beamSpecs.at(i)->getBeamedNoteCount());
            }
        }
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
