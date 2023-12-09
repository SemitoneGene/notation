// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "ChordSymbolEnums.h"
#include "ChordSymbolSemantics.h"
#include "Pitch.h"

namespace komp
{
    /// \brief  Bridge between chord semantics and chord representation.
    ///
    /// \discussion We allow a user to put any chord characters in any order
    /// which may or may not have musical meaning.  This class is responsible
    /// for parsing these characters and determining if the chord string is
    /// semantically viable.  The semantics and the chord string are kept
    /// in sync, so any mutable function can be expected to change both.
    ///
	class ChordSymbolValue
	{
	public:
		ChordSymbolValue();
        ChordSymbolValue(const ChordSymbolSemantics& inSemantics);
        ChordSymbolValue(const ChordSymbolString& inChordSymbolString);
        
    public:
    	const ChordSymbolString& getString() const;
        void setString(const ChordSymbolString& inString);
        const ChordSymbolSemantics& getSemantics() const;
        void setSemantics(const ChordSymbolSemantics& inSemantics);

        /// get a json representation of the chord string
        std::string getJson() const;

        /// parses the json and sets this ChordSymbolValue
        /// based on its contents. throws if json is bad.
        void parseJson(const std::string& inJson);

        /// returns the lowest numbered displayed, zero if
        /// no numbers are displayed
        int getFirstExtension() const;

    private:
        class TempData;
    	ChordSymbolString mString;
        ChordSymbolSemantics mSemantics;

    private:
    	void generateString(TempData& ioTempData);
        void generateSemantics(TempData& ioTempData);
        void invalidateSemantics();
        void separateStringParts(TempData& ioTempData);
        void setRootPitch(const TempData& inTempData);
        void setBassPitch(const TempData& inTempData);
        void setQualityAndExtensions(const TempData& inTempData);
        static PitchStep convertRootValue(const ChordSymbolCharValue inChar);
        static int alterValue(const ChordSymbolChar& inChar);
        void setLetterOnlyMajorChord(const Pitch& inRoot);
        bool isCharValuePresent(const ChordSymbolString& inCleanString, const ChordSymbolCharValue inValue, ChordSymbolChar& outPrecedingChar, ChordSymbolChar& outChar);
        void setTriad(ChordQuality inQuality, ChordQualityIndicator inIndicator);
        static ChordExtensionQuality charToExtQuality(ChordSymbolCharValue inCharValue);
        static ChordExtensionAlter charToExtAlter(ChordSymbolCharValue inCharValue);
        void setVisible5(const TempData& inTempData);
        static ChordSymbolExtension getVisibleExt(ChordSymbolCharValue inValue, ChordExtensionQuality inDefaultExtQuality);
	};

    bool operator<(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight);
    bool operator>(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight);
    bool operator==(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight);
    bool operator!=(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight);
    bool operator<=(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight);
    bool operator>=(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight);

    std::ostream& operator<<(std::ostream&, const ChordSymbolValue& inValue);
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
