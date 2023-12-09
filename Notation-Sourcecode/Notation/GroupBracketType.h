// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class GroupBracketType
    {
        Unknown,
        None,
        PianoBrace,
        Bracket,
    };
    
    inline std::string getGroupBracketTypeString(GroupBracketType value)
    {
        switch (value) {
            case GroupBracketType::Unknown: return "Unknown";
            case GroupBracketType::None: return "None";
            case GroupBracketType::PianoBrace: return "PianoBrace";
            case GroupBracketType::Bracket: return "Bracket";
            default: return "Unknown";
        }
    }
    
    inline GroupBracketType parseGroupBracketTypeString(const std::string& value)
    {
        if (value == "Unknown") {
            return GroupBracketType::Unknown;
        } else if (value == "None") {
            return GroupBracketType::None;
        } else if (value == "PianoBrace") {
            return GroupBracketType::PianoBrace;
        } else if (value == "Bracket") {
            return GroupBracketType::Bracket;
        } else {
            return GroupBracketType::Unknown;
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
