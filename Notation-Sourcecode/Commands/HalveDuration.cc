// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "HalveDuration.h"

// Local
#include "Score.h"
#include "Temporal.h"
#include "TemporalValue.h"

namespace komp
{
    HalveDuration::~HalveDuration() = default;

HalveDuration::HalveDuration(CommandFactory& inFactory,
                             ItemRegistry& inRegistry,
                             const TemporalRefVec& inTemporals)
    : komp::Command{"Halve Duration", inFactory, inRegistry}
    , mTemporalIDs{}
    {
        for (const Temporal& temporal : inTemporals) {
            mTemporalIDs.emplace_back(temporal.getItemID());
        }
    }

    CommandResult
    HalveDuration::executeImpl()
    {
        KOMP_LOG_COMMAND
        std::set<Temporal*> temporals;

        for (const auto id : mTemporalIDs) {
            auto& item = mRegistry.getItem(id);
            KOMP_ASSERT(item.is<Temporal>());
            auto& temporal = item.as<Temporal>();
            if (temporal.getIsChordMember()) {
                temporals.emplace(temporal.getChord());
            } else {
                temporals.emplace(&temporal);
            }
        }

        for (auto temporal : temporals) {
            auto memento = temporal->getMemento();
            auto cmd = mFactory.itemStateCommand(std::move(memento),
                                                 temporal->getMemento());
            auto duration = temporal->getDuration();
            auto oldTemporalType = duration.getTemporalType();
            auto newTemporalType = TemporalValue::halve(oldTemporalType);
            duration.setTemporalType(newTemporalType);
            temporal->setDuration(duration);
            addSubCommand(std::move(cmd));
        }

        return true;
    }


    void
    HalveDuration::undo()
    {
        KOMP_LOG_COMMAND
        undoSubCommands();
    }


    void
    HalveDuration::redo()
    {
        KOMP_LOG_COMMAND
        redoSubCommands();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
