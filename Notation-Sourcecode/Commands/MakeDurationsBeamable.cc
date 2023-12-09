// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MakeDurationsBeamable.h"

// Local
#include "Score.h"
#include "ItemRegistry.h"
#include "BeamFactory.h"
#include "Note.h"
#include "TemporalValue.h"

namespace komp
{
    MakeDurationsBeamable::~MakeDurationsBeamable() = default;

    MakeDurationsBeamable::MakeDurationsBeamable(CommandFactory& inFactory,
                                                 ItemRegistry& inRegistry,
                                                 const TemporalRefVec inTemporals)
    : komp::Command{"Make Durations Beamable", inFactory, inRegistry}
    , mItemIDs{}
    {
        for (const Temporal& temporal : inTemporals) {
            if (temporal.getIsChordMember()) {
                mItemIDs.emplace(temporal.getItemID());
            } else if (!temporal.getIsRest()) {
                mItemIDs.emplace(temporal.getItemID());
            }
        }
    }

    CommandResult
    MakeDurationsBeamable::executeImpl()
    {
        KOMP_LOG_COMMAND
        std::set<Temporal*> temporals;
        for (const auto& itemID : mItemIDs) {
            Item& item = mRegistry.getItem(itemID);
            if (item.is<Temporal>()) {
                auto& temporal = item.as<Temporal>();
                if (!temporal.getIsChordMember() && !temporal.getIsRest()) {
                    const auto ttype = temporal.getDuration().getTemporalType();
                    if (TemporalValue::getNumFlags(ttype) < 1 || temporal.getIsRaw()) {
                        auto memento = temporal.getMemento();
                        if (temporal.getIsRaw()) {
                            temporal.setIsRaw(false);
                        }
                        auto dur = temporal.getDuration();
                        dur.setTemporalType(TemporalType::EIGHTH);
                        temporal.setDuration(dur);
                        auto cmd = mFactory.itemStateCommand(std::move(memento),
                                                             temporal.getMemento());
                        addSubCommand(std::move(cmd));
                    }
                }
            }
        }
        return true;
    }


    void
    MakeDurationsBeamable::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }


    void
    MakeDurationsBeamable::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
