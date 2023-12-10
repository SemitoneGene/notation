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
#include "Measure.h"
#include "FileMediatorFactory.h"
#include "Mark.h"

namespace
{
    inline komp::Mark*
    getDynamic(const komp::Measure& measure)
    {
        const int voice = 0;
        const int hand = 0;
        const auto markVec = measure.items<komp::Mark>(*measure.getScore().getTracks().at(0).get(),
                                                       voice,
                                                       [&](komp::Item& filterItem) {
            
            if (filterItem.getItemTraits().getItemType() != komp::ItemType::Mark) {
                return false;
            }
            
            auto& mark = filterItem.as<komp::Mark>();
            auto& markData = mark.getMarkData();
            return filterItem.getLocation().getHand() == hand
                && markData.is(komp::MarkCategory::DYNAMIC);
        });
        
        KOMP_ASSERT(markVec.size() < 2);
        if (markVec.size() > 0) {
            return &markVec.front().get();
        }
        
        return nullptr;
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( noteAttachedDynamicsP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(0);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_P;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsPP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(1);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_PP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsPPP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(2);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_PPP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(6);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_F;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsFF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(7);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsFFF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(8);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FFF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsMP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(12);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_MP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsMF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(13);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_MF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsSF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(14);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_SF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsFP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(17);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsSFZ, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(20);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_SFZ;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( noteAttachedDynamicsFZ, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(22);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FZ;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(0);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_P;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsPP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(1);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_PP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsPPP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(2);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_PPP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(6);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_F;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsFF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(7);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsFFF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(8);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FFF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsMP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(12);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_MP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsMF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(13);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_MF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsSF, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(14);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_SF;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsFP, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(17);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FP;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsSFZ, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(20);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_SFZ;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( measureAttachedDynamicsFZ, MxDynamicsReader )
    {
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        CHECK_EQUAL( 24, score->getMeasureCount() );
        auto& measure = score->getMeasure(22);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FZ;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( roundTripDynamicsFZ, MxDynamicsWriter )
    {
        using namespace komp;
        auto score = TestFileRepository::loadScore("k007c_Directions_Dynamics.xml");
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, ss);
        std::istringstream iss{ss.str()};
        auto resultScore = fileMediator->loadStream(iss);
        CHECK_EQUAL( 24, resultScore->getMeasureCount() );
        auto& measure = resultScore->getMeasure(22);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_FZ;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
    TEST( roundTripDynamicsP, MxDynamicsWriter )
    {
        using namespace komp;
        auto score = TestFileRepository::loadScore("k007a_Notations_Dynamics.xml");
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, ss);
        std::istringstream iss{ss.str()};
        FileMessages messages;
        auto resultScore = fileMediator->loadStream(iss);
        CHECK_EQUAL( 24, resultScore->getMeasureCount() );
        auto& measure = resultScore->getMeasure(0);
        auto dynamic = getDynamic(*measure.get());
        CHECK(dynamic != nullptr);
        auto expected = komp::MarkType::DYNAMIC_P;
        CHECK(expected == dynamic->getMarkData().getMarkType());
    }
    
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
