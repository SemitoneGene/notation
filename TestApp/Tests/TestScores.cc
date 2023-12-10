// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TestScores.h"

// KompTest
#include "AttachedItemScore.h"
#include "BeamSaveScores.h"
#include "BeamTestScores.h"
#include "BracketTest.h"
#include "ChordScore.h"
#include "ChordSymbolScores.h"
#include "CurveScores.h"
#include "FermataTestScore.h"
#include "MxStemScores.h"
#include "MxWedgeScore.h"
#include "NoteSpacerScores.h"
#include "TupletScores.h"

namespace komp
{
    TestScores::TestScores()
    : mMap{}
    {
        mMap.emplace("AttachedItemTest", &getAttachedItemScores);
        mMap.emplace("BeamSaveTest", &getBeamSaveScores);
        mMap.emplace("BeamTest", &getBeamTestScores);
        mMap.emplace("BracketScores", &getBracketScores);
        mMap.emplace("ChordScores", &getChordScores);
        mMap.emplace("ChordSymbolScores", &getChordSymbolScores);
        mMap.emplace("CurveScores", &getCurveScores);
        mMap.emplace("FermataTest", &getFermataTestScores);
        mMap.emplace("MxStemScores", &getMxStemScores);
        mMap.emplace("MxWedgeScore", &getMxWedgeScore);
        mMap.emplace("NoteSpacerScores", &getNoteSpacerScores);
        mMap.emplace("TupletScores", &getTupletScores);
    }
    
    komp::ScoreUPtrVec
    TestScores::create(const std::string& name)
    {
        const auto& scoreMap = TestScores::getMap();
        auto pairIter = scoreMap.find(name);
        
        if (pairIter == scoreMap.cend()) {
            return komp::ScoreUPtrVec{};
        }
        
        auto scoreFunction = pairIter->second;
        return scoreFunction();
    }
    
    const TestScores&
    TestScores::getInstance()
    {
        static TestScores theInstance;
        return theInstance;
    }
    
    const TestScores::ScoreFunctionMap&
    TestScores::getMap()
    {
        return getInstance().mMap;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
