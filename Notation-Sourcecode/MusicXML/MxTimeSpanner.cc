// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxTimeSpanner.h"

// Local
#include "Measure.h"
#include "Track.h"
#include "Score.h"

namespace komp
{

MxTimeSpanner::MxTimeSpanner(int inNumberLevel, TimePoint& inTimePoint)
: mNumberLevel{inNumberLevel}
, mStart(inTimePoint)
{
    validateTimePoint(inTimePoint);
}

const TimePoint&
MxTimeSpanner::getStart() const
{
    return mStart;
}

const TimePoint&
MxTimeSpanner::getStop() const
{
    return mStop;
}

bool
MxTimeSpanner::setStop(TimePoint& inTimePoint)
{
    validateTimePoint(inTimePoint);
    const auto startMeasureIndex = mStart.measure->getIndex();
    const auto stopMeasureIndex = inTimePoint.measure->getIndex();
    
    if (stopMeasureIndex < startMeasureIndex) {
        LOG_F(WARNING, "a spanner was encountered with stopMeasureIndex < startMeasureIndex");
        return false;
    }
    
    if (stopMeasureIndex == startMeasureIndex) {
        if (inTimePoint.time >= mStart.time) {
            LOG_F(WARNING, "a spanner was encountered with stopTime < startTime");
            return false;
        }
    }
    
    mStop = inTimePoint;
    return true;
}

int
MxTimeSpanner::getNumberLevel() const
{
    return mNumberLevel;
}

SpannerParams
MxTimeSpanner::createSpannerParams() const
{
    KOMP_ASSERT(mStop.measure != nullptr);
    SpannerParams params;
    params.hand = mStart.hand;
    params.startMeasure = mStart.measure;
    params.startTime = mStart.time;
    params.endMeasure = mStop.measure;
    params.endTime = mStop.time;
    return params;
}

void
MxTimeSpanner::validateTimePoint(const TimePoint& inTimePoint) const
{
    KOMP_ASSERT(inTimePoint.measure != nullptr);
    KOMP_ASSERT(inTimePoint.track != nullptr);
    KOMP_ASSERT(inTimePoint.hand >= 0);
    KOMP_ASSERT(inTimePoint.time >= 0);
    KOMP_ASSERT(inTimePoint.hand < inTimePoint.track->getHands());
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
