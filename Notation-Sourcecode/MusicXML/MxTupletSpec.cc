// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxTupletSpec.h"

// Local
#include "Chord.h"
#include "Temporal.h"

namespace komp
{
    std::atomic<int> MxTupletSpec::mCounter{100000};

    MxTupletSpec::MxTupletSpec(const MusicLocation& inLocation)
    : mCreationOrder{mCounter++}
    , numberLevel{}
    , mLocation{inLocation}
    , measure{inLocation.getMeasure()}
    , track{inLocation.getTrack()}
    , hand{inLocation.getHand()}
    , voice{inLocation.getVoice()}
    , temporals{}
    , mxFirstNote{}
    , mxTupletStart{}
    , mxTupletStop{}
    {
    }


    int
    MxTupletSpec::getCreationOrder() const
    {
        return mCounter;
    }


    void
    MxTupletSpec::replaceChordMembers()
    {
        TemporalSet newSet;
        for (auto temporal : temporals) {
            if (temporal->getIsChordMember()) {
                newSet.insert(temporal->getChord());
            } else {
                newSet.insert(temporal);
            }
        }
        std::swap(temporals, newSet);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
