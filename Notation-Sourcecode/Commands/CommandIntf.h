// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// comment/uncomment this define to turn verbose command logging off/on.
//#define KOMP_VERBOSE_COMMAND_LOGGING

#ifdef KOMP_VERBOSE_COMMAND_LOGGING
#define KOMP_LOG_COMMAND LOG_F(INFO, "");
#else
#define KOMP_LOG_COMMAND
#endif

// System
#include <string_view>

// Local
#include "Forward.h"
#include "CommandResult.h"

namespace komp
{
    /// \brief  Represents a command that can be undone/redone
    ///
    class CommandIntf
    {
    public:
        CommandIntf() = delete;
        
        CommandIntf(Score& inScore);
        
        virtual ~CommandIntf() {}

        /// obtains a score lock and then calls executeImpl
        /// put your execute code in the executeImpl func
        virtual CommandResult execute() final;
        
        /// undo the command
        virtual void undo() = 0;

        /// redo the command
        virtual void redo() = 0;

        /// get the command name
        virtual std::string_view getName() const = 0;

        /// \brief  get the count of nested sub-commands
        ///
        /// \discussion for composite commands, a command
        /// object will hold a collection of nested sub-
        /// command objects. This function tells us how
        /// many nested sub-commands there are.
        /// 
        virtual int getSubCommandCount() const = 0;

        /// add a nested sub-command
        virtual void addSubCommand(CommandIntfUPtr inCommand) = 0;

        /// execute undo() on each of the sub-commands
        virtual void undoSubCommands() = 0;

        /// execute redo() on each of the sub-commands
        virtual void redoSubCommands() = 0;
        
        /// removes all sub-commands
        virtual void clearSubCommands() = 0;

        virtual const CommandIntfUPtrVec& getSubCommands() const = 0;
        
        /// Checks if item is contained in command
        virtual bool containsItem(const Item& inItem) const = 0;

        Score& getScore() const { return mScore; }
        
    protected:
        /// put your implementation for execute() here.
        virtual CommandResult executeImpl() = 0;
        
    private:
        Score& mScore;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
