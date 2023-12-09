// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <unordered_map>

// Local
#include "Instrument.h"
#include "InstrumentOrder.h"
#include "ResourceFilepathProviderIntf.h"
#include "SoundCollection.h"
#include "SoundID.h"


namespace komp
{
    class InstrumentConfig
    {
        
    public:
        InstrumentConfig();
        InstrumentConfig(ResourceFilepathProviderPtr&& inFilepathProvider);
        InstrumentConfig(const InstrumentConfig& other);
        InstrumentConfig(InstrumentConfig&& other) = default;
        InstrumentConfig& operator=(const InstrumentConfig& other);
        InstrumentConfig& operator=(InstrumentConfig&& other) = default;

        /// clears the objects state and replaces it
        /// by reading from a file. throws
        void loadFromFile(const char* const fileName);
        
        /// clears the objects state and replaces it
        /// by reading from a stream. throws
        void loadFromStream(std::istream& istream);
        
        /// saves the object's state to a file. throws
        void saveToFile(const char* const fileName) const;
       
        /// saves the object's state to a file. throws
        void saveToStream(std::ostream& ostream) const;
        
        std::unordered_map<SoundID, Instrument> instruments;
        
        /// returns a collection of score order lists
        const std::vector<InstrumentOrder>& getInstrumentOrders() const;

        /// sound information
        const SoundCollections& getSoundCollections() const;

        /// throws if inName is bad
        const SoundCollection& getSoundCollection(std::string_view inName) const;

        /// throws if either inCollection name or inSoundID is bad
        const SoundResource& getSoundResource(std::string_view inCollectionName,
                                              SoundID inSoundID) const;

        /// throws if either inCollection name or inSoundID is bad
        std::string getFilepath(std::string_view inCollectionName,
                                SoundID inSoundID) const;

    private:
        std::vector<InstrumentOrder> mInstrumentOrders;
        SoundCollections mSoundCollections;
        ResourceFilepathProviderPtr mFilepathProvider;

    private:
        SoundCollections::iterator findCollection(std::string_view inName);
        SoundCollections::const_iterator findCollection(std::string_view inName) const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
