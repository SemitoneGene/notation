// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>
#include <string>

// Local
#include "Forward.h"
#include "ItemType.h"

namespace komp
{
    /// \brief  private Flyweight container
    ///
    /// \discussion private Flyweight container
    ///
    class ItemTraitsInfoMaps
    {
    public:
        static const ItemTraitsInfoMaps& getInstance();
        std::string_view getName(ItemType inType) const;
        const int getSortID(ItemType inType) const;

    private:
        ItemTraitsInfoMaps();
        std::map<const ItemType, const std::string> mNames;
        std::map<const ItemType, const int> mSortIDs;
    };

    /// \brief  private Flyweight container
    ///
    /// \discussion private Flyweight container
    ///
    class ItemTraitsRepo
    {
    public:
        static const ItemTraitsRepo& getInstance();
        const ItemTraits& getItemTraits(ItemType inType) const;

    private:
        ItemTraitsRepo();
        std::map<const ItemType, const ItemTraits> mItemTraits;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
