// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "CommandFactory.h"
#include "Constants.h"
#include "Forward.h"


namespace komp
{
    using ClipboardStaff = std::pair<Measure*, Staff*>;

    // Contains the measure and staff range of a score selection
    struct ClipboardRange
    {
        komp::Measure* startMeasure{nullptr};
        komp::Measure* endMeasure{nullptr};
        komp::Staff* startStaff{nullptr};
        komp::Staff* endStaff{nullptr};
        
        bool isValid() const
        {
            return startMeasure
                && endMeasure
                && startStaff
                && endStaff;
        }
    };

    class Clipboard
    {
    public:
        Clipboard(CommandFactory& inFactory);
        
        void cut(const ClipboardRange& inRange);
        void copy(const ClipboardRange& inRange);
        
        void paste(Score& inScore, const PasteItemsContext& inContext);
        void paste(Score& inScore, const PasteRangeContext& inDestination);
        
        void clear();
        
    private:

        // Types
        enum class Mode {
            CUT,
            COPY,
            CLEAR,
            PASTE
        };

        CommandFactory& mFactory;
        std::unique_ptr<PasteRangeContext> mOrigin;
    };
        
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
