// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ItemTraits.h"

// Local
#include "Throw.h"
#include "ItemTraitsRepo.h"

namespace komp
{
ItemTraits::ItemTraits(ItemType inItemType)
: mItemType{inItemType}
, mName{ItemTraitsInfoMaps::getInstance().getName(inItemType)}
, mSortID{ItemTraitsInfoMaps::getInstance().getSortID(inItemType)}
{
    
}


const ItemTraits&
ItemTraits::getItemTraits(ItemType inType)
{
    return ItemTraitsRepo::getInstance().getItemTraits(inType);
}


ItemType
ItemTraits::getItemType() const
{
    return mItemType;
}


int
ItemTraits::getSortID() const
{
    return mSortID;
}


std::string_view
ItemTraits::getName() const
{
    return mName;
}


int
ItemTraits::compareSortIDs(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    const auto left = inLeft.getSortID();
    const auto right = inRight.getSortID();
    
    if (left < right) {
        return -1;
    }
    
    if (left > right) {
        return 1;
    }
    
    return 0;
}

bool
operator<(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    return -1 == ItemTraits::compareSortIDs(inLeft, inRight);
}


bool
operator>(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    return 1 == ItemTraits::compareSortIDs(inLeft, inRight);
}


bool
operator<=(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    return (inLeft < inRight) || (inLeft == inRight);
}


bool
operator>=(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    return (inLeft > inRight) || (inLeft == inRight);
}

bool
operator==(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    return 0 == ItemTraits::compareSortIDs(inLeft, inRight);
}


bool
operator!=(const ItemTraits& inLeft, const ItemTraits& inRight)
{
    return !(inLeft == inRight);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
