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
class DrawableText : public Drawable , public ItemObserver
{
public:
    DrawableText();
    virtual ~DrawableText() = default;
    
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
    
    const DrawableChars& getText() const;
    void setText(DrawableChars inText);
    
    /// Get the font size
    double getFontSize() const;
    
    /// Set the font size
    void setFontSize(double inValue);
    
    /// If the font name has not been set, then the font manager
    /// will be queried and the notation font will be used.
    std::string getFontName() const;
    
    /// If the font name has not been set, then the font manager
    /// will be queried and the notation font will be used.
    void setFontName(std::string_view inName);
    
private:
    Point mOffset{0.0, 0.0};
    int mZOrder{0};
    double mScale{1.0};
    double mRotation{0.0};
    DrawableChars mText;
    double mFontSize;
    std::string mFontName;
    DrawResultCache mCache;
    mutable DrawableRectangle mOutline{false, true};
    
private:
    Rect calculateGlyphFrame(const DrawableTextChar& inChar,
                             const Point& inPosition,
                             const Size& inSize) const;
    
    Point applyRawFrameOffsets(const Point& inPoint,
                               const Rect& inDrawableFrame) const;
    
    void accumulateResultFrame(const Rect& inGlyphFrame,
                               Rect& ioResultFrame,
                               bool& inIsFirst) const;
    
    mutable std::unique_ptr<DrawableLayer> mLayer;
};


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
