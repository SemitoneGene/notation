// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteTemporalCommand.h"

// Local
#include "Chord.h"
#include "CommandFactory.h"
#include "Curve.h"
#include "Note.h"
#include "Score.h"
#include "Temporal.h"

namespace komp
{
    DeleteTemporalCommand::~DeleteTemporalCommand() = default;
    
DeleteTemporalCommand::DeleteTemporalCommand(CommandFactory& inFactory,
                                             ItemRegistry& inRegistry,
                                             const Temporal& inTemporal)
    : Command("Delete Temporal", inFactory, inRegistry)
    , mItemID(inTemporal.getItemID())
    , mArchivedTemporal(nullptr)
    , mMeasure{inTemporal.getLocation().getMeasure()}
    {
        // pesky bug
        KOMP_ASSERT(inTemporal.getItemTraits().getItemType() != ItemType::Chord);
    }
    
    /// \brief deletes the temporal
    CommandResult
    DeleteTemporalCommand::executeImpl()
    {
        KOMP_LOG_COMMAND

        auto& temporal = mRegistry.getItem(mItemID).as<Temporal>();

        for (Mark& mark : temporal.getAttachments().getMarks()) {
            auto deleteMark = mFactory.deleteMarkCommand(mark);
            deleteMark->execute();
            addSubCommand(std::move(deleteMark));
        }
        
        if (temporal.getItemTraits().getItemType() == ItemType::Note) {
            auto& note = temporal.as<Note>();
            Chord* chord = nullptr;
            
            if (note.getIsChordMember()) {
                chord = note.getChord();
            }
            
            if (chord) {
                auto chordMemento = chord->getMemento();
                auto noteMemento = note.getMemento();
                chord->removeNote(note);
                auto chordStateCommand = mFactory.itemStateCommand(std::move(chordMemento),
                                                                   chord->getMemento());
                addSubCommand(std::move(chordStateCommand));
                auto noteStateCommand = mFactory.itemStateCommand(std::move(noteMemento),
                                                                  note.getMemento());
                addSubCommand(std::move(noteStateCommand));
            }
        }
        
        if (temporal.getAttachments().hasIncomingCurve()) {
            const auto& curves = temporal.getAttachments().getIncomingCurves();
            for (const auto& curve : curves) {
                auto deleteCommand = mFactory.deleteCurveCommand(curve);
                deleteCommand->execute();
                addSubCommand(std::move(deleteCommand));
            }
        }
        
        if (temporal.getAttachments().hasOutgoingCurve()) {
            const auto& curves = temporal.getAttachments().getOutgoingCurves();
            for (const auto& curve : curves) {
                auto deleteCommand = mFactory.deleteCurveCommand(curve);
                deleteCommand->execute();
                addSubCommand(std::move(deleteCommand));
            }
        }
        
        // Save the current state of the temporal
        mArchivedTemporal = temporal.getMemento();
        const auto doUpdateTimes = false;
        mMeasure.removeItem(temporal, doUpdateTimes);
        return true;
    }
    
    /// \brief restores the temporal
    void
    DeleteTemporalCommand::undo()
    {
        KOMP_LOG_COMMAND
        KOMP_ASSERT(mArchivedTemporal);
        auto& temporal = mRegistry.getItem(mItemID).as<Temporal>();
        temporal.restoreMemento(*mArchivedTemporal);
        mMeasure.insertItem(temporal, temporal.getTime());
        undoSubCommands();
    }
    
    /// \brief re-deletes the temporal
    void
    DeleteTemporalCommand::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
        
        auto& temporal = mRegistry.getItem(mItemID).as<Temporal>();
        mArchivedTemporal = temporal.getMemento();
        auto& measure = temporal.getLocation().getMeasure();
        const auto doUpdateTimes = false;
        measure.removeItem(temporal, doUpdateTimes);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
