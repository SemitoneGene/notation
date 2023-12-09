// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>
#include <set>
#include <string>

// Local
#include "Forward.h"
#include "ItemType.h"

namespace komp
{
/// \brief  Information about a sub-class of Item
///
/// \discussion Alows us to get information about a
/// sub-class of Item without downcasting
///
class ItemTraits
{
public:
    /// \brief Get the flyweight instantiated value for the given inType
    static const ItemTraits& getItemTraits(ItemType inType);
    
    ItemType getItemType() const;
    int getSortID() const;
    std::string_view getName() const;
    
    /// \discussion returns -1 if inLeft < inRight
    /// returns 0 if inLeft == inRight
    /// returns 1 if inLeft > inRight
    static int compareSortIDs(const ItemTraits& inLeft, const ItemTraits& inRight);
    
private:
    friend class ItemTraitsRepo;
    ItemTraits(ItemType inItemType);
    const ItemType mItemType;
    const std::string mName;
    const int mSortID;
};

///@{ These compare SortID
bool operator<(const ItemTraits& inLeft, const ItemTraits& inRight);
bool operator>(const ItemTraits& inLeft, const ItemTraits& inRight);
bool operator<=(const ItemTraits& inLeft, const ItemTraits& inRight);
bool operator>=(const ItemTraits& inLeft, const ItemTraits& inRight);
bool operator==(const ItemTraits& inLeft, const ItemTraits& inRight);
bool operator!=(const ItemTraits& inLeft, const ItemTraits& inRight);
///@}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
