// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "ChordSymbolValue.h"

// Mx
#include "mx/api/ChordData.h"

#include <map>

namespace komp
{

    /// \brief  Convert to and from MusicXML Chord Data
    ///
    /// \discussion Stateless functions for converting back and forth
    /// between Komp chord symbols and MusicXML chord symbols.
    ///
	class MxChordConverter
	{
	private:
		MxChordConverter() = default;
        
    public:
        static mx::api::ChordData convert(const ChordSymbolValue& inValue);
        static ChordSymbolValue convert(const mx::api::ChordData& inValue);

        class MapProvider;
        class MxMapKey;
        class MxKindMap;

    private:
        static ChordSymbolValue convertFromKompData(const mx::api::ChordData& inValue);
        static ChordSymbolValue convertFromMusicXml(const mx::api::ChordData& inValue);

        static void addExtension(mx::api::ExtensionNumber inExtensionNumber,
                                 const ChordSymbolExtension& inPrototype,
                                 const ChordSymbolExtension& inActual,
                                 mx::api::ChordData& outChordData);
	};

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
