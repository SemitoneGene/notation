// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <map>

// Local
#include "Spatial.h"

namespace komp
{
    class Temporal;

    class MxNoteSpanner;
    using MxNoteSpannerUPtr = std::unique_ptr<MxNoteSpanner>;
    
    class MxNoteSpanner
    {
    public:
        MxNoteSpanner(int inNumberLevel, Temporal& inStartTemporal);
        virtual ~MxNoteSpanner() {};
        virtual Temporal& getStart() const final;
        virtual Temporal& getStop() const final;
        virtual void setStop(Temporal& inTemporal) final;
        virtual int getNumberLevel() const final;
        virtual PlacementType getPlacementType() const { return mPlacementType; }
        virtual void setPlacementType( PlacementType value ) { mPlacementType = value; }
        using PAYLOAD_TYPE = Temporal;
        
    private:
        int mNumberLevel;
        Temporal* mStart;
        Temporal* mStop;
        PlacementType mPlacementType;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
