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

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"
#include "ChordSymbolRepository.h"

namespace komp
{
    using namespace komp;

    inline ScoreUPtr makeChordSymbolScore1()
    {
        auto score = getBasicScore(1);

        std::vector<ChordSymbolChar> roots =
        {
            ChordSymbolChar{ChordSymbolCharValue::ROOT_A, false},
            ChordSymbolChar{ChordSymbolCharValue::ROOT_B, false},
            ChordSymbolChar{ChordSymbolCharValue::ROOT_C, false},
            ChordSymbolChar{ChordSymbolCharValue::ROOT_D, false},
            ChordSymbolChar{ChordSymbolCharValue::ROOT_E, false},
            ChordSymbolChar{ChordSymbolCharValue::ROOT_F, false},
            ChordSymbolChar{ChordSymbolCharValue::ROOT_G, false},
        };

        std::vector<ChordSymbolChar> rootAlters =
        {
            ChordSymbolChar{ChordSymbolCharValue::ACCIDENTAL_NATURAL, false},
            ChordSymbolChar{ChordSymbolCharValue::ACCIDENTAL_FLAT, false},
            ChordSymbolChar{ChordSymbolCharValue::ACCIDENTAL_SHARP, false},
            ChordSymbolChar{ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT, false},
            ChordSymbolChar{ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP, false},
        };

        std::vector<ChordSymbolChar> qualities =
        {
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_AUG_SYMBOL, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_AUG_TEXT, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_DIM_SYMBOL, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_DIM_TEXT, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_LOWERCASE_M, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_SUS_TEXT, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_UPPERCASE_M, false},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_AUG_SYMBOL, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_AUG_TEXT, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_DIM_SYMBOL, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_DIM_TEXT, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_LOWERCASE_M, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_SUS_TEXT, true},
            ChordSymbolChar{ChordSymbolCharValue::QUALITY_UPPERCASE_M, true},
        };

        TestNoteFactory noteFactory{*score.get()};
        noteFactory.autoAddMeasures = true;
        noteFactory.temporalType = TemporalType::QUARTER;

        auto& commandFactory = score->getCommandFactory();
        
        for (const auto& root : roots) {
            for (const auto& rootAlter : rootAlters) {
                for (const auto& quality : qualities) {

                    const int isSus4 = quality.chordSymbolCharValue == ChordSymbolCharValue::QUALITY_SUS_TEXT;
                    const int numLoops = isSus4 ? 1 : 2;

                    for (int i = 0; i < numLoops; ++i) {


                        noteFactory.insertNote();
                        const auto time = noteFactory.time;
                        const auto measureIndex = noteFactory.measureIndex;
                        noteFactory.advancePosition();
                        noteFactory.insertNote();
                        noteFactory.advancePosition();

                        ChordSymbolString str{};
                        str.emplace_back(root);
                        str.emplace_back(rootAlter);
                        str.emplace_back(quality);

                        if (isSus4) {
                            str.emplace_back(ChordSymbolCharValue::NUMBER_4, true);
                        } else if (i > 0) {
                            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
                        }
                        
                        ChordSymbolParams params(*score->getTracks().front().get());
                        params.value.setString(str);
                        params.time = static_cast<int>(time);
                        const auto& measure = score->getMeasure(measureIndex);
                        const auto cmd = commandFactory.insertChordSymbolCommand(*measure.get(),
                                                                                 params);
                        cmd->execute();
                    }
                }
            }
        }

        score->layout();
        return score;
    }

    inline ScoreUPtr makeChordSymbolScore2()
    {
        auto score = getBasicScore(1);
        TestNoteFactory noteFactory{*score.get()};
        noteFactory.autoAddMeasures = true;
        const auto& random = ChordSymbolRepository::getRandomlyGenerated();
        const auto& logical = ChordSymbolRepository::getLogicallyGenerated();
        const auto& handpicked = ChordSymbolRepository::getHandpicked();
        auto& commandFactory = score->getCommandFactory();
        auto& track = score->getTracks().front();
        
        for (int i = 0; i < 1000; i += 101) {
            noteFactory.insertNote();
            const auto time = noteFactory.time;
            const auto measureIndex = noteFactory.measureIndex;
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();

            ChordSymbolParams params(*track.get());
            params.value = logical.at(i);
            params.time = static_cast<int>(time);
            const auto& measure = score->getMeasure(measureIndex);
            const auto cmd = commandFactory.insertChordSymbolCommand(*measure.get(),
                                                                     params);
            cmd->execute();
        }

        for (int i = 0; i < 1000; i += 203) {
            noteFactory.insertNote();
            const auto time = noteFactory.time;
            const auto measureIndex = noteFactory.measureIndex;
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();

            ChordSymbolParams params(*track.get());
            params.value = random.at(i);
            params.time = static_cast<int>(time);
            const auto& measure = score->getMeasure(measureIndex);
            const auto cmd = commandFactory.insertChordSymbolCommand(*measure.get(),
                                                                     params);
            cmd->execute();
        }

        for (const auto& value : handpicked) {
            noteFactory.insertNote();
            const auto time = noteFactory.time;
            const auto measureIndex = noteFactory.measureIndex;
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();
            noteFactory.insertNote();
            noteFactory.advancePosition();

            ChordSymbolParams params(*track.get());
            params.value = value;
            params.time = static_cast<int>(time);
            const auto& measure = score->getMeasure(measureIndex);
            const auto cmd = commandFactory.insertChordSymbolCommand(*measure.get(),
                                                                     params);
            cmd->execute();
        }

        score->layout();
        return score;
    }

    inline ScoreUPtrVec getChordSymbolScores()
    {
        ScoreUPtrVec vec;
        auto score1 = makeChordSymbolScore1();
        vec.emplace_back(std::move(score1));
        auto score2 = makeChordSymbolScore2();
        vec.emplace_back(std::move(score2));
        return vec;
    }
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
