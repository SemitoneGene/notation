// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class SystemBracketType
    {
        NONE,
        BRACKET,
        BRACE,
        LINE,
        SQUARE
    };

    class SystemBracketInfo
    {
    public:
        SystemBracketInfo(SystemBracketType inType,
                          int inLevel,
                          int inTrackStart,
                          int inTrackEnd)
        : type(inType)
        , level(inLevel)
        , trackStart(inTrackStart)
        , trackEnd(inTrackEnd)
        {
        }
        
        SystemBracketInfo()
        : type{SystemBracketType::BRACE}
        , level{0}
        , trackStart{0}
        , trackEnd{0}
        {}
        
        SystemBracketType type;
        int level;
        int trackStart;
        int trackEnd;
    };
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
