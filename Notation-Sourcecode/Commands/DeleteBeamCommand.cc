// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteBeamCommand.h"

// Local
#include "BeamIntf.h"
#include "ItemImpl.h"
#include "ItemRegistry.h"
#include "Measure.h"
#include "Throw.h"
#include "Temporal.h"


namespace komp
{
    DeleteBeamCommand::~DeleteBeamCommand() = default;
    
    DeleteBeamCommand::DeleteBeamCommand(CommandFactory& inFactory,
                                         ItemRegistry& inRegistry,
                                         const BeamIntf& inBeam)
    : komp::Command("Delete Beam", inFactory, inRegistry)
    , mItemID(inBeam.getItemID())
    , mBeamMemento{nullptr}
    {

    }
    
    
    CommandResult
    DeleteBeamCommand::executeImpl()
    {
        KOMP_LOG_COMMAND
        auto& item = mRegistry.getItem(mItemID);
        KOMP_ASSERT(item.is<BeamIntf>());
        auto& beam = item.as<BeamIntf>();
        auto& measure = beam.getLocation().getMeasure();
        mMeasure = &measure;

        ItemUPtrVec temporalMementos;
        std::vector<ChordMemento> chordMementos;
        auto beamTemporals = beam.getNotes();

        // Save a memento for each of the affected temporals
        for (Temporal& temporal : beamTemporals) {
            if (temporal.getIsChord()) {
                const auto& chord = temporal.as<Chord>();
                chordMementos.emplace_back(chord.getChordMemento());
            } else {
                temporalMementos.emplace_back(temporal.getMemento());
            }
        }
        
        // Save a memento of the beam
        mBeamMemento = beam.getMemento();

        beam.removeSelf();

        for (Temporal& temporal : beamTemporals) {
            temporal.clearBeam();
            ItemImpl::setNeedsDisplay(temporal);
        }

        for (auto& memento : temporalMementos) {
            const auto& item = mRegistry.getItem(memento->getItemID());
            auto cmd = mFactory.itemStateCommand(std::move(memento),
                                                 item.getMemento());
            addSubCommand(std::move(cmd));
        }

        for (auto& memento : chordMementos) {
            const auto& item = mRegistry.getItem(memento.chordMemento->getItemID());
            auto cmd = mFactory.chordStateCommand(std::move(memento.noteMementos),
                                                  std::move(memento.chordMemento),
                                                  item.getMemento());
            addSubCommand(std::move(cmd));
        }
        
        const auto& beamItem = mRegistry.getItem(mBeamMemento->getItemID());
        auto cmd = mFactory.itemStateCommand(mBeamMemento->getMemento(),
                                             beamItem.getMemento());
        addSubCommand(std::move(cmd));

        return true;
    }
    
    
    void
    DeleteBeamCommand::undo()
    {
        KOMP_LOG_COMMAND
        KOMP_ASSERT(mBeamMemento);
        KOMP_ASSERT(mMeasure);
        auto& item = mRegistry.getItem(mItemID);
        KOMP_ASSERT(item.is<BeamIntf>());
        auto& beam = item.as<BeamIntf>();
        undoSubCommands();
        mMeasure->insertItem(beam);
        beam.restoreMemento(*mBeamMemento);
        ItemImpl::setNeedsDisplay(beam);
    }
    
    
    void
    DeleteBeamCommand::redo()
    {
        KOMP_LOG_COMMAND
        KOMP_LOG_COMMAND
        KOMP_ASSERT(mBeamMemento);
        KOMP_ASSERT(mMeasure);
        auto& item = mRegistry.getItem(mItemID);
        KOMP_ASSERT(item.is<BeamIntf>());
        auto& beam = item.as<BeamIntf>();
        beam.removeSelf();
        redoSubCommands();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
