// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>

namespace komp
{   /// Used by Measure to keep track of the relationship between
    /// spatial-x distance and time.
    class TimeMap
    {
    public:
        TimeMap();
        
        /// get the width value in pixels
        double getWidth() const;
        
        /// set the width value in pixels
        void setWidth(double inWidth);

        /// returns true if both the width and time are
        /// set to positive values
        bool getIsSet() const;
        
        /// returns true if there are no time points
        bool getIsEmpty() const;
        
        /// get the total clocks
        int getClocks() const;
        
        /// set the total clocks and update all existing
        /// time points accordingly
        void setClocks(int inClocks);
        
        /// adds a time point to the map, so long as the time
        /// is positive, is less than total clocks, and is not
        /// out-of-order with respect to the existing points
        /// returns false if it failed for any of the above
        bool addPoint(double inTime, double inX);
        
        /// for a given X, calculate the equivalent time position
        int getTimeForX(double inX);
        
        /// for a given Time, calculate the equivalent x position
        double getXForTime(double inTime);
        
        /// removes all time points from the map
        void clearPoints();
        
        /// removes all time points from the map nd sets the
        /// width and clocks to invalid values of -1.
        void invalidate();
        
    private:
        int mClocks; /// the number of clocks in the measure
        double mWidth;  /// the width of the measure
        std::map<int, double> mTimePoints; /// <time, x>
        
    private:
        
        /// calculates slope with x as the horizontal-axis
        double calculateSlopeX(double prevX,
                               int prevTime,
                               double nextX,
                               int nextTime) const;
        
        /// calculates slope with time as the horizontal-axis
        double calculateSlopeT(double prevX,
                               int prevTime,
                               double nextX,
                               int nextTime) const;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
