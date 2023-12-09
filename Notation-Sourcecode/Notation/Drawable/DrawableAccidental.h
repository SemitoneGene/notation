// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "AccidentalType.h"
#include "DrawResultCache.h"
#include "DrawableText.h"
#include "ItemObserver.h"

namespace komp
{
class DrawableAccidental : public Drawable, public ItemObserver
{
public:
    ///@{ Drawable
    DrawableAccidental();
    DrawableAccidental(const Item& inItem);
    virtual ~DrawableAccidental();
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

    void clearState();
    
    /// set the font size
    void setFontSize(double inFontSize);
    
    /// get the font size
    inline double getFontSize() const { return mText.getFontSize(); }
    
    /// get the accidental type
    const AccidentalType& getAccidentalType() const;
    
    /// set the accidental type
    void setAccidentalType(AccidentalType inAccidentalType);
    
    /// get the unichar
    unsigned short getCharacter() const;
    
private:
    DrawableText mText;
    AccidentalType mAccidentalType{AccidentalType::NONE};
    DrawResultCache mCache;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
