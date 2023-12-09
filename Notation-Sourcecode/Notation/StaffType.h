// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
enum class StaffType
{
    NOTATION,
    TABLATURE,
    PERCUSSION
};

inline StaffType parseStaffTypeString(const std::string& value)
{
    if (value == "NOTATION") {
        return StaffType::NOTATION;
    } else if (value == "TABLATURE") {
        return StaffType::TABLATURE;
    } else if (value == "PERCUSSION") {
        return StaffType::PERCUSSION;
    }
    return StaffType::NOTATION;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
