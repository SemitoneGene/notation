// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <mutex>

// Local
#include "Drawable.h"
#include "DrawResult.h"

namespace komp
{
    class DrawResultCache
    {
    public:
        DrawResultCache();
        ~DrawResultCache();
        DrawResultCache(const DrawResultCache& other);
        DrawResultCache(DrawResultCache&& other) noexcept;
        DrawResultCache& operator=(const DrawResultCache& other);
        DrawResultCache& operator=(DrawResultCache&& other) noexcept;

        bool getIsDirty() const;
        void setIsDirty() const;

        DrawResult getCache(const Point& inPoint) const;
        void setCache(const DrawResult& inDrawResult, const Point& inPoint) const;
        DrawResult getDrawResult(const Drawable& inDrawable, const Point& inPoint) const;
        Rect getFrame(const Drawable& inDrawable, const Point& inPoint) const;
        double getWidth(const Drawable& inDrawable) const;
        double getHeight(const Drawable& inDrawable) const;
        Region getRegion(const Drawable& inDrawable, const Point& inPoint) const;
        Point getAnchor(const Drawable& inDrawable, const Point& inPoint) const;
        
    private:
        mutable DrawResult mCache;
        mutable bool mIsDirty;
        mutable std::mutex mMutex;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

