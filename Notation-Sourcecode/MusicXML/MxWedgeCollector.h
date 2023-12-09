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
#include "MxWedgeSpec.h"
#include "Wedge.h"
#include "TemporalType.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{
    class MxScoreReader;
    using MxWedgeList = std::list<MxWedgeSpec>;
    using MxWedgeVec = std::vector<MxWedgeSpec>;

    class MxWedgeCollector
    {
    public:
        MxWedgeCollector();

        void startWedge(Score& inScore,
                        mx::api::WedgeStart inStart,
                        mx::api::Placement inPlacement,
                        mx::api::NoteData inFirstNote,
                        MxLocation inLocation);

        void stopWedge(mx::api::WedgeStop inWedgeStop,
                       MxLocation inMxLocation);

        MxWedgeVec getWedgeSpecs() const;
        void insertWedges(const MxScoreReader& inMxScoreReader);

    private:
        void warnIfOpenWedgesExist() const;

        bool validateAndWarn(const WedgeParams& inParams) const;

        bool validateAndWarnNulls(const WedgeParams& inParams) const;

        void insertWedge(const MxScoreReader& inMxScoreReader,
                         const WedgeParams& inParams,
                         const MxWedgeSpec& inSpec) const;

    private:
        MxWedgeList mOpenWedges;
        MxWedgeVec mClosedWedges;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
