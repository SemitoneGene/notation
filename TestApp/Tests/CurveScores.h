// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "Chord.h"
#include "BeamFactory.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{
    using namespace komp;

    inline ScoreUPtr makeCurveTest1()
    {
        auto score = getBasicScore(1);

        TestNoteFactory factory{*score.get()};
        NoteRefVec chordNotes;

        ////////////////////////////////////////////////////////////////////////////////////////
        factory.pitch.step = PitchStep::G;
        factory.pitch.octave = 5;
        factory.temporalType = TemporalType::QUARTER;
        auto& firstNote = factory.insertNote();
        factory.advancePosition();
        auto& secondNote = factory.insertNote();
        factory.advancePosition();

        auto& cfac = score->getCommandFactory();
        auto command = cfac.insertTieCommand(firstNote, secondNote);
        command->execute();

        score->layout();

        return score;
    }

    inline ScoreUPtr makeCurveTest2()
    {
        auto score = getBasicScore(1);
        return score;
    }

    inline ScoreUPtr getCurveScore()
    {
        return makeCurveTest1();
    }

    inline komp::ScoreUPtrVec getCurveScores()
    {
        komp::ScoreUPtrVec outScores;
        outScores.emplace_back(getCurveScore());
        return outScores;
    }

};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
