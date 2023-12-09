// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

// Local
#include "Instrument.h"
#include "SoundID.h"
#include "StaffInfo.h"
#include "Uuid.h"


namespace komp
{
    struct TrackInfo;
    using TrackInfoVec = std::vector<TrackInfo>;
    
    struct TrackInfo
    {
    public:
        TrackInfo();
        TrackInfo(Instrument inInstrument,
                  std::string_view inUuid = "");

        std::string_view getName() const;
        void setName(std::string inName);
        
        std::string_view getAbbreviation() const;
        void setAbbreviation(std::string inAbbreviation);
        
        std::string_view getUuid() const { return mUuid; }
        
        int getNumStaves() const;
        void addStaffInfo(StaffInfo inStaffInfo);
        void addStaffInfo(ClefType inClefType);
        void clearStaffInfoList();
        SoundID getSoundID() const;
        void setSoundID(SoundID inSoundID);
        const Instrument& getInstrument() const;

    private:
        Instrument mInstrument;
        std::string mUuid{get_uuid()};
    };
    
    using TrackInfoList = std::vector<TrackInfo>;
    
} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
