// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "AttachedItem.h"
#include "Forward.h"
#include "MarkData.h"
#include "MarkType.h"

namespace komp
{

struct MarkRange;

class KOMP_EXPORT Mark : public AttachedItem
{
public:
    virtual ~Mark() = default;
    Mark(const Mark& other);
    Mark& operator=(const Mark& other);
    Mark(Mark&& other);
    Mark& operator=(Mark&& other);
    virtual const ItemTraits& getItemTraits() const override;
    virtual ItemUPtr getMemento() const override;
    virtual void restoreMemento(const Item& inItem) override;
    virtual void update() override;
    virtual void observedItemChanged(const Item& observedItem,
                                     const ItemChanges& inChanges) override;
    virtual Rect getFrame() const override;
    inline virtual Point getAnchor() const override { return POINT_ZERO; }
    virtual Region getRegion() const override;
    virtual void inserted() override;
    virtual void removed() override;
    virtual double getX() const override;
    virtual void setX(double inValue) override;
    virtual double getScoreX() const override;
    virtual double getScoreY() const override;
    virtual int getSlot() const override;
    virtual void setSlot(int inValue) override;
    virtual void beginDrag(const CurrentState& inState) override;
    virtual void drag(const CurrentState& inState) override;
    virtual void endDrag(const CurrentState& inState) override;
    
    // Mark Interface
    virtual const MarkData& getMarkData() const;
    virtual MarkType getMarkType() const;
    
    virtual double getPlaybackVolume() const;
    virtual double getPlaybackDurationScale() const;
    
    Rect getTechniquesFrame(const Point& inPoint) const;
    
private:
    Mark(const MusicLocation& inLocation,
         MarkType inMarkType,
         PlacementType inPlacementType,
         const RelativePoint& inRelativePoint);

    virtual void createDrawable() override;

    /// operator=() implementation
    void copyState(const Mark& other);
        
    bool getIsExpression() const;
    bool getIsFermata() const;
    bool getIsTechnique() const;
    void setDrawableData();
    
    friend class ItemRegistry;
        
    void calculateAboveBelow();
    
    bool handleMarkHorizontalDrag();
    bool handleMarkVerticalDrag();
    bool handleMarkRangeVerticalDrag(const MarkRange& inRange);
    
    MarkData mMarkData;
    
    // Used to calculate offset into drag mark maps
    MarkType mDragStartMark{MarkType::NONE};
    
    // Indicates dragging has been locked to an axis
    bool mDragAxisLock{false};
    
    // Dragging is locked to the X-axis
    bool mDragAxisX{false};
    
    // Dragging is locked to the Y-axis
    bool mDragAxisY{false};
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
