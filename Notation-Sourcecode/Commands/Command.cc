// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Command.h"

// System
#include <sstream>

// Local
#include "BeamIntf.h"
#include "Measure.h"
#include "Note.h"
#include "Rest.h"
#include "Score.h"
#include "StaffSystem.h"
#include "Selection.h"
#include "Spatial.h"
#include "Staff.h"
#include "Temporal.h"
#include "Track.h"
#include "Throw.h"

namespace komp
{

Command::~Command() = default;

/// \brief The basic command to be subclassed
Command::Command(std::string_view inName,
                 CommandFactory& inFactory,
                 ItemRegistry& inRegistry)
: CommandIntf(inFactory.getScore())
, mName(inName)
, mFactory(inFactory)
, mRegistry(inRegistry)
{
}

/// Return the command name
std::string_view
Command::getName() const
{
    return mName;
}

/// \brief default implementation does nothing
CommandResult
Command::executeImpl()
{
    KOMP_LOG_COMMAND
    return false;
}

/// \brief default implementation - most subclasses will override this method
void
Command::undo()
{
    KOMP_LOG_COMMAND
    undoSubCommands();
}

/// \brief default implementation - most subclasses will override this method
void
Command::redo()
{
    KOMP_LOG_COMMAND
    redoSubCommands();
}

/// \brief get the count of nested sub-commands
int
Command::getSubCommandCount() const
{
    return static_cast<int>(mSubCommands.size());
}

/// \brief add a sub-command
void
Command::addSubCommand(CommandIntfUPtr inCommand)
{
    mSubCommands.emplace_back(std::move(inCommand));
}

/// \brief call undo() on all of the sub-commands
void
Command::undoSubCommands()
{
    for (auto c = mSubCommands.crbegin();
         c != mSubCommands.crend(); ++c) {
        (*c)->undo();
    }
}

/// \brief call redo() on all of the sub-commands
void
Command::redoSubCommands()
{
    for (const auto& c : mSubCommands) {
        c->redo();
    }
}

/// \brief clear sub commands
void
Command::clearSubCommands()
{
    mSubCommands.clear();
}


const CommandIntfUPtrVec&
Command::getSubCommands() const
{
    return mSubCommands;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
