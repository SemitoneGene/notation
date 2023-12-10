// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"
#include "Cpul/cpulTest.h"

// Test Support
#include "TestSupport/TestFileRepository.h"

// Komp
#include "Score.h"
#include "Temporal.h"
#include "Measure.h"
#include "Curve.h"

namespace
{
    inline std::vector<komp::TemporalRef>
    getTemporals(komp::Measure& measure)
    {
        const int voice = 0;
        const int hand = 1;

        const auto& track = measure.getScore().getTracks().front();
        const auto temporals = measure.items<komp::Temporal>(*track.get(),
                                                              [&](komp::Item& filterItem) {
            const auto& loc = filterItem.getLocation();
            return loc.getHand() == hand && loc.getVoice() == voice;
        });
        return temporals;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS    
    TEST( SlurPlacementAboveBelow, MxSlur )
    {
        using namespace komp;
        auto score = TestFileRepository::loadScore("k009a_Slurs.xml");
        CHECK_EQUAL( 15, score->getMeasureCount() );
        const auto& firstMeasure = score->getMeasure(0);
        
        {
            const auto temporals = getTemporals(*firstMeasure.get());
            CHECK_EQUAL( 4, temporals.size() );
            
            auto iter = temporals.cbegin();
            
            {
                // note[0]
                const Temporal& note = *iter;
                const auto outgoingCurves = note.getAttachments().getOutgoingCurves();
                const auto incomingCurves = note.getAttachments().getIncomingCurves();
                CHECK_EQUAL( 1, outgoingCurves.size() );
                CHECK_EQUAL( 0, incomingCurves.size() );
            }
            
            {
                // note[1]
                ++iter;
                const Temporal& note = *iter;
                const auto outgoingCurves = note.getAttachments().getOutgoingCurves();
                const auto incomingCurves = note.getAttachments().getIncomingCurves();
                CHECK_EQUAL( 0, outgoingCurves.size() );
                CHECK_EQUAL( 0, incomingCurves.size() );
            }
            
            {
                // note[2]
                ++iter;
                const Temporal& note = *iter;
                const auto outgoingCurves = note.getAttachments().getOutgoingCurves();
                const auto incomingCurves = note.getAttachments().getIncomingCurves();
                CHECK_EQUAL( 0, outgoingCurves.size() );
                CHECK_EQUAL( 0, incomingCurves.size() );
            }
            
            {
                // note[3]
                ++iter;
                const Temporal& note = *iter;
                const auto outgoingCurves = note.getAttachments().getOutgoingCurves();
                const auto incomingCurves = note.getAttachments().getIncomingCurves();
                CHECK_EQUAL( 1, outgoingCurves.size() );
                CHECK_EQUAL( 0, incomingCurves.size() );
            }
        }
        
        {
            // next measure
            const auto& nextMeasure = score->getMeasure(1);
            const auto temporals = getTemporals(*nextMeasure.get());
            CHECK_EQUAL( 4, temporals.size() );
            
            // note[0]
            auto iter = temporals.cbegin();
            const Temporal& note = *iter;
            const auto outgoingCurves = note.getAttachments().getOutgoingCurves();
            const auto incomingCurves = note.getAttachments().getIncomingCurves();
            CHECK_EQUAL( 0, outgoingCurves.size() );
            CHECK_EQUAL( 2, incomingCurves.size() );
        }
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
