// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Drawable.h"
#include "DrawableRectangle.h"
#include "DrawResultCache.h"
#include "ItemObserver.h"

// System
#include <shared_mutex>

namespace komp
{
/// DrawableComposite is a collection of Drawable objects
/// stored with their relative offsets.  When the
/// DrawableComposite is asked to draw, each of the Drawables
/// will be drawn by adding their DrawPoint to the DrawPoint
/// passed to the draw function.
///
class DrawableComposite : public Drawable, public ItemObserver
{
public:
    DrawableComposite();
    DrawableComposite(const Item& inItem);
    virtual ~DrawableComposite() = default;
    DrawableComposite(DrawableComposite&& other) = delete;
    DrawableComposite& operator=(DrawableComposite&& other) = delete;
    
    ///@{ Drawable Interface
    virtual void removeFromParent() const override;
    virtual void restoreParent() const override;
    virtual Point getOffset() const override;
    virtual void setOffset(const Point& inOffset) override;
    virtual void setZOrder(int inOrder) override;
    virtual void setScale(double inScale) override;
    virtual void setRotation(double inDegrees) override;
    virtual Rect getFrame(const Point& inPoint) const override;
    virtual double getWidth() const override;
    virtual double getHeight() const override;
    virtual Region getRegion(const Point& inPoint) const override;
    virtual Point getAnchor(const Point& inPoint) const override;
    virtual DrawResult getMetrics(const Point& inPoint) const override;
    virtual void draw(const DrawParams& inDrawParams) const override;
    ///@}
    
    ///@{ ItemObserver
    virtual void observedItemChanged(const Item& observedItem,
                                     const ItemChanges& inChanges) override;
    ///@}

    /// adds the Drawable
    void addDrawable(DrawableUPtr&& inDrawable);
    
    /// removes all elements
    void clearDrawables();
    
    /// removes all Drawables with an address matching inDrawable
    bool removeDrawable(const Drawable& inDrawable);
    
    /// gets the number of elements in the collection
    int getDrawableCount() const;
    
    /// gets a const reference of the drawable at index, throws if out-of-bounds
    const Drawable& getDrawable(int inIndex) const;
    
    /// gets a mutable reference of the drawable at index, throws if out-of-bounds
    Drawable& getDrawable(int inIndex);
    
    /// sets the element at the given index, throws if out-of-bounds
    void setDrawable(int inIndex, DrawableUPtr&& inDrawable);
    
private:
    mutable std::timed_mutex mMutex;
    Point mOffset{0.0, 0.0};
    DrawableUPtrVec mDrawables;
    DrawableRectangle mOutline{false, true};
    DrawResultCache mCache;
};


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

