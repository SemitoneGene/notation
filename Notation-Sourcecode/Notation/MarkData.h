// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <sstream>

// Local
#include "Forward.h"
#include "PlacementType.h"
#include "MarkType.h"
#include "MarkPlacement.h"

namespace komp
{
struct MarkTextParams
{
    MarkType markType;
    double offsetX;
    double offsetY;
    PlacementType placement;
    MarkCategory category;
    MarkPlacementUPtr markPlacement;
    std::string expressionText;
};

struct MarkSmuflParams
{
    MarkType markType;
    unsigned short character;
    double offsetX;
    double offsetY;
    PlacementType placement;
    MarkCategory category;
    MarkPlacementUPtr markPlacement;
    std::string smuflName;
};

enum class MarkDataType
{
    TEXT,
    SMUFL
};

/// \discussion MarkData is populated with information about placement information
/// for a symbol.  Horizontal offset is specified in pixels.  Vertical
/// offset is specified in staff slots.  Placement type defaults to
/// automatic, but can be set explicitly for symbols that have specific
/// placement constraints.
class MarkData
{
public:
    MarkData(MarkSmuflParams inSmuflParams);
    MarkData(MarkTextParams inTextParams);
    
    MarkData(const MarkData& other);
    MarkData(MarkData&& other);
    MarkData& operator=(const MarkData& other);
    MarkData& operator=(MarkData&& other);
    
    MarkType getMarkType() const;
    
    unsigned short getCharacter() const;
    std::string_view getString() const;
    
    double getOffsetX() const;
    double getOffsetY() const;
    
    PlacementType getPlacementType() const;
    void setPlacementType(PlacementType inType) { mPlacement = inType; }
    
    MarkCategory getMarkCategory() const;
    MarkPlacementUPtr getMarkPlacement() const;
    
    /// Returns true if this MarkData's category matches
    /// the input MarkCategory.  For example:
    /// markData.is(MarkType::DYNAMIC) returns true if
    /// the MarkData is a dynamic.
    bool is(MarkCategory inMarkCategory) const;
    
private:
    MarkType mMarkType;               ///< The MarkType enum value
    unsigned short mCharacter;        ///< The unicode symbol
    std::string mSmufl;               ///< The SMUFL Name
    double mOffsetX;                  ///< Horizontal offset in pixels
    double mOffsetY;                  ///< Vertical offset in slots
    PlacementType mPlacement;         ///< TODO - replace with a more robust placement system
    MarkCategory mMarkCategory;       ///< Articulation vs. Dynamic, etc.
    MarkPlacementUPtr mMarkPlacement; ///< Strategy object for placing the mark
    MarkDataType mMarkDataType;       ///< Does the mark represent a Smufl Glyph or a Text Expression
};

bool operator<(const MarkData& l, const MarkData& r);
bool operator>(const MarkData& l, const MarkData& r);
bool operator==(const MarkData& l, const MarkData& r);
bool operator!=(const MarkData& l, const MarkData& r);
bool operator<=(const MarkData& l, const MarkData& r);
bool operator>=(const MarkData& l, const MarkData& r);

} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
