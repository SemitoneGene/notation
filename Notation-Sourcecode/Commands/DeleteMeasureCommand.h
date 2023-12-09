// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    class DeleteMeasureCommand : public Command
    {
    public:
        DeleteMeasureCommand(CommandFactory& inFactory,
                             ItemRegistry& inRegistry,
                             Score& inScore,
                             size_t inIndex);
        
        virtual ~DeleteMeasureCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        size_t mIndex;
        Score& mScore;
        MeasureUPtr mMeasure;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
