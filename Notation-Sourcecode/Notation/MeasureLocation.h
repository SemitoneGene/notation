// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{    
    /// Encapsulates a Measure, Track and Hand into a single structure
	///
	class MeasureLocation
	{
    public:
        MeasureLocation(Measure& inMeasure,
                        Track& inTrack,
                        int inHand);

        MeasureLocation() = delete;
        
        /// \brief Returns true if all pointers are non-expired and the location is valid
        bool getIsValid() const;

        Measure& getMeasure() const;
        Track& getTrack() const;
        int getHand() const;
        Staff& getStaff() const;
        
    private:
        Measure& mMeasure;
        Track& mTrack;
        int mHand;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
