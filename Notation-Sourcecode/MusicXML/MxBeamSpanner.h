// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <map>

// Local
#include "Forward.h"
#include "StemPolicy.h"
#include "PlacementType.h"

namespace komp
{
    class MxBeamSpanner
    {
    public:
        MxBeamSpanner(const BeamSpecIntfUPtr& inBeamSpec);
        virtual ~MxBeamSpanner();
        virtual const BeamSpecIntfUPtr& getStart() const final;
        virtual const BeamSpecIntfUPtr& getStop() const final;
        virtual void setStop(const BeamSpecIntfUPtr& inBeamSpec) final;
        virtual int getNumberLevel() const final;
        virtual PlacementType getPlacementType() const { return PlacementType::AUTOMATIC; }
        virtual void setPlacementType( PlacementType value ) {}
        using PAYLOAD_TYPE = BeamSpecIntfUPtr;

        StemPolicy getStemPolicy() const;
        void setStemPolicy(StemPolicy inStemPolicy);

    private:
        BeamSpecIntfUPtr mBeamSpec;
        StemPolicy mStemPolicy;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
