// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"
#include "Text.h"
#include "Throw.h"

namespace komp
{
    class CurrentState
    {
    public:
        CurrentState() = delete;
        
        const DrawableContext& getContext() const { return mContext; }
        Measure& getMeasure() const { return mMeasure; }
        Staff& getStaff() const { return mStaff; }
        Track& getTrack() const { return mTrack; }
        int getVoice() const { return static_cast<int>(mVoice); }
        int getHand() const { return static_cast<int>(mHand); }
        int getX() const { return mX; }
        char getSlot() const { return mSlot; }
        const Point& getPoint() const { return mPoint; }

    private:
        friend class Score;
        
        CurrentState(const DrawableContext& inContext,
                     Measure& inMeasure,
                     Staff& inStaff,
                     Track& inTrack,
                     int inHand,
                     int inVoice,
                     char inSlot,
                     int inX,
                     const Point& inPoint)
        : mContext{inContext}
        , mMeasure{inMeasure}
        , mStaff{inStaff}
        , mTrack{inTrack}
        , mX{inX}
        , mSlot{inSlot}
        , mVoice{inVoice}
        , mHand{inHand}
        , mPoint{inPoint}
        {
        }
        
        const DrawableContext& mContext;
        Measure& mMeasure;
        Staff& mStaff;
        Track& mTrack;
        int mX;             // The x offset relative to the measure DataX
        char mSlot;         // Staff space position
        int mVoice;
        int mHand;
        Point mPoint;
    };

} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
