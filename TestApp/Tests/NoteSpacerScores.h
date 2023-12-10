// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Forward.h"
#include "BeamFactory.h"
#include "Tuplet.h"
#include "Rest.h"
#include "Track.h"

// Test Support
#include "BasicScore.h"
#include "TestNoteFactory.h"

namespace komp
{
    using namespace komp;

    inline ScoreUPtr makeNoteSpacerScore1(bool doRunLayout = true)
    {
        auto score = getBasicScore(3, 3);
        auto& reg = score->getItemRegistry();

        TestNoteFactory factory{*score.get()};
        NoteRefVec notes;

        const std::vector<TemporalType> dur =
        {
            TemporalType::EIGHTH,
            TemporalType::SIXTEENTH,
            TemporalType::QUARTER,
            TemporalType::SIXTEENTH,
            TemporalType::EIGHTH,
            TemporalType::SIXTEENTH,
            TemporalType::SIXTEENTH
        };

        const std::vector<int> dot =
        {
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
        };

        const std::vector<AccidentalType> acc =
        {
            AccidentalType::NONE,
            AccidentalType::SHARP,
            AccidentalType::SHARP,
            AccidentalType::FLAT,
            AccidentalType::NONE,
            AccidentalType::NATURAL,
            AccidentalType::NONE,
            AccidentalType::SHARP,
            AccidentalType::FLAT,
            AccidentalType::DOUBLE_FLAT,
            AccidentalType::NATURAL,
            AccidentalType::NONE,
            AccidentalType::NONE,
            AccidentalType::DOUBLE_SHARP,
            AccidentalType::NONE,
        };

        const std::vector<int> count =
        {
            1, // a single note (not a chord)
            2, // a chord with two members
            1,
            1,
            1,
            0, // a rest
            1,
            1,
            0,
            3,
            0,
            1,
            0,
            1,
            1,
            0,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
        };



        const std::vector<PitchStep> step =
        {
            PitchStep::F,
            PitchStep::G,
            PitchStep::B,
            PitchStep::C,
            PitchStep::E,
            PitchStep::F,
            PitchStep::A,
            PitchStep::G,
            PitchStep::B,
            PitchStep::F,
            PitchStep::C,
            PitchStep::D,
            PitchStep::G,
            PitchStep::B,
            PitchStep::D,
            PitchStep::E,
            PitchStep::D,
            PitchStep::F,
            PitchStep::C,
            PitchStep::A,
            PitchStep::F,
            PitchStep::G,
        };

        const std::vector<int> octave =
        {
            4,
            4,
            4,
            4,
            5,
            5,
            5,
        };

        ////////////////////////////////////////////////////////////////////////////////////////
        factory.temporalType = TemporalType::EIGHTH;
        factory.pitch.step = PitchStep::G;
        factory.pitch.octave = 5;
        factory.autoAdvanceMeasure = true;
        factory.autoAddMeasures = true;
        factory.autoReconcilePitchAfterInsert = true;

        const auto tbegin = score->getTracks().cbegin();
        const auto tend = score->getTracks().cend();
        size_t counter = 0;

        for (auto t = tbegin; t != tend; ++t) {
            auto& track = *t;
            factory.trackIndex = score->getTrackIndex(*track.get());
            factory.measureIndex = 0;
            factory.time = 0.0;
            
            for (int n = 0; n < 100; ++n, ++counter) {
                factory.pitch.step = step.at(counter % step.size());
                factory.pitch.octave = octave.at(counter % octave.size());
                const auto accidental = acc.at(counter % acc.size());
                factory.pitch.alter = AccidentalInfo::getPitchAlter(accidental);
                factory.accidental = accidental;
                factory.temporalType = dur.at(counter % dur.size());
                factory.dots = dot.at(counter % dot.size());
                const int pitchCount = count.at(counter % count.size());
                const bool isRest = pitchCount == 0;
                const bool isChord = pitchCount > 1;


                if (isRest) {
                    auto& measure = score->getMeasure(factory.measureIndex);
                    const MusicLocation loc(*measure.get(), *track.get());
                    auto& rest = reg.createItem<Rest>(loc, factory.temporalType);
                    auto duration = rest.getDuration();
                    duration.setDots(factory.dots);
                    rest.setDuration(duration);
                    rest.setTime(factory.time);
                    if (factory.measureIndex > score->getMeasureCount() - 1) {
                        score->insertMeasure(static_cast<size_t>(factory.measureIndex));
                        score->layout();
                    }
                    rest.setIsQuiescent(false);
                    measure->insertItem(rest);

                } else if (isChord) {
                    NoteRefVec chordNotes;
                    for (int p = 0; p < pitchCount; ++p) {
                        chordNotes.emplace_back(factory.insertNote());
                        ++counter;
                        factory.pitch.step = step.at(counter % step.size());
                        factory.pitch.octave = octave.at(counter % octave.size());
                        const auto localAccidental = acc.at(counter % acc.size());
                        factory.pitch.alter = AccidentalInfo::getPitchAlter(accidental);
                        factory.accidental = localAccidental;
                    }
                    KOMP_ASSERT(chordNotes.size() > 1);
                    auto& measure = score->getMeasure(factory.measureIndex);
                    const MusicLocation loc(*measure.get(), *track.get());
                    auto& chord = reg.createItem<Chord>(loc, chordNotes);
                    if (factory.measureIndex > score->getMeasureCount() - 1) {
                        score->insertMeasure(static_cast<size_t>(factory.measureIndex));
                        score->layout();
                    }
                    chord.setIsQuiescent(false);
                    measure->insertItem(chord);
                } else {
                    factory.insertNote();
                }

                factory.advancePosition();
            }
        }

        if (doRunLayout) {
            score->layout();
        }

        return score;
    }

    inline ScoreUPtr makeNoteSpacerScore2(bool doRunLayout = true)
    {
        auto score = getBasicScore(1);
        return score;
    }

    inline ScoreUPtrVec getNoteSpacerScoresImpl(bool doRunLayout)
    {
        ScoreUPtrVec vec;
        vec.emplace_back(makeNoteSpacerScore1(doRunLayout));
        //vec.emplace_back(makeNoteSpacerScore2(doRunLayout));
        return vec;
    }

    inline ScoreUPtrVec getNoteSpacerScores()
    {
        return getNoteSpacerScoresImpl(true);
    }
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
