// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "SoundResource.h"

#include <string>
#include <memory>

namespace komp
{
    class ResourceFilepathProviderIntf;
    using ResourceFilepathProviderPtr = std::unique_ptr<ResourceFilepathProviderIntf>;

    /// \brief  Gets filepaths to resources.
    ///
    /// \discussion Allows the lookup of resource filepaths to be
    /// abstracted away from the platform.  In iOS we are using
    /// tagged downloadable resources so Komp iOS will implement
    /// this interface to pass into the Player class.
    ///
    /// inResourceCollection is the 'tag' in iOS for a collection
    /// of downloadable files. This also corresponds to the player-
    /// config 'collection-name' in instruments.json.
    ///
    /// inResourceFilenameIncludingExtension is the filename, for
    /// example 'AltoSax.sfz'.  This corresponds to the 'player-
    /// config' -> 'filename' in instruments.json
    ///
    class ResourceFilepathProviderIntf
    {

    public:
        virtual ~ResourceFilepathProviderIntf() = default;
        virtual ResourceFilepathProviderPtr clone() const = 0;

    public:

        /// get the full filepath to the resource
        virtual std::string getFilepath(std::string_view inResourceCollection,
                                        std::string_view inResourceFilenameIncludingExtension) = 0;
    };

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
