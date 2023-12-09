// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "DrawableText.h"
#include "DrawableComposite.h"
#include "DrawResultCache.h"

namespace komp
{
    class DrawableNoteDots : public Drawable
    {
    public:
        ///@{ Drawable
        DrawableNoteDots();
        virtual ~DrawableNoteDots();
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

        /// set zero dots and Offset 0,0
        bool clearDots();

        /// set the amount of space (in staff spaces) between the
        /// notehead and the first dot.
        void setSpacesBeforeFirstDot(double inValue);

        /// set the amount of space (in staff spaces) between the dots
        /// when there is more than one dot. The first dot will be
        /// positioned based on the setSpaceBeforeFirstDot value, only
        /// subsequent dots are affected by setSpaceBetweenDots
        void setSpacesBetweenDots(double inValue);

        /// automatically set the number of dots and their positions
        /// by querying inTemporal, Offset will be set to the correct
        /// value automatically by update()
        void update(const Temporal& inTemporal);


    private:
        std::unique_ptr<DrawableText> makeDot(const Temporal& inTemporal);
        
        DrawableComposite mComposite;
        double mSpacesBeforeFirstDot;
        double mSpacesBetweenDots;
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
