// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ItemRegistry.h"

// Local
#include "Item.h"
#include "Score.h"

namespace komp
{
    ItemRegistry::ItemRegistry()
    : mCurrentId(0)
    {
        
    }
    

    Measure*
    ItemRegistry::getMeasure(const Item& inItem) const
    {
        const auto& registrant = findItem(inItem.getItemID());
        return registrant.measure;
    }
    
    
    void
    ItemRegistry::setMeasure(const Item& inItem, Measure* inMeasure)
    {
        auto& registrant = findItem(inItem.getItemID());
        registrant.measure = inMeasure;
    }


    Item&
    ItemRegistry::getItem(ItemID inItemID) const
    {
        const auto& registrant = findItem(inItemID);
        return *registrant.item.get();
    }


    bool
    ItemRegistry::getIsQuiescent() const
    {
        for (const auto& registrant : mItems) {
            if (!registrant.second.item->getIsQuiescent()) {
                return false;
            }
        }
        return true;
    }


    void
    ItemRegistry::setIsQuiescent(bool inValue)
    {
        for (const auto& registrant : mItems) {
            registrant.second.item->setIsQuiescent(inValue);
        }
    }


    ItemRegistry::Registrant&
    ItemRegistry::findItem(ItemID inItemID)
    {
        auto& item = mItems.at(inItemID);
        return item;
    }
    
    
    const ItemRegistry::Registrant&
    ItemRegistry::findItem(ItemID inItemID) const
    {
        auto& item = mItems.at(inItemID);
        return item;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
