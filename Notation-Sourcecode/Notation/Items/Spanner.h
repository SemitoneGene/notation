// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "SpannerParams.h"
#include "Spatial.h"

namespace komp
{
    class KOMP_EXPORT Spanner : public Spatial
    {
    public:
        Spanner(const MusicLocation& inLocation,
                const SpannerParams& inParams);
        Spanner(const Spanner& inObject);
        
        Spanner& operator=(const Spanner& other);
        
        virtual ~Spanner() {}

        virtual const ItemTraits& getItemTraits() const override = 0;
        virtual double getX() const override;
        virtual void setX(double inValue) override;
        
        void setEndMeasure(Measure* inValue);
        Measure* getEndMeasure() const;
        int getEndTime() const;
        void setEndTime(int inValue);
        double getEndX() const;
        
        int duration() const;
        
    private:
        Spanner() = delete;
        
        void copyState(const Spanner& other);
        void configure(const SpannerParams& inParams);
        
        int calculateDuration(Measure& inStartMeasure,
                              int inStartTime,
                              Measure& inStopMeasure,
                              int inStopTime) const;
        
    protected:
        Measure* mEndMeasure;
        int mEndSlot;
        int mEndTime;
        int mDuration;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
