// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawResultCache.h"

// Local
#include "Throw.h"
#include "DrawParams.h"

namespace komp
{
    DrawResultCache::DrawResultCache()
    : mCache{}
    , mIsDirty{true}
    , mMutex{}
    {

    }


    DrawResultCache::~DrawResultCache()
    {

    }


    DrawResultCache::DrawResultCache(const DrawResultCache& other)
    : mCache{other.mCache}
    , mIsDirty{other.mIsDirty}
    , mMutex{}
    {

    }


    DrawResultCache::DrawResultCache(DrawResultCache&& other) noexcept
    : mCache{std::move(other.mCache)}
    , mIsDirty{std::move(other.mIsDirty)}
    , mMutex{}
    {

    }


    DrawResultCache&
    DrawResultCache::operator=(const DrawResultCache& other)
    {
        mCache = other.mCache;
        mIsDirty = other.mIsDirty;
        return *this;
    }


    DrawResultCache&
    DrawResultCache::operator=(DrawResultCache&& other) noexcept
    {
        mCache = std::move(other.mCache);
        mIsDirty = std::move(other.mIsDirty);
        return *this;
    }


    bool
    DrawResultCache::getIsDirty() const
    {
        return mIsDirty;
    }


    void
    DrawResultCache::setIsDirty() const
    {
        std::lock_guard<std::mutex> lock{mMutex};
        mIsDirty = true;
    }

    DrawResult
    DrawResultCache::getCache(const Point& inPoint) const
    {
        if (mIsDirty) {
            KOMP_THROW("attempt to retrieve a dirty cache");
        }
        auto copy = mCache;
        copy.moveBy(inPoint.x, inPoint.y);
        return copy;
    }


    void
    DrawResultCache::setCache(const DrawResult& inDrawResult,
                              const Point& inPoint) const
    {
        std::lock_guard<std::mutex> lock{mMutex};
        const auto xAmount = -1 * inPoint.x;
        const auto yAmount = -1 * inPoint.y;
        mCache = inDrawResult;
        mCache.moveBy(xAmount, yAmount);
        mIsDirty = false;
    }


    DrawResult
    DrawResultCache::getDrawResult(const Drawable& inDrawable,
                                   const Point& inPoint) const
    {
        if (!getIsDirty()) {
            return getCache(inPoint);
        }
        const auto result = inDrawable.getMetrics(inPoint);
        setCache(result, inPoint);
        return result;
    }


    Rect
    DrawResultCache::getFrame(const Drawable& inDrawable,
                              const Point& inPoint) const
    {
        return getDrawResult(inDrawable, inPoint).frame;
    }


    double
    DrawResultCache::getWidth(const Drawable& inDrawable) const
    {
        return getFrame(inDrawable, Point{}).getWidth();
    }


    double
    DrawResultCache::getHeight(const Drawable& inDrawable) const
    {
        return getFrame(inDrawable, Point{}).getHeight();
    }


    Region
    DrawResultCache::getRegion(const Drawable& inDrawable,
                               const Point& inPoint) const
    {
        return getDrawResult(inDrawable, inPoint).region;
    }


    Point
    DrawResultCache::getAnchor(const Drawable& inDrawable,
                               const Point& inPoint) const
    {
        return getDrawResult(inDrawable, inPoint).anchor;
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
