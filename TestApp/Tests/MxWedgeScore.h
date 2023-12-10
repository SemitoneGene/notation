// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <sstream>

// Komp
#include "Forward.h"
#include "Wedge.h"
#include "PlacementType.h"
#include "ItemRegistry.h"
#include "Throw.h"
#include "FileMediatorFactory.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"


namespace komp
{
    using namespace komp;
    
    class MxWedgeScore
    {
    public:
        static const int slot = -7;
        static const int startTime = 341;
        static const int endTime = 211;
        
        MxWedgeScore();
        MxWedgeScore(ScoreUPtr inScore);
        MxWedgeScore(const std::string& inXml);
        
        const ScoreUPtr& getScore() const;
        Wedge& getWedge() const;
        
        std::string getXml() const;
        MxWedgeScore roundTrip();
        
    private:
        ScoreUPtr mScore;
    };
    
    inline void insertNote(TestNoteFactory& factory,
                           komp::PitchStep inStep,
                           int inOctave)
    {
        factory.pitch.step = inStep;
        factory.pitch.octave = inOctave;
        factory.insertNote();
        factory.advancePosition();
    }
    
    inline komp::ScoreUPtrVec getMxWedgeScore()
    {
        auto score = getBasicScore(2);
        
        TestNoteFactory factory{*score.get()};
        insertNote(factory, PitchStep::A, 3);
        insertNote(factory, PitchStep::G, 3);
        insertNote(factory, PitchStep::E, 4);
        insertNote(factory, PitchStep::F, 4);
        insertNote(factory, PitchStep::D, 4);
        insertNote(factory, PitchStep::E, 4);
        insertNote(factory, PitchStep::A, 4);
        insertNote(factory, PitchStep::B, 4);
        
        SpannerParams params;
        params.hand = 0;
        params.startMeasure = score->getMeasure(0).get();
        params.startTime = MxWedgeScore::startTime;
        params.endMeasure = score->getMeasure(1).get();
        params.endTime = MxWedgeScore::endTime;
        
        const auto& track = score->getTracks().front();
        
        const MusicLocation loc(*params.startMeasure, *track.get());
        auto& reg = score->getItemRegistry();
        auto& wedge = reg.createItem<Wedge>(loc,
                                            WedgeParams{
            params,
            WedgeType::DIMINUENDO,
            PlacementType::AUTOMATIC
        });
        wedge.setIsQuiescent(false);
        score->getMeasure(0)->insertItem(wedge, wedge.getTime());
        wedge.setSlot(MxWedgeScore::slot);
        
        score->layout();
        
        ScoreUPtrVec scores;
        scores.emplace_back(std::move(score));
        
        // TODO: FIXME
        //MxWedgeScore object{std::move(score)};
        //scores.push_back(object.roundTrip().getScore());
        
        return scores;
    }
    

    inline MxWedgeScore::MxWedgeScore()
    : mScore{nullptr}
    {
        auto scores = getMxWedgeScore();
        mScore = std::move(scores.front());
        KOMP_ASSERT(mScore);
    }
    
    
    inline MxWedgeScore::MxWedgeScore(ScoreUPtr inScore)
    : mScore{std::move(inScore)}
    {
        KOMP_ASSERT(mScore);
    }


    inline MxWedgeScore::MxWedgeScore(const std::string& inXml)
    : mScore{nullptr}
    {
        auto mediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::istringstream iss{inXml};
        mScore = mediator->loadStream(iss);
    }
    
    
    inline const ScoreUPtr& MxWedgeScore::getScore() const
    {
        return mScore;
    }
    
    
    inline Wedge& MxWedgeScore::getWedge() const
    {
        auto& measure = getScore()->getMeasure(0);
        auto& track = getScore()->getTracks().front();
        auto items = measure->items<Wedge>(*track.get());
        KOMP_ASSERT(items.size() == 1);
        return items.front().get().as<Wedge>();
    }
    
    
    inline std::string MxWedgeScore::getXml() const
    {
        auto mediator = FileMediatorFactory::create(FileMediatorType::MusicXML);
        std::stringstream ss;
        FileSaveParams fparams{*getScore().get()};
        mediator->saveStream(fparams, ss);
        return ss.str();
    }
    
    
    inline MxWedgeScore MxWedgeScore::roundTrip()
    {
        const std::string xml = getXml();
        return MxWedgeScore{xml};
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
