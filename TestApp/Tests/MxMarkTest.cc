// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "Mark.h"
#include "MarkType.h"

// Test Support
#include "BasicScore.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"
#include "TestNoteFactory.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( TrySavingAll, MxMark )
    {
        const auto count = static_cast<int>(MarkType::TOTAL_COUNT_MARK_TYPES);

        for (int i = 1; i < count; ++i) {
            
            const auto& score = getBasicScore(1, 1);
            auto& reg = score.get()->getItemRegistry();
            
            TestNoteFactory f{*score.get()};
            Note& theInsertedNote = f.insertNote();
            score->layout();
            
            const auto markType = static_cast<MarkType>(i);
            auto attachments = theInsertedNote.getAttachments();
            attachments.addMark(reg, theInsertedNote, markType, PlacementType::ABOVE_STAFF);
            attachments.addMark(reg, theInsertedNote, markType, PlacementType::BELOW_STAFF);
            attachments.addMark(reg, theInsertedNote, markType, PlacementType::AUTOMATIC);
            attachments.addMark(reg, theInsertedNote, markType, PlacementType::ABOVE_ITEM);
            attachments.addMark(reg, theInsertedNote, markType, PlacementType::BELOW_ITEM);
            const auto xml = saveScore(score);
            readScore(xml);

            // This test will fail if an exception is thrown
            CHECK( true );
        }

    }
    T_END;

    TEST( RoundTripMarkType, MxMark )
    {
        const auto count = static_cast<int>(MarkType::TOTAL_COUNT_MARK_TYPES);

        for (int i = 1; i < count; ++i) {
            const auto& score = getBasicScore(1, 1);
            auto& reg = score.get()->getItemRegistry();
            TestNoteFactory f{*score.get()};
            Note& theInsertedNote = f.insertNote();
            score->layout();
            const auto markType = static_cast<MarkType>(i);

            Mark& myLittleMark = theInsertedNote.getAttachments().addMark(reg, theInsertedNote, markType, PlacementType::ABOVE_STAFF);
            theInsertedNote.getAttachments().addMark(reg, theInsertedNote, markType, PlacementType::BELOW_STAFF);
            theInsertedNote.getAttachments().addMark(reg, theInsertedNote, markType, PlacementType::AUTOMATIC);
            theInsertedNote.getAttachments().addMark(reg, theInsertedNote, markType, PlacementType::ABOVE_ITEM);
            theInsertedNote.getAttachments().addMark(reg, theInsertedNote, markType, PlacementType::BELOW_ITEM);
            const auto xml = saveScore(score);
            const auto oscore = readScore(xml);
            const auto notes = getItems<Note>(*oscore.get(), 0);
            CHECK_EQUAL( 1, notes.size() );
            const Note& note = notes.front();
            const auto& attachments = note.getAttachments();
            const auto& marks = attachments.getMarks();

            if (marks.size() != 5) {
                std::stringstream ss;
                ss << "Round trip of the MarkType failed " << myLittleMark.getMarkData().getString();
                KOMP_THROW( ss.str() );
            }
            CHECK_EQUAL( 5, marks.size() );

            for (const Mark& mark : marks) {
                if (mark.getMarkData().getMarkType() != markType) {

                    // allow test to pass if the only difference is flippage
                    const auto actualMarkData = mark.getMarkData();
                    const auto actualSmuflString = actualMarkData.getString();
                    const auto expectedMarkData = myLittleMark.getMarkData();
                    const auto expectedSmuflString = expectedMarkData.getString();

                    if (expectedSmuflString.size() > 6) {
                        const auto expected = expectedSmuflString.substr(0, expectedSmuflString.size() - 5);
                        const auto actual = actualSmuflString.substr(0, actualSmuflString.size() - 5);

                        if (expected != actual) {
                            std::cout << xml << std::endl;
                            KOMP_THROW(std::string{expected});
                        }
                    } else {
                        KOMP_THROW("test failed");
                    }
                }
            }
        }
        
    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
