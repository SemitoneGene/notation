// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Selection.h"

// Local
#include "Clef.h"
#include "Command.h"
#include "Item.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Score.h"
#include "UndoManager.h"

namespace komp
{
    Selection::Selection(Score& inScore)
    : mScore(inScore)
    {
    }
    
    void
    Selection::clear()
    {
        // Get a copy of the selection before clearing it.
        // We need to operate on a copy as the Item will check against
        // mItems when drawing after ItemImpl::setNeedsDisplay.
        const auto selection = mItems;
        mItems.clear();
        
        for (Item& item : selection) {
            ItemImpl::setNeedsDisplay(item);
        }
    }
    
    void
    Selection::addItem(Item& inItem)
    {
        const auto& findIter = std::find_if(mItems.cbegin(),
                                            mItems.cend(),
                                            [&](const Item& item) {
            return &item == &inItem;
        });
        
        if (findIter == mItems.cend()) {
            mItems.push_back(inItem);
            ItemImpl::setNeedsDisplay(inItem);
        }
    }
    
    void
    Selection::removeItem(Item& inItem)
    {
        const auto& findIter = std::find_if(mItems.cbegin(),
                                            mItems.cend(),
                                            [&](const Item& item) {
            return &item == &inItem;
        });

        if (findIter != mItems.cend()) {
            mItems.erase(findIter);
            ItemImpl::setNeedsDisplay(inItem);
        }
    }
        
    void
    Selection::deleteItems()
    {
        if (mItems.empty()) {
            LOG_F(3, "Delete called on empty selection");
            return;
        }
        
        // Verify that this is an item that can be deleted.
        // TODO: Do we need a deletion policy?
        // The following items cannot be deleted:
        //      • The first time signature of the first measure
        //      • The first clef of the first measure
        //
        
        auto itemsToDelete = mItems;
        auto iter = itemsToDelete.begin();
        const auto iterEnd = itemsToDelete.end();
        for ( ; iter != iterEnd; ++iter) {
            
            Item& item = *iter;
            const auto& measure = item.getLocation().getMeasure();
            const auto measureIndex = measure.getIndex();
            
            bool removeItem = false;
            
            if (measureIndex == 0) {
                
                // Handle first measure items
                if (item.getItemTraits().getItemType() == ItemType::Clef) {
                    // Check for the first clef in the first measure
                    removeItem = true;
                    LOG_F(INFO, "Protecting first clef from deletion");
                }
            }
            
            if (item.getItemTraits().getItemType() == ItemType::Clef) {
                removeItem = true;
            }
                        
            if (removeItem) {
                iter = itemsToDelete.erase(iter);
            }
        }
        
        if (!itemsToDelete.empty()) {
            // Create the deletion command
            auto& commandFactory = mScore.getCommandFactory();
            auto command = commandFactory.deleteItemsCommand(itemsToDelete);
            command->execute();
            mScore.getUndoManager().addCommand(std::move(command));
            mItems.clear();
        }
    }
    

/// \brief Return a rect that contains all selected items
    /// \result A rect of the union of all item frames
    Rect
    Selection::getFrame() const
    {
        Rect frame;
        
        auto initalSet = false;
        
        for (const Item& item : mItems) {
            
            if (!initalSet) {
                frame = item.getFrame();
                initalSet = true;
            } else {
                frame.unionRect(item.getFrame());
            }
        }
        
        return frame;
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

