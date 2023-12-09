// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Self
#include "Item.h"

// Local
#include "Measure.h"
#include "Score.h"
#include "ScoreColors.h"

namespace komp
{
Item::Item()
: mItemID(std::numeric_limits<ItemID>::max())
{
}

ItemID
Item::getItemID() const
{
    return mItemID;
}

void
Item::setItemID(ItemID inValue)
{
    mItemID = inValue;    
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
