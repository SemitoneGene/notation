// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <map>

// Local
#include "Spanner.h"

namespace komp
{
    class Measure;

    class Track;

    struct TimePoint
    {
        Measure* measure{nullptr};
        Track* track{nullptr};
        int hand{0};
        int time{0};
    };

    class MxTimeSpanner;
    using MxTimeSpannerPtr = std::unique_ptr<MxTimeSpanner>;    
    
    class MxTimeSpanner
    {
    public:
        MxTimeSpanner(int inNumberLevel, TimePoint& inTimePoint);
        virtual ~MxTimeSpanner() {};
        virtual const TimePoint& getStart() const final;
        virtual const TimePoint& getStop() const final;
        virtual bool setStop(TimePoint& inTimePoint) final;
        virtual int getNumberLevel() const final;
        virtual SpannerParams createSpannerParams() const final;

        using PAYLOAD_TYPE = TimePoint;
        

    private:
        int mNumberLevel;
        TimePoint mStart;
        TimePoint mStop;

    private:
        void validateTimePoint(const TimePoint& inTimePoint) const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
