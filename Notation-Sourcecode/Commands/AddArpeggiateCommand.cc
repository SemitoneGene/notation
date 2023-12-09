// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "AddArpeggiateCommand.h"

// Local
#include "Chord.h"
#include "ItemImpl.h"
#include "Throw.h"

namespace komp
{

// MARK: - AddArpeggiateCommand

AddArpeggiateCommand::~AddArpeggiateCommand() = default;

/// \brief The basic command to be subclassed
AddArpeggiateCommand::AddArpeggiateCommand(CommandFactory& inFactory,
                                           ItemRegistry& inRegistry,
                                           ItemID inItemID,
                                           komp::ArpeggiateType inArpeggiateType)
: komp::Command("Add Arpeggiate", inFactory, inRegistry)
, mRegistry{inRegistry}
, mItemID{inItemID}
, mArpeggiateType(inArpeggiateType)
{
}


CommandResult
AddArpeggiateCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    const auto& item = mRegistry.getItem(mItemID);
    KOMP_ASSERT(item.is<Chord>());
    auto& chord = mRegistry.getItem(mItemID).as<Chord>();
    chord.setArpeggiate(mArpeggiateType);
    ItemImpl::setNeedsDisplay(chord);
    return true;
}


void
AddArpeggiateCommand::undo()
{
    KOMP_LOG_COMMAND
    const auto& item = mRegistry.getItem(mItemID);
    KOMP_ASSERT(item.is<Chord>());
    auto& chord = mRegistry.getItem(mItemID).as<Chord>();
    chord.setArpeggiate(komp::ArpeggiateType::NONE);
    ItemImpl::setNeedsDisplay(chord);
    undoSubCommands();
}

void
AddArpeggiateCommand::redo()
{
    KOMP_LOG_COMMAND
    const auto& item = mRegistry.getItem(mItemID);
    KOMP_ASSERT(item.is<Chord>());
    auto& chord = mRegistry.getItem(mItemID).as<Chord>();
    chord.setArpeggiate(mArpeggiateType);
    ItemImpl::setNeedsDisplay(chord);
    redoSubCommands();
}

// MARK: - RemoveArpeggiateCommand

RemoveArpeggiateCommand::~RemoveArpeggiateCommand() = default;

/// \brief The basic command to be subclassed
RemoveArpeggiateCommand::RemoveArpeggiateCommand(CommandFactory& inFactory,
                                                 ItemRegistry& inRegistry,
                                                 ItemID inItemID)
: komp::Command("Remove Arpeggiate", inFactory, inRegistry)
, mRegistry{inRegistry}
, mItemID{inItemID}
, mArpeggiateType{komp::ArpeggiateType::NONE}
{
}


CommandResult
RemoveArpeggiateCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    const auto& item = mRegistry.getItem(mItemID);
    KOMP_ASSERT(item.is<Chord>());
    auto& chord = mRegistry.getItem(mItemID).as<Chord>();
    mArpeggiateType = chord.getArpeggiate();
    chord.setArpeggiate(komp::ArpeggiateType::NONE);
    ItemImpl::setNeedsDisplay(chord);
    return true;
}


void
RemoveArpeggiateCommand::undo()
{
    KOMP_LOG_COMMAND
    const auto& item = mRegistry.getItem(mItemID);
    KOMP_ASSERT(item.is<Chord>());
    auto& chord = mRegistry.getItem(mItemID).as<Chord>();
    chord.setArpeggiate(mArpeggiateType);
    ItemImpl::setNeedsDisplay(chord);
    undoSubCommands();
}

void
RemoveArpeggiateCommand::redo()
{
    KOMP_LOG_COMMAND
    const auto& item = mRegistry.getItem(mItemID);
    KOMP_ASSERT(item.is<Chord>());
    auto& chord = mRegistry.getItem(mItemID).as<Chord>();
    chord.setArpeggiate(komp::ArpeggiateType::NONE);
    ItemImpl::setNeedsDisplay(chord);
    redoSubCommands();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
