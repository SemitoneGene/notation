// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#ifndef LRUCACHE_H_INCLUDED
#define	LRUCACHE_H_INCLUDED

// System
#include <cstddef>
#include <list>
#include <stdexcept>
#include <unordered_map>

namespace komp
{    
    template<typename key_t, typename value_t>
    
    class LRUCache {
    
    public:
        typedef typename std::pair<key_t, value_t> key_value_pair_t;
        typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;
        
        LRUCache(size_t inMaxSize)
        : mMaxSize(inMaxSize)
        {
        }
        
        void put(const key_t& key, const value_t& value)
        {
            auto it = mItemsMap.find(key);
            mItemsList.push_front(key_value_pair_t(key, value));
            if (it != mItemsMap.end()) {
                mItemsList.erase(it->second);
                mItemsMap.erase(it);
            }
            mItemsMap[key] = mItemsList.begin();
            
            if (mItemsMap.size() > mMaxSize) {
                auto last = mItemsList.end();
                last--;
                mItemsMap.erase(last->first);
                mItemsList.pop_back();
            }
        }
        
        const value_t&
        get(const key_t& key)
        {
            auto it = mItemsMap.find(key);
            if (it == mItemsMap.end()) {
                throw std::range_error("There is no such key in cache");
            } else {
                mItemsList.splice(mItemsList.begin(), mItemsList, it->second);
                return it->second->second;
            }
        }
        
        bool
        exists(const key_t& key) const
        {
            return mItemsMap.find(key) != mItemsMap.end();
        }
        
        size_t
        size() const
        {
            return mItemsMap.size();
        }
        
    private:
        std::list<key_value_pair_t> mItemsList;
        std::unordered_map<key_t, list_iterator_t> mItemsMap;
        size_t mMaxSize;
    };
    
} // namespace lru

#endif	// LRUCACHE_H_INCLUDED

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

