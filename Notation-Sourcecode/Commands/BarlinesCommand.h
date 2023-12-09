// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BarlineType.h"
#include "Command.h"

namespace komp
{
    /// Changes the measure barlines (unless the new barlines are equal to the old)
    class BarlinesCommand : public Command
    {
    public:
        virtual ~BarlinesCommand();
        BarlinesCommand(CommandFactory& inFactory,
                        ItemRegistry& inRegistry,
                        Measure& inMeasure,
                        BarlineType inLeftBarline,
                        BarlineType inRightBarline);
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        Measure& mMeasure;
        BarlineType mOriginalLeftBarline;
        BarlineType mOriginalRightBarline;
        BarlineType mUpdatedLeftBarline;
        BarlineType mUpdatedRightBarline;
        
        virtual CommandResult executeImpl() override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
