// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "DrawableBracket.h"
#include "SystemBracketInfo.h"

namespace komp
{
    using SystemBracketInfoList = std::vector<SystemBracketInfo>;

    class SystemBracket
    {
    public:        
        SystemBracket(SystemBracketType inType,
                      int inLevel,
                      Staff& inStart,
                      Staff& inStop);
        SystemBracket(const SystemBracket& inSystemBracket);
        
        void configure();
        
        void draw(const DrawableContext& inContext, double inX) const;
        
        double getWidth() const;
        
        Staff& getStartStaff() const { return mStaffStart; }
        Staff& getStopStaff() const { return mStaffStop; }
        SystemBracketType getType() const { return mType; }
        
        SystemBracketInfo getInfo() const;
        
    private:
        SystemBracket() = delete;
        
        SystemBracketType mType;
        
        // The indentation level
        int mLevel;
        
        // Page staff at top
        Staff& mStaffStart;
        
        // Page staff at bottom
        Staff& mStaffStop;
        
        mutable std::unique_ptr<DrawableBracket> mDrawableBracket;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
