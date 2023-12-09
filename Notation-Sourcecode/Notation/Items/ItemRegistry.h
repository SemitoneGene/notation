// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <atomic>
#include <map>
#include <memory>

// Local frameworks
#include <parallel_hashmap/phmap.h>

// Local
#include "Forward.h"
#include "Item.h"

namespace komp
{
template <class K, class V>
using ParallelHashMap = phmap::parallel_node_hash_map<K, V>;
}

namespace komp
{
class ItemRegistry
{
public:
    ItemRegistry();
    ~ItemRegistry() = default;
    
    /// ItemRegistry cannot be copy-constructed
    ItemRegistry(const ItemRegistry& other) = delete;
    
    /// ItemRegistry cannot be move-constructed
    ItemRegistry(ItemRegistry&& other) = delete;
    
    /// ItemRegistry cannot be copy-assigned
    ItemRegistry& operator=(const ItemRegistry& other) = delete;
    
    /// ItemRegistry cannot be move-assigned
    ItemRegistry& operator=(ItemRegistry&& other) = delete;
    
    /// \brief  Creates a Score Item
    ///
    /// \discussion All Items and decendents of Item must be constructed
    /// using this method in order to keep the Item registry accurate.
    /// Any item that is created outside of this method and inserted in
    /// the score will cause undefined behavior, especially related to
    /// undo redo and clipboard operations
    ///
    template <typename T, typename... Args>
    T& createItem(Args&&... a)
    {
        static_assert(std::is_base_of<Item, T>::value, "Type must be derived from Item");
        auto ptr = std::unique_ptr<T>{new T{std::forward<Args>(a)...}};
        
        const auto itemID = mCurrentId++;
        ptr->setItemID(itemID);
        ptr->createDrawable();
        ptr->itemIDChanged();
        
        Registrant registrant;
        registrant.measure = nullptr;
        registrant.item = std::move(ptr);
        mItems[itemID] = std::move(registrant);
        
        auto& item = *mItems[itemID].item.get();
        return item.as<T>();
    }
    
    /// \brief find and return the measure to which the inItem belongs
    /// \result The item measure or nullptr if the item is not inserted in a measure
    Measure* getMeasure(const Item& inItem) const;
    
    /// find and set the measure to which the inItem belongs. would
    /// be a horrible mess if this were to get out-of-sync with the
    /// measure's Item vector
    void setMeasure(const Item& inItem, Measure* inMeasure);
    
    /// find and return the item. throws if inItemID is bad
    Item& getItem(ItemID inItemID) const;
    
    /// When an Item is Quiescent it will refrain from sending or responding
    /// to ItemObserver notifications. The registry is Quiescent when all of
    /// its items are quiescent.
    bool getIsQuiescent() const;
    
    /// When an Item is Quiescent it will refrain from sending or responding
    /// to ItemObserver notifications. The registry is Quiescent when all of
    /// its items are quiescent.
    void setIsQuiescent(bool inValue);
    
private:
    struct Registrant
    {
        ItemUPtr item;
        Measure* measure;
    };
    
    std::atomic<ItemID> mCurrentId;
    ParallelHashMap<ItemID, Registrant> mItems;
    
private:
    Registrant& findItem(ItemID inItemID);
    const Registrant& findItem(ItemID inItemID) const;
};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
