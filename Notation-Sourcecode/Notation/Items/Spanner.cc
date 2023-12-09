// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Spanner.h"

// System
#include <iostream>

// Local
#include "Measure.h"
#include "Score.h"

namespace komp
{
    Spanner::Spanner(const MusicLocation& inLocation,
                     const SpannerParams& inParams)
    : Spatial(inLocation)
    , mEndSlot(0)
    {
        // Spatial's mX value will not be used by Spanners
        //Spatial::setX(0.0);
        
        configure(inParams);
    }
    
    Spanner::Spanner(const Spanner& inObject)
    : Spatial(inObject)
    , mEndSlot(inObject.mEndSlot)
    , mEndTime(inObject.mEndTime)
    , mEndMeasure(inObject.mEndMeasure)
    {
        copyState(inObject);
    }
    
    Spanner&
    Spanner::operator=(const Spanner& other)
    {
        copyState(other);
        return *this;
    }
    
    void
    Spanner::copyState(const Spanner& other)
    {
        if (this != &other) {
            Spatial::operator=(other);
            mEndMeasure = other.mEndMeasure;
            mEndSlot = other.mEndSlot;
            mEndTime = other.mEndTime;
        }
    }

    /// \brief Calculate the x-position based on times, not based on a stored value
    double
    Spanner::getX() const
    {
        const auto& measure = getLocation().getMeasure();
        const auto x = measure.positionForTime(getRoundedTime());
        return x;
    }
    
    /// \brief Intercepts the value and sets the Wedge's start time instead
    void
    Spanner::setX(double inValue)
    {
        const auto& measure = getLocation().getMeasure();
        const auto& loc = getLocation();
        auto& track = loc.getTrack();
        const auto time = measure.time(track, inValue);
        setTime(time);
    }
    
    /// \brief Calculate the total duration and the measure specific start and
    /// stop times. A Spanner can cross multiple measures and lines
    void
    Spanner::configure(const SpannerParams& inParams)
    {
        setTime(inParams.startTime);
        mEndMeasure = inParams.endMeasure;
        mEndTime = inParams.endTime;
        
#ifdef DEBUG
        const auto duration = calculateDuration(*inParams.startMeasure,
                                                inParams.startTime,
                                                *inParams.endMeasure,
                                                inParams.endTime);
        KOMP_ASSERT(duration >= 0);
#endif
    }
    
    void
    Spanner::setEndMeasure(Measure* inValue)
    {
        mEndMeasure = inValue;
    }
    
    void
    Spanner::setEndTime(int inValue)
    {
        mEndTime = inValue;
    }
    
    Measure*
    Spanner::getEndMeasure() const
    {
        return mEndMeasure;
    }

    int
    Spanner::getEndTime() const
    {
        return mEndTime;
    }
    
    double
    Spanner::getEndX() const
    {
        const auto& endMeasure = getEndMeasure();
        const auto endTime = getEndTime();
        return endMeasure->positionForTime(endTime);
    }

    int
    Spanner::duration() const
    {
        auto& startMeasure = getLocation().getMeasure();
        const auto& endMeasure = getEndMeasure();
        KOMP_ASSERT(endMeasure);
        const auto duration = calculateDuration(startMeasure,
                                                getRoundedTime(),
                                                *endMeasure,
                                                getEndTime());
        KOMP_ASSERT(duration > 0);
        return duration;
    }
    
    int
    Spanner::calculateDuration(Measure& inStartMeasure,
                               int inStartTime,
                               Measure& inStopMeasure,
                               int inStopTime) const
    {
        int outDuration = 0;
        
        const auto& startIter = inStartMeasure.iter();
        const auto endIter = inStopMeasure.iter();
        auto iter = startIter;
        
        for ( ; iter <= endIter; ++iter) {
            const auto& measure = *iter;
            int duration = measure->getClocks();
            
            if (startIter == iter) {
                duration -= getTime();
            }
            
            if (endIter == iter) {
                duration -= (duration - mEndTime);
            }
            
            outDuration += duration;
        }
        
        return outDuration;
    }
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
