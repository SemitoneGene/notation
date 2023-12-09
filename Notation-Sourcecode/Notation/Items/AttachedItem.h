// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Spatial.h"
#include "RelativePoint.h"

namespace komp
{
    class KOMP_EXPORT AttachedItem : public Spatial
    {
    public:
        AttachedItem(const AttachedItem& other);
        AttachedItem& operator=(const AttachedItem& other);
        AttachedItem(AttachedItem&& other);
        AttachedItem& operator=(AttachedItem&& other);
        virtual const ItemTraits& getItemTraits() const override = 0;
        virtual void update() override {}
        virtual Rect getFrame() const override = 0;
        virtual Region getRegion() const override = 0;
        virtual void inserted() override = 0;
        virtual void removed() override = 0;
        virtual double getX() const override;
        virtual void setX(double inValue) override;
        virtual double getScoreX() const override;
        virtual double getScoreY() const override;
        virtual int getSlot() const override;
        virtual void setSlot(int inValue) override;
        virtual double getTime() const override;
        virtual int getRoundedTime() const override;
        virtual void setTime(double inValue) override;
        
        ///@{ AttachedItem
        const RelativePoint& getRelativePoint() const;
        RelativePoint& getRelativePoint();
        ///@}
        
    protected:
        friend class ItemRegistry;
        AttachedItem() = delete;
        AttachedItem(const MusicLocation& inLocation,
                     const RelativePoint& inRelativePoint);
        
    private:
        RelativePoint mRelativePoint;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
