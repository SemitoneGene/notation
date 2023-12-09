// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Slur.h"

// Local
#include "Throw.h"

namespace komp
{

// MARK: - Slur

Slur::Slur(const MusicLocation& inLocation,
           const CurveParams& inParams)
: Curve(inLocation, inParams)
{
}


const ItemTraits&
Slur::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Slur);
}

void
Slur::copyState(const Slur& other)
{
    Curve::operator=(other);
}

ItemUPtr
Slur::getMemento() const
{
    return std::make_unique<Slur>(*this);
}

void
Slur::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Slur* downcastedP = dynamic_cast<const Slur*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
