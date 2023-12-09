// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "AccidentalType.h"
#include "Forward.h"
#include "KeySignatureType.h"
#include "Spatial.h"

namespace komp
{
    class KOMP_EXPORT KeySignature : public Spatial
    {
    public:
        virtual ~KeySignature() = default;
        
        virtual const ItemTraits& getItemTraits() const override final;
        virtual ItemUPtr getMemento() const override final;
        virtual void restoreMemento(const Item& inItem) override final;
        virtual void observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges) override final {}
        virtual void inserted() override final;
        virtual void removed() override final;
        virtual void removeSelf() override final;
        virtual Point getDrawPoint() const override final;
        virtual double getScoreX() const override final;
        virtual double getScoreY() const override final;
        virtual Rect getFrame() const override final;
        inline virtual Region getRegion() const override final;
        inline virtual Point getAnchor() const override final { return POINT_ZERO; }
        
        virtual void update() override;
        
        /// Calculates the effective accidental for a given clef and slot
        AccidentalType getAccidental(ClefType inClef, int inSlot);
        
        const KeySignatureValue& getKey() const;

        const KeySignatureValue& getPreviousKey() const;
        void setPreviousKey(const KeySignatureValue& inValue);
            
        static AccidentalType getAccidental(KeySignatureValue inKeyValue,
                                            ClefType inClef,
                                            int inSlot);
        
        static double calculateSpace(int inOldKey, int inNewKey);

    private:
        friend class ItemRegistry;
        
        KeySignature() = delete;
        
        KeySignature(const MusicLocation& inLocation,
                     const KeySignatureValue& inKey);
        
        ///@{ Item
        virtual void createDrawable() override;
        ///@}

        KeySignatureValue mKey;
        KeySignatureValue mPrevious;
    };
    
    inline const KeySignatureValue& KeySignature::getKey() const { return mKey; }
    
    inline const KeySignatureValue& KeySignature::getPreviousKey() const { return mPrevious; }
    inline void KeySignature::setPreviousKey(const KeySignatureValue& inValue) { mPrevious = inValue; }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
