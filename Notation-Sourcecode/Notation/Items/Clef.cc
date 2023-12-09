// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Clef.h"

// Local
#include "DrawableClef.h"
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Item.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"

namespace komp
{
const ClefSymbolInfo gInfo{ ClefSymbolType::G, 4, 2 };
const ClefSymbolInfo fInfo{ ClefSymbolType::F, -4, 4 };
const ClefSymbolInfo cInfo{ ClefSymbolType::C, 0, 3 };
const ClefSymbolInfo pInfo{ ClefSymbolType::PERCUSSION, 6, 3 };
const ClefSymbolInfo p2Info{ ClefSymbolType::PERCUSSION, 6, 4 };
const ClefSymbolInfo tInfo{ ClefSymbolType::TAB, 6, 3 };

constexpr int octLoc = 0;
constexpr int oct8va = 1;
constexpr int oct8vb = -1;
constexpr int oct15a = 2;
constexpr int oct15b = -2;

constexpr int line1 = 1;
constexpr int line2 = 2;
constexpr int line3 = 3;
constexpr int line4 = 4;
constexpr int line5 = 5;

constexpr int octaveForClef1 = 1;
constexpr int octaveForClef2 = 2;
constexpr int octaveForClef3 = 3;
constexpr int octaveForClef4 = 4;
constexpr int octaveForClef5 = 5;
constexpr int octaveForClef6 = 6;
constexpr int octaveForClef7 = 7;

const std::map<ClefType, ClefInfo> Clef::ClefInfoTable =
{
    // name, middleCSlot, delineationLine
    std::pair<ClefType, ClefInfo>{ ClefType::G, ClefInfo{ "Treble", line2, gInfo, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::G_LINE1, ClefInfo{ "French", line1, gInfo, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::G_8VA, ClefInfo{ "Treble 8va", line2, gInfo, oct8va, octaveForClef6 } },
    std::pair<ClefType, ClefInfo>{ ClefType::G_8VB, ClefInfo{ "Treble 8vb", line2, gInfo, oct8vb, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::G_15MA, ClefInfo{ "Treble 15ma", line2, gInfo, oct15a, octaveForClef7 } },
    std::pair<ClefType, ClefInfo>{ ClefType::G_15MB, ClefInfo{ "Treble 15mb", line2, gInfo, oct15b, octaveForClef3 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F, ClefInfo{ "Bass", line4, fInfo, octLoc, octaveForClef3 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F_LINE3, ClefInfo{ "Varbaritone", line3, fInfo, octLoc, octaveForClef3 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F_LINE5, ClefInfo{ "Subbass", line5, fInfo, octLoc, octaveForClef3 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F_8VA, ClefInfo{ "Bass 8va", line4, fInfo, oct8va, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F_8VB, ClefInfo{ "Bass 8vb", line4, fInfo, oct8vb, octaveForClef2 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F_15MA, ClefInfo{ "Bass 15ma", line4, fInfo, oct15a, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::F_15MB, ClefInfo{ "Bass 15mb", line4, fInfo, oct15b, octaveForClef1 } },
    std::pair<ClefType, ClefInfo>{ ClefType::C, ClefInfo{ "Alto", line3, cInfo, octLoc, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::C_LINE1, ClefInfo{ "Soprano", line1, cInfo, octLoc, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::C_LINE2, ClefInfo{ "Mezzo-Soprano", line2, cInfo, octLoc, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::C_LINE4, ClefInfo{ "Tenor", line4, cInfo, octLoc, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::C_LINE5, ClefInfo{ "Baritone", line5, cInfo, octLoc, octaveForClef4 } },
    std::pair<ClefType, ClefInfo>{ ClefType::PERCUSSION, ClefInfo{ "Percussion1", line2, pInfo, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::PERCUSSION_2, ClefInfo{ "Percussion2", line2, pInfo, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::PERCUSSION_3, ClefInfo{ "Percussion1", line3, p2Info, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::TAB, ClefInfo{ "Tab", line3, tInfo, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::TAB_2, ClefInfo{ "??", line3, tInfo, octLoc, octaveForClef5 } },
    std::pair<ClefType, ClefInfo>{ ClefType::UNKNOWN, ClefInfo{ "Unknown", line2, gInfo, octLoc, octaveForClef5 } },
};

Clef::Clef(const MusicLocation& inLocation,
           ClefType inType,
           bool inSmall)
: Spatial(inLocation)
, mType(inType)
, mSmall(inSmall)
{
    setSlot(mType == static_cast<ClefType>(3) ? 4 : 2);
}

const ItemTraits&
Clef::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Clef);
}

ItemUPtr
Clef::getMemento() const
{
    return std::make_unique<Clef>(*this);
}

void
Clef::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Clef* downcastedP = dynamic_cast<const Clef*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    update();
    
    setIsQuiescent(wasQuiescent);
}

void
Clef::update()
{
    if (getIsQuiescent()) {
        return;
    }
    notifyObservers({ItemChangeType::Updated});
}

void
Clef::inserted()
{
    update();
    notifyObservers({ItemChangeType::Inserted});
}

void
Clef::removed()
{
    notifyObservers({ItemChangeType::Removed});
}

void
Clef::removeSelf()
{
    Spatial::removeSelf();
    notifyObservers({ItemChangeType::Removed});
}

void
Clef::createDrawable()
{
    ItemImpl::createDrawable<DrawableClef>(*this);
}

Rect
Clef::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    return drawable->getFrame(getDrawPoint());
}

Region
Clef::getRegion() const
{
    return ItemImpl::getRegion<DrawableClef>(*this);
}

double
Clef::getScoreX() const
{
    double result = 0;
    
    const auto& measure = getLocation().getMeasure();
    const auto clefX = getX();
    const auto measureX = measure.getX();
    result = measureX + clefX;
    
    return result;
}

double
Clef::getScoreY() const
{
    const auto& clefData = findClefData();
    const auto slot = clefData.NormalSizeDrawSlot;
    const auto& location = getLocation();
    const auto& staff = location.getStaff();
    const auto pixel = staff.slotToPixel(slot);
    const auto posY = pixel + clefData.NormalSizePixelOffset;
    return posY;
}

Point
Clef::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}

/// \brief Get the info from FontInstance, reduces cut-and-paste code
const ClefData&
Clef::findClefData() const
{
    return FontTheme::instance().ClefTable.at(mType);
}

/// \brief Get the info about clef
const ClefInfo&
Clef::clefInfo() const
{
    return Clef::clefInfo(mType);
}

/// \brief Look up information about the ClefType in the static ClefInfoTable
const ClefInfo&
Clef::clefInfo(ClefType clefType)
{
    const auto iter = ClefInfoTable.find(clefType);
    if (iter == ClefInfoTable.cend()) {
        KOMP_ASSERT(ClefInfoTable.size() > 0);
        return ClefInfoTable.cbegin()->second;
    }
    return iter->second;
}

} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
