// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Forward declarations
namespace komp
{
    enum class MeasureNumberLocation;
    enum class MeasureNumberType;
}

namespace komp
{
    struct MeasureNumberInfo
    {
        MeasureNumberInfo();
        MeasureNumberInfo(const MeasureNumberInfo& inInfo);
        
        bool Box;                       // If true, enclose numbers in box
        int Increment;                  // Number increment
        MeasureNumberType Type;         // The type of measure number displayed
        MeasureNumberLocation Location; // The location of the number above or belowe the staff
        double Offset;                  // Number of spaces above or below the staff
    };
    
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
