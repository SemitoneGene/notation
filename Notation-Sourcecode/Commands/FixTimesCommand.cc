// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FixTimesCommand.h"

// Local
#include "Score.h"
#include "Temporal.h"
#include "Track.h"

namespace komp
{
    FixTimesCommand::~FixTimesCommand() = default;


FixTimesCommand::FixTimesCommand(CommandFactory& inFactory,
                                 ItemRegistry& inRegistry,
                                 Measure& inMeasure)
    : komp::Command("Fix Times", inFactory, inRegistry)
    , mMeasure{inMeasure}
    {

    }
    
    
    CommandResult
    FixTimesCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        fixMeasure();
        return true;
    }
    
    
    void
    FixTimesCommand::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }
    
    
    void
    FixTimesCommand::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
    }


    void
    FixTimesCommand::fixMeasure()
    {
        const auto& score = mMeasure.getScore();
        for (const auto& track : score.getTracks()) {
            fixTrack(*track.get());
        }
    }


    void
    FixTimesCommand::fixTrack(Track& inTrack)
    {
        const auto handCount = static_cast<int>(inTrack.getHands());
        for (int hand = 0; hand < handCount; ++hand) {
            fixHand(inTrack, hand);
        }
    }


    void
    FixTimesCommand::fixHand(Track& inTrack, int inHand)
    {
        const auto voiceCount = static_cast<int>(MAX_HANDS);
        for (int voice = 0; voice < voiceCount; ++voice) {
            fixVoice(inTrack, inHand, voice);
        }
    }


    void
    FixTimesCommand::fixVoice(Track& inTrack,
                              int inHand,
                              int inVoice)
    {
        auto filter = [&](Item& inFilterItem) {
            
            const auto& loc = inFilterItem.getLocation();
            
            if (loc.getVoice() != inVoice) {
                return false;
            }

            if (loc.getHand() != inHand) {
                return false;
            }

            if (&loc.getTrack() != &inTrack) {
                return false;
            }

            if (!inFilterItem.is<Temporal>()) {
                return false;
            }

            const auto& inFilterTemporal = inFilterItem.as<Temporal>();
            if (inFilterTemporal.getIsChordMember()) {
                return false;
            }

            return true;
        };

        auto temporals = mMeasure.items<Temporal>(inTrack, filter);

        auto sortCompare = [&](Temporal& inLeft, Temporal& inRight) {

            if (inLeft.getX() < inRight.getX()) {
                return true;
            } else if (inLeft.getX() > inRight.getX()) {
                return false;
            }

            if (inLeft.getTime() < inRight.getTime()) {
                return true;
            } else if (inLeft.getTime() > inRight.getTime()) {
                return false;
            }

            if (inLeft.getItemID() < inRight.getItemID()) {
                return true;
            } else if (inLeft.getItemID() > inRight.getItemID()) {
                return false;
            }

            return &inLeft < &inRight;
        };

        std::ranges::sort(temporals, sortCompare);
        fixTemporals(temporals);
        mMeasure.sort();
    }


    void
    FixTimesCommand::fixTemporals(const TemporalRefVec& inTemporals)
    {
        auto iter = std::cbegin(inTemporals);
        const auto end = std::cend(inTemporals);
        int currentClock = 0;
        int durationClocks = 0;

        auto updateClock = [&]() {
            currentClock += durationClocks;
        };

        auto updateDuration = [&](decltype(iter) inIter) {
            if (inIter == end) {
                return;
            }
            durationClocks = (*inIter).get().getDuration().getRoundedClocks();
        };

        for ( ; iter != end; updateDuration(iter), updateClock(), ++iter) {

            Temporal& temporal = *iter;
            const auto time = temporal.getRoundedTime();

            if (time == currentClock) {
                continue;
            }

            if (temporal.getIsChord()) {
                const auto& chord = temporal.as<Chord>();
                auto memento = chord.getChordMemento();
                temporal.setTime(static_cast<double>(currentClock));
                auto stateCmd = mFactory.chordStateCommand(std::move(memento.noteMementos),
                                                           std::move(memento.chordMemento),
                                                           chord.getMemento());
                addSubCommand(std::move(stateCmd));
            } else {
                auto memento = temporal.getMemento();
                temporal.setTime(static_cast<double>(currentClock));
                auto stateCmd = mFactory.itemStateCommand(std::move(memento),
                                                          temporal.getMemento());
                addSubCommand(std::move(stateCmd));
            }
        }
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
