// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "ChordSymbolEnums.h"

namespace komp
{
    /// \brief  The class is named ChordSymbolExtension.
    ///
    /// \discussion The class named ChordSymbolExtension does stuff.
    ///
	class ChordSymbolExtension
	{
	public:
		ChordSymbolExtension();
        ChordSymbolExtension(bool inIsShown, ChordExtensionQuality inQuality, ChordExtensionAlter inAlter);
        
    public:
        bool isShown;
        ChordExtensionQuality quality;
        ChordExtensionAlter alter;
        int getAlterValue() const;
	};

    bool operator<(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight);
    bool operator>(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight);
    bool operator==(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight);
    bool operator!=(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight);
    bool operator<=(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight);
    bool operator>=(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight);

    std::ostream& operator<<(std::ostream& os, const ChordSymbolExtension& inValue);
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
