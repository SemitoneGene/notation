// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxLocation.h"
#include "Constants.h"
#include "Throw.h"

// Local Frameworks
#include "mx/api/ApiCommon.h"

namespace komp
{
    MxLocation::MxLocation(MxLocationParameters&& inParams)
    : partIndex{0}
    , handIndex{0}
    , mxMeasureIndex{0}
    , voiceIndex{0}
    , mxTickPosition{0}
    , mParams{ std::move( inParams ) }
    {
        if (mParams.mxTicksPerQuarter < 1) {
            mParams.mxTicksPerQuarter = 1;
        }
        KOMP_ASSERT(mParams.kompTicksPerQuarter > 0);
        KOMP_ASSERT(mParams.totalMxMeasures > 0);
        if (mParams.multiMeasureRests.size() > 0) {
            KOMP_ASSERT(mParams.multiMeasureRests.cbegin()->first >= 0);
        }
        createMeasureIndexMap();
    }

    int
    MxLocation::getKompTickPosition() const
    {
        const auto mxTime = static_cast<long double>(mxTickPosition);
        const auto returnValue = mxTime * getTickConversionRatio();
        return static_cast<int>(std::ceil(returnValue - 0.5L));
    }

    int
    MxLocation::getMxTicksPerQuarter() const
    {
        return mParams.mxTicksPerQuarter;
    }
    
    int
    MxLocation::getKompTicksPerQuarter() const
    {
        return mParams.kompTicksPerQuarter;
    }
    
    int
    MxLocation::getTotalMxMeasures() const
    {
        return mParams.totalMxMeasures;
    }
    
    long double
    MxLocation::getTickConversionRatio() const
    {
        KOMP_ASSERT(mParams.mxTicksPerQuarter > 0);
        return static_cast<long double>(mParams.kompTicksPerQuarter) /
               static_cast<long double>(mParams.mxTicksPerQuarter);
    }
    
    void
    MxLocation::createMeasureIndexMap()
    {
        const auto& rests = multiMeasureRestInfo();
        if (rests.size() == 0) {
            for(auto i = 0; i < getTotalMxMeasures(); ++i ) {
                mMeasureIndexMap[i] = i;
            }
        }
        int mxMeasureIndexCounter = 0;
        int kompMeasureIndexCounter = 0;
        int remainingMeasuresInRest = 0;
        
        for (; mxMeasureIndexCounter < getTotalMxMeasures(); ++mxMeasureIndexCounter) {
            bool isFirstMeasureOfMultiMeasureRest = false;
            auto iter = rests.find(mxMeasureIndexCounter);
            if (iter != rests.end()) {
                if (iter->second > 1 && remainingMeasuresInRest <= 0) {
                    remainingMeasuresInRest = iter->second;
                    isFirstMeasureOfMultiMeasureRest = true;
                }
            }
            
            if (remainingMeasuresInRest > 0 && !isFirstMeasureOfMultiMeasureRest) {
                mMeasureIndexMap[mxMeasureIndexCounter] = -1;
            } else {
                mMeasureIndexMap[mxMeasureIndexCounter] = kompMeasureIndexCounter++;
            }
            --remainingMeasuresInRest;
        }
    }
    
    int
    MxLocation::getKompMeasureIndex() const
    {
        KOMP_ASSERT(mxMeasureIndex < getTotalMxMeasures());
        return mMeasureIndexMap.at(mxMeasureIndex);
    }
    
    int
    MxLocation::getTotalKompMeasures() const
    {
        auto iter = mMeasureIndexMap.crbegin();
        auto cend = mMeasureIndexMap.crend();
        for ( ; iter != cend; ++iter) {
            if (iter->second >= 0) {
                return iter->second + 1;
            }
        }
        return -1;
    }
    
    const std::map<int, int>&
    MxLocation::multiMeasureRestInfo()
    {
        return mParams.multiMeasureRests;
    }
    
    bool
    operator==(const MxLocation& lhs, const MxLocation& rhs)
    {
        bool result = true;
        result &= (lhs.partIndex == rhs.partIndex);
        result &= (lhs.handIndex == rhs.handIndex);
        result &= (lhs.mxMeasureIndex == rhs.mxMeasureIndex);
        result &= (lhs.voiceIndex == rhs.voiceIndex);
        result &= (lhs.mxTickPosition == rhs.mxTickPosition);
        result &= (lhs.getMxTicksPerQuarter() == rhs.getMxTicksPerQuarter());
        result &= (lhs.getKompTicksPerQuarter() == rhs.getKompTicksPerQuarter());
        result &= (lhs.getKompTickPosition() == rhs.getKompTickPosition());
        result &= (lhs.getKompMeasureIndex() == rhs.getKompMeasureIndex());
        result &= (lhs.getTotalMxMeasures() == rhs.getTotalMxMeasures());
        result &= (lhs.getTotalKompMeasures() == rhs.getTotalKompMeasures());
        return result;
    }
    
    
    bool
    operator!=(const MxLocation& lhs, const MxLocation& rhs)
    {
        return !(lhs == rhs);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
