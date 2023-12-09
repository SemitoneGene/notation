// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxTieCollector.h"

// Local
#include "Throw.h"
#include "Tie.h"
#include "Temporal.h"
#include "Score.h"
#include "Measure.h"

namespace komp
{
    MxTieCollector::MxTieCollector()
    : mOpenTies{}
    , mClosedTies{}
    {

    }

    void
    MxTieCollector::startTie(const mx::api::NoteData& inNoteData,
                             Temporal& inTemporal)
    {
        MxTieSpec spec{};
        spec.mxFirstNote = std::move(inNoteData);
        spec.firstTemporal = &inTemporal;
        const auto& loc = inTemporal.getLocation();
        auto& measure = loc.getMeasure();
        spec.firstMeasure = &measure;
        spec.track = &loc.getTrack();
        spec.voice = loc.getVoice();
        spec.hand = loc.getHand();
        mOpenTies.emplace_back(std::move(spec));
    }


    void
    MxTieCollector::finishTie(const mx::api::NoteData& inNoteData,
                              Temporal& inTemporal)
    {
        if (mOpenTies.empty()) {
            LOG_F(WARNING, "attempted to stop a tie when no ties were active");
            return;
        }

        const auto& loc = inTemporal.getLocation();
        const auto& track = loc.getTrack();
        
        auto iter = mOpenTies.begin();
        const auto e = mOpenTies.end();

        // find the best possible tie (matching track, hand, voice, and pitch)
        for (; iter != e; ++iter) {
            auto& spec = *iter;
            if (spec.track != &track) {
                continue;
            }

            if (spec.hand != loc.getHand()) {
                continue;
            }

            const auto startPitch = spec.mxFirstNote.pitchData;
            const auto endPitch = inNoteData.pitchData;

            if (startPitch.octave != endPitch.octave) {
                continue;
            }

            if (startPitch.step == endPitch.step &&
                startPitch.alter == endPitch.alter) {
                spec.mxLastNote = inNoteData;
                spec.lastTemporal = &inTemporal;
                mClosedTies.push_back(*iter);
                mOpenTies.erase(iter);
                return;
            }
        }


        LOG_F(INFO, "tie start and stop could not be matched in the same track, hand, voice and slot");

        iter = mOpenTies.begin();
        for (; iter != e; ++iter) {
            auto& spec = *iter;
            if (spec.track != &track) {
                continue;
            }

            if (spec.hand != inTemporal.getLocation().getHand()) {
                continue;
            }

            const auto startMidi = spec.firstTemporal->getPitch().getMidiNote();
            const auto endMidi = inTemporal.getPitch().getMidiNote();

            if (startMidi == endMidi) {
                LOG_F(INFO, "enharmonically equivalent notes found to tie together");
                mClosedTies.push_back(*iter);
                mOpenTies.erase(iter);
                return;
            }
        }

        LOG_F(INFO, "tie could not be completed");
    }


    MxTieVec
    MxTieCollector::getTieSpecs() const
    {
        return mClosedTies;
    }


    void
    MxTieCollector::insertTies(Score& inScore)
    {
        warnIfOpenTiesExist();

        auto iter = mClosedTies.begin();
        const auto end = mClosedTies.end();

        for ( ; iter != end; ++iter) {

            auto& spec = *iter;
            const CurveParams params{*spec.firstTemporal, *spec.lastTemporal};
            auto& registry = inScore.getItemRegistry();
            
            auto& tie = registry.createItem<Tie>(spec.firstTemporal->getLocation(),
                                                 params);
            tie.setIsQuiescent(false);
            spec.firstMeasure->insertItem(tie);
        }
    }


    void
    MxTieCollector::warnIfOpenTiesExist() const
    {
        if (mOpenTies.size() > 0) {
            LOG_F(WARNING, "unfinished ties still exist");
        }
    }


    bool
    MxTieCollector::validateAndWarn(const MxTieSpec& inSpec) const
    {
//        if (!validateAndWarnNulls(inSpec)) {
//            return false;
//        }
//
//        const auto actualCount = inSpec.mxTieStart.actualNumber;
//        const auto actualType = parseActualType(inSpec);
//        const auto actualDots = std::max(inSpec.mxTieStart.actualDots, 0);
//        const auto normalCount = inSpec.mxTieStart.normalNumber;
//        const auto normalType = parseNormalType(inSpec, actualType);
//        const auto normalDots = std::max(inSpec.mxTieStart.normalDots, 0);
//        DurationCount actual{actualCount, actualType, actualDots};
//        DurationCount normal{normalCount, normalType, normalDots};
//        TieRatio ratio{std::move(actual), std::move(normal)};
//        outParams.tieRatio = std::move(ratio);
//        populateTemporals(inSpec, outParams);
//
//        if (!outParams.getIsDurationValid()) {
//            LOG_F(INFO, "tie specification and temporal duration total do not match");
//            return false;
//        }

        return true;
    }


    bool
    MxTieCollector::validateAndWarnNulls(const MxTieSpec& inSpec) const
    {
        if (!inSpec.firstMeasure) {
            LOG_F(ERROR, "null measure");
            return false;
        }

        if (!inSpec.track) {
            LOG_F(ERROR, "null track");
            return false;
        }

        if (!inSpec.firstTemporal) {
            LOG_F(ERROR, "null temporal");
            return false;
        }

        if (!inSpec.lastTemporal) {
            LOG_F(ERROR, "null temporal");
            return false;
        }

        return true;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
