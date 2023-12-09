// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeamedNote.h"

// Local
#include "Throw.h"
#include "Note.h"
#include "TemporalValue.h"

namespace komp
{
	BeamedNote::BeamedNote(Temporal& inTemporal)
    : mTemporal(inTemporal)
    , mStemNoteheadPoint(inTemporal)
    , mStemBeamsidePoint(inTemporal)
	{
        const auto type = inTemporal.getDuration().getTemporalType();
        const auto flags = TemporalValue::getNumFlags(type);
        mIsBroken.resize(flags, false);
        mBeamChunks.resize(flags, BeamChunk::Unknown);
	}
    
    
    BeamedNoteIntfUPtr
    BeamedNote::clone() const
    {
        return std::make_unique<BeamedNote>(*this);
    }


    BeamedNoteIntfUPtr
    BeamedNote::clone(Temporal& inTemporal) const
    {
        auto cloned = std::make_unique<BeamedNote>(inTemporal);
        cloned->mIsBroken = mIsBroken;
        cloned->mBeamChunks = mBeamChunks;
        return cloned;
    }
    
    
    int
    BeamedNote::getCount() const
    {
        return static_cast<int>(mIsBroken.size());
    }
    
    
    bool
    BeamedNote::getIsBroken(int inBeamLevel) const
    {
        KOMP_ASSERT(inBeamLevel >= 0);
        KOMP_ASSERT(inBeamLevel < getCount());
        return mIsBroken.at(inBeamLevel);
    }
    
    
    void
    BeamedNote::setIsBroken(int inBeamLevel, bool inValue)
    {
        KOMP_ASSERT(inBeamLevel >= 0);
        KOMP_ASSERT(inBeamLevel < getCount());
        mIsBroken.at(inBeamLevel) = inValue;
    }
    
    
    BeamChunk
    BeamedNote::getBeamChunk(int inBeamLevel) const
    {
        KOMP_ASSERT(inBeamLevel >= 0);
        KOMP_ASSERT(inBeamLevel < getCount());
        return mBeamChunks.at(inBeamLevel);
    }
    
    
    const std::vector<BeamChunk>&
    BeamedNote::getBeamChunks() const
    {
        return mBeamChunks;
    }
    
    
    void
    BeamedNote::setBeamChunk(int inBeamLevel, BeamChunk inBeamChunk)
    {
        KOMP_ASSERT(inBeamLevel >= 0);
        KOMP_ASSERT(inBeamLevel < getCount());
        mBeamChunks.at(inBeamLevel) = inBeamChunk;
    }
    
    
    Temporal&
    BeamedNote::getNote() const
    {
        return mTemporal;
    }
    
    
    bool
    BeamedNote::getDoDrawStem() const
    {
        return mDoDrawStem;
    }
    
    
    void
    BeamedNote::setDoDrawStem(bool inValue)
    {
        mDoDrawStem = inValue;
    }
    
    
    double
    BeamedNote::getStemThickness() const
    {
        return mStemThickness;
    }
    
    
    void
    BeamedNote::setStemThickness(double inValue)
    {
        KOMP_ASSERT(inValue > 0.0);
        mStemThickness = inValue;
    }
    
    
    RelativePoint&
    BeamedNote::getStemNoteheadPoint()
    {
        return mStemNoteheadPoint;
    }
    
    
    const RelativePoint&
    BeamedNote::getStemNoteheadPoint() const
    {
        return mStemNoteheadPoint;
    }
    
    
    RelativePoint&
    BeamedNote::getStemBeamsidePoint()
    {
        return mStemBeamsidePoint;
    }
    
    
    const RelativePoint&
    BeamedNote::getStemBeamsidePoint() const
    {
        return mStemBeamsidePoint;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
