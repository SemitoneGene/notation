// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Tie.h"

// Local
#include "ItemTraits.h"
#include "Throw.h"

namespace komp
{
Tie::Tie(const MusicLocation& inLocation,
         const CurveParams& inParams)
: Curve(inLocation, inParams)
{
    
}

const ItemTraits&
Tie::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Tie);
}

void
Tie::copyState(const Tie& other)
{
    Curve::operator=(other);
}

void
Tie::inserted()
{
    Curve::inserted();
}

ItemUPtr
Tie::getMemento() const
{
    return std::make_unique<Tie>(*this);
}

void
Tie::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const auto downcastedP = dynamic_cast<const Tie*>(itemP);
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
