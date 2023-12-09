// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "SoundResource.h"

// Local
#include "ResourceFilepathProviderIntf.h"

namespace komp
{
SoundResource::SoundResource()
{
    
}


SoundResource::SoundResource(SoundID inSoundID,
                             std::string inPath,
                             SoundVariations inVariations)
: mID{inSoundID}
, mRootPath{std::move(inPath)}
, mVariations{std::move(inVariations)}
{
    
}


SoundID
SoundResource::getID() const
{
    return mID;
}


std::string
SoundResource::getFilepath(std::string_view inCollectionName,
                           const ResourceFilepathProviderPtr& inFilepathProvider) const
{
    const auto filePath = inFilepathProvider->getFilepath(inCollectionName, mRootPath);
    return filePath;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
