// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>
#include <list>

// Local
#include "Forward.h"
#include "MxTieSpec.h"
#include "TemporalType.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{

    using MxTieList = std::list<MxTieSpec>;
    using MxTieVec = std::vector<MxTieSpec>;

    class MxTieCollector
    {
    public:
        MxTieCollector();

        void startTie(const mx::api::NoteData& inNoteData,
                      Temporal& inTemporal);
        void finishTie(const mx::api::NoteData& inNoteData,
                       Temporal& inTemporal);

        MxTieVec getTieSpecs() const;
        void insertTies(Score& inScore);

    private:
        void warnIfOpenTiesExist() const;

        bool validateAndWarn(const MxTieSpec& inSpec) const;

        bool validateAndWarnNulls(const MxTieSpec& inSpec) const;
        TemporalType parseActualType(const MxTieSpec& inSpec) const;

    private:
        MxTieList mOpenTies;
        MxTieVec mClosedTies;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
