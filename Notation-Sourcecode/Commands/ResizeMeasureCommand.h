// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{
    class ResizeMeasureCommand : public Command
    {
    public:
        ResizeMeasureCommand(CommandFactory& inFactory,
                             ItemRegistry& inRegistry,
                             const Score& inScore,
                             size_t inIndex,
                             double inOriginalWidth,
                             double inNewWidth);
        
        virtual ~ResizeMeasureCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        void setMeasureWidth(double inWidth, double inOffset);
        
        const Score& mScore;
        size_t mIndex;
        double mOriginalWidth;
        double mNewWidth;
        double mOffset;
        virtual CommandResult executeImpl() override;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
