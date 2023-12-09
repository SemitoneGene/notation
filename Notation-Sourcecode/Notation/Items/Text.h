// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>

// Local
#include "DrawableString.h"
#include "Spatial.h"


namespace komp
{
struct TextInfo
{
    std::string Font;
    double Size{0};
};

struct TextRecord
{
    std::string Text;
    TextInfo FontInfo;
};


class KOMP_EXPORT Text : public Spatial
{
public:
    virtual ~Text() = default;
    
    Text(const Text& other);
    Text& operator=(const Text& other);
    Text(Text&& other);
    Text& operator=(Text&& other);
    
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
    
    virtual void beginDrag(const CurrentState& inState) override;
    virtual void drag(const CurrentState& inState) override;
    virtual void endDrag(const CurrentState& inState) override;
    
private:
    Text() = delete;

    Text(const MusicLocation& inMusicLocation,
         std::string_view inText,
         PlacementType inPlacement);

    /// operator=() implementation
    void copyState(const Text& other);

    void calculateAboveBelow();
    
    friend class ItemRegistry;
    
    ///@{ Item
    virtual void createDrawable() override;
    ///@}
    
    std::string mText;
    PlacementType mPlacement;
    double mX{0.0};
    double mY{0.0};
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
