// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InsertTupletCommand.h"

// Local
#include "BeamIntf.h"
#include "Chord.h"
#include "CommandFactory.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Score.h"
#include "Tuplet.h"
#include "Temporal.h"

namespace komp
{
    InsertTupletCommand::~InsertTupletCommand() = default;

InsertTupletCommand::InsertTupletCommand(CommandFactory& inFactory,
                                         ItemRegistry& inRegistry,
                                         Measure& inMeasure,
                                         const TupletParams& inParams)
    : komp::Command{"Insert Tuplet", inFactory, inRegistry}
    , mMeasure{inMeasure} 
    , mTupletRatio{inParams.tupletRatio}
    , mTemporals{inParams.getTemporals()}
    , mTupletID{initializeItemID()}
    {
        KOMP_ASSERT(inParams.getIsDurationValid());
        KOMP_ASSERT(!mTemporals.empty());
    }


    CommandResult
    InsertTupletCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        TupletParams params;
        params.tupletRatio = mTupletRatio;
        params.setTemporals(mTemporals);

        std::vector<std::pair<ItemUPtr, TemporalRef>> temporalMementos;
        std::vector<std::pair<ChordMemento, ChordRef>> chordMementos;

        for (Temporal& temporal : mTemporals) {
            if (temporal.getIsChord()) {
                auto& chord = temporal.as<Chord>();
                chordMementos.emplace_back(chord.getChordMemento(),
                                           chord);
            } else {
                temporalMementos.emplace_back(temporal.getMemento(),
                                              temporal);
            }
        }

        KOMP_ASSERT(params.getIsDurationValid());
        
        const auto& temporal = mTemporals.front().get();
        const MusicLocation loc(mMeasure,
                                temporal.getLocation().getTrack());
        auto& tuplet = mRegistry.createItem<Tuplet>(loc, params);
        tuplet.setIsQuiescent(false);
        mMeasure.insertItem(tuplet);
        
        for (auto& pair : chordMementos) {
            auto cmd = mFactory.chordStateCommand(std::move(pair.first.noteMementos),
                                                  std::move(pair.first.chordMemento),
                                                  pair.second.get().getMemento());
            addSubCommand(std::move(cmd));
        }

        for (const auto& pair : temporalMementos) {
            auto cmd = mFactory.itemStateCommand(pair.first->getMemento(),
                                                 pair.second.get().getMemento());
            addSubCommand(std::move(cmd));
        }

        mTupletID = tuplet.getItemID();
        auto fixTimesCommand = mFactory.fixTimesCommand(mMeasure);
        fixTimesCommand->execute();
        addSubCommand(std::move(fixTimesCommand));
        
        ItemImpl::setNeedsDisplay(tuplet);
        if (temporal.getIsBeamed()) {
            const auto& beam = temporal.getBeam();
            ItemImpl::setNeedsDisplay(*beam);
        }
        
        return true;
    }


    void
    InsertTupletCommand::undo()
    {
        KOMP_LOG_COMMAND
        auto& tupletItem = mRegistry.getItem(mTupletID);
        KOMP_ASSERT(tupletItem.is<TupletIntf>());
        auto& tuplet = tupletItem.as<TupletIntf>();

        if (!mDeleteTupletCommand) {
            mDeleteTupletCommand = mFactory.deleteTuplet(mMeasure, tuplet);
            mDeleteTupletCommand->execute();
        } else {
            mDeleteTupletCommand->redo();
        }
        undoSubCommands();
    }


    void
    InsertTupletCommand::redo()
    {
        KOMP_LOG_COMMAND
        KOMP_ASSERT(mDeleteTupletCommand);
        mDeleteTupletCommand->undo();
        redoSubCommands();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
