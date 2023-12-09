// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MarkData.h"

// Local
#include "DefaultMarkPlacement.h"

namespace komp
{
MarkData::MarkData(MarkSmuflParams inSmuflParams)
: mMarkType(std::move(inSmuflParams.markType))
, mCharacter(std::move(inSmuflParams.character))
, mSmufl(std::move(inSmuflParams.smuflName))
, mOffsetX(std::move(inSmuflParams.offsetX))
, mOffsetY(std::move(inSmuflParams.offsetY))
, mPlacement(std::move(inSmuflParams.placement))
, mMarkCategory(std::move(inSmuflParams.category))
, mMarkPlacement(std::move(inSmuflParams.markPlacement))
, mMarkDataType(MarkDataType::SMUFL)
{
    
}

MarkData::MarkData(MarkTextParams inTextParams)
: mMarkType(std::move(inTextParams.markType))
, mCharacter(0)
, mSmufl(std::move(inTextParams.expressionText))
, mOffsetX(std::move(inTextParams.offsetX))
, mOffsetY(std::move(inTextParams.offsetY))
, mPlacement(std::move(inTextParams.placement))
, mMarkCategory(std::move(inTextParams.category))
, mMarkPlacement(std::move(inTextParams.markPlacement))
, mMarkDataType(MarkDataType::TEXT)
{
    
}

MarkData::MarkData(const MarkData& other)
: mMarkType(other.mMarkType)
, mCharacter(other.mCharacter)
, mSmufl(other.mSmufl)
, mOffsetX(other.mOffsetX)
, mOffsetY(other.mOffsetY)
, mPlacement(other.mPlacement)
, mMarkCategory(other.mMarkCategory)
, mMarkPlacement(other.mMarkPlacement->clone())
, mMarkDataType(other.mMarkDataType)
{
    
}


MarkData::MarkData(MarkData&& other) = default;


MarkData& MarkData::operator=(const MarkData& other)
{
    mMarkType = other.mMarkType;
    mCharacter = other.mCharacter;
    mSmufl = other.mSmufl;
    mOffsetX = other.mOffsetX;
    mOffsetY = other.mOffsetY;
    mPlacement = other.mPlacement;
    mMarkCategory = other.mMarkCategory;
    mMarkPlacement = other.mMarkPlacement->clone();
    mMarkDataType = other.mMarkDataType;
    return *this;
}

MarkData& MarkData::operator=(MarkData&& other) = default;

MarkType
MarkData::getMarkType() const
{
    return mMarkType;
}

unsigned short
MarkData::getCharacter() const
{
    return mCharacter;
}

std::string_view
MarkData::getString() const
{
    return mSmufl;
}

double
MarkData::getOffsetX() const
{
    return mOffsetX;
}

double
MarkData::getOffsetY() const
{
    return mOffsetY;
}

PlacementType
MarkData::getPlacementType() const
{
    return mPlacement;
}


MarkCategory
MarkData::getMarkCategory() const
{
    return mMarkCategory;
}


MarkPlacementUPtr
MarkData::getMarkPlacement() const
{
    return mMarkPlacement->clone();
}


bool
MarkData::is(MarkCategory inMarkCategory) const
{
    return inMarkCategory == getMarkCategory();
}


bool
operator<(const MarkData& l, const MarkData& r)
{
    return static_cast<int>(l.getMarkType()) < static_cast<int>(r.getMarkType());
}


bool
operator>(const MarkData& l, const MarkData& r)
{
    return r < l;
}


bool
operator==(const MarkData& l, const MarkData& r)
{
    const bool isLessThan = (l < r);
    const bool isGreaterThan = (r < l);
    return !isLessThan && !isGreaterThan;
}


bool
operator!=(const MarkData& l, const MarkData& r)
{
    return !(l == r);
}


bool
operator<=(const MarkData& l, const MarkData& r)
{
    return (l < r) || ( l == r);
}


bool
operator>=(const MarkData& l, const MarkData& r)
{
    return (l > r) || ( l == r);
}


} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
