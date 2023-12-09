// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Local
#include "Forward.h"
#include "Spanner.h"

namespace komp
{    
    class KOMP_EXPORT LineSpanner : public Spanner
    {
    public:
        
        enum class Style
        {
            DASHED,
            SOLID,
            WAVY
        };
        
        virtual ~LineSpanner();
        
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        virtual void observedItemChanged(const Item& observedItem, const ItemChanges& inChanges) override {}

        virtual Rect getFrame() const override;
        inline virtual Region getRegion() const override { return {}; }
        inline virtual Point getAnchor() const override { return POINT_ZERO; }
        
    private:
        friend class ItemRegistry;
        
        LineSpanner(const MusicLocation& inLocation,
                    const SpannerParams& inParams,
                    Style inStyle,
                    PlacementType inPlacement);
        LineSpanner(const MusicLocation& inLocation,
                    const SpannerParams& inParams,
                    Style inStyle);

        ///@{ Item
        virtual void createDrawable() override;
        ///@}

        Style mStyle;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
