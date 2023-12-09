// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ItemTraitsRepo.h"

// Local
#include "Throw.h"
#include "ItemTraits.h"

namespace komp
{
ItemTraitsInfoMaps::ItemTraitsInfoMaps()
: mNames{}
, mSortIDs{}
{
    // please keep this instantiation ordered alphabetically
    mNames.emplace(ItemType::Beam,          "Beam");
    mNames.emplace(ItemType::Chord,         "Chord");
    mNames.emplace(ItemType::ChordSymbol,   "ChordSymbol");
    mNames.emplace(ItemType::Clef,          "Clef");
    mNames.emplace(ItemType::Data,          "Data");
    mNames.emplace(ItemType::Ending,        "Ending");
    mNames.emplace(ItemType::KeySignature,  "KeySignature");
    mNames.emplace(ItemType::LineSpanner,   "LineSpanner");
    mNames.emplace(ItemType::Mark,          "Mark");
    mNames.emplace(ItemType::Marker,        "Marker");
    mNames.emplace(ItemType::Note,          "Note");
    mNames.emplace(ItemType::Octave,        "Octave");
    mNames.emplace(ItemType::RawAccidental, "RawAccidental");
    mNames.emplace(ItemType::RehearsalMark, "RehearsalMark");
    mNames.emplace(ItemType::Rest,          "Rest");
    mNames.emplace(ItemType::Slur,          "Slur");
    mNames.emplace(ItemType::TempoMark,     "TempoMark");
    mNames.emplace(ItemType::Text,          "Text");
    mNames.emplace(ItemType::Tie,           "Tie");
    mNames.emplace(ItemType::TimeSignature, "TimeSignature");
    mNames.emplace(ItemType::Tuplet,        "Tuplet");
    mNames.emplace(ItemType::Wedge,         "Wedge");
    
    KOMP_ASSERT(mNames.size() == static_cast<size_t>(ItemType::NumberOfItemTypes));
    
    // please keep this instantiation ordered by the value of map->second
    mSortIDs.emplace(ItemType::Clef,          100101);
    mSortIDs.emplace(ItemType::KeySignature,  100202);
    mSortIDs.emplace(ItemType::TimeSignature, 100303);
    mSortIDs.emplace(ItemType::TempoMark,     100404);
    mSortIDs.emplace(ItemType::ChordSymbol,   100450);
    mSortIDs.emplace(ItemType::Text,          100505);
    mSortIDs.emplace(ItemType::Tie,           400101);
    mSortIDs.emplace(ItemType::Slur,          400202);
    mSortIDs.emplace(ItemType::Wedge,         400303);
    mSortIDs.emplace(ItemType::LineSpanner,   400333);
    mSortIDs.emplace(ItemType::Octave,        400404);
    mSortIDs.emplace(ItemType::Ending,        400405);
    mSortIDs.emplace(ItemType::Mark,          600505);
    mSortIDs.emplace(ItemType::RawAccidental, 600606);
    mSortIDs.emplace(ItemType::Data,          600707);
    mSortIDs.emplace(ItemType::Marker,        600808);
    mSortIDs.emplace(ItemType::RehearsalMark, 600909);
    mSortIDs.emplace(ItemType::Tuplet,        700505);
    mSortIDs.emplace(ItemType::Beam,          700909);
    mSortIDs.emplace(ItemType::Chord,         900101);
    mSortIDs.emplace(ItemType::Note,          900202);
    mSortIDs.emplace(ItemType::Rest,          900303);
    
    KOMP_ASSERT(mSortIDs.size() == static_cast<size_t>(ItemType::NumberOfItemTypes));
    
    std::set<int> sortIDs{};
    for (const auto& pair : mSortIDs) {
        const auto result = sortIDs.emplace(pair.second);
        if (!result.second) {
            KOMP_THROW("error instantiating ItemTypeTraits, duplicate SortIDs are not allowed");
        }
    }
    
    std::set<std::string> names{};
    for (const auto& pair : mNames) {
        const auto result = names.emplace(pair.second);
        if (!result.second) {
            KOMP_THROW("error instantiating ItemTypeTraits, duplicate names are not allowed");
        }
    }
}


const ItemTraitsInfoMaps&
ItemTraitsInfoMaps::getInstance()
{
    static ItemTraitsInfoMaps instance{};
    return instance;
}


std::string_view
ItemTraitsInfoMaps::getName(ItemType inType) const
{
    return getInstance().mNames.at(inType);
}


const int
ItemTraitsInfoMaps::getSortID(ItemType inType) const
{
    return getInstance().mSortIDs.at(inType);
}


ItemTraitsRepo::ItemTraitsRepo()
: mItemTraits{}
{
    // please keep this instantiation ordered alphabetically
    mItemTraits.emplace(ItemType::Beam,          ItemTraits{ItemType::Beam});
    mItemTraits.emplace(ItemType::Chord,         ItemTraits{ItemType::Chord});
    mItemTraits.emplace(ItemType::Clef,          ItemTraits{ItemType::Clef});
    mItemTraits.emplace(ItemType::ChordSymbol,   ItemTraits{ItemType::ChordSymbol});
    mItemTraits.emplace(ItemType::Data,          ItemTraits{ItemType::Data});
    mItemTraits.emplace(ItemType::Ending,        ItemTraits{ItemType::Ending});
    mItemTraits.emplace(ItemType::KeySignature,  ItemTraits{ItemType::KeySignature});
    mItemTraits.emplace(ItemType::LineSpanner,   ItemTraits{ItemType::LineSpanner});
    mItemTraits.emplace(ItemType::Mark,          ItemTraits{ItemType::Mark});
    mItemTraits.emplace(ItemType::Marker,        ItemTraits{ItemType::Marker});
    mItemTraits.emplace(ItemType::Note,          ItemTraits{ItemType::Note});
    mItemTraits.emplace(ItemType::Octave,        ItemTraits{ItemType::Octave});
    mItemTraits.emplace(ItemType::RehearsalMark, ItemTraits{ItemType::RehearsalMark});
    mItemTraits.emplace(ItemType::Rest,          ItemTraits{ItemType::Rest});
    mItemTraits.emplace(ItemType::Slur,          ItemTraits{ItemType::Slur});
    mItemTraits.emplace(ItemType::TempoMark,     ItemTraits{ItemType::TempoMark});
    mItemTraits.emplace(ItemType::Text,          ItemTraits{ItemType::Text});
    mItemTraits.emplace(ItemType::Tie,           ItemTraits{ItemType::Tie});
    mItemTraits.emplace(ItemType::TimeSignature, ItemTraits{ItemType::TimeSignature});
    mItemTraits.emplace(ItemType::Tuplet,        ItemTraits{ItemType::Tuplet});
    mItemTraits.emplace(ItemType::Wedge,         ItemTraits{ItemType::Wedge});
    mItemTraits.emplace(ItemType::RawAccidental, ItemTraits{ItemType::RawAccidental});
    
    KOMP_ASSERT(mItemTraits.size() == static_cast<size_t>(ItemType::NumberOfItemTypes));
    
    std::set<ItemTraits> iTraits{};
    for (const auto& pair : mItemTraits) {
        const auto result = iTraits.emplace(pair.second);
        if (!result.second) {
            KOMP_THROW("error instantiating ItemTypeTraits, duplicate ItemTraits instances are not allowed");
        }
    }
}


const ItemTraitsRepo&
ItemTraitsRepo::getInstance()
{
    static ItemTraitsRepo instance{};
    return instance;
}


const ItemTraits&
ItemTraitsRepo::getItemTraits(ItemType inType) const
{
    return getInstance().mItemTraits.at(inType);
}


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
