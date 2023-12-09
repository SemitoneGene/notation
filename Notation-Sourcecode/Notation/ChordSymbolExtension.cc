// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordSymbolExtension.h"

namespace komp
{
	ChordSymbolExtension::ChordSymbolExtension()
	: isShown{false}
    , quality{ChordExtensionQuality::UNKNOWN}
    , alter{ChordExtensionAlter::UNKNOWN}
	{

	}


    ChordSymbolExtension::ChordSymbolExtension(bool inIsShown, ChordExtensionQuality inQuality, ChordExtensionAlter inAlter)
    : isShown{inIsShown}
    , quality{inQuality}
    , alter{inAlter}
    {
        
    }


    int
    ChordSymbolExtension::getAlterValue() const
    {
        switch (this->alter) {
            case ChordExtensionAlter::DOUBLE_FLAT: return -2;
            case ChordExtensionAlter::FLAT: return -1;
            case ChordExtensionAlter::NONE: return 0;
            case ChordExtensionAlter::SHARP: return 1;
            case ChordExtensionAlter::DOUBLE_SHARP: return 2;
            default: return 0;
        }
    }


    bool operator<(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight)
    {
        if (static_cast<int>(inLeft.isShown) < static_cast<int>(inRight.isShown)) {
            return true;
        } else if (static_cast<int>(inLeft.isShown) > static_cast<int>(inRight.isShown)) {
            return false;
        }

        if (static_cast<int>(inLeft.quality) < static_cast<int>(inRight.quality)) {
            return true;
        } else if (static_cast<int>(inLeft.quality) > static_cast<int>(inRight.quality)) {
            return false;
        }

        if (static_cast<int>(inLeft.alter) < static_cast<int>(inRight.alter)) {
            return true;
        } else if (static_cast<int>(inLeft.alter) > static_cast<int>(inRight.alter)) {
            return false;
        }

        return false;
    }


    bool operator>(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight)
    {
        return inRight < inLeft;
    }


    bool operator==(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight)
    {
        return (!(inLeft < inRight)) && (!(inLeft > inRight));
    }


    bool operator!=(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight)
    {
        return !(inLeft == inRight);
    }


    bool operator<=(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight)
    {
        return (inLeft < inRight) || (inLeft == inRight);
    }


    bool operator>=(const ChordSymbolExtension& inLeft, const ChordSymbolExtension& inRight)
    {
        return (inLeft > inRight) || (inLeft == inRight);
    }

    std::ostream& operator<<(std::ostream& os, const ChordSymbolExtension& inValue)
    {
        os << "{ \"isShown\": \"" << std::boolalpha << inValue.isShown << "\", ";
        os << "\"quality\": \"" << ChordExtensionQualityToString(inValue.quality) << "\", ";
        os << "\"alter\": \"" << ChordExtensionAlterToString(inValue.alter) << "\" }";
        return os;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
