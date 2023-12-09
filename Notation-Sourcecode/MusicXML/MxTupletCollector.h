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
#include "MxTupletSpec.h"
#include "TupletIntf.h"
#include "TemporalType.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{

    using MxTupletList = std::list<MxTupletSpec>;
    using MxTupletVec = std::vector<MxTupletSpec>;

    class MxTupletCollector
    {
    public:
        MxTupletCollector();

        void startTuplet(mx::api::TupletStart inStart,
                         mx::api::NoteData inFirstNote,
                         Temporal& inTemporal);

        void addToCurrentTuplet(Temporal& inTemporal);
        void stopTuplet(mx::api::TupletStop, Temporal& inTemporal);
        MxTupletVec getTupletSpecs() const;
        void insertTuplets();

    private:
        void warnIfOpenTupletsExist() const;

        bool validateAndWarn(const MxTupletSpec& inSpec,
                             TupletParams& outParams) const;

        TemporalType parseActualType(const MxTupletSpec& inSpec) const;


        TemporalType parseNormalType(const MxTupletSpec& inSpec,
                                     const TemporalType inActualType) const;

        void populateTemporals(const MxTupletSpec& inSpec,
                               TupletParams& outParams) const;

        void insertTuplet(const MxTupletSpec& inSpec,
                          TupletParams&& inParams) const;

    private:
        MxTupletList mOpenTuplets;
        MxTupletVec mClosedTuplets;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
