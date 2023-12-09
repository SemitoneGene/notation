// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <vector>

// Local
#include "Constants.h"
#include "Forward.h"
#include "Item.h"
#include "Iterators.h"
#include "Geometry.h"

namespace komp
{    
    class Selection
    {
    public:
        Selection(Score& inScore);
        
        bool isEmpty() const { return mItems.empty(); }
        size_t size() const { return mItems.size(); }
        
        void clear();
        void deleteItems();

        void addItem(Item& inItem);
        void removeItem(Item& inItem);
        
        Rect getFrame() const;

        /// \brief Check to see that the selection contains an item of the requested type
        /// \result true if the requested type is in the selection
        template<typename T>
        bool contains() const
        {
            for (const Item& item : mItems) {
                if (item.is<T>()) {
                    return true;
                }
            }
            return false;
        }
        
        bool contains(const Item& inItem) const
        {
            for (const Item& item : mItems) {
                if (item.getItemID() == inItem.getItemID()) {
                    return true;
                }
            }
            return false;
        }

        /// \brief Return the first item of the requested type in the selection
        /// \result The item found or nullptr
        template<typename T>
        T* front() const
        {
            for (Item& item : mItems) {
                if (item.is<T>()) {
                    auto result = dynamic_cast<T*>(&item);
                    return result;
                }
            }
            return nullptr;
        }
        
        /// \brief Return the last item of the requested type in the selection
        /// \result The item found or nullptr
        template<typename T>
        T* back() const
        {
            for (Item& item : reverse(mItems)) {
                if (item.is<T>()) {
                    auto result = dynamic_cast<T*>(&item);
                    return result;
                }
            }
            return nullptr;
        }
        
        
        /// \brief Return a list of all items of type in the selection
        /// \result A populated list of items
        template<typename T>
        std::vector<std::reference_wrapper<T>> getItems() const
        {
            std::vector<std::reference_wrapper<T>> items;
            for (Item& item : mItems) {
                if (item.is<T>()) {
                    const auto typedItem = dynamic_cast<T*>(&item);
                    items.push_back(*typedItem);
                }
            }
            return items;
        }

    private:
        Selection& operator=(const Selection& other);

        ItemRefVec mItems;
        Score& mScore;
    };
    
}   // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
