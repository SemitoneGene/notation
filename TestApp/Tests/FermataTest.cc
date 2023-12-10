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
#include "FermataTestScore.h"

// Komp
#include "Score.h"
#include "Temporal.h"
#include "Measure.h"
#include "FileMediatorFactory.h"

// Mx
#include "mx/api/ScoreData.h"
#include "mx/api/DocumentManager.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS 

    TEST( MusicXMLExport, FermataTest )
    {
        // note this test currently just shows that we are finding fermata marks and exporting them.
        // preserving fermata placement is difficult and work needs to be done. if you are refactoring
        // fermatas, do not be surprised if the values in this test need to change
        
        using namespace komp;
        auto score = getFermataTestScore();
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        
        FileSaveParams fparams{*score.get()};
        fileMediator->saveStream(fparams, oss);
        std::string xml{oss.str()};
        //std::cout << xml << std::endl;
        std::istringstream iss{xml};
        auto& mgr = mx::api::DocumentManager::getInstance();
        auto id = mgr.createFromStream(iss);
        auto scoreData = mgr.getData(id);
        mgr.destroyDocument(id);
        mx::api::MarkData markData;
        
        const auto& measureOneNotes = scoreData.parts.at(0).measures.at(0).staves.at(0).voices.at(0).notes;
        markData = measureOneNotes.at(0).noteAttachmentData.marks.at(0);
        CHECK(mx::api::MarkType::fermata == markData.markType);

        markData = measureOneNotes.at(1).noteAttachmentData.marks.at(0);
        CHECK(mx::api::MarkType::fermata == markData.markType);
        
        markData = measureOneNotes.at(2).noteAttachmentData.marks.at(0);
        CHECK(mx::api::MarkType::fermataAngled == markData.markType);
        
        markData = measureOneNotes.at(3).noteAttachmentData.marks.at(0);
        CHECK(mx::api::MarkType::fermataAngledInverted == markData.markType);
        
        const auto& measureTwoNotes = scoreData.parts.at(0).measures.at(1).staves.at(0).voices.at(0).notes;
        markData = measureTwoNotes.at(1).noteAttachmentData.marks.at(0);
        CHECK(mx::api::MarkType::fermataSquareInverted == markData.markType);
    }
#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
