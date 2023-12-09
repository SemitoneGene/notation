// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Drawable.h"
#include "DrawableComposite.h"
#include "ChordSymbolEnums.h"
#include "ItemObserver.h"

namespace komp
{
/// \brief  Draws a Chord Symbol.
///
/// \discussion Holds the atomic components of a chord symbol
/// and draws them into a context.
///
class DrawableChordString : public Drawable, public ItemObserver
{
public:
    DrawableChordString();
    virtual ~DrawableChordString() = default;
    
public:
    ///@{ Drawable
    virtual void removeFromParent() const override;
    virtual void restoreParent() const override;
    virtual Point getOffset() const override;
    virtual void setOffset(const Point& inPoint) override;
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
    
private:
    ///@{ DrawableChordString
    ChordSymbolString getString() const;
    void setString(const ChordSymbolString& inString);
    ///@}

    DrawableComposite mDrawables;
    
};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
