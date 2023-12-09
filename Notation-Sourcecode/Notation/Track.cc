// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Track.h"

// Local
#include "Clef.h"
#include "Score.h"

namespace komp
{
Track::Track(const Score& inScore, const TrackInfo& inInfo)
: mScore{inScore}
, mUuid{inInfo.getUuid()}
, mInstrument{inInfo.getInstrument()}
{
    KOMP_ASSERT(mInstrument.startingClefs.size() >= 1);
}

int
Track::getIndex() const
{
    return mScore.getTrackIndex(*this);
}

const Instrument&
Track::getInstrument() const
{
    return mInstrument;
}

void
Track::setInstrument(const Instrument& inInstrument)
{
    // TODO: check for changes and update if needed
    mInstrument = inInstrument;
}

void
Track::addHand(ClefType inClefType, int inHand)
{
    KOMP_ASSERT(inHand >= 0);
    
    if (inHand > getHands()) {
        KOMP_THROW("inHand must be no greater than lastHand + 1");
    }
    
    if (inHand == static_cast<int>(mInstrument.startingClefs.size())) {
        mInstrument.startingClefs.push_back(inClefType);
    } else {
        const auto iter = mInstrument.startingClefs.cbegin() + static_cast<size_t>(inHand);
        mInstrument.startingClefs.insert(iter, inClefType);
    }
}

void
Track::removeHand(int inHand)
{
    KOMP_ASSERT(inHand >= 0);
    
    if (inHand > getHands()) {
        KOMP_THROW("index out of range");
    }
    
    const auto iter = mInstrument.startingClefs.cbegin() + static_cast<size_t>(inHand);
    mInstrument.startingClefs.erase(iter);
}

int
Track::getHands() const
{
    return static_cast<int>(mInstrument.startingClefs.size());
}


ClefType
Track::getClefType(int inIndex) const
{
    return mInstrument.startingClefs.at(static_cast<size_t>(inIndex));
}


void
Track::setClefType(int inHand, ClefType inValue)
{
    mInstrument.startingClefs.at(static_cast<size_t>(inHand)) = inValue;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
