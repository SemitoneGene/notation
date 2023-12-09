// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "DrawableText.h"
#include "DrawableStem.h"
#include "StemInfo.h"
#include "StemPolicy.h"
#include "DrawResultCache.h"

namespace komp
{
    class DrawableNotehead : public Drawable
    {
    public:
        ///@{ Drawable
        DrawableNotehead();
        virtual ~DrawableNotehead();
        virtual void removeFromParent() const override final;
        virtual void restoreParent() const override final;
        virtual Point getOffset() const override final;
        virtual void setOffset(const Point& inPoint) override final;
        virtual void setZOrder(int inOrder) override final;
        virtual void setScale(double inScale) override final;
        virtual void setRotation(double inDegrees) override final;
        virtual Rect getFrame(const Point& inPoint) const override final;
        virtual double getWidth() const override final;
        virtual double getHeight() const override final;
        virtual Region getRegion(const Point& inPoint) const override final;
        virtual Point getAnchor(const Point& inPoint) const override final;
        virtual DrawResult getMetrics(const Point& inPoint) const override final;
        virtual void draw(const DrawParams& inDrawParams) const override final;
        ///@}

        // return true of the cache was dirtied
        void update(const Temporal& inTemporal);

        // return true of the cache was dirtied
        void clearState();

    private:
        DrawableText mHead;
        DrawResultCache mCache;

    private:
        void updateNote(const Temporal& inTemporal);
        void updateRest(const Temporal& inTemporal);
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
