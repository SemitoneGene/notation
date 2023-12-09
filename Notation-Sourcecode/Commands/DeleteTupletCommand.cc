// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteTupletCommand.h"

// Local
#include "Beam.h"
#include "Score.h"
#include "Tuplet.h"
#include "Temporal.h"
#include "Measure.h"
#include "CommandFactory.h"

namespace komp
{
DeleteTupletCommand::~DeleteTupletCommand() = default;

DeleteTupletCommand::DeleteTupletCommand(CommandFactory& inFactory,
                                         ItemRegistry& inRegistry,
                                         Measure& inMeasure,
                                         const TupletIntf& inTuplet)
: komp::Command{"Delete Tuplet", inFactory, inRegistry}
, mMeasure{inMeasure}
, mTupletID{inTuplet.getItemID()}
{
    
}

struct ChordState
{
    ItemUPtr itemPtr;
    ItemRef itemRef;
    ItemUPtrVec mementos;
};

CommandResult
DeleteTupletCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    auto& item = mRegistry.getItem(mTupletID);
    KOMP_ASSERT(item.is<TupletIntf>());
    auto& tuplet = item.as<TupletIntf>();
    
    std::vector<ChordState> chordMementos;
    std::vector<std::pair<ItemUPtr, ItemRef>> nonChordMementos;
    std::vector<std::pair<ItemUPtr, ItemRef>> beamMementos;
    auto temporals = tuplet.getTemporals();
    for (Temporal& temporal : temporals) {
        if (temporal.getItemTraits().getItemType() == ItemType::Chord) {
            auto& chord = temporal.as<Chord>();            
            auto chordState = ChordState{chord.getMemento(), chord};
            auto chordNotes = chord.getNotes();
            for (Note& n : chordNotes) {
                chordState.mementos.emplace_back(n.getMemento());
            }
            chordMementos.emplace_back(std::move(chordState));
        } else {
            nonChordMementos.emplace_back(temporal.getMemento(), temporal);
        }
        
        const auto beam = temporal.getBeam();
        if (beam) {
            beamMementos.emplace_back(beam->getMemento(), *beam);
        }
    }
    
    auto tupletMemento = tuplet.getMemento();
    mMeasure.removeItem(tuplet);
    tuplet.unsetTemporals();
    
    for (auto& chordState : chordMementos) {
        auto cmd = mFactory.chordStateCommand(std::move(chordState.mementos),
                                              std::move(chordState.itemPtr),
                                              chordState.itemRef.get().getMemento());
        addSubCommand(std::move(cmd));
    }
    
    for (auto& pair : nonChordMementos) {
        auto cmd = mFactory.itemStateCommand(std::move(pair.first),
                                             pair.second.get().getMemento());
        addSubCommand(std::move(cmd));
    }
    
    for (auto& pair : beamMementos) {
        auto cmd = mFactory.itemStateCommand(std::move(pair.first),
                                             pair.second.get().getMemento());
        addSubCommand(std::move(cmd));
    }
    
    auto tupletStateCmd = mFactory.itemStateCommand(std::move(tupletMemento),
                                                    tuplet.getMemento());
    addSubCommand(std::move(tupletStateCmd));
    
    auto fixTimesCommand = mFactory.fixTimesCommand(mMeasure);
    fixTimesCommand->execute();
    addSubCommand(std::move(fixTimesCommand));
    
    // make sure it worked
    for (const Temporal& temporal : temporals) {
        KOMP_ASSERT(temporal.getTuplets().size() == 0);
        KOMP_ASSERT(temporal.getDuration().getTupletRatios().size() == 0);
    }
    
    return true;
}


void
DeleteTupletCommand::undo()
{
    KOMP_LOG_COMMAND
    auto& item = mRegistry.getItem(mTupletID);
    KOMP_ASSERT(item.is<TupletIntf>());
    auto& tuplet = item.as<TupletIntf>();
    mMeasure.insertItem(tuplet);
    undoSubCommands();
}


void
DeleteTupletCommand::redo()
{
    KOMP_LOG_COMMAND
    auto& item = mRegistry.getItem(mTupletID);
    KOMP_ASSERT(item.is<TupletIntf>());
    auto& tuplet = item.as<TupletIntf>();
    mMeasure.removeItem(tuplet);
    redoSubCommands();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
