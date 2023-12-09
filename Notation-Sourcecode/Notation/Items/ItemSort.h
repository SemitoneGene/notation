// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Item.h"

namespace komp
{
    Temporal* convertItemToTemporal(Item& inItem);

    int compareTemporals(const Temporal& inLeft, const Temporal& inRight);
    int compareMeasureIndex(const Item& inLeft, const Item& inRight);
    int compareTrackIndex(const Item& inLeft, const Item& inRight);

    /// A re-usable sorting comparison function (i.e. for std::sort) which aranges
    /// items by Measure, Track, Hand, Voice, Time, Non-Temporals-before-other-Temporals,
    /// [some additional Item type ordering], Chords-before-Notes-and-Rests, slot,
    /// midiPitch, x-position, ID, then address
    /// this is a template so that it will work with, for example
    /// both ItemRefVec and TemporalRefVec
    template <typename I>
    bool isItemLessThan(Item& inLeft, Item& inRight)
    {
        const auto measureCompareResult = compareMeasureIndex(inLeft, inRight);

        if (measureCompareResult == -1) {
            return true;
        } else if (measureCompareResult == 1) {
            return false;
        }

        const auto trackCompareResult = compareTrackIndex(inLeft, inRight);
        if (trackCompareResult == -1) {
            return true;
        } else if (trackCompareResult == 1) {
            return false;
        }

        const auto& leftLoc = inLeft.getLocation();
        const auto& rightLoc = inRight.getLocation();
        
        if (leftLoc.getHand() < rightLoc.getHand()) {
            return true;
        } else if (leftLoc.getHand() > rightLoc.getHand()) {
            return false;
        }

        if (leftLoc.getVoice() < rightLoc.getVoice()) {
            return true;
        } else if (leftLoc.getVoice() > rightLoc.getVoice()) {
            return false;
        }

        if (inLeft.getTime() < inRight.getTime()) {
            return true;
        } else if (inLeft.getTime() > inRight.getTime()) {
            return false;
        }

        const auto leftSortID = inLeft.getItemTraits().getSortID();
        const auto rightSortID = inRight.getItemTraits().getSortID();

        if (leftSortID < rightSortID) {
            return true;
        } else if (leftSortID > rightSortID) {
            return false;
        }

        if (inLeft.getSlot() < inRight.getSlot()) {
            return true;
        } else if (inLeft.getSlot() > inRight.getSlot()) {
            return false;
        }

        const auto leftTemporal = convertItemToTemporal(inLeft);
        const auto rightTemporal = convertItemToTemporal(inRight);

        if (leftTemporal && rightTemporal) {
            const int temporalResult = compareTemporals(*leftTemporal, *rightTemporal);

            if (temporalResult == -1) {
                return true;
            } else if (temporalResult == 1) {
                return false;
            }
        }

        if (inLeft.getX() < inRight.getX()) {
            return true;
        } else if (inLeft.getX() > inRight.getX()) {
            return false;
        }

        if (!leftTemporal || !rightTemporal) {
            if (inLeft.getSlot() < inRight.getSlot()) {
                return true;
            } else if (inLeft.getSlot() > inRight.getSlot()) {
                return false;
            }
        }

        if (inLeft.getItemID() < inRight.getItemID()) {
            return true;
        } else if (inLeft.getItemID() > inRight.getItemID()) {
            return false;
        }
        
        return &inLeft < &inRight;
    }

    /// Sort an item vector using inSortPredicate.
    /// This is a template so that it will work with, for example
    /// both ItemRefVec and TemporalRefVec
    template <typename T>
    static void sortItems(std::vector<std::reference_wrapper<T>>& outItems,
                          decltype(isItemLessThan<T>) inSortPredicate)
    {
        std::ranges::sort(outItems, inSortPredicate);
    }

    /// Sort an item vector using the standard Item::sortCompare.
    /// This is a template so that it will work with, for example
    /// both ItemRefVec and TemporalRefVec
    template <typename T>
    static void sortItems(std::vector<std::reference_wrapper<T>>& outItems)
    {
        sortItems(outItems, isItemLessThan<T>);
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

