// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Set
#include <set>

// Local
#include "Command.h"

namespace komp
{
    class MakeDurationsBeamable : public Command
    {
    public:
        MakeDurationsBeamable(CommandFactory& inFactory,
                              ItemRegistry& inRegistry,
                              const TemporalRefVec inTemporals);

        virtual ~MakeDurationsBeamable();

        virtual void undo() override;
        virtual void redo() override;

    private:
        std::set<ItemID> mItemIDs;
        virtual CommandResult executeImpl() override;

    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
