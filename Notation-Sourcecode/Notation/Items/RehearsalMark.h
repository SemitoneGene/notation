// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"
#include "RehearsalParams.h"
#include "Spatial.h"

namespace komp
{
class KOMP_EXPORT RehearsalMark : public Spatial
{
public:
    RehearsalMark(const MusicLocation& inLocation,
                  const RehearsalParams& inParams);
    RehearsalMark(const RehearsalMark& inObject);
    
    RehearsalMark& operator=(const RehearsalMark& other);
    
    virtual ~RehearsalMark() = default;
    
    virtual const ItemTraits& getItemTraits() const override;
    virtual void update() override final;
    virtual ItemUPtr getMemento() const override;
    virtual void restoreMemento(const Item& inItem) override;
    virtual void observedItemChanged(const Item& observedItem,
                                     const ItemChanges& inChanges) override {}
    
    virtual Rect getFrame() const override;
    virtual Region getRegion() const override;
    inline virtual Point getAnchor() const override { return POINT_ZERO; }
    
    virtual Point getDrawPoint() const override final;
    virtual double getScoreX() const override;
    virtual double getScoreY() const override;
    
    virtual void inserted() override;
    virtual void removed() override;

    ///@{ Draggable /////////////////////////////////////////////
    virtual void beginDrag(const CurrentState& inState) override;
    virtual void drag(const CurrentState& inState) override;
    virtual void endDrag(const CurrentState& inState) override;
    ///@} Draggable /////////////////////////////////////////////
    
private:
    friend class ItemRegistry;
    
    RehearsalMark() = delete;
    
    ///@{ Item
    virtual void createDrawable() override;
    ///@}
    
    void copyState(const RehearsalMark& other);
    void configure(const RehearsalParams& inParams);
    
    RehearsalParams mParams;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
