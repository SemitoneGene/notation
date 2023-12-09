// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <array>
#include <string>

// Local
#include "Constants.h"
#include "Forward.h"
#include "Instrument.h"

namespace komp
{
    class Track
    {
    public:
        Track(const Score& inScore, const TrackInfo& inInfo);
        
        Track() = delete;
        Track(const Track& inTrack) = delete;
        Track& operator=(const Track& inTrack) = delete;
        Track(Track&& inTrack) = delete;
        Track& operator=(Track&& inTrack) = delete;
        
        ~Track() = default;
        
        const Instrument& getInstrument() const;
        void setInstrument(const Instrument& inInstrument);

        std::string_view getUuid() const { return mUuid; }
        
        int getIndex() const;

        int getHands() const;
        void addHand(ClefType inClefType, int inHand);
        void removeHand(int inHand);

        ClefType getClefType(int inIndex) const;
        void setClefType(int inHand, ClefType inValue);

    private:
        const Score& mScore;
        
        std::string mUuid;
        Instrument mInstrument;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
