// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>

// Local
#include "CommandFactory.h"
#include "CommandIntf.h"
#include "ItemRegistry.h"

namespace komp
{
    class Command : public CommandIntf
    {
    public:
        Command(std::string_view inName,
                CommandFactory& inFactory,
                ItemRegistry& inRegistry);
        virtual ~Command();

        /// default implementation calls undoSubCommands()
        virtual void undo() override;
        
        /// default implementation calls redoSubCommands()
        virtual void redo() override;
        
        std::string_view getName() const override;
        
        virtual int getSubCommandCount() const override;
        virtual void addSubCommand(CommandIntfUPtr inCommand) override;
        virtual void undoSubCommands() override;
        virtual void redoSubCommands() override;
        virtual void clearSubCommands() override;
        const CommandIntfUPtrVec& getSubCommands() const override;
        virtual bool containsItem(const Item& inItem) const override { return false; }
        
    protected:
        virtual CommandResult executeImpl() override;

        std::string mName;
        CommandFactory& mFactory;
        ItemRegistry& mRegistry;
        
    private:
        CommandIntfUPtrVec mSubCommands;
    };
            
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
