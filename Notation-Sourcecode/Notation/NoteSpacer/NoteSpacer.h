// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "NoteSpacerIntf.h"

namespace komp
{
    class NoteSpacer : public NoteSpacerIntf
    {
    public:

        virtual ~NoteSpacer() = default;
        NoteSpacer() = default;
        NoteSpacer(const NoteSpacer& other);
        NoteSpacer(NoteSpacer&& other) = default;
        NoteSpacer& operator=(const NoteSpacer& other);
        NoteSpacer& operator=(NoteSpacer&& other) = default;

        virtual void applySpacing(Measure& outMeasure) override;

    private:
        void centerMeasureRests(Measure& outMeasure) const;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
