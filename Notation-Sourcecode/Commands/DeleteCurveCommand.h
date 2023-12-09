// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  deletes a curve
    ///
    /// \discussion deletes a curve and saves
    /// the state of the affected notes
    ///
    class DeleteCurveCommand : public Command
    {
    public:
        DeleteCurveCommand(CommandFactory& inFactory,
                           ItemRegistry& inRegistry,
                           ItemID inItemID);
        
        virtual ~DeleteCurveCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mCurveID;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
