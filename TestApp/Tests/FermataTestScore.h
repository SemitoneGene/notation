// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{

inline void insertFermataNote(TestNoteFactory& factory,
                              komp::PitchStep inStep,
                              int inOctave)
{
    factory.pitch.step = inStep;
    factory.pitch.octave = inOctave;
    auto note = factory.insertNote();
    note.deducePitch();
    factory.advancePosition();
}


inline komp::ScoreUPtr
getFermataTestScore()
{
    using namespace komp;
    
    auto score = getBasicScore();
            
    TestNoteFactory factory{*score.get()};
    factory.mark = MarkType::FERMATA;
    insertFermataNote(factory, PitchStep::F, 4);
    
    factory.mark = MarkType::FERMATA;
    insertFermataNote(factory, PitchStep::A, 3);

    factory.mark = MarkType::FERMATA_SHORT_ABOVE;
    insertFermataNote(factory, PitchStep::C, 4);

    factory.mark = MarkType::FERMATA_SHORT_BELOW;
    insertFermataNote(factory, PitchStep::E, 6);
    
    // measures[1]
    factory.mark = MarkType::FERMATA_LONG_ABOVE;
    factory.placement = PlacementType::ABOVE_ITEM;
    insertFermataNote(factory, PitchStep::D, 5);

    factory.mark = MarkType::FERMATA_LONG_BELOW;
    factory.placement = PlacementType::BELOW_ITEM;
    insertFermataNote(factory, PitchStep::B, 4);

    score->layout();
    return score;
}

inline komp::ScoreUPtrVec getFermataTestScores()
{
    komp::ScoreUPtrVec outScores;
    outScores.emplace_back(getFermataTestScore());
    return outScores;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
