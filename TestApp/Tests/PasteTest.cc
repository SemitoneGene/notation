// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Tuplet.h"
#include "BeamFactory.h"
#include "Duration.h"
#include "Measure.h"
#include "Note.h"
#include "FileMediatorFactory.h"
#include "MxTupletSpec.h"
#include "Selection.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

TEST( SimplePaste, PasteTest )
{
    const auto score = getBasicScore(2, 1);
    TestNoteFactory f{*score.get()};
    f.temporalType = TemporalType::QUARTER;
    const auto& originalNote = f.insertNote();
    
    const auto& measureOne = score->getMeasure(0);
    const auto& measureTwo = score->getMeasure(1);
    const auto& staff = measureOne->getStaffSystem()->getStaves().at(0);
    PasteRangeParams params {
        {
            *measureOne.get(),
            *staff.get(),
            *measureOne.get(),
            *staff.get()
        },
        {
            *measureTwo.get(),
            *staff.get(),
            *measureTwo.get(),
            *staff.get()
        }
    };
    
    const auto pasteCmd = score->getCommandFactory().clipboardPasteRangeCommand(params);
    pasteCmd->execute();
    const auto pastedNotes = getItems<Temporal>(*score.get(), 1, 0, 0, 0);
    const Temporal& pastedNote = pastedNotes.at(0);
    CHECK( &pastedNote != &originalNote );
    CHECK( pastedNote.getItemID() != originalNote.getItemID() );
}
T_END;

TEST( ChordPasteOneMeasure, SinglePasteTest )
{
    const auto score = TestFileRepository::loadScore("Chord.xml");
    TestNoteFactory f{*score.get()};
    
    const auto& measureOne = score->getMeasure(0);
    const auto& measureTwo = score->getMeasure(1);
    const auto& staff = measureOne->getStaffSystem()->getStaves().at(0);
    PasteRangeParams params {
        {
            *measureOne.get(),
            *staff.get(),
            *measureOne.get(),
            *staff.get()
        },
        {
            *measureTwo.get(),
            *staff.get(),
            *measureTwo.get(),
            *staff.get()
        }
    };
    const auto pasteCmd = score->getCommandFactory().clipboardPasteRangeCommand(params);
    pasteCmd->execute();
    
    int measureIndex = 0;
    int trackIndex = 0;
    int handIndex = 0;
    int voiceIndex = 0;
    auto origTemporals = getItems<Temporal>(*score.get(),
                                            measureIndex,
                                            trackIndex,
                                            handIndex,
                                            voiceIndex);
    komp::sortItems(origTemporals);
    
    measureIndex = 1;
    auto destTemporals = getItems<Temporal>(*score.get(),
                                            measureIndex,
                                            trackIndex,
                                            handIndex,
                                            voiceIndex);
    komp::sortItems(destTemporals);
    
    const auto isCorrect = [&](Temporal& inLhs, Temporal& inRhs) {
        if (inLhs.getIsChord() != inRhs.getIsChord()) {
            KOMP_THROW("not the same type - chord");
        }
        
        if (inLhs.getIsNote() != inRhs.getIsNote()) {
            KOMP_THROW("not the same type - note");
        }
        
        if (inLhs.getIsRest() != inRhs.getIsRest()) {
            KOMP_THROW("not the same type - rest");
        }
        
        if (inLhs.getSlot() != inRhs.getSlot()) {
            KOMP_THROW("not the same slot");
        }
        
        if (&inLhs == &inRhs) {
            KOMP_THROW("pointers are to the same object");
        }
        
        if (inLhs.getIsChordMember() != inRhs.getIsChordMember()) {
            KOMP_THROW("not the same chord membership status");
        }
        
        if (inLhs.getIsChord()) {
            const auto& lchord = inLhs.as<Chord>();
            const auto& rchord = inLhs.as<Chord>();
            if (lchord.getNotes().size() != rchord.getNotes().size()) {
                KOMP_THROW("not the same number of chord notes");
            }
        }
        
        return true;
    };
    
    CHECK_EQUAL( origTemporals.size(), destTemporals.size() );
    auto orig = origTemporals.cbegin();
    auto dest = destTemporals.cbegin();
    const auto end = origTemporals.cend();
    
    for ( ; orig != end; ++orig, ++dest) {
        const auto& origTemporal = *orig;
        const auto& destTemporal = *dest;
        CHECK( isCorrect(origTemporal, destTemporal) );
    }
}
T_END;

//
//TEST( BombePasteOneMeasure, PasteTest )
//{
//    const auto score = TestFileRepository::loadScore("Bombe.xml");
//    TestNoteFactory f{*score.get()};
//
//    const auto& measureOne = score->getMeasure(0);
//    const auto& measureTwo = score->getMeasure(1);
//    const auto& staff = measureOne->getStaffSystem()->getStaves().at(0);
//    PasteRangeParams params {
//        {
//            *measureOne.get(),
//            *staff.get(),
//            *measureOne.get(),
//            *staff.get()
//        },
//        {
//            *measureTwo.get(),
//            *staff.get(),
//            *measureTwo.get(),
//            *staff.get()
//        }
//    };
//    const auto pasteCmd = score->getCommandFactory().clipboardPasteRangeCommand(params);
//    pasteCmd->execute();
//
//    int measureIndex = 0;
//    int trackIndex = 0;
//    int handIndex = 0;
//    int voiceIndex = 0;
//    auto origTemporals = getItems<Temporal>(*score.get(),
//                                            measureIndex,
//                                            trackIndex,
//                                            handIndex,
//                                            voiceIndex);
//    komp::sortItems(origTemporals);
//
//    handIndex = 1;
//    auto destTemporals = getItems<Temporal>(*score.get(),
//                                            measureIndex,
//                                            trackIndex,
//                                            handIndex,
//                                            voiceIndex);
//    komp::sortItems(destTemporals);
//
//    const auto isCorrect = [&](Temporal& inLhs, Temporal& inRhs) {
//        if (inLhs.getIsChord() != inRhs.getIsChord()) {
//            KOMP_THROW("not the same type - chord");
//        }
//
//        if (inLhs.getIsNote() != inRhs.getIsNote()) {
//            KOMP_THROW("not the same type - note");
//        }
//
//        if (inLhs.getIsRest() != inRhs.getIsRest()) {
//            KOMP_THROW("not the same type - rest");
//        }
//
//        if (inLhs.getSlot() != inRhs.getSlot()) {
//            KOMP_THROW("not the same slot");
//        }
//
//        if (&inLhs == &inRhs) {
//            KOMP_THROW("pointers are to the same object");
//        }
//
//        if (inLhs.getIsChordMember() != inRhs.getIsChordMember()) {
//            KOMP_THROW("not the same chord membership status");
//        }
//
//        if (inLhs.getIsChord()) {
//            const auto& lchord = inLhs.as<Chord>();
//            const auto& rchord = inLhs.as<Chord>();
//            if (lchord.getNotes().size() != rchord.getNotes().size()) {
//                KOMP_THROW("not the same number of chord notes");
//            }
//        }
//
//        return true;
//    };
//
//    CHECK_EQUAL( origTemporals.size(), destTemporals.size() );
//    auto orig = origTemporals.cbegin();
//    auto dest = destTemporals.cbegin();
//    const auto end = origTemporals.cend();
//
//    for ( ; orig != end; ++orig, ++dest) {
//        const auto& origTemporal = *orig;
//        const auto& destTemporal = *dest;
//        CHECK( isCorrect(origTemporal, destTemporal) );
//    }
//
//}
//T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

