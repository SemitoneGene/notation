// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <bitset>
#include <vector>

// Local
#include "Forward.h"
#include "MeasureNumberInfo.h"

namespace komp
{
    class StaffSystem
    {
    public:
        StaffSystem(const Score& inScore);
        
        StaffSystem() = delete;
        StaffSystem(const StaffSystem& inStaffSystem) = delete;
        StaffSystem& operator=(const StaffSystem& inStaffSystem) = delete;
        StaffSystem(StaffSystem&& inStaffSystem) = delete;
        StaffSystem& operator=(StaffSystem&& inStaffSystem) = delete;
        
        void draw(const DrawableContext& inContext);
        
        const std::vector<SystemBracketUPtr>& getBrackets() const;

        double getInsetLeft() const { return mInsetLeft; }
        double getInsetRight() const { return mInsetRight; }
        
        double getX() const;
        void setX(double inX);

        double getY() const { return mInsetTop; }
        void setY(double inValue) { mInsetTop = inValue; }

        double getRight() const;
        double getWidth() const;
        double getHeight() const;

        size_t getFirstMeasure() const { return mFirstMeasure; }
        size_t getLastMeasure() const  { return mFirstMeasure + mMeasureCount - 1; }
        size_t getMeasureCount() const { return mMeasureCount; }
        
        int getIndex() const;
        std::vector<StaffSystemUPtr>::const_iterator iter() const;
        
        double getMaximumBracketWidth() const;
        double getLeftMostBracketPosition() const;
        
        const StaffUPtrVec& getStaves() const { return mStaves; }
        Staff& getStaff(const Track& inTrack, int inHand) const;
        
        /// Tell the staves to update their measure indices
        void updateMeasures();

    private:
        friend class Score;
        friend class ScoreLayout;
        
        void addStaff(StaffUPtr inStaff);
        void removeStaff(Track& inTrack, int inHand);

        void clearBrackets();
        void addBracket(const SystemBracketInfo& inInfo);
        
        void setFirstMeasure(size_t inValue);
        void setMeasureCount(size_t inValue) { mMeasureCount = inValue; }
        
        void setInsetLeft(double inValue);
        void setInsetRight(double inValue) { mInsetRight = inValue; }

        StaffUPtrVec::const_iterator getStaffIter(const Track& inTrack,
                                                  int inHand) const;
        void configure();
        void reconcile(const TrackUPtrVec& inTracks);

        double getMeasureWidth() const;

        void updateBracketInset();

        const Score& mScore;
        size_t mFirstMeasure{0};        ///< Index of measure at left edge
        size_t mMeasureCount{0};        ///< Number of measures on line
        double mInsetLeft{0};           ///< Distance from left margin (1/72)
        double mInsetRight{0};          ///< Distance from right margin (1/72)
        double mInsetTop{0};            ///< Distance from top of page (1/72)
        
        SystemBracketUPtrVec mBrackets;
        StaffUPtrVec mStaves;
        DrawableBarlineUPtr mSystemBarline{nullptr};
    };
    
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
