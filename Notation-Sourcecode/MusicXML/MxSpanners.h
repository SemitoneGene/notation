// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Spanner.h"
#include "Score.h"

namespace komp
{
	template <typename T>
	SpannerParams createSpannerParams(const T& mxSpannerCollector,
                                      const Score& inScore)
	{
            KOMP_ASSERT(mxSpannerCollector.startMeasureIndex >= 0);
            KOMP_ASSERT(mxSpannerCollector.startMeasureIndex < inScore.getMeasureCount());
            const auto& startMeasure = inScore.getMeasure(mxSpannerCollector.startMeasureIndex);
            KOMP_ASSERT(mxSpannerCollector.trackIndex >= 0);
            KOMP_ASSERT(mxSpannerCollector.trackIndex < inScore.getTracks().size());
            const auto& stopMeasure = inScore.getMeasure(mxSpannerCollector.stopMeasureIndex);
            
            SpannerParams params;
            params.hand = mxSpannerCollector.handIndex;
            params.startTime = mxSpannerCollector.startTime;
            params.startMeasure = startMeasure;
            params.endTime = mxSpannerCollector.stopTime;
            params.endMeasure = stopMeasure;
            
            return params;
    }

	inline void insertSpanner(const SpannerParams& spannerParams,
                              Spanner& spanner)
	{
    	KOMP_ASSERT(spannerParams.startMeasure != nullptr);
        spanner.setIsQuiescent(false);
        spannerParams.startMeasure->insertItem(spanner, spannerParams.startTime);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
