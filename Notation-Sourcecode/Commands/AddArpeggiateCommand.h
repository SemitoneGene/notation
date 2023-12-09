// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
/// \brief  Adds an arpeggiate to a chord
class AddArpeggiateCommand : public Command
{
public:
    AddArpeggiateCommand(CommandFactory& inFactory,
                         ItemRegistry& inRegistry,
                         ItemID inItemID,
                         komp::ArpeggiateType inArpeggiateType);
    
    virtual ~AddArpeggiateCommand();
    
    virtual void undo() override;
    virtual void redo() override;
    
private:
    ItemRegistry& mRegistry;
    ItemID mItemID;
    ArpeggiateType mArpeggiateType;
    
    virtual CommandResult executeImpl() override;
};

/// \brief  Removes an arpeggiate from a chord
class RemoveArpeggiateCommand : public Command
{
public:
    RemoveArpeggiateCommand(CommandFactory& inFactory,
                            ItemRegistry& inRegistry,
                            ItemID inItemID);
    
    virtual ~RemoveArpeggiateCommand();
    
    virtual void undo() override;
    virtual void redo() override;
    
private:
    ItemRegistry& mRegistry;
    ItemID mItemID;
    ArpeggiateType mArpeggiateType;
    
    virtual CommandResult executeImpl() override;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
