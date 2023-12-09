// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "DrawResultCache.h"
#include "DrawableAccidental.h"
#include "DrawableArpeggiate.h"
#include "DrawableLegerLines.h"
#include "DrawableNoteDots.h"
#include "DrawableNoteFlags.h"
#include "DrawableNotehead.h"
#include "DrawableRectangle.h"
#include "DrawableStem.h"
#include "ItemObserver.h"

namespace komp
{
class DrawableNoteParts : public Drawable, public ItemObserver
{
public:
    ///@{ Drawable
    DrawableNoteParts();
    virtual ~DrawableNoteParts() = default;
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
    
    /// Get the Accidental's offset, in pixels, relative to the
    /// Temporal's draw point
    komp::Point getAccidentalOffset() const;
    
    /// Set the Accidental's offset, in pixels, relative to the
    /// Temporal's draw point
    void setAccidentalOffset(const Point& inOffset);

    /// Get the Arpeggiates's offset, in pixels, relative to the
    /// Temporal's draw point
    komp::Point getArpeggiateOffset() const;

    /// Set the Arpeggiates's offset, in pixels, relative to the
    /// Temporal's draw point
    void setArpeggiateOffset(const Point& inOffset);

    /// Set the amount of space (in staff spaces) between the
    /// notehead and the first dot.
    void setSpacesBeforeFirstDot(double inValue);
    
    /// Set the amount of space (in staff spaces) between the dots
    /// when there is more than one dot. The first dot will be
    /// positioned based on the setSpaceBeforeFirstDot value, only
    /// subsequent dots are affected by setSpaceBetweenDots
    void setSpacesBetweenDots(double inValue);
    
    /// Get the accidental type
    const AccidentalType& getAccidentalType() const;
    
    /// Get the accidental type
    void setAccidentalType(AccidentalType inAccidentalType);

    /// Get the arpeggiateType type
    const ArpeggiateType& getArpeggiateType()const;

    /// Set the arpeggiateType type
    void setArpeggiateType(ArpeggiateType inArpeggiateType);
        
    DrawResult getNoteheadMetrics(const Point& inPoint) const;
    DrawResult getNoteDotsMetrics(const Point& inPoint) const;
    DrawResult getAccidentalMetrics(const Point& inPoint) const;
    DrawResult getStemMetrics(const Point& inPoint) const;
    DrawResult getNoteFlagsMetrics(const Point& inPoint) const;
    DrawResult getLegerLinesMetrics(const Point& inPoint) const;
    DrawResult getArpeggiateMetrics(const Point& inPoint) const;
    
    void drawNotehead(const DrawParams& inDrawParams) const;
    void drawNoteDots(const DrawParams& inDrawParams) const;
    void drawAccidental(const DrawParams& inDrawParams) const;
    void drawStem(const DrawParams& inDrawParams) const;
    void drawNoteFlags(const DrawParams& inDrawParams) const;
    void drawLegerLines(const DrawParams& inDrawParams) const;
    void drawArpeggiate(const DrawParams& inDrawParams) const;
    void drawOutline(const DrawResult& inDrawResult,
                     const DrawParams& inDrawParams) const;
    
    Rect getNoteheadFrame(const Point& inPoint) const;
    Rect getNoteDotsFrame(const Point& inPoint) const;
    Rect getAccidentalFrame(const Point& inPoint) const;
    Rect getStemFrame(const Point& inPoint) const;
    Rect getNoteFlagsFrame(const Point& inPoint) const;
    Rect getLegerLinesFrame(const Point& inPoint) const;
    Rect getArpeggiateFrame(const Point& inPoint) const;
    
    Region getNoteheadRegion(const Point& inPoint) const;
    Region getNoteDotsRegion(const Point& inPoint) const;
    Region getAccidentalRegion(const Point& inPoint) const;
    Region getStemRegion(const Point& inPoint) const;
    Region getNoteFlagsRegion(const Point& inPoint) const;
    Region getLegerLinesRegion(const Point& inPoint) const;
    Region getArpeggiateRegion(const Point& inPoint) const;
    
    Point getNoteheadAnchor(const Point& inPoint) const;
    Point getNoteDotsAnchor(const Point& inPoint) const;
    Point getAccidentalAnchor(const Point& inPoint) const;
    Point getStemAnchor(const Point& inPoint) const;
    Point getNoteFlagsAnchor(const Point& inPoint) const;
    Point getLegerLinesAnchor(const Point& inPoint) const;
    Point getArpeggiateAnchor(const Point& inPoint) const;

private:
    Point mOffset{0.0, 0.0};
    bool mDrawNotehead{false};
    DrawableNotehead mNotehead;
    bool mDrawNoteDots{false};
    DrawableNoteDots mNoteDots;
    DrawableAccidental mAccidental;
    bool mDrawStem{false};
    DrawableStem mStem;
    bool mDrawNoteFlags{false};
    DrawableNoteFlags mNoteFlags;
    bool mDrawLegerLines{false};
    DrawableLegerLines mLegerLines;
    DrawableArpeggiate mArpeggiate;
    mutable DrawableRectangle mOutline{false, true};
    DrawResultCache mCache;
    
private:
    bool getIsAccidentalDrawable(const Temporal* inTemporal) const;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
