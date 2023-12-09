// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableComposite.h"

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "Preferences.h"
#include "ScoreColors.h"
#include "Throw.h"

namespace komp
{

DrawableComposite::DrawableComposite()
{
}

DrawableComposite::DrawableComposite(const Item& inItem)
{
}

void
DrawableComposite::removeFromParent() const
{
    for (const auto& drawable : mDrawables) {
        drawable->removeFromParent();
    }
    mOutline.removeFromParent();
}

void
DrawableComposite::restoreParent() const
{
    for (const auto& drawable : mDrawables) {
        drawable->restoreParent();
    }
    mOutline.restoreParent();
}

Point
DrawableComposite::getOffset() const
{
    return mOffset;
}


void
DrawableComposite::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableComposite::setZOrder(int inOrder)
{
    for (const auto& drawable : mDrawables) {
        drawable->setZOrder(inOrder);
    }
    mOutline.setZOrder(inOrder);
}

void
DrawableComposite::setScale(double inScale)
{
    for (const auto& drawable : mDrawables) {
        drawable->setScale(inScale);
    }
    mOutline.setScale(inScale);
}

void
DrawableComposite::setRotation(double inDegrees)
{
}

Rect
DrawableComposite::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableComposite::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableComposite::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableComposite::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableComposite::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableComposite::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        
        DrawResult result;
        
        bool isFirst = true;
        for (const auto& drawable : mDrawables) {
            
            KOMP_ASSERT(drawable);
            auto currentResult = drawable->getMetrics(inPoint);
            
            if (isFirst) {
                isFirst = false;
                result.frame = currentResult.frame;
                result.region = currentResult.region;
            } else {
                result.frame.unionRect(currentResult.frame);
#ifdef KOMP_BUILD_APPLE
                const auto mutablePath = result.region.getMutable();
                if (mutablePath && currentResult.region) {
                    CGPathAddPath(mutablePath, nullptr, currentResult.region);
                }
#elif KOMP_BUILD_LINUX
                result.region.addPath(currentResult.region);
#endif
            }
        }
        mCache.setCache(result, inPoint);
    }
    
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableComposite::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    
    auto drawPoint = inDrawParams.drawPoint;
    const auto& offset = getOffset();
    drawPoint.moveBy(offset);
    
    std::lock_guard<std::timed_mutex> lock(mMutex);
    
    const DrawParams dparams{
        inDrawParams.context,
        drawPoint,
        inDrawParams.colors
    };
    
    for (const auto& drawable : mDrawables) {
        KOMP_ASSERT(drawable);
        drawable->draw(dparams);
    }
    
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();
        mOutline.draw(dparams);
    } else {
        mOutline.removeFromParent();
    }
}

void
DrawableComposite::addDrawable(DrawableUPtr&& inDrawable)
{
    std::lock_guard<std::timed_mutex> lock(mMutex);
    KOMP_SET_DIRTY
    mDrawables.emplace_back(std::move(inDrawable));
    mOutline.setBounds(getFrame(Point{}));
}


void
DrawableComposite::clearDrawables()
{
    std::lock_guard<std::timed_mutex> lock(mMutex);
    if (mDrawables.empty()) {
        return;
    }
    
    KOMP_SET_DIRTY
    for (const auto& drawable : mDrawables) {
        drawable->removeFromParent();
    }
    mDrawables.clear();
    
    mOutline.removeFromParent();
}


bool
DrawableComposite::removeDrawable(const Drawable& inDrawable)
{
    std::lock_guard<std::timed_mutex> lock(mMutex);
    KOMP_SET_DIRTY
    
    bool result = false;
    for (auto it = mDrawables.begin(); it != mDrawables.end(); ++it) {
        if (it->get() == &inDrawable) {
            it = mDrawables.erase(it);
            --it;
            result = true;
        }
    }
    return result;
}


int
DrawableComposite::getDrawableCount() const
{
    std::lock_guard<std::timed_mutex> lock(mMutex);
    return static_cast<int>(mDrawables.size());
}


const Drawable&
DrawableComposite::getDrawable(int inIndex) const
{
    if (inIndex < 0 || inIndex > (getDrawableCount() -  1)) {
        KOMP_THROW("index out of bounds");
    }
    
    std::lock_guard<std::timed_mutex> lock(mMutex);
    const auto& drawablePtr = mDrawables.at(inIndex);
    KOMP_ASSERT(drawablePtr);
    const auto& drawable = *drawablePtr;
    return drawable;
}


Drawable&
DrawableComposite::getDrawable(int inIndex)
{
    KOMP_SET_DIRTY
    
    if (inIndex < 0 || inIndex > (getDrawableCount() -  1)) {
        KOMP_THROW("index out of bounds");
    }
    
    std::lock_guard<std::timed_mutex> lock(mMutex);
    const auto& drawablePtr = mDrawables.at(inIndex);
    KOMP_ASSERT(drawablePtr);
    auto& drawable = *drawablePtr;
    return drawable;
}


void
DrawableComposite::setDrawable(int inIndex, DrawableUPtr&& inDrawable)
{
    KOMP_ASSERT(inDrawable);
    KOMP_SET_DIRTY
    
    if (inIndex < 0 || inIndex > (getDrawableCount() -  1)) {
        KOMP_THROW("index out of bounds");
    }
    
    std::lock_guard<std::timed_mutex> fontCacheLock(mMutex);
    auto iter = mDrawables.begin() + inIndex;
    *iter = std::move(inDrawable);
}

// MARK: - ItemObserver

void
DrawableComposite::observedItemChanged(const Item& observedItem,
                                       const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

