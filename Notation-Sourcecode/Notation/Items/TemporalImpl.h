// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"
#include "StemInfo.h"
#include "StemPolicy.h"
#include "Temporal.h"

namespace komp
{
    
    /// \brief  a container for code that is shared between most Temporals
    ///
    class TemporalImpl
    {
    public:
        static void setDuration(Temporal& outTemporal,
                                Duration inDuration,
                                Duration& mOutDuration);
        
        static void initializeStemPolicy(Temporal& outTemporal, StemPolicy& outStemPolicy);
        
        static void setStemPolicy(Temporal& outTemporal,
                                  const StemPolicy& inStemPolicy,
                                  StemPolicy& outStemPolicy);
        
        static PlaybackNote getPlaybackNote(const Temporal& inTemporal);
        
        static bool getIsTupleted(const TupletIntfRefVec& inTuplets);
        static TupletIntfRefVec getTuplets(const TupletIntfRefVec& inTuplets);
        
        static bool addTuplet(Temporal& outTemporal,
                              TupletIntfRefVec& outTuplets,
                              TupletIntf& inTuplet);
        
        static bool removeTuplet(Temporal& outTemporal,
                                 TupletIntfRefVec& outTuplets,
                                 TupletIntf& inTuplet);
        
        static void updateStemDirection(Temporal& outTemporal);
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
