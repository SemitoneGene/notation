// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "SoundResource.h"

#include <string>
#include <vector>

namespace komp
{
/// \brief  A group of sound resources.
///
/// \discussion A group of sound resources.
///
class SoundCollection
{
public:
    SoundCollection();
    
public:
    std::string_view getName() const;
    void setName(std::string inName);
    const SoundResources& getSoundResources() const;
    void clearSoundResources();
    void addSoundResource(SoundResource inSoundResource);
    SoundResource findSoundResource(SoundID inID) const;
    
private:
    std::string mName;
    std::string mPath;
    SoundResources mSoundResources;
};

using SoundCollections = std::vector<SoundCollection>;
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
