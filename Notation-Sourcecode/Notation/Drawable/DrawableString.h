// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

// Local
#include "Forward.h"
#include "Drawable.h"
#include "DrawResultCache.h"
#include "DrawableRectangle.h"
#include "DrawableTextChar.h"
#include "ItemObserver.h"

namespace komp
{    
    class DrawableString : public Drawable
                         , public ItemObserver
    {
    public:
        DrawableString();
        virtual ~DrawableString() = default;

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

        std::string_view getString() const;
        void setString(std::string_view inString);

        /// get the font size
        double getFontSize() const;

        /// set the font size
        void setFontSize(double inValue);

        /// if the font name has not been set, then the font mgr
        /// will be queried and the notation font will be used.
        std::string getFontName() const;

        /// if the font name has not been set, then the font mgr
        /// will be queried and the notation font will be used.
        void setFontName(std::string inName);

    private:
        Point mOffset;
        int mZOrder;
        double mScale{1.0};
        std::string mString;
        double mFontSize;
        std::string mFontName;
        DrawResultCache mCache;
        mutable std::unique_ptr<DrawableLayer> mLayer;
        mutable DrawableRectangle mOutline;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
