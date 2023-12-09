// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <deque>

// Local
#include "Curve.h"
#include "Forward.h"
#include "Throw.h"

namespace komp
{
    class MxCurveStack
    {
    public:

        /// returns the numberLevel of the added curve
        int push(Curve& inCurve)
        {
            auto predicate = [&](const std::pair<int, Curve&>& inFilterPair) {
                return &inFilterPair.second == &inCurve;
            };

            const auto iter = std::find_if(mCurves.cbegin(),
                                           mCurves.cend(),
                                           predicate);

            if (iter != mCurves.cend()) {
                KOMP_THROW("curve already added to the stack");
            }

            const int max = getCurrentMax();
            const int newNumberLevel = max + 1;
            mCurves.emplace_back(newNumberLevel, inCurve);
            return newNumberLevel;
        }

        /// returns the numberLevel of the popped curve
        int pop(Curve& inCurve)
        {
            auto found = mCurves.cend();

            for (auto it = mCurves.cbegin(); it != mCurves.cend(); ++it) {
                if (&it->second.get() == &inCurve) {
                    found = it;
                    break;
                }
            }

            if (found == mCurves.cend()) {
                LOG_F(WARNING, "curve not found in stack");
                return 0;
            }

            const auto result = found->first;
            mCurves.erase(found);
            return result;
        }

        /// returns the current maximum numberLevel
        int getCurrentMax() const
        {
            int max = 0;
            for (const auto& pair : mCurves) {
                if (pair.first > max) {
                    max = pair.first;
                }
            }
            return max;
        }

    private:
        std::deque<std::pair<int, CurveRef>> mCurves;

    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
