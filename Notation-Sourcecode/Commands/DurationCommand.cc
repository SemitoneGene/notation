// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DurationCommand.h"

// Local
#include "BeamIntf.h"
#include "Chord.h"
#include "Note.h"
#include "Rest.h"
#include "Score.h"
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{
DurationCommand::~DurationCommand() = default;

DurationCommand::DurationCommand(CommandFactory& inFactory,
                                 ItemRegistry& inRegistry,
                                 Measure& inMeasure,
                                 ItemID inItemID,
                                 Duration inDuration)
: komp::Command("Duration Change", inFactory, inRegistry)
, mMeasure(inMeasure)
, mItemID{inItemID}
, mNewDuration(inDuration)
, mBeamNotes()
{
    
}


CommandResult
DurationCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    clearSubCommands();
    
    auto& temporal = mRegistry.getItem(mItemID).as<Temporal>();
    
    Chord* chordParent = nullptr;
    ChordMemento chordMemento;
    if (temporal.getIsChordMember()) {
        chordParent = temporal.getChord();
        chordMemento = chordParent->getChordMemento();
    }
    
    Chord* chordSelf = nullptr;
    ItemUPtr memento;
    if (temporal.getIsChord()) {
        chordSelf = &temporal.as<Chord>();
        chordMemento = chordSelf->getChordMemento();
    } else {
        memento = temporal.getMemento();
    }
    
    BeamIntf* beam = nullptr;
    
    if (chordSelf) {
        beam = chordSelf->getBeam();
    } else if (chordParent) {
        beam = chordParent->getBeam();
    } else {
        beam = temporal.getBeam();
    }
    
    if (beam) {
        saveBeamState(*beam);
        deleteBeam(*beam);
    }
    
    if (chordSelf) {
        chordSelf->setDuration(mNewDuration);
        auto cmd = mFactory.chordStateCommand(std::move(chordMemento.noteMementos),
                                              std::move(chordMemento.chordMemento),
                                              chordSelf->getMemento());
        cmd->execute();
        addSubCommand(std::move(cmd));
    } else if (chordParent) {
        chordParent->setDuration(mNewDuration);
        auto cmd = mFactory.chordStateCommand(std::move(chordMemento.noteMementos),
                                              std::move(chordMemento.chordMemento),
                                              chordParent->getMemento());
        cmd->execute();
        addSubCommand(std::move(cmd));
    } else {
        temporal.setDuration(mNewDuration);
        auto cmd = mFactory.itemStateCommand(std::move(memento),
                                             temporal.getMemento());
        cmd->execute();
        addSubCommand(std::move(cmd));
    }
    
    if (beam) {
        remakeBeam();
    }
    
    // Fix times
    auto fixTimesCmd = mFactory.fixTimesCommand(mMeasure);
    fixTimesCmd->execute();
    addSubCommand(std::move(fixTimesCmd));
    return true;
}


void
DurationCommand::undo()
{
    KOMP_LOG_COMMAND
    undoSubCommands();
}


void
DurationCommand::redo()
{
    KOMP_LOG_COMMAND
    execute();
}


void
DurationCommand::saveBeamState(BeamIntf& inBeam)
{
    mBeamNotes = inBeam.getNotes();
}


void
DurationCommand::deleteBeam(BeamIntf& beam)
{
    auto cmd = mFactory.deleteBeamCommand(beam);
    cmd->execute();
    addSubCommand(std::move(cmd));
}


void
DurationCommand::remakeBeam()
{
    std::vector<std::vector<ItemID>> rebeams;
    std::vector<ItemID> current;
    
    for (Temporal& temporal : mBeamNotes) {
        const auto type = temporal.getDuration().getTemporalType();
        const auto isBeamable = TemporalValue::getHasFlags(type);
        
        if (isBeamable) {
            current.push_back(temporal.getItemID());
        } else {
            if (current.size() >= 2) {
                rebeams.push_back(current);
            }
            current.clear();
        }
        
    }
    
    if (current.size() >= 2) {
        rebeams.push_back(current);
    }
    current.clear();
    
    for (const auto& temporals : rebeams) {
        if (temporals.size() >= 2) {
            auto cmd = mFactory.insertBeamCommand(temporals);
            cmd->execute();
            addSubCommand(std::move(cmd));
        }
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
