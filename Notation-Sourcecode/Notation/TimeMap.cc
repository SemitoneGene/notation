// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TimeMap.h"

// System
#include <cmath>

// Local
#include "Throw.h"

namespace komp
{
	TimeMap::TimeMap()
    : mClocks{-1}
    , mWidth{-1.0}
    , mTimePoints{}
	{
		
	}

    double
    TimeMap::getWidth() const
    {
        return mWidth;
    }
    
    
    void
    TimeMap::setWidth(double inWidth)
    {
        KOMP_ASSERT(inWidth > 0.0);
        auto oldWidth = mWidth;
        mWidth = inWidth;
        
        if (oldWidth <= 0) {
            return;
        }
        
        if (oldWidth == mWidth) {
            return;
        }
        
        double factor = inWidth / oldWidth;
        
        for (auto& timePoint : mTimePoints) {
            timePoint.second *= factor;
        }
    }
    
    
    bool
    TimeMap::getIsSet() const
    {
        return getWidth() > 0 && getClocks() > 0;
    }
    
    
    bool
    TimeMap::getIsEmpty() const
    {
        return mTimePoints.empty();
    }
    
    
    int
    TimeMap::getClocks() const
    {
        return mClocks;
    }
    
    
    void
    TimeMap::setClocks(int inClocks)
    {
        KOMP_ASSERT(inClocks > 0);
        int oldClocks = mClocks;
        mClocks = inClocks;
        
        if (oldClocks <= 0) {
            return;
        }
        
        if (oldClocks == mClocks) {
            return;
        }
        
        double factor = static_cast<double>(inClocks) / static_cast<double>(oldClocks);
        std::map<int, double> newPoints;
        
        for (const auto& timePoint : mTimePoints) {
            double newTime = timePoint.first * factor;
            int newTimeRounded = static_cast<int>(std::ceil(newTime - 0.5));
            newPoints[newTimeRounded] = timePoint.second;
        }
        std::swap(mTimePoints, newPoints);
    }
    
    
    bool
    TimeMap::addPoint(double inTime, double inX)
    {
        if (inTime < 0) {
            return false;
        }
        
        if (inTime >= getClocks()) {
            return false;
        }
        
        if (inX >= getWidth()) {
            return false;
        }
        
        auto iter = mTimePoints.find(inTime);
        
        if (iter != mTimePoints.cend()) {
            return false;
        }
        
        auto lowerBound = mTimePoints.lower_bound(inTime);

        if (lowerBound != mTimePoints.cend() && lowerBound->second <= inX) {
            return false;
        }
        
        auto upperBound = mTimePoints.upper_bound(inTime);
        
        if (upperBound != mTimePoints.cend() && upperBound->second >= inX) {
            return false;
        }
        
        mTimePoints.emplace(inTime, inX);
        return true;
    }
    
    
    int
    TimeMap::getTimeForX(double inX)
    {
        KOMP_ASSERT(getIsSet());
        const auto firstPair = mTimePoints.cbegin();
        const auto pointsEnd = mTimePoints.cend();

        if (firstPair != pointsEnd) {
            if (firstPair->first == 0.0) {
                if (inX <= firstPair->second) {
                    return 0.0;
                }
            }
        } else if (inX <= 0.0) {
            return 0.0;
        }

        if (inX >= getWidth()) {
            return getClocks();
        }
        
        auto finderNext = [&](const std::pair<int, double>& inPoint) {
            return inPoint.second >= inX;
        };
        
        auto nextPoint = std::find_if(mTimePoints.cbegin(), mTimePoints.cend(), finderNext);

        int nextTime = getClocks();
        double nextX = getWidth();
        
        if (nextPoint != mTimePoints.cend()) {
            nextTime = nextPoint->first;
            nextX = nextPoint->second;
        }
        
        auto finderPrev = [&](const std::pair<int, double>& inPoint) {
            return inPoint.second <= inX;
        };
        
        auto prevPoint = std::find_if(mTimePoints.crbegin(), mTimePoints.crend(), finderPrev);

        int prevTime = 0;
        double prevX = 0.0;
        
        if (prevPoint != mTimePoints.crend()) {
            prevTime = prevPoint->first;
            prevX = prevPoint->second;
        }
        
        if (inX == prevX) {
            return prevTime;
        }

        const double slope = calculateSlopeX(prevX, prevTime, nextX, nextTime);
        const double distanceFromPrev = inX - prevX;
        const double result = static_cast<double>(prevTime) + (distanceFromPrev * slope);
        return static_cast<int>(std::ceil(result - 0.5));
    }
    
    
    double
    TimeMap::getXForTime(double inTime)
    {
        KOMP_ASSERT(getIsSet());
        
        if (inTime <= 0) {
            const auto firstPair = mTimePoints.cbegin();

            if (firstPair != mTimePoints.cend()) {
                if (firstPair->first == 0.0) {
                    return firstPair->second;
                } else {
                    return 0.0;
                }
            } else {
                return 0.0;
            }
        }
        
        if (inTime >= getClocks()) {
            return getWidth();
        }

        auto nextPoint = mTimePoints.find(inTime);
        
        if (nextPoint == mTimePoints.cend()) {
            nextPoint = mTimePoints.upper_bound(inTime);
        }
        
        int nextTime = getClocks();
        double nextX = getWidth();
        
        if (nextPoint != mTimePoints.cend()) {
            nextTime = nextPoint->first;
            nextX = nextPoint->second;
        }

        auto finderPrev = [&](const std::pair<int, double>& inPoint) {
            return inPoint.first <= inTime;
        };
        
        auto prevPoint = std::find_if(mTimePoints.crbegin(), mTimePoints.crend(), finderPrev);
        
        int prevTime = 0;
        double prevX = 0.0;
        
        if (prevPoint != mTimePoints.crend()) {
            prevTime = prevPoint->first;
            prevX = prevPoint->second;
        }
        
        if (prevTime == inTime) {
            return prevX;
        }
        
        const double slope = calculateSlopeT(prevX, prevTime, nextX, nextTime);
        const double distanceFromPrev = inTime - prevTime;
        const double result = static_cast<double>(prevX) + (distanceFromPrev * slope);
        return result;
    }
    
    
    void
    TimeMap::clearPoints()
    {
        mTimePoints.clear();
    }
    
    
    void
    TimeMap::invalidate()
    {
        clearPoints();
        mWidth = -1;
        mClocks = -1;
    }
    
    double
    TimeMap::calculateSlopeX(double prevX,
                             int prevTime,
                             double nextX,
                             int nextTime) const
    {
        const double deltaX = nextX - prevX;
        const double deltaT = static_cast<double>(nextTime - prevTime);
        KOMP_ASSERT(deltaX != 0.0);
        double slope = deltaT / deltaX;
        return slope;
    }
    
    double
    TimeMap::calculateSlopeT(double prevX,
                             int prevTime,
                             double nextX,
                             int nextTime) const
    {
        const double deltaX = nextX - prevX;
        const double deltaT = static_cast<double>(nextTime - prevTime);
        KOMP_ASSERT(deltaT != 0.0);
        double slope = deltaX / deltaT;
        return slope;
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
