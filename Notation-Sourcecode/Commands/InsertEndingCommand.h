// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "EndingParams.h"

namespace komp
{
    class InsertEndingCommand : public Command
    {
    public:
        virtual ~InsertEndingCommand();
        InsertEndingCommand(CommandFactory& inFactory,
                            ItemRegistry& inRegistry,
                            Measure& inMeasure,
                            const EndingParams& inParams);
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        virtual CommandResult executeImpl() override;
        
        Measure& mMeasure;
        EndingParams mParams;
        ItemID mItemID;

    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
