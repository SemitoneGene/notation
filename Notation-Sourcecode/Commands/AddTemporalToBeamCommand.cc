// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "AddTemporalToBeamCommand.h"

// Local
#include "BeamIntf.h"
#include "Measure.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{
    AddTemporalToBeamCommand::~AddTemporalToBeamCommand() = default;

    AddTemporalToBeamCommand::AddTemporalToBeamCommand(CommandFactory& inFactory,
                                                       ItemRegistry& inRegistry,
                                                       Measure& inMeasure,
                                                       ItemID inBeamID,
                                                       ItemID inTemporalID)
    : komp::Command{"Add Temporal to Beam", inFactory, inRegistry}
    , mBeamID{inBeamID}
    , mTemporalID{inTemporalID}
    , mMeasure{inMeasure}
    {

    }


    CommandResult
    AddTemporalToBeamCommand::executeImpl()
    {
        KOMP_LOG_COMMAND;
        auto& beamItem = mRegistry.getItem(mBeamID);
        KOMP_ASSERT(beamItem.is<BeamIntf>());
        auto& beam = beamItem.as<BeamIntf>();
        auto& temporalItem = mRegistry.getItem(mTemporalID);
        KOMP_ASSERT(temporalItem.is<Temporal>());
        auto& temporal = temporalItem.as<Temporal>();
        auto beamedNotes = beam.getNotes();
        auto deleteBeam = mFactory.deleteBeamCommand(beam);
        deleteBeam->execute();
        addSubCommand(std::move(deleteBeam));

        if (temporal.getIsRaw()) {
            temporal.setIsRaw(false);
            temporal.setDuration(TemporalType::EIGHTH);
        }

        auto duration = temporal.getDuration();

        if (!TemporalValue::getHasFlags(duration.getTemporalType())) {
            duration.setTemporalType(TemporalType::EIGHTH);
        }

        auto insertTemporal = mFactory.insertTemporalCommand(temporal.getItemID(),
                                                             mMeasure);
        insertTemporal->execute();
        addSubCommand(std::move(insertTemporal));
        beamedNotes.push_back(temporal);
            
        std::vector<ItemID> itemIDS;
        for (const Temporal& item : beamedNotes) {
            itemIDS.push_back(item.getItemID());
        }
        auto insertBeam = mFactory.insertBeamCommand(itemIDS);
        insertBeam->execute();
        addSubCommand(std::move(insertBeam));
        return true;
    }


    void
    AddTemporalToBeamCommand::undo()
    {
        KOMP_LOG_COMMAND;
        undoSubCommands();
    }


    void
    AddTemporalToBeamCommand::redo()
    {
        KOMP_LOG_COMMAND;
        redoSubCommands();
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
