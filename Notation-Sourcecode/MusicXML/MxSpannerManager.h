// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <memory>
#include <map>

namespace komp
{
    class Temporal;
    
    template<typename T>
    class MxSpannerManager
    {
    public:
        
        using SpannerTypeUPtr = std::unique_ptr<T>;

        struct MapSort
        {
            bool operator()(int a, int b) const
            {
                if (a > 0) {

                    if (b > 0) {
                        return a > b;
                    } else {
                        return false;
                    }

                } else {

                    if (b > 0) {
                        return true;
                    } else {
                        return a > b;
                    }

                }
            }
        };

        using SpannerMap = std::map<int, SpannerTypeUPtr, MapSort>;
        using SpannerVec = std::vector<SpannerTypeUPtr>;
        using SpannerMapIter = typename SpannerMap::const_iterator;
        using SpannerVecIter = typename SpannerMap::const_iterator;
        
        /// returns the 'stack id' which is similar to musicxml's
        /// 'number-level', used to identify the corresponding start
        /// or stop when spanners are encountered declaratively. in
        /// our case we will also use it to identify the spanners
        /// which have no 'number-level' in the musicxml document.
        /// these will have negative values that become more negative
        /// as the un-numbered stack grows deeper. the positive
        /// numbered stack id's will always match those found in
        /// musicxml 'number' attributes.  basically use this function
        /// when the musicxml did not specify a 'number' attribute and
        /// probably just ignore the return value.
        int push(SpannerTypeUPtr&& spanner);

        /// use this function when the musicxml specified a 'number'
        /// attribute. returns true if successful, returns false if
        /// failed because a spanner with the same numberLevel already
        /// existed on the stack
        bool push(int numberLevel, SpannerTypeUPtr&& spanner);

        /// find best candidate spanner on the stack
        /// complete the spanner, add it to the completed
        /// spanners vector and return true. else false.
        bool completeSpanner(typename T::PAYLOAD_TYPE& stopItem);

        /// if numberLevel is positive, find the matching spanner on the
        /// stack, complete it, and add it to the completed spanners vector.
        /// if numberLevel is <= 0, find the most recently created un-
        /// numbered spanner, complete it and add to the spanners vector.
        /// returns true if successful, false if failed to complete a spanner.
        bool completeSpanner(int numberLevel, typename T::PAYLOAD_TYPE& stopItem);

        /// returns the collection of completed/valid spanners
        const SpannerVec& getSpanners() const;

        // returns iter into the collection of completed/valid spanners
        SpannerVecIter begin() const;
        
        /// returns end iter of the collection of completed/valid spanners
        SpannerVecIter end() const;

        // returns the number of completed/valid spanners
        int count() const;

        /// get the most recently created spanner (whether or not it is completed)
        const SpannerTypeUPtr& peek() const;

        void sortSpanners();
        void sortSpanners(const std::function<bool (const SpannerTypeUPtr& a,
                                                    const SpannerTypeUPtr& b)>& sortingFunction);

        inline int getIsSpannerStackEmpty() { return mSpannerStack.empty(); }

    private:
        SpannerMap mSpannerStack;
        SpannerVec mCompletedSpanners;
    };


}

// define the template class inline
#include "MxSpannerManagerImpl.h"

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
