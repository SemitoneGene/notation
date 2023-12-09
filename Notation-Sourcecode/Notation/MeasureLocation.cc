// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MeasureLocation.h"

// Local
#include "Forward.h"
#include "Measure.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "Track.h"

namespace komp
{
    MeasureLocation::MeasureLocation(Measure& inMeasure,
                                     Track& inTrack,
                                     const int inHand)
    : mMeasure{inMeasure}
    , mTrack{inTrack}
    , mHand{inHand}
    {        
    }
    
    bool
    MeasureLocation::getIsValid() const
    {
        bool isValid = true;
        
        bool isHandValid = mHand >= 0;
        isHandValid &= (mHand < mTrack.getHands());
        
        isValid &= isHandValid;
        
        return isValid;
    }
    
    Measure&
    MeasureLocation::getMeasure() const
    {
        return mMeasure;
    }
        
    Track&
    MeasureLocation::getTrack() const
    {
        return mTrack;
    }
        
    int
    MeasureLocation::getHand() const
    {
        return mHand;
    }
        
    Staff&
    MeasureLocation::getStaff() const
    {
        const auto& system = mMeasure.getStaffSystem();
        return system->getStaff(mTrack, mHand);
    }
            
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
