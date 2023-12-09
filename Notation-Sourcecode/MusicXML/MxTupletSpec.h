// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>

// Local
#include "Forward.h"
#include "MusicLocation.h"

// Local Frameworks
#include "mx/api/ScoreData.h"

namespace komp
{
    using TemporalSet = std::set<Temporal*>;

    class MxTupletSpec
    {
    public:
        
        MxTupletSpec(const MusicLocation& inLocation);
        
        MxTupletSpec() = delete;

        int numberLevel;
        Measure& measure;
        Track& track;
        int hand;
        int voice;
        TemporalSet temporals;
        mx::api::NoteData mxFirstNote;
        mx::api::TupletStart mxTupletStart;
        mx::api::TupletStop mxTupletStop;

        int getCreationOrder() const;

        /// if a temporal is a member of a chord, this will remove it
        /// from the set and instead insert the chord parent
        void replaceChordMembers();

    private:
        int mCreationOrder;
        MusicLocation mLocation;
        
        static std::atomic<int> mCounter;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
