// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"
#include "SpannerParams.h"
#include "WedgeType.h"

namespace komp
{
    /// \brief  deletes a beam
    ///
    class InsertWedgeCommand : public Command
    {
    public:
        InsertWedgeCommand(CommandFactory& inFactory,
                           ItemRegistry& inRegistry,
                           const SpannerParams& inParams,
                           WedgeType inWedgeType,
                           PlacementType inPlacementType);
        
        virtual ~InsertWedgeCommand();
        
        virtual void undo() override;
        virtual void redo() override;
        
    private:
        ItemID mWedgeID;
        SpannerParams mSpannerParams;
        WedgeType mWedgeType;
        PlacementType mPlacementType;
        virtual CommandResult executeImpl() override;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
