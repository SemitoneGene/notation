// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    class InsertMeasureCommand : public Command
    {
    public:
        InsertMeasureCommand(CommandFactory& inFactory,
                             ItemRegistry& inRegistry,
                             Score& inScore,
                             size_t inIndex);
        
        virtual ~InsertMeasureCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        Score& mScore;
        size_t mIndex;
        MeasureUPtr mMeasure;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
