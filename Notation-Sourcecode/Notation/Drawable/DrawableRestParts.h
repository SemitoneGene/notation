// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "DrawableNotehead.h"
#include "DrawableNoteDots.h"
#include "DrawableAccidental.h"
#include "DrawableStem.h"
#include "DrawableNoteFlags.h"
#include "DrawableLegerLines.h"
#include "DrawableRectangle.h"
#include "DrawResultCache.h"
#include "ItemObserver.h"
#include "NoteFrames.h"

namespace komp
{
    class DrawableRestParts : public Drawable
                            , public ItemObserver
    {
    public:
        ///@{ Drawable
        DrawableRestParts();
        virtual ~DrawableRestParts() = default;
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

        ///@{ ItemObserver
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override;
        ///@}

        void update(const Item& inItem);
        void clearState();

        /// set the amount of space (in staff spaces) between the
        /// notehead and the first dot.
        void setSpacesBeforeFirstDot(double inValue);

        /// set the amount of space (in staff spaces) between the dots
        /// when there is more than one dot. The first dot will be
        /// positioned based on the setSpaceBeforeFirstDot value, only
        /// subsequent dots are affected by setSpaceBetweenDots
        void setSpacesBetweenDots(double inValue);

        /// get the accidental type
        const AccidentalType& getAccidentalType() const;

        /// set the accidental type
        void setAccidentalType(AccidentalType inAccidentalType);
        
        DrawResult getNoteheadMetrics(const Point& inPoint) const;
        DrawResult getDrawableNoteDotsMetrics(const Point& inPoint) const;

        void drawNotehead(const DrawParams& inDrawParams) const;
        void drawDrawableNoteDots(const DrawParams& inDrawParams) const;
        void drawDrawableOutline(const DrawResult& inDrawResult,
                                 const DrawParams& inDrawParams) const;
        
        Rect getNoteheadFrame(const Point& inPoint) const;
        Rect getNoteDotsFrame(const Point& inPoint) const;

        Region getNoteheadRegion(const Point& inPoint) const;
        Region getNoteDotsRegion(const Point& inPoint) const;

        Point getNoteheadAnchor(const Point& inPoint) const;
        Point getNoteDotsAnchor(const Point& inPoint) const;

    private:
        Point mOffset{0.0, 0.0};
        bool mDrawNotehead{false};
        DrawableNotehead mNotehead;
        bool mDrawDrawableNoteDots{false};
        DrawableNoteDots mDrawableNoteDots;
        mutable DrawableRectangle mOutline{false, true};
        DrawResultCache mCache;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
