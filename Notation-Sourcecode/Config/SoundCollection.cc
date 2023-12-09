// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "SoundCollection.h"

// Local
#include "Throw.h"

namespace komp
{
SoundCollection::SoundCollection()
: mName{}
, mPath{}
, mSoundResources{}
{
    
}


std::string_view
SoundCollection::getName() const
{
    return mName;
}


void
SoundCollection::setName(std::string inName)
{
    mName = std::move(inName);
}


const SoundResources&
SoundCollection::getSoundResources() const
{
    return mSoundResources;
}


void
SoundCollection::clearSoundResources()
{
    mSoundResources.clear();
}


void
SoundCollection::addSoundResource(SoundResource inSoundResource)
{
    mSoundResources.emplace_back(std::move(inSoundResource));
}


SoundResource
SoundCollection::findSoundResource(SoundID inID) const
{
    auto finder = [&](const SoundResource& inSoundResource) {
        return inSoundResource.getID() == inID;
    };
    
    const auto iter = std::find_if(mSoundResources.cbegin(), mSoundResources.cend(), finder);
    KOMP_ASSERT(iter != mSoundResources.cend());
    return *iter;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
