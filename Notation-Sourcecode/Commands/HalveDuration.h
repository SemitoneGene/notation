// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Command.h"

namespace komp
{

    /// \brief Halves the duration of each temporal
    class HalveDuration : public Command
    {
    public:
        HalveDuration(CommandFactory& inFactory,
                      ItemRegistry& inRegistry,
                      const TemporalRefVec& inTemporals);

        virtual ~HalveDuration();

        virtual void undo() override;
        virtual void redo() override;

    private:
        std::vector<ItemID> mTemporalIDs;
        virtual CommandResult executeImpl() override;

    private:
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
