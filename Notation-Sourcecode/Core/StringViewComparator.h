// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

struct StringViewComparator
{
    // Enables heterogenous lookup
    using is_transparent = void;
    
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return lhs < rhs;
    }
    
    bool operator()(const std::string_view lhs, const std::string& rhs) const
    {
        return lhs < rhs;
    }
    
    bool operator()(const std::string& lhs, const std::string_view rhs) const
    {
        return lhs < rhs;
    }
};

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
