// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "ChordSymbolValue.h"

namespace komp
{
    /// \brief  The class is named ChordSymbolParams.
    ///
    /// \discussion The class named ChordSymbolParams does stuff.
    ///
	class ChordSymbolParams
	{
	public:
        ChordSymbolParams() = delete;
        
        ChordSymbolParams(Track& inTrack);
        
    public:
        Track& track;
        int hand;
        int voice;
        int time;
        ChordSymbolValue value;
        bool isXDifferentFromTime;
        double measureRelativeX;
        bool isSlotSpecified;
        double slot;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
