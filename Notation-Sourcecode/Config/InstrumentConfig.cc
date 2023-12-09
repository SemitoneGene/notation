// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "InstrumentConfig.h"

// System
#include <fstream>

// Local
#include "SoundID.h"
#include "Staff.h"
#include "Throw.h"
#include "Uuid.h"

// Local Frameworks
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace komp
{

InstrumentConfig::InstrumentConfig()
: instruments{}
, mInstrumentOrders{}
, mSoundCollections{}
,mFilepathProvider{nullptr}
{
    
}


InstrumentConfig::InstrumentConfig(ResourceFilepathProviderPtr&& inFilepathProvider)
: instruments{}
, mInstrumentOrders{}
, mSoundCollections{}
, mFilepathProvider{std::move(inFilepathProvider)}
{
    
}


InstrumentConfig::InstrumentConfig(const InstrumentConfig& other)
: instruments{other.instruments}
, mInstrumentOrders{other.mInstrumentOrders}
, mSoundCollections{other.mSoundCollections}
, mFilepathProvider{nullptr}
{
    if (other.mFilepathProvider )
    {
        mFilepathProvider = other.mFilepathProvider->clone();
    }
}


InstrumentConfig& InstrumentConfig::operator=(const InstrumentConfig& other)
{
    instruments = other.instruments;
    mInstrumentOrders = other.mInstrumentOrders;
    mSoundCollections = other.mSoundCollections;
    mFilepathProvider = other.mFilepathProvider->clone();
    return *this;
}


void
InstrumentConfig::loadFromFile(const char* const fileName)
{
    std::ifstream ifile{fileName};
    if (!ifile.is_open()) {
        KOMP_THROW( std::string{"unable to open the file "} + std::string{fileName} );
    }
    loadFromStream(ifile);
}

void
InstrumentConfig::loadFromStream(std::istream& istream)
{
    instruments.clear();
    mInstrumentOrders.clear();
    
    json root;
    istream >> root;
    
    json emptyValue;
    auto instrumentsList = root["instruments"];
    for (auto iter = instrumentsList.begin(); iter != instrumentsList.end(); ++iter) {
        const auto& instrumentNode = *iter;
        Instrument instrument;
        instrument.name = instrumentNode["name"];
        instrument.abbreviation = instrumentNode["abbreviation"];
        instrument.family = instrumentNode["family"];
        instrument.setSoundID(SoundIDFromString(instrumentNode["canonicalId"]));
        
        auto startingClefs = instrumentNode["staves"];
        for (auto c = startingClefs.begin(); c != startingClefs.end(); ++c) {
            const auto& clefNode = *c;
            const std::string clefString = clefNode;
            const auto clefType = parseClefTypeString(clefString);
            instrument.startingClefs.emplace_back(clefType);
        }
        
        auto allowableClefs = instrumentNode["allowableClefs"];
        for (auto c = allowableClefs.begin(); c != allowableClefs.end(); ++c) {
            const auto& clefNode = *c;
            const std::string clefString = clefNode;
            const auto clefType = parseClefTypeString(clefString);
            instrument.allowableClefs.emplace_back(clefType);
        }
        
        if (instrumentNode.count("transposingClef")) {
            instrument.transposingClef = parseClefTypeString(instrumentNode["transposingClef"]);
        }

        if (instrumentNode.count("concertClef")) {
            instrument.concertClef = parseClefTypeString(instrumentNode["concertClef"]);
        }

        if (instrumentNode.count("transposition")) {
            instrument.transposition = instrumentNode["transposition"];
        }

        instrument.instrumentBracket = parseGroupBracketTypeString(instrumentNode["instrumentBracket"]);
        instrument.getRangeLow().setString(instrumentNode["rangeLow"]);
        instrument.getRangeHigh().setString(instrumentNode["rangeHigh"]);
        
        if (instrumentNode.count("staffType")) {
            const auto staffType = parseStaffTypeString(instrumentNode["staffType"]);
            instrument.setStaffType(staffType);
        }
        
        if (instrumentNode.count("staffLines")) {
            instrument.setStaffLines(instrumentNode["staffLines"]);
        }
        

        instruments[instrument.getSoundID()] = instrument;
    }
    
    auto instrumentOrders = root["instrumentOrders"];
    for (auto iter = instrumentOrders.begin(); iter != instrumentOrders.end(); ++iter) {
        const auto& instrumentOrderNode = *iter;
        const std::string name = instrumentOrderNode["instrumentOrderName"];
        InstrumentOrder order;
        order.instrumentOrderName = name;
        
        const auto& namesNode = instrumentOrderNode["instrumentFamilyNames"];
        for (auto nameIter = namesNode.begin(); nameIter != namesNode.end(); ++nameIter) {
            const auto& n = *nameIter;
            order.instrumentFamilyNames.emplace_back(n.get<std::string>());
        }
        
        mInstrumentOrders.emplace_back(std::move(order));
    }
    
    auto playerConfig = root["player-config"];
    mSoundCollections.clear();
    for (auto iter = playerConfig.begin(); iter != playerConfig.end(); ++iter) {
        SoundCollection collection;
        const auto& soundCollectionNode = *iter;
        collection.setName(soundCollectionNode["collection-name"]);
        auto soundResources = soundCollectionNode["sound-resources"];
        for (auto inner = soundResources.begin(); inner != soundResources.end(); ++inner) {
            const auto& soundResourceNode = *inner;
            const std::string soundID = soundResourceNode["id"];
            const std::string path = soundResourceNode["path"];
            const auto& variationsNode = soundResourceNode["variations"];
            
            SoundVariations variations;
            if (variationsNode.contains("accent")) {
                variations.accent = variationsNode["accent"];
            }

            if (variationsNode.contains("dampened")) {
                variations.dampened = variationsNode["dampened"];
            }

            if (variationsNode.contains("pizzicato")) {
                variations.pizzicato = variationsNode["pizzicato"];
            }

            if (variationsNode.contains("staccato")) {
                variations.staccato = variationsNode["staccato"];
            }

            if (variationsNode.contains("sustain")) {
                variations.sustain = variationsNode["sustain"];
            }

            if (variationsNode.contains("tremolo")) {
                variations.tremolo = variationsNode["tremolo"];
            }
            
            collection.addSoundResource({
                SoundIDFromString(soundID),
                path,
                variations
            });
        }
        mSoundCollections.emplace_back(std::move(collection));
    }
}


void
InstrumentConfig::saveToFile(const char* const fileName) const
{
    std::ofstream ofile{fileName};
    if (!ofile.is_open()) {
        KOMP_THROW( std::string{"unable to open the file "} + std::string{fileName} );
    }
    saveToStream(ofile);
}


void
InstrumentConfig::saveToStream(std::ostream& ostream) const
{
    json root;
    json instrumentsNode;
    
    int index = 0;
    
    for (const auto& [key, instrument] : instruments) {
        json i;
        i["name"] = instrument.name.c_str();
        i["abbreviation"] = instrument.abbreviation.c_str();
        i["family"] = instrument.family.c_str();
        i["canonicalId"] = SoundIDToString(instrument.getSoundID());
        
        json startingClefsNode;
        int clefIndex = 0;
        for (const auto c : instrument.startingClefs) {
            startingClefsNode[clefIndex++] = getClefTypeString(c);
        }
        i["staves"] = startingClefsNode;
        
        json allowableClefsNode;
        clefIndex = 0;
        for (const auto c : instrument.allowableClefs) {
            allowableClefsNode[clefIndex++] = getClefTypeString(c);
        }
        i["allowableClefs"] = allowableClefsNode;
        
        
        i["instrumentBracket"] = getGroupBracketTypeString(instrument.instrumentBracket).c_str();
        i["rangeLow"] = instrument.getRangeLow().getString().c_str();
        i["rangeHigh"] = instrument.getRangeHigh().getString().c_str();
        
        instrumentsNode[index++] = i;
    }
    
    root["instruments"] = instrumentsNode;
    
    json instrumentOrdersNode;
    
    int instrumentOrderListNumber = 0;
    for (const auto& o : getInstrumentOrders()) {
        json orderNode;
        json familyNames;
        int familyNameIndex = 0;
        for (const auto& n : o.instrumentFamilyNames) {
            familyNames[familyNameIndex++] = n.c_str();
        }
        orderNode["instrumentOrderName"] = o.instrumentOrderName;
        orderNode["instrumentFamilyNames"] = familyNames;
        instrumentOrdersNode[instrumentOrderListNumber++] = orderNode;
    }
    
    root["instrumentOrders"] = instrumentOrdersNode;
    
    // TODO: serialize the "player-config" i.e. mSoundCollections
    ostream << root;
}


const std::vector<InstrumentOrder>&
InstrumentConfig::getInstrumentOrders() const
{
    return mInstrumentOrders;
}


const SoundCollections&
InstrumentConfig::getSoundCollections() const
{
    return mSoundCollections;
}


const SoundCollection&
InstrumentConfig::getSoundCollection(std::string_view inName) const
{
    const auto iter = findCollection(inName);
    KOMP_ASSERT(iter != mSoundCollections.cend());
    return *iter;
}


const SoundResource&
InstrumentConfig::getSoundResource(std::string_view inCollectionName,
                                   SoundID inSoundID) const
{
    const auto& collection = getSoundCollection(inCollectionName);
    const auto& resources = collection.getSoundResources();
    
    for (const auto& r : resources) {
        if (r.getID() == inSoundID) {
            return r;
        }
    }
    
    KOMP_THROW("bad resource name");
}


std::string
InstrumentConfig::getFilepath(std::string_view inCollectionName,
                              SoundID inSoundID) const
{
    const auto& resource = getSoundResource(inCollectionName, inSoundID);
    return resource.getFilepath(inCollectionName, mFilepathProvider);
}


SoundCollections::iterator
InstrumentConfig::findCollection(std::string_view inName)
{
    const auto isFound = [&](SoundCollection& inSoundCollection) {
        return inSoundCollection.getName() == inName;
    };
    
    auto iter = std::find_if(mSoundCollections.begin(), mSoundCollections.end(), isFound);
    KOMP_ASSERT(iter != mSoundCollections.end());
    return iter;
}


SoundCollections::const_iterator
InstrumentConfig::findCollection(std::string_view inName) const
{
    const auto isFound = [&](const SoundCollection& inSoundCollection) {
        return inSoundCollection.getName() == inName;
    };
    
    auto iter = std::find_if(mSoundCollections.begin(), mSoundCollections.end(), isFound);
    KOMP_ASSERT(iter != mSoundCollections.end());
    return iter;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
