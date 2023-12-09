// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <string>
#include <vector>

// Local
#include "SoundID.h"

namespace komp
{
class ResourceFilepathProviderIntf;
using ResourceFilepathProviderPtr = std::unique_ptr<ResourceFilepathProviderIntf>;

struct SoundVariations
{
    std::string accent;
    std::string dampened;
    std::string pizzicato;
    std::string staccato;
    std::string sustain;
    std::string tremolo;
};


/// \brief  Soundfont Info.
///
/// \discussion Filenames and ID's of the sound resources.
///
class SoundResource
{
public:
    SoundResource();
    SoundResource(SoundID inSoundID,
                  std::string inPath,
                  SoundVariations inVariations);
    
public:
    SoundID getID() const;
    std::string getFilepath(std::string_view inCollectionName,
                            const ResourceFilepathProviderPtr& inFilepathProvider) const;
    
    
    const SoundVariations& getVariations() const { return mVariations; }
    
private:
    SoundID mID;
    std::string mRootPath;
    SoundVariations mVariations;
};

using SoundResources = std::vector<SoundResource>;
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
