// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <iostream>
#include <memory>

// Local
#include "Draggable.h"
#include "DrawableColors.h"
#include "DrawableContext.h"
#include "Forward.h"
#include "Geometry.h"
#include "ItemObserver.h"
#include "ItemTraits.h"
#include "MusicLocation.h"

namespace komp
{
    class KOMP_EXPORT Item : public Draggable,
                             public ItemObserver
    {
    public:
        Item();
        virtual ~Item() = default;

        /// gets information about the Item's sub-type
        virtual const ItemTraits& getItemTraits() const = 0;
        
        /// creates a shallow copy of the ItemRef which will have the
        /// same ItemID as it's prototype and will not not cause a
        /// a new entry in the score's ItemRegistry.
        virtual ItemUPtr getMemento() const = 0;
        
        /// takes the input ItemRef and copies its guts restoring
        /// this Item to the state of the passed item.  Throws if
        /// the inItem cannot be downcasted to the correct type
        virtual void restoreMemento(const Item& inItem) = 0;

        /// tell the item to do any calculations that it needs to
        virtual void update() = 0;
        
        /// return the Item's ID value which is used to identify
        /// it in the Score's ItemRegistry
        virtual ItemID getItemID() const final;
        
        /// Item will call this (like a callback) when its ID changes
        virtual void itemIDChanged() = 0;

        /// Item will call this (like a callback) when its ID is invalidated
        virtual void itemIDInvalidated() = 0;

        /// request callbacks from this item when this item's state change
        virtual void registerObserver(ItemObserver& inObserver) = 0;
        
        /// stop receiving item state change callbacks from this item
        virtual void unregisterObserver(ItemObserver& inObserver) = 0;

        /// cause the observers of this item to be notified about a state change
        virtual void notifyObservers(const ItemChanges& inChanges) = 0;
        
        /// this is the callback that will be called when an observed item's
        /// state changes.  The in param is the item which changed.
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override = 0;

        /// when an item is Quiescent it will refrain from sending or responding
        /// to ItemObserver notifications
        virtual bool getIsQuiescent() const = 0;

        /// when an item is Quiescent it will refrain from sending or responding
        /// to ItemObserver notifications
        virtual void setIsQuiescent(bool inValue) = 0;

        /// Returns the bounding box for this item.
        /// Can return an empty rect if the item is never supposed to be drawn
        virtual Rect getFrame() const = 0;
        
        /// Returns the bounding box for this item.
        /// isValid will return false if the item is never supposed to be drawn
        virtual Region getRegion() const = 0;
        
        /// Returns the visual anchor point of the item.
        /// Can return POINT_ZERO if the item is never supposed to be drawn.
        virtual Point getAnchor() const = 0;

        /// Return true is the item frame, or region is inPrecise is true, contains the point
        /// Can return POINT_ZERO if the item is never supposed to be drawn.
        virtual bool contains(const Point& inPoint, bool inPrecise) const = 0;

        /// This method will be called after an item has been inserted into a measure
        virtual void inserted() = 0;
        
        /// This method will be called after an item has been removed from a measure
        virtual void removed() = 0;
        
        /// The item will remove itself from the measure
        virtual void removeSelf() = 0;
        
        /// Get the item MusicLocation passed in by inserted()
        virtual const MusicLocation& getLocation() const = 0;
        
        /// get x relative to the beginning of the measure's data space
        virtual double getX() const = 0;
        
        /// set x relative to the beginning of the measure's data space
        virtual void setX(double inValue) = 0;
        
        /// the Spatial's x-position relative to the score in pixels
        virtual double getScoreX() const = 0;
        
        /// \brief  Get the item’s slot location.
        ///
        /// \discussion Slot refers to position on the staff. If we start counting
        /// at the first ledger line below a normal treble cleff staff as zero and
        /// count upwards from there we get 0=C4, 1=D4, E=D4=BottomStaffLine, etc.
        /// The position in the center of staff is defined as Slot 6.  So for a
        /// 5-line staff, 6 is the middle line.  In a 2-line staff, 6 would be the
        /// space between the two lines.  In a 1-line staff, 6 would be the line.
        ///
        virtual int getSlot() const = 0;
        
        /// \brief  Set the item’s slot location.
        ///
        /// \discussion See the getSlot() function for definition of the slot value.
        ///
        virtual void setSlot(int inValue) = 0;
        
        /// the Spatial's y-position relative to the score in  pixels
        virtual double getScoreY() const = 0;

        /// the point that defines the origin of the item's draw operation
        /// in pixels.  Includes the space size as these are frequently needed by clients
        virtual Point getDrawPoint() const = 0;

        /// get the item's time location within a measure
        virtual double getTime() const = 0;
        
        /// get the item's time location within a measure
        /// rounded to the nearest integer
        virtual int getRoundedTime() const = 0;
        
        /// set the item's time location within a measure
        virtual void setTime(double inValue) = 0;

        /// get the size of a staff space, in pixels
        virtual double getSpaceSize() const = 0;

        /// get the font size
        virtual double getFontSize() const = 0;
        
        /// Create the drawable object
        virtual void createDrawable() = 0;
        
        /// \brief Return true if the item is an instance of the templated type
        template <typename T>
        bool is() const
        {
            return dynamic_cast<const T*>(this) != nullptr;
        }

        /// Return a reference cast to the type
        template <typename T>
        T& as()
        {
            return *dynamic_cast<T*>(this);
        }
        
        /// Notify the item that its measure has changed x-position
        virtual void notifyMeasureXChanged() {}

        /// Notify the item that its staff has changed y-position
        virtual void notifyStaffYChanged() {}

    private:
        friend class ItemImpl;
        friend class ItemRegistry;
                
        /// Sets the item id
        void setItemID(ItemID inValue);

        ItemID mItemID;
    };
    
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

