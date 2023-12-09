// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Local
#include "Throw.h"
#include "Temporal.h"
#include "Measure.h"

namespace komp
{
    template<typename T>
    int
    MxSpannerManager<T>::push(SpannerTypeUPtr&& spanner)
    {
        int spannerId = -1;
        if (!mSpannerStack.empty()) {
            const int firstId = mSpannerStack.cbegin()->first;
            if (firstId <= 0) {
                spannerId = firstId - 1;
            }
        }
        mSpannerStack[spannerId] = std::move(spanner);
        return spannerId;
    }
    
    template<typename T>
    bool
    MxSpannerManager<T>::push(int numberLevel, SpannerTypeUPtr&& spanner)
    {
        KOMP_ASSERT(spanner != nullptr);
        
        if (numberLevel <= 0) {
            push(std::move(spanner));
            return true;
        }
        
        const auto iter = mSpannerStack.find(numberLevel);
        bool exists = iter != mSpannerStack.cend();
        if (exists) {
            return false;
        }
        
        mSpannerStack.emplace(std::pair<int, SpannerTypeUPtr>{numberLevel,std::move(spanner)});
        return true;
    }
    
    template<typename T>
    bool
    MxSpannerManager<T>::completeSpanner(typename T::PAYLOAD_TYPE& stopItem)
    {
        
        if (mSpannerStack.empty()) {
            return false;
        }
        
        auto iter = mSpannerStack.begin();
   
        // we prefer to pop using an un-named spanner using one from the negative numbers,
        // but if none is present then we will zip to the end of the map to pop the most
        // recently added 'named' spanner
        if (iter->first > 0) {
            for (auto i = mSpannerStack.begin(); i != mSpannerStack.end(); ++i) {
                iter = i;
            }
        }

        if (iter == mSpannerStack.end()) {
            LOG_F(ERROR, "Bad things are happening");
            return false;
        }

        if (!iter->second) {
            LOG_F(ERROR, "Very bad things are happening");
            mSpannerStack.erase(iter);
            return false;
        }

        SpannerTypeUPtr tempSpanner{std::move(iter->second)};

        if (!tempSpanner) {
            LOG_F(ERROR, "This is very bad");
            mSpannerStack.erase(iter);
            return false;
        }

        tempSpanner->setStop(stopItem);
        mCompletedSpanners.emplace_back(std::move(tempSpanner));
        mSpannerStack.erase(iter);
        return true;
    }
    
    template<typename T>
    bool
    MxSpannerManager<T>::completeSpanner(int numberLevel,
                                         typename T::PAYLOAD_TYPE& stopItem)
    {
        if (numberLevel <= 0) {
            return completeSpanner(stopItem);
        }
        
        if (mSpannerStack.empty()) {
            return false;
        }
        
        auto iter = mSpannerStack.find(numberLevel);
        
        if (iter == mSpannerStack.end()) {
            return false;
        }
        
        auto tempSpanner = std::move(iter->second);
        KOMP_ASSERT(tempSpanner != nullptr);
        tempSpanner->setStop(stopItem);
        mCompletedSpanners.emplace_back(std::move(tempSpanner));
        mSpannerStack.erase(iter);
        return true;
    }
    
    template<typename T>
    const typename MxSpannerManager<T>::SpannerVec&
    MxSpannerManager<T>::getSpanners() const
    {
        return mCompletedSpanners;
    }
    
    template<typename T>
    typename MxSpannerManager<T>::SpannerVecIter
    MxSpannerManager<T>::begin() const
    {
        return mCompletedSpanners.cbegin();
    }
    
    template<typename T>
    typename MxSpannerManager<T>::SpannerVecIter
    MxSpannerManager<T>::end() const
    {
        return mCompletedSpanners.cend();
    }
    
    template<typename T>
    int
    MxSpannerManager<T>::count() const
    {
        return static_cast<int>(mCompletedSpanners.size());
    }

    template<typename T>
    void
    MxSpannerManager<T>::sortSpanners()
    {
        // sort by starting measure, then by slur/tie numbering
        auto sortingMethod = [](const SpannerTypeUPtr& a,
                                const SpannerTypeUPtr& b) {
            const auto aMeasureIndex = a->getStart().getLocation().getMeasure().getIndex();
            const auto bMeasureIndex = b->getStart().getLocation().getMeasure().getIndex();
            if (aMeasureIndex != bMeasureIndex) {
                return aMeasureIndex < bMeasureIndex;
            }
            return a->getNumberLevel() < b->getNumberLevel();
        };
        
        sortSpanners(sortingMethod);
    }
    
    template<typename T>
    void
    MxSpannerManager<T>::sortSpanners(const std::function<bool (const SpannerTypeUPtr& a,
                                                                const SpannerTypeUPtr& b)>& sortingFunction)
    {
        std::ranges::sort(mCompletedSpanners, sortingFunction);
    }
    
    template<typename T>
    const typename MxSpannerManager<T>::SpannerTypeUPtr&
    MxSpannerManager<T>::peek() const
    {
        if (!mSpannerStack.empty()) {
            return mSpannerStack.cbegin()->second;
        }
        
        if (!mCompletedSpanners.empty()) {
            return mCompletedSpanners.front();
        }
        
        KOMP_THROW("peek called when empty");
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
