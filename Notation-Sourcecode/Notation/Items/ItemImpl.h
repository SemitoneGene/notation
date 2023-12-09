// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"
#include "Measure.h"
#include "ItemObserver.h"
#include "Score.h"

namespace komp
{
static const double ANCHOR_SIZE_H = 1.2;
static const double ANCHOR_SIZE_V = 3.0;

class DrawableText;
class SpaceTimePoint;
class SpatialPointIntf;

/// \brief  a container for code that is shared between most Items
///
class ItemImpl
{
public:
    
    static constexpr const int MAX_OBSERVER_SPIN_COUNT = 2;
    
    static bool contains(const Item& inItem,
                         const Point& inPoint,
                         bool inPrecise);
    
    static void registerObserver(ItemObserverRefVec& inObservers,
                                 ItemObserver& inObserver);
    
    static void unregisterObserver(ItemObserverRefVec& outObservers,
                                   ItemObserver& inObserver);
    
    static void notifyObservers(const Item& item,
                                const ItemObserverRefVec& inObservers,
                                const ItemChanges& inChanges);
    
    static void notifyObservers(const Item& item,
                                const ItemObserverRefVec& outObservers,
                                const ItemChanges& inChanges,
                                int& outObserverSpinCount);
    
    static bool selected(bool inCurrentSelectionState);
    static void select(bool& outCurrentSelectionState);
    static void deselect(bool& outCurrentSelectionState);
    static void toggleSelection(bool& outCurrentSelectionState);
    
    static double getSpaceSize(const Item& inItem);
    static double getFontSize(const Item& inItem);
    
    static double characterWidth(const Item& inItem,
                                 const unsigned short inChar);
    
    static double characterHeight(const Item& inItem,
                                  const unsigned short inChar);
    
    static Rect characterFrame(const Item& inItem,
                               const unsigned short inChar);
    
    static Size measureCharacter(const Item& inItem,
                                 const unsigned short inChar);
    
    static void removeSelf(Item& inItem);
    
    static int getSlot(const SpaceTimePoint& inSpaceTimePoint);
    
    static Measure& getMeasure(const Item& inItem);
    
    static int getRoundedTime(const Item& inItem);
    
    static void setTime(Item& outItem,
                        SpaceTimePoint& outSpaceTimePoint,
                        double inValue);
    
    static void drawItemTime(const Item& inItem,
                             const DrawableContext& inContext,
                             DrawableText& inText);
    
    static void setX(Item& outItem,
                     double inValue,
                     SpaceTimePoint& outSpaceTimePoint);
    static void setSlot(Item& outItem,
                        int inValue,
                        SpaceTimePoint& outSpaceTimePoint);
    
    static Point getDrawPoint(const Item& inItem,
                              const SpatialPointIntf& inSpatialPoint);
    
    static void setNeedsDisplay(Item& inItem);
    
    template<typename T>
    static void createDrawable(const Item& inItem)
    {
        const auto itemID = inItem.getItemID();
        const auto& measure = inItem.getLocation().getMeasure();
        auto& score = measure.getScore();
        auto& registry = score.getDrawableRegistry();
        if (!registry.contains(itemID)) {
            auto drawable = std::make_unique<T>();
            const auto self = const_cast<Item*>(&inItem);
            self->registerObserver(*drawable);
            registry[itemID] = std::move(drawable);
        }
    }

    static Drawable*
    getDrawable(const Item& inItem)
    {
        const auto itemID = inItem.getItemID();
        const auto& measure = inItem.getLocation().getMeasure();
        auto& score = measure.getScore();
        auto& registry = score.getDrawableRegistry();
        KOMP_ASSERT(registry.contains(itemID));
        return registry.at(itemID).get();
    }
    
    template<typename T>
    static T* getDrawable(const Item& inItem)
    {
        const auto itemID = inItem.getItemID();
        const auto& measure = inItem.getLocation().getMeasure();
        auto& score = measure.getScore();
        auto& registry = score.getDrawableRegistry();
        KOMP_ASSERT(registry.contains(itemID));
        return dynamic_cast<T*>(registry.at(itemID).get());
    }
    
    template<typename T>
    static Region getRegion(const Item& inItem)
    {
#ifdef KOMP_BUILD_APPLE
        CGMutablePathRef path = CGPathCreateMutable();
        CGPathAddRect(path, NULL, inItem.getFrame());
        return path;
#elif KOMP_BUILD_LINUX
        QPainterPath path;
        const auto frame = inItem.getFrame();
        path.addRect(frame);
        return Region{path};
#endif
    }
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
