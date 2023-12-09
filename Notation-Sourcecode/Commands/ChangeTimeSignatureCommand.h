// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    class ChangeTimeSignatureCommand : public Command
    {
    public:
        ChangeTimeSignatureCommand(CommandFactory& inFactory,
                                   ItemRegistry& inRegistry,
                                   Score& inScore,
                                   size_t inMeasureIndex,
                                   const TimeSignatureParams& inParams);
        
        virtual ~ChangeTimeSignatureCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        Score& mScore;
        size_t mMeasureIndex;
        size_t mSpan;
        TimeSignatureParams mParams;
        TimeSignatureParams mPreviousParams;
        bool mShow;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
