// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

// Local
#include "Curve.h"

// Forward declarations
namespace komp
{
    class Note;
}

namespace komp
{    
    class KOMP_EXPORT Slur : public Curve
    {
    public:
        
        virtual ~Slur() {}
        
        virtual const ItemTraits& getItemTraits() const override;
        virtual ItemUPtr getMemento() const override;
        virtual void restoreMemento(const Item& inItem) override;
        
    private:
        friend class ItemRegistry;
        Slur() = delete;
        Slur(const MusicLocation& inLocation,
             const CurveParams& inParams);
        
        /// operator=() implementation
        void copyState(const Slur& other);
    };
    
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
