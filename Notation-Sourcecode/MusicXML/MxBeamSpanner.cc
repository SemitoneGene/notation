// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxBeamSpanner.h"

// Local
#include "BeamSpecIntf.h"
#include "PlacementType.h"
#include "Throw.h"

namespace komp
{

MxBeamSpanner::MxBeamSpanner(const BeamSpecIntfUPtr& inBeamSpec)
: mBeamSpec(inBeamSpec->clone())
, mStemPolicy(StemDirection::NONE, Policy::AUTOMATIC)
{
    KOMP_ASSERT(mBeamSpec != nullptr);
}

MxBeamSpanner::~MxBeamSpanner() = default;

const BeamSpecIntfUPtr&
MxBeamSpanner::getStart() const
{
    return mBeamSpec;
}

const BeamSpecIntfUPtr&
MxBeamSpanner::getStop() const
{
    return mBeamSpec;
}

void
MxBeamSpanner::setStop(const BeamSpecIntfUPtr& inBeamSpec)
{
}

int
MxBeamSpanner::getNumberLevel() const
{
    return -1;
}


StemPolicy
MxBeamSpanner::getStemPolicy() const
{
    return mStemPolicy;
}


void
MxBeamSpanner::setStemPolicy(StemPolicy inStemPolicy)
{
    mStemPolicy = inStemPolicy;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
