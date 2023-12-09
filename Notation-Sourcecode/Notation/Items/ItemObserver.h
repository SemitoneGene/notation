// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

// System
#include <algorithm>
#include <set>

namespace komp
{
enum class ItemChangeType
{
    Beam,          ///< The Beam Updated or Changed
    Chord,         ///< The Chord Updated or Changed
    Duration,      ///< Some aspect of the Duration changed
    Hand,          ///< The Hand Changed
    Inserted,      ///< The Item was inserted to the score
    Measure,       ///< The Measure Changed
    Pitch,          ///< Some aspect of the Pitch changed
    Removed,        ///< The Item was removed from the score
    StemDirection,  ///< The StemDirection Changed
    TemporalType,   ///< The TemporalType Changed
    Time,           ///< The Time Changed
    Updated,        ///< The Item needs to be redrawn
    Voice,         ///< The Voice Changed
    X,             ///< The X value changed
    Slot,          ///< The slot changed
};

using ItemChanges = std::set<ItemChangeType>;

class ItemObserver
{
public:
    virtual ~ItemObserver() {};
    
    /// an item should call this function on its observers when its
    /// state changes.
    /// @param observedItem - the Item that changed
    /// @param inChanges - the list of values that changed on the item
    ///
    virtual void observedItemChanged(const Item& observedItem,
                                     const ItemChanges& inChanges) = 0;
    
    static inline bool contains(const ItemChanges& inChanges,
                                ItemChangeType inType)
    {
        return inChanges.contains(inType);
    }
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
