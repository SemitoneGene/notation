// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <queue>
#include <vector>
#include <set>
#include <map>

// Komp
#include "Wedge.h"
#include "Octave.h"
#include "MxSpannerManager.h"
#include "MxNoteSpanner.h"
#include "MxOttava.h"
#include "MxBeamSpanner.h"
#include "MxEndingCollector.h"
#include "MxTupletCollector.h"
#include "MxTieCollector.h"
#include "MxWedgeCollector.h"

// Mx
#include "mx/api/TupletData.h"

namespace komp
{
    struct MxSpannerCollector
    {
        MxSpannerCollector();

        MxSpannerManager<MxNoteSpanner> slurMgr;
        MxTieCollector ties;
        MxWedgeCollector wedges;
        MxSpannerManager<MxOttava> ottavaMgr;
        MxSpannerManager<MxBeamSpanner> beamMgr;
        MxTupletCollector tuplets;
        MxEndingCollector endings;
    };
}
