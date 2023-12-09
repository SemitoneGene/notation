// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordSymbolParams.h"

namespace komp
{
	ChordSymbolParams::ChordSymbolParams(Track& inTrack)
    : track{inTrack}
    , hand{0}
    , voice{0}
    , time{0}
    , value{}
    , isXDifferentFromTime{false}
    , measureRelativeX{0.0}
    , isSlotSpecified{false}
    , slot{0.0}

	{

	}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
