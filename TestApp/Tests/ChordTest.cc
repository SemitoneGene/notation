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
#include "ChordScore.h"
#include "TestHelpers.h"

// Komp
#include "Note.h"
#include "Chord.h"

namespace
{

}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS    
    TEST( TestNothing, Chord)
    {
        auto scores = getChordScores();
        CHECK(scores.size() > 0);
    }

    TEST( Chord_Bug_PIVOTAL_147058063, Chord)
    {
        ScoreInfo scoreInfo{};
        komp::Instrument instrument;
        instrument.name = "Piano";
        komp::TrackInfo trackInfo(instrument);
        trackInfo.addStaffInfo(ClefType::G);
        trackInfo.addStaffInfo(ClefType::F);
        scoreInfo.trackInfoList.push_back(trackInfo);
        auto score = Score::create(scoreInfo);
        
        auto& reg = score->getItemRegistry();
        const auto& measure = score->getMeasure(0);
        const auto& track = score->getTracks().at(0);
        const MusicLocation loc(*measure.get(), *track);
        auto& note1 = reg.createItem<Note>(loc, TemporalType::WHOLE);
        note1.setSlot(0);
        loc.getMeasure().insertItem(note1);
        auto& note2 = reg.createItem<Note>(loc, TemporalType::WHOLE);
        note2.setSlot(2);
        loc.getMeasure().insertItem(note2);
        auto& note3 = reg.createItem<Note>(loc, TemporalType::WHOLE);
        note3.setSlot(4);
        loc.getMeasure().insertItem(note3);
        NoteRefVec notesTreble{ note1, note2, note3 };
        auto& chordTreble = reg.createItem<Chord>(loc, notesTreble);
        loc.getMeasure().insertItem(chordTreble);
        CHECK(note1.getIsChordMember());
        CHECK(note2.getIsChordMember());
        CHECK(note3.getIsChordMember());

        const MusicLocation loc1(*measure.get(), *track, 1);
        auto& note4 = reg.createItem<Note>(loc1, TemporalType::WHOLE);
        note4.setSlot(0);
        loc1.getMeasure().insertItem(note4);
        auto& note5 = reg.createItem<Note>(loc1, TemporalType::WHOLE);
        note5.setSlot(2);
        loc1.getMeasure().insertItem(note5);
        auto& note6 = reg.createItem<Note>(loc1, TemporalType::WHOLE);
        note6.setSlot(4);
        loc1.getMeasure().insertItem(note6);
        NoteRefVec notesBass{ note4, note5, note6 };
        auto& chordBass = reg.createItem<Chord>(loc1, notesBass);
        loc1.getMeasure().insertItem(chordBass);
        CHECK(note4.getIsChordMember());
        CHECK(note5.getIsChordMember());
        CHECK(note6.getIsChordMember());

        const auto xml = saveScore(score);
        //std::cout << xml << std::endl;
        const auto savedScore = komp::readScore(xml);
        const auto& scoreRef = *savedScore.get();

        const auto savedNotesTreble = komp::getItems<Note>(scoreRef, 0, 0, 0, 0);
        const Chord& savedChordTreble = komp::getItems<Chord>(scoreRef, 0, 0, 0, 0).at(0);
        CHECK( savedNotesTreble.at(0).get().getIsChordMember() );
        CHECK( savedNotesTreble.at(1).get().getIsChordMember() );
        CHECK( savedNotesTreble.at(2).get().getIsChordMember() );
        CHECK_EQUAL( 3, savedChordTreble.getNotes().size() );

        const auto savedNotesBass = komp::getItems<Note>(scoreRef, 0, 0, 1, 0);
        const auto savedChordsBass = komp::getItems<Chord>(scoreRef, 0, 0, 1, 0);
        
        CHECK(!savedNotesBass.empty());
        CHECK(!savedChordsBass.empty());
        CHECK( savedNotesBass.at(0).get().getIsChordMember() );
        CHECK( savedNotesBass.at(1).get().getIsChordMember() );
        CHECK( savedNotesBass.at(2).get().getIsChordMember() );
        CHECK_EQUAL( 3, savedChordsBass.front().get().getNotes().size() );
    }

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
