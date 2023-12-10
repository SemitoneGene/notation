// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// System
#include <sstream>

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "FileMediatorFactory.h"
#include "BeamFactory.h"
#include "BeamIntf.h"

// Test Support
#include "MxStemScores.h"
#include "TestHelpers.h"
#include "TestFileRepository.h"

namespace komp
{
#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS
    TEST( XmlRoundTrip, MxStemTest )
    {
        auto beforeScore = makeMxStemTestQuartersScore();
        
        // Round Trip through File
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        FileSaveParams fparams{*beforeScore.get()};
        fileMediator->saveStream(fparams, oss);
        std::stringstream iss{oss.str()};
        auto afterScore = fileMediator->loadStream(iss);
        
        auto beforeNotes = beforeScore->getMeasure(0)->items<Note>(*beforeScore->getTracks().front().get());
        auto afterNotes = afterScore->getMeasure(0)->items<Note>(*afterScore->getTracks().front().get());
        
        auto beforeIter = beforeNotes.cbegin();
        auto afterIter = afterNotes.cbegin();
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemUp() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemUp() );

        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemUp() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemUp() );

        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemDown() );

        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemDown() );
    }
    T_END;
    
    TEST( XmlRoundTripWithBeams, MxStemTest )
    {
        auto beforeScore = makeMxStemTestBeamScore();
        
        // Round Trip through File
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        FileSaveParams fparams{*beforeScore.get()};
        fileMediator->saveStream(fparams, oss);
        std::stringstream iss{oss.str()};
        auto afterScore = fileMediator->loadStream(iss);
        
        auto beforeNotes = beforeScore->getMeasure(0)->items<Note>(*beforeScore->getTracks().front().get());
        auto afterNotes = afterScore->getMeasure(0)->items<Note>(*afterScore->getTracks().front().get());
        
        auto beforeIter = beforeNotes.cbegin();
        auto afterIter = afterNotes.cbegin();
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemUp() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemUp() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemUp() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemUp() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemUp() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemUp() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemUp() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemUp() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemDown() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemDown() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemDown() );
        
        CHECK( (*(beforeIter++)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter++)).get().getStemPolicy().getIsStemDown() );
    }
    T_END;

    TEST( StemPolicy, MxStemTest )
    {
        auto beforeScore = getBasicScore(1, 1);
        TestNoteFactory f{*beforeScore.get()};
        f.temporalType = TemporalType::QUARTER;
        f.pitch.step = PitchStep::C;
        f.pitch.octave = 4;
        f.insertNote().setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        f.advancePosition();
        f.insertNote().setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::SPECIFIED});
        f.advancePosition();


        // Round Trip through File
        auto fileMediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::ostringstream oss;
        FileSaveParams fparams{*beforeScore.get()};
        fileMediator->saveStream(fparams, oss);
        std::stringstream iss{oss.str()};
        auto afterScore = fileMediator->loadStream(iss);

        auto beforeNotes = beforeScore->getMeasure(0)->items<Note>(*beforeScore->getTracks().front().get());
        auto afterNotes = afterScore->getMeasure(0)->items<Note>(*afterScore->getTracks().front().get());

        auto beforeIter = beforeNotes.cbegin();
        auto afterIter = afterNotes.cbegin();

        CHECK( (*(beforeIter)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(beforeIter)).get().getStemPolicy().getIsAutomatic() );
        CHECK( (*(afterIter)).get().getStemPolicy().getIsAutomatic() );
        ++beforeIter;
        ++afterIter;

        CHECK( (*(beforeIter)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(afterIter)).get().getStemPolicy().getIsStemDown() );
        CHECK( (*(beforeIter)).get().getStemPolicy().getIsSpecified() );
        CHECK( (*(afterIter)).get().getStemPolicy().getIsSpecified() );
    }
    T_END;

    TEST( LockedBeamStems1, MxStemTest )
    {
        const int measureIndex = 1;
        const int trackIndex = 0;
        const int handIndex = 0;
        const int voiceIndex = 0;
        const int beamIndex = 0;

        auto score = TestFileRepository::loadScore("Beam Stem Locking.xml");
        auto beams = getItems<BeamIntf>(*score.get(), measureIndex, trackIndex, handIndex, voiceIndex);
        BeamIntf& beam = beams.at(beamIndex);
        const auto stemPolicy = beam.getStemPolicy();
        CHECK( stemPolicy.direction == StemDirection::UP );
        CHECK( stemPolicy.policy == Policy::SPECIFIED );

        for (const auto& bn : beam.getBeamedNotes()) {
            const auto noteStemPolicy =  bn->getNote().getStemPolicy();
            CHECK( stemPolicy.direction == noteStemPolicy.direction );
            CHECK( stemPolicy.policy == noteStemPolicy.policy );
        }
    }
    T_END;

    TEST( LockedBeamStems2, MxStemTest )
    {
        const int measureIndex = 2;
        const int trackIndex = 0;
        const int handIndex = 0;
        const int voiceIndex = 0;
        const int beamIndex = 0;

        auto score = TestFileRepository::loadScore("Beam Stem Locking.xml");
        auto beams = getItems<BeamIntf>(*score.get(), measureIndex, trackIndex, handIndex, voiceIndex);
        BeamIntf& beam = beams.at(beamIndex);
        const auto stemPolicy = beam.getStemPolicy();
        CHECK( stemPolicy.direction == StemDirection::UP );
        CHECK( stemPolicy.policy == Policy::AUTOMATIC );

        for (const auto& bn : beam.getBeamedNotes()) {
            const auto noteStemPolicy =  bn->getNote().getStemPolicy();
            CHECK( stemPolicy.direction == noteStemPolicy.direction );
            CHECK( stemPolicy.policy == noteStemPolicy.policy );
        }
    }
    T_END;

    TEST( LockedBeamStems3, MxStemTest )
    {
        const int measureIndex = 2;
        const int trackIndex = 0;
        const int handIndex = 0;
        const int voiceIndex = 0;
        const int beamIndex = 1;

        auto score = TestFileRepository::loadScore("Beam Stem Locking.xml");
        auto beams = getItems<BeamIntf>(*score.get(), measureIndex, trackIndex, handIndex, voiceIndex);
        BeamIntf& beam = beams.at(beamIndex);
        const auto stemPolicy = beam.getStemPolicy();
        CHECK( stemPolicy.direction == StemDirection::DOWN );
        CHECK( stemPolicy.policy == Policy::SPECIFIED );

        for (const auto& bn : beam.getBeamedNotes()) {
            const auto noteStemPolicy =  bn->getNote().getStemPolicy();
            CHECK( stemPolicy.direction == noteStemPolicy.direction );
            CHECK( stemPolicy.policy == noteStemPolicy.policy );
        }
    }
    T_END;

#endif
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

