// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "AdditionalBeam.h"

// Local
#include "BeamFactory.h"
#include "DeleteBeamCommand.h"
#include "InsertBeamCommand.h"
#include "ItemRegistry.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Throw.h"

namespace komp
{
    AdditionalBeam::~AdditionalBeam() = default;

    AdditionalBeam::AdditionalBeam(CommandFactory& inFactory,
                                   ItemRegistry& inRegistry,
                                   const BeamStrokeParams& inData)
    : komp::Command{"Add Beam", inFactory, inRegistry}
    , mData{std::move(inData)}
    {
        KOMP_ASSERT(mData.isContinuousBeam);
        KOMP_ASSERT(mData.getTemporals().size() >= 2);
        KOMP_ASSERT(mData.isMultiTemporalType);
        KOMP_ASSERT(mData.getStart().getBeam());
    }

    CommandResult
    AdditionalBeam::executeImpl()
    {
        KOMP_LOG_COMMAND
        const auto& temporals = mData.getTemporals();
        int numFlags = TemporalValue::MAX_NUM_FLAGS;
        const auto beam = mData.getStart().getBeam();
        KOMP_ASSERT(beam);

        std::set<Temporal*> rebeamables;

        for (Temporal& temporal : temporals) {
            rebeamables.emplace(&temporal);
        }

        for (const auto& beamedNote : beam->getBeamedNotes()) {
            auto& note = beamedNote->getNote();
            rebeamables.emplace(&note);
        }

        // find the duration with the longest temporal type (i.e.
        // the least number of beams (i.e. flags)
        for (const Temporal& temporal : temporals) {
            const auto temporalType = temporal.getDuration().getTemporalType();
            const int currentNumFlags = TemporalValue::getNumFlags(temporalType);
            if (currentNumFlags < numFlags) {
                numFlags = currentNumFlags;
            }
        }

        // we will increase the numFlags of the duration with the least flags
        // by one (i.e. we will halve its duration). We will do the same with
        // any other notes that are the same.
        const auto targetNumFlags = numFlags + 1;
        const auto targetTemporalType = TemporalValue::getTemporalTypeForNumFlags(targetNumFlags);

        // delete the existing beam
        auto deleteCommand = mFactory.deleteBeamCommand(*beam);
        deleteCommand->execute();
        addSubCommand(std::move(deleteCommand));

        for (Temporal& temporal : temporals) {
            auto duration = temporal.getDuration();
            const auto thisNotesTemporalType = duration.getTemporalType();
            const int thisNotesNumFlags = TemporalValue::getNumFlags(thisNotesTemporalType);
            if (thisNotesNumFlags < targetNumFlags) {
                auto memento = temporal.getMemento();
                duration.setTemporalType(targetTemporalType);
                temporal.setDuration(duration);
                auto stateCmd = mFactory.itemStateCommand(std::move(memento),
                                                          temporal.getMemento());
                addSubCommand(std::move(stateCmd));
            }
        }

        TemporalRefVec rebeam;
        for (const auto& temporal : rebeamables) {
            rebeam.emplace_back(*temporal);
        }
        sortItems(rebeam);

        std::vector<ItemID> itemIDS;
        for (const Item& item : rebeam) {
            itemIDS.emplace_back(item.getItemID());
        }

        auto newBeamCommand = mFactory.insertBeamCommand(itemIDS);
        newBeamCommand->execute();
        addSubCommand(std::move(newBeamCommand));
        return true;
    }


    void
    AdditionalBeam::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }


    void
    AdditionalBeam::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
