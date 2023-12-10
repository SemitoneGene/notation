// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "DrawableBracket.h"
#include "Duration.h"
#include "FileMediatorFactory.h"
#include "Measure.h"
#include "MxTupletSpec.h"
#include "Note.h"
#include "BeamFactory.h"
#include "Selection.h"
#include "Tuplet.h"
#include "TupletRatio.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "TupletScores.h"
#include "TestFileRepository.h"
#include "TestHelpers.h"

namespace
{

    constexpr const double TOLERANCE = 5.0;

    inline komp::ScoreUPtr makeScore()
    {
        return komp::makeTupletScore1();
    }

    inline komp::TupletIntfRefVec getTuplets(const komp::ScoreUPtr& inScore)
    {
        using namespace komp;
        TupletIntfRefVec tuplets;
        for (auto it = inScore->getMeasuresBegin();
             it != inScore->getMeasuresEnd();
             ++it) {
            for (const auto& track : inScore->getTracks()) {
                auto lam = [&](Item& inFilterItem) {
                    return inFilterItem.is<TupletIntf>();
                };

                auto items = (*it)->items<TupletIntf>(*track.get(), lam);
                for (const auto& item : items) {
                    tuplets.push_back(item);
                }
            }
        }
        return tuplets;
    }

    inline komp::TemporalRefVec getOwningTemporals(komp::Measure& inMeasure)
    {
        using namespace komp;

        auto filter = [&](Item& inFilterItem) {
            if (inFilterItem.getItemTraits().getItemType() == ItemType::Note) {
                const auto& note = inFilterItem.as<Note>();
                if (note.getIsChordMember()) {
                    return false;
                }
            }

            return true;
        };

        const auto& score = inMeasure.getScore();
        const auto& track = score.getTracks().front();
        return inMeasure.items<Temporal>(*track.get(), filter);
    }
}

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( TripletRatio, TupletTest )
    {
        DurationCount actualNotes{3, TemporalType::EIGHTH, 0};
        DurationCount normalNotes{2, TemporalType::EIGHTH, 0};
        TupletRatio tuplet{actualNotes, normalNotes};
        CHECK_DOUBLES_EQUAL(0.66666667, tuplet.getModifier(), 0.000001);
    }
    T_END;
    
    
    TEST( getActual, TupletTest )
    {
        DurationCount actualNotes{5, TemporalType::SIXTEENTH, 1};
        DurationCount normalNotes{8, TemporalType::QUARTER, 2};
        TupletRatio tuplet{actualNotes, normalNotes};
        CHECK_EQUAL( 5, tuplet.getActual().getCount() );
        CHECK( TemporalType::SIXTEENTH == tuplet.getActual().getTemporalType() );
        CHECK_EQUAL( 1, tuplet.getActual().getDots() );
    }
    T_END;
    
    
    TEST( getNormal, TupletTest )
    {
        DurationCount actualNotes{5, TemporalType::SIXTEENTH, 1};
        DurationCount normalNotes{8, TemporalType::QUARTER, 2};
        TupletRatio tuplet{actualNotes, normalNotes};
        CHECK_EQUAL( 8, tuplet.getNormal().getCount() );
        CHECK( TemporalType::QUARTER == tuplet.getNormal().getTemporalType() );
        CHECK_EQUAL( 2, tuplet.getNormal().getDots() );
    }
    T_END;
    
    
    TEST( DurationModified, TupletTest )
    {
        DurationCount actualNotes{3, TemporalType::EIGHTH, 0};
        DurationCount normalNotes{2, TemporalType::EIGHTH, 0};
        TupletRatio tupletRatio{actualNotes, normalNotes};
        komp::Duration duration{TemporalType::QUARTER};
        duration.addTupletRatio(tupletRatio);
        CHECK_DOUBLES_EQUAL(160.0, duration.getClocks(), 0.000001);
    }
    T_END;
    
    
    TEST( TupletConstructor, TupletTest )
    {
        // create a score
        const auto score = getBasicScore(1);
        
        auto& reg = score->getItemRegistry();
        
        // add some notes to the score
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.temporalType = TemporalType::EIGHTH;
        NoteRefVec notes;
        for (int i = 0; i < 5; ++i) {
            notes.emplace_back(noteFactory.insertNote());
            noteFactory.addSteps(1);
            noteFactory.advancePosition();
        }
        score->layout();
        
        // beam the notes
        BeamFactory beamFactory{};
        beamFactory.beamSpan(reg,
                             notes.front(),
                             notes.back());
        
        // create the tuplet ratio
        DurationCount actualNotes{5, TemporalType::EIGHTH, 0};
        DurationCount normalNotes{4, TemporalType::EIGHTH, 0};
        TupletRatio tupletRatio{actualNotes, normalNotes};
        
        // create the tuplet params object
        TupletParams tupletParams{};
        tupletParams.tupletRatio = tupletRatio;
        for (Note& note : notes) {
            tupletParams.addTemporal(note.as<Temporal>());
        }
        
        auto& measure = score->getMeasures().front();
        const auto& track = score->getTracks().front();
        const MusicLocation loc(*measure.get(), *track);
        reg.createItem<Tuplet>(loc, std::move(tupletParams));
    }
    T_END;


    TEST( Bracket_LeftSide, TupletTest )
    {
        const auto score = makeScore();
        const auto& tuplets = getTuplets(score);
        const TupletIntf& tuplet = tuplets.at(3);
        const auto& temporals = tuplet.getTemporals();
        const Temporal& firstTemporal = temporals.front();
        const auto& temporalFrame = firstTemporal.getFrame();
        const auto& tupletFrame = tuplet.getFrame();
        const auto& temporalValue = temporalFrame.left;
        const auto& tupletValue = tupletFrame.left;
        CHECK( tupletValue <= (temporalValue + TOLERANCE) );
        CHECK( tupletValue >= (temporalValue - TOLERANCE) );
    }
    T_END;


    TEST( Bracket_RightSide, TupletTest )
    {
        const auto score = makeScore();
        const auto& tuplets = getTuplets(score);
        const TupletIntf& tuplet = tuplets.at(3);
        const auto& temporals = tuplet.getTemporals();
        const Temporal& lastTemporal = temporals.back();
        const auto& temporalFrame = lastTemporal.getFrame();
        const auto& tupletFrame = tuplet.getFrame();
        const auto& temporalValue = temporalFrame.right;
        const auto& tupletValue = tupletFrame.right;
        CHECK( tupletValue <= (temporalValue + TOLERANCE) );
        CHECK( tupletValue >= (temporalValue - TOLERANCE) );
    }
    T_END;


    TEST( SlotChangeUpdate_ChordedNote, TupletTest )
    {
        const auto score = makeScore();
        const auto& tuplets = getTuplets(score);
        const TupletIntf& tuplet = tuplets.at(3);
        const auto& temporals = tuplet.getTemporals();
        Temporal& firstTemporal = temporals.front();
        CHECK( firstTemporal.getItemTraits().getItemType() == ItemType::Chord );
        const Chord& chord = firstTemporal.as<Chord>();
        chord.getTopNote().setSlot(6);
        const auto& temporalFrame = firstTemporal.getFrame();
        const auto& tupletFrame = tuplet.getFrame();
        const auto& temporalValue = temporalFrame.top;
        const auto& tupletValue = tupletFrame.bottom;
        CHECK( tupletValue <= (temporalValue + TOLERANCE) );
        CHECK( tupletValue >= (temporalValue - (2 * TOLERANCE) ) );
    }
    T_END;


    TEST( CppScoreTimes00, TupletTest )
    {
        const int temporalIndex = 0;
        const double expectedTime = 0.0;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes01, TupletTest )
    {
        const int temporalIndex = 1;
        const double expectedTime = 80.0;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes02, TupletTest )
    {
        const int temporalIndex = 2;
        const double expectedTime = 160.0;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes03, TupletTest )
    {
        const int temporalIndex = 3;
        const double expectedTime = 240;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        auto temporals = getOwningTemporals(*measure.get());
        komp::sortItems(temporals);
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes04, TupletTest )
    {
        const int temporalIndex = 4;
        const double expectedTime = 288;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes05, TupletTest )
    {
        const int temporalIndex = 5;
        const double expectedTime = 336;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes06, TupletTest )
    {
        const int temporalIndex = 6;
        const double expectedTime = 384;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes07, TupletTest )
    {
        const int temporalIndex = 7;
        const double expectedTime = 432;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes08, TupletTest )
    {
        const int temporalIndex = 8;
        const double expectedTime = 480;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes09, TupletTest )
    {
        const int temporalIndex = 9;
        const double expectedTime = 560;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( CppScoreTimes10, TupletTest )
    {
        const int temporalIndex = 10;
        const double expectedTime = 640;
        const auto score = makeScore();
        const auto& measure = score->getMeasure(0);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_A, TupletTest )
    {
        const int measureIndex = 0;
        const int temporalIndex = 3;
        const double expectedTime = 560;
        const double expectedClocks = 160;
        const auto score = TestFileRepository::loadScore("k011a_Tuplets.xml");
        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_B, TupletTest )
    {
        const int measureIndex = 1;
        const int temporalIndex = 11;
        const double expectedTime = 672;
        const double expectedClocks = 48;
        const auto score = TestFileRepository::loadScore("k011a_Tuplets.xml");
        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_C, TupletTest )
    {
        const int measureIndex = 2;
        const int temporalIndex = 1;
        const double expectedTime = 160;
        const double expectedClocks = 80;
        const auto score = TestFileRepository::loadScore("k011a_Tuplets.xml");
        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_D, TupletTest )
    {
        const int measureIndex = 2;
        const int temporalIndex = 3;
        const double expectedTime = 432;
        const double expectedClocks = 48;
        const auto score = TestFileRepository::loadScore("k011a_Tuplets.xml");
        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_E, TupletTest )
    {
        const int measureIndex = 2;
        const int temporalIndex = 7;
        const double expectedTime = 720;
        const double expectedClocks = 240;
        const auto score = TestFileRepository::loadScore("k011a_Tuplets.xml");
        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_RoundTrip_A, TupletTest )
    {
        const int measureIndex = 0;
        const int temporalIndex = 3;
        const double expectedTime = 560;
        const double expectedClocks = 160;
        const auto originalScore = TestFileRepository::loadScore("k011a_Tuplets.xml");

        // round trip the score
        auto fileMed = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*originalScore.get()};
        fileMed->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::stringstream iss{xml};
        auto score = fileMed->loadStream(iss);

        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_RoundTrip_B, TupletTest )
    {
        const int measureIndex = 1;
        const int temporalIndex = 11;
        const double expectedTime = 672;
        const double expectedClocks = 48;
        const auto originalScore = TestFileRepository::loadScore("k011a_Tuplets.xml");

        // round trip the score
        auto fileMed = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*originalScore.get()};
        fileMed->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::stringstream iss{xml};
        auto score = fileMed->loadStream(iss);

        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_RoundTrip_C, TupletTest )
    {
        const int measureIndex = 2;
        const int temporalIndex = 1;
        const double expectedTime = 160;
        const double expectedClocks = 80;
        const auto originalScore = TestFileRepository::loadScore("k011a_Tuplets.xml");

        // round trip the score
        auto fileMed = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*originalScore.get()};
        fileMed->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::stringstream iss{xml};
        auto score = fileMed->loadStream(iss);

        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_RoundTrip_D, TupletTest )
    {
        const int measureIndex = 2;
        const int temporalIndex = 3;
        const double expectedTime = 432;
        const double expectedClocks = 48;
        const auto originalScore = TestFileRepository::loadScore("k011a_Tuplets.xml");

        // round trip the score
        auto fileMed = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*originalScore.get()};
        fileMed->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::stringstream iss{xml};
        auto score = fileMed->loadStream(iss);

        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( TupletTimesXml_RoundTrip_E, TupletTest )
    {
        const int measureIndex = 2;
        const int temporalIndex = 7;
        const double expectedTime = 720;
        const double expectedClocks = 240;
        const auto originalScore = TestFileRepository::loadScore("k011a_Tuplets.xml");

        // round trip the score
        auto fileMed = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*originalScore.get()};
        fileMed->saveStream(fparams, ss);
        const std::string xml = ss.str();
        std::stringstream iss{xml};
        auto score = fileMed->loadStream(iss);

        const auto& measure = score->getMeasure(measureIndex);
        const auto& temporals = getOwningTemporals(*measure.get());
        const Temporal& temporal = temporals.at(temporalIndex);
        const auto actualTime = temporal.getTime();
        const auto actualClocks = temporal.getDuration().getClocks();
        CHECK_EQUAL( 0, temporal.getTuplets().size() );
        CHECK_DOUBLES_EQUAL( expectedTime, actualTime, 0.000001 );
        CHECK_DOUBLES_EQUAL( expectedClocks, actualClocks, 0.000001 );
    }
    T_END;


    TEST( replaceChordMembers, MxTupletSpec )
    {
        auto score = makeTupletScore1();
        const auto temporals = getItems<Temporal>(*score.get(), 0);
        
        MxTupletSpec tupletSpec(temporals.front().get().getLocation());
        
        auto iter = temporals.cbegin() + 20;
        auto stop = temporals.cend();

        for ( ; iter != stop; ++iter) {
            tupletSpec.temporals.insert(&(*iter).get());
        }

        CHECK_EQUAL( 9, tupletSpec.temporals.size() );
        tupletSpec.replaceChordMembers();
        CHECK_EQUAL( 3, tupletSpec.temporals.size() );
    }


    TEST( InsertTupletCommandCorrectness, Tuplet )
    {
        const int measureIndex = 0;
        const int trackIndex = 0;
        const int handIndex = 0;
        const int voiceIndex = 0;
        const auto score = TestFileRepository::loadScore("t-001.xml");
        auto temporals = getItems<Temporal>(*score.get(),
                                            measureIndex,
                                            trackIndex,
                                            handIndex,
                                            voiceIndex);
        TupletParams params;
        for (int i = 0; i < 3; ++i) {
            params.addTemporal(temporals.at(i));
        }

        DurationCount actual{3, TemporalType::QUARTER, 0};
        DurationCount normal{2, TemporalType::QUARTER, 0};
        TupletRatio ratio{actual, normal};
        params.tupletRatio = ratio;

        CHECK( params.getIsDurationValid() );

        auto insertTupletCommand = score->getCommandFactory().insertTuplet(*score->getMeasures().front().get(),
                                                                           params);
        insertTupletCommand->execute();

        temporals = getItems<Temporal>(*score.get(),
                                       measureIndex,
                                       trackIndex,
                                       handIndex,
                                       voiceIndex);

        int expectedTime = 0;
        const auto expectedDuration = roundToInt((static_cast<double>(TIME_BASE)
                                               * (static_cast<double>(normal.getCount())
                                                / static_cast<double>(actual.getCount()))));

        for (const Temporal& temporal : temporals) {
            const auto actualTime = temporal.getRoundedTime();
            CHECK_EQUAL(expectedTime, actualTime);
            expectedTime += expectedDuration;
        }

        for (int i = 0; i < 3; ++i) {
            const Temporal& temporal = temporals.at(i);
            CHECK( temporal.getIsTupleted() );
            CHECK_EQUAL( 1, temporal.getTuplets().size() )
            const TupletIntf& tuplet = temporal.getTuplets().front();
            CHECK( tuplet.getTupletRatio().equals(params.tupletRatio) )
        }
    }


    TEST( InsertTupletUndoCorrectness, Tuplet )
    {
        const int measureIndex = 0;
        const int trackIndex = 0;
        const int handIndex = 0;
        const int voiceIndex = 0;
        const auto score = TestFileRepository::loadScore("t-001.xml");
        const auto& before = getItems<Temporal>(*score.get(),
                                                measureIndex,
                                                trackIndex,
                                                handIndex,
                                                voiceIndex);

        TupletParams params;

        for (int i = 0; i < 3; ++i) {
            params.addTemporal(before.at(i));
        }

        DurationCount actual{3, TemporalType::QUARTER, 0};
        DurationCount normal{2, TemporalType::QUARTER, 0};
        TupletRatio ratio{actual, normal};
        params.tupletRatio = ratio;

        CHECK( params.getIsDurationValid() );

        auto insertTupletCommand = score->getCommandFactory().insertTuplet(*score->getMeasures().front().get(),
                                                                           params);
        insertTupletCommand->execute();
        insertTupletCommand->undo();

        const auto& after = getItems<Temporal>(*score.get(),
                                               measureIndex,
                                               trackIndex,
                                               handIndex,
                                               voiceIndex);

        int expectedTime = 0;
        const int expectedDuration = static_cast<int>(TIME_BASE);

        for (const Temporal& temporal : after) {
            const auto actualTime = temporal.getRoundedTime();
            CHECK_EQUAL(expectedTime, actualTime);
            expectedTime += expectedDuration;
        }

        for (int i = 0; i < 3; ++i) {
            const Temporal& temporal = after.at(i);
            CHECK( !temporal.getIsTupleted() );
            CHECK_EQUAL( 0, temporal.getTuplets().size() )
        }
    }

#endif
};

    // Semitone Inc. Confidential Information.
    // TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
    // Reproduction in whole or in part without prior written permission of a
    // duly authorized representative is prohibited.

