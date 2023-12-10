// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BombeTest.h"

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "TupletRatio.h"
#include "Tuplet.h"
#include "BeamFactory.h"
#include "Duration.h"
#include "Measure.h"
#include "Note.h"
#include "MxTupletSpec.h"
#include "TempoMark.h"
#include "Chord.h"
#include "Note.h"
#include "Rest.h"
#include "Measure.h"
#include "Item.h"
#include "Temporal.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

    TEST( ExceptionPrevention00, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;

        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto notes = getItems<Note>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        const auto rests = getItems<Rest>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        const auto chords = getItems<Chord>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        const auto tempos = getItems<TempoMark>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        const auto tuplets = getItems<TupletIntf>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);

        ItemRefVec selection;
        for (int i = 0; i < 6; ++i) {
            selection.emplace_back(notes.at(i));
        }

        for (int i = 0; i < 2; ++i) {
            selection.emplace_back(rests.at(i));
        }

        selection.emplace_back(chords.front());
        selection.emplace_back(tempos.front());
        selection.emplace_back(tuplets.front());

        auto deleteCmd = score->getCommandFactory().deleteItemsCommand(selection);
        deleteCmd->execute();
        deleteCmd->undo();
        auto xml = saveAnyScore(score);

        CHECK(xml.size() > 0);
    }
    T_END;

    TEST( M0_T0_H0_Temporal_00, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 0;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsChord() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_EQUAL( 3, temporal.as<Chord>().getNotes().size() );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 5, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 4, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 5, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 4, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_Temporal_01, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 1;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::G == temporal.getPitch().step );
        
        TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 5, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 4, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 5, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 4, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_Temporal_17, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 17;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsRest() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_Temporal_19, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 19;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsChord() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_EQUAL( 2, temporal.as<Chord>().getNotes().size() );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_Temporal_20, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 20;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::C == temporal.getPitch().step );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_Temporal_21, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 21;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::E == temporal.getPitch().step );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M3_T0_H0_Temporal_00, Bombe )
    {
        int measureIndex = 3;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 0;
        
        using namespace komp;
        const auto score = loadBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::C == temporal.getPitch().step );
        CHECK( temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 7, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 4, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 7, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 4, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_RT_Temporal_00, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 0;
        
        using namespace komp;
        const auto score = roundTripBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsChord() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_EQUAL( 3, temporal.as<Chord>().getNotes().size() );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 5, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 4, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 5, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 4, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_RT_Temporal_01, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 1;
        
        using namespace komp;
        const auto score = roundTripBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        CHECK( temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::G == temporal.getPitch().step );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 5, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 4, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 5, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 4, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_RT_Temporal_19, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 19;
        
        using namespace komp;
        const auto score = roundTripBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsChord() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK_EQUAL( 2, temporal.as<Chord>().getNotes().size() );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_RT_Temporal_20, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 20;
        
        using namespace komp;
        const auto score = roundTripBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);

        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::C == temporal.getPitch().step );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( !temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( M0_T0_H0_RT_Temporal_21, Bombe )
    {
        int measureIndex = 0;
        int trackIndex = 0;
        int handIndex = 0;
        int voiceIndex = 0;
        int temporalIndex = 21;
        
        using namespace komp;
        const auto score = roundTripBombe();
        const auto& scoreRef = *score.get();
        const auto temporals = getItems<Temporal>(scoreRef, measureIndex, trackIndex, handIndex, voiceIndex);
        Temporal& temporal = temporals.at(temporalIndex);
        
        CHECK( temporal.getIsNote() );
        CHECK_EQUAL( 1, temporal.getTuplets().size() );
        CHECK( PitchStep::E == temporal.getPitch().step );
        CHECK( !temporal.getIsTupletStart() );
        CHECK( temporal.getIsTupletStop() );
        
        const TupletIntf& tuplet = temporal.getTuplets().at(0);
        CHECK_EQUAL( 3, tuplet.getTupletRatio().getActual().getCount() );
        CHECK_EQUAL( 2, tuplet.getTupletRatio().getNormal().getCount() );
        const auto duration = temporal.getDuration();
        CHECK_EQUAL( 1, duration.getTupletRatios().size() );
        CHECK_EQUAL( 3, duration.getTupletRatios().at(0).getActual().getCount() );
        CHECK_EQUAL( 2, duration.getTupletRatios().at(0).getNormal().getCount() );
    }
    T_END;
    
    TEST( Fuzz01, Bombe )
    {
        const int deleteSeed = 1;
        BombeFuzz fuzzer;
        fuzzer.initializeScore();
        fuzzer.runDeleteItemsCommand(*fuzzer.score.get(), deleteSeed);
        
        const auto originalItemCount = fuzzer.mementos.size();
        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size();
        CHECK( originalItemCount > diminishedItemCount );
        
        fuzzer.deleteItemsCommand->undo();
        
        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
        fuzzer.deleteItemsCommand->redo();
        
        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
        fuzzer.deleteItemsCommand->undo();
        
        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
    }
    T_END;
    
    TEST( Fuzz02, Bombe )
    {
        const int deleteSeed = 2;
        BombeFuzz fuzzer;
        fuzzer.initializeScore();
        fuzzer.runDeleteItemsCommand(*fuzzer.score.get(), deleteSeed);
        
        const auto originalItemCount = fuzzer.mementos.size();
        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size();
        CHECK( originalItemCount > diminishedItemCount );
        
        fuzzer.deleteItemsCommand->undo();
        
        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
        fuzzer.deleteItemsCommand->redo();
        
        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
        fuzzer.deleteItemsCommand->undo();
        
        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
    }
    T_END;

//    TEST( Fuzz10, Bombe )
//    {
//        const int deleteSeed = 10;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz11, Bombe )
//    {
//        const int deleteSeed = 11;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz12, Bombe )
//    {
//        const int deleteSeed = 12;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz13, Bombe )
//    {
//        const int deleteSeed = 13;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz14, Bombe )
//    {
//        const int deleteSeed = 14;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz15, Bombe )
//    {
//        const int deleteSeed = 15;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz16, Bombe )
//    {
//        const int deleteSeed = 16;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz17, Bombe )
//    {
//        const int deleteSeed = 17;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz18, Bombe )
//    {
//        const int deleteSeed = 18;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz19, Bombe )
//    {
//        const int deleteSeed = 19;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz20, Bombe )
//    {
//        const int deleteSeed = 20;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz21, Bombe )
//    {
//        const int deleteSeed = 21;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz22, Bombe )
//    {
//        const int deleteSeed = 22;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz23, Bombe )
//    {
//        const int deleteSeed = 23;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz24, Bombe )
//    {
//        const int deleteSeed = 24;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz25, Bombe )
//    {
//        const int deleteSeed = 25;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz26, Bombe )
//    {
//        const int deleteSeed = 26;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz27, Bombe )
//    {
//        const int deleteSeed = 27;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz28, Bombe )
//    {
//        const int deleteSeed = 28;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz29, Bombe )
//    {
//        const int deleteSeed = 29;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz30, Bombe )
//    {
//        const int deleteSeed = 30;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz31, Bombe )
//    {
//        const int deleteSeed = 31;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz32, Bombe )
//    {
//        const int deleteSeed = 32;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz33, Bombe )
//    {
//        const int deleteSeed = 33;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz34, Bombe )
//    {
//        const int deleteSeed = 34;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz35, Bombe )
//    {
//        const int deleteSeed = 35;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz36, Bombe )
//    {
//        const int deleteSeed = 36;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz37, Bombe )
//    {
//        const int deleteSeed = 37;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz38, Bombe )
//    {
//        const int deleteSeed = 38;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz39, Bombe )
//    {
//        const int deleteSeed = 39;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz40, Bombe )
//    {
//        const int deleteSeed = 40;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz41, Bombe )
//    {
//        const int deleteSeed = 41;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz42, Bombe )
//    {
//        const int deleteSeed = 42;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz43, Bombe )
//    {
//        const int deleteSeed = 43;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz44, Bombe )
//    {
//        const int deleteSeed = 44;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz45, Bombe )
//    {
//        const int deleteSeed = 45;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz46, Bombe )
//    {
//        const int deleteSeed = 46;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz47, Bombe )
//    {
//        const int deleteSeed = 47;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz48, Bombe )
//    {
//        const int deleteSeed = 48;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz49, Bombe )
//    {
//        const int deleteSeed = 49;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz50, Bombe )
//    {
//        const int deleteSeed = 50;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz51, Bombe )
//    {
//        const int deleteSeed = 51;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz52, Bombe )
//    {
//        const int deleteSeed = 52;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz53, Bombe )
//    {
//        const int deleteSeed = 53;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz54, Bombe )
//    {
//        const int deleteSeed = 54;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz55, Bombe )
//    {
//        const int deleteSeed = 55;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz56, Bombe )
//    {
//        const int deleteSeed = 56;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz57, Bombe )
//    {
//        const int deleteSeed = 57;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz58, Bombe )
//    {
//        const int deleteSeed = 58;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz59, Bombe )
//    {
//        const int deleteSeed = 59;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz60, Bombe )
//    {
//        const int deleteSeed = 60;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz61, Bombe )
//    {
//        const int deleteSeed = 61;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz62, Bombe )
//    {
//        const int deleteSeed = 62;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz63, Bombe )
//    {
//        const int deleteSeed = 63;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz64, Bombe )
//    {
//        const int deleteSeed = 64;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz65, Bombe )
//    {
//        const int deleteSeed = 65;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz66, Bombe )
//    {
//        const int deleteSeed = 66;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz67, Bombe )
//    {
//        const int deleteSeed = 67;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz68, Bombe )
//    {
//        const int deleteSeed = 68;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz69, Bombe )
//    {
//        const int deleteSeed = 69;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz70, Bombe )
//    {
//        const int deleteSeed = 70;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz71, Bombe )
//    {
//        const int deleteSeed = 71;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz72, Bombe )
//    {
//        const int deleteSeed = 72;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz73, Bombe )
//    {
//        const int deleteSeed = 73;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz74, Bombe )
//    {
//        const int deleteSeed = 74;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz75, Bombe )
//    {
//        const int deleteSeed = 75;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz76, Bombe )
//    {
//        const int deleteSeed = 76;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz77, Bombe )
//    {
//        const int deleteSeed = 77;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz78, Bombe )
//    {
//        const int deleteSeed = 78;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz79, Bombe )
//    {
//        const int deleteSeed = 79;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz80, Bombe )
//    {
//        const int deleteSeed = 80;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz81, Bombe )
//    {
//        const int deleteSeed = 81;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz82, Bombe )
//    {
//        const int deleteSeed = 82;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz83, Bombe )
//    {
//        const int deleteSeed = 83;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz84, Bombe )
//    {
//        const int deleteSeed = 84;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz85, Bombe )
//    {
//        const int deleteSeed = 85;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz86, Bombe )
//    {
//        const int deleteSeed = 86;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz87, Bombe )
//    {
//        const int deleteSeed = 87;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz88, Bombe )
//    {
//        const int deleteSeed = 88;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz89, Bombe )
//    {
//        const int deleteSeed = 89;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz90, Bombe )
//    {
//        const int deleteSeed = 90;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz91, Bombe )
//    {
//        const int deleteSeed = 91;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz92, Bombe )
//    {
//        const int deleteSeed = 92;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz93, Bombe )
//    {
//        const int deleteSeed = 93;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz94, Bombe )
//    {
//        const int deleteSeed = 94;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz95, Bombe )
//    {
//        const int deleteSeed = 95;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz96, Bombe )
//    {
//        const int deleteSeed = 96;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz97, Bombe )
//    {
//        const int deleteSeed = 97;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz98, Bombe )
//    {
//        const int deleteSeed = 98;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
//    
//    TEST( Fuzz99, Bombe )
//    {
//        const int deleteSeed = 99;
//        BombeFuzz fuzzer;
//        fuzzer.initializeScore();
//        fuzzer.runDeleteItemsCommand(deleteSeed);
//        
//        const auto originalItemCount = fuzzer.mementos.size();
//        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size();
//        CHECK( originalItemCount > diminishedItemCount );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->redo();
//        
//        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//        fuzzer.deleteItemsCommand->undo();
//        
//        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score).size() );
//        
//    }
//    T_END;
    
    TEST( Fuzz100, Bombe )
    {
        const int deleteSeed = 100;
        BombeFuzz fuzzer;
        fuzzer.initializeScore();
        fuzzer.runDeleteItemsCommand(*fuzzer.score.get(), deleteSeed);
        
        const auto originalItemCount = fuzzer.mementos.size();
        const auto diminishedItemCount = BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size();
        CHECK( originalItemCount > diminishedItemCount );
        
        fuzzer.deleteItemsCommand->undo();
        
        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
        fuzzer.deleteItemsCommand->redo();
        
        CHECK_EQUAL( diminishedItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
        fuzzer.deleteItemsCommand->undo();
        
        CHECK_EQUAL( originalItemCount, BombeFuzz::retreiveAllItemsFromScore(fuzzer.score.get()).size() );
        
    }
    T_END;

#endif
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
