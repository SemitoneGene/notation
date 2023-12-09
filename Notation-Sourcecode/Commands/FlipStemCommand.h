// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    /// \brief  flips the stem direction
    ///
    class FlipStemCommand : public Command
    {
    public:
        FlipStemCommand(CommandFactory& inFactory,
                        ItemRegistry& inRegistry,
                        const Note& inNote);
        
        virtual ~FlipStemCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mTemporalID;
        bool mStemUp;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
