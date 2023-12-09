// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>

// Local
#include "DrawableChordAtom.h"
#include "ChordSymbolEnums.h"

namespace komp
{
    /// \brief  Singleton of DrawableChordAtoms.
    ///
    /// \discussion Singleton of DrawableChordAtoms.
    ///
	class DrawableChordAtomFactory
	{
    public:
        static const DrawableChordAtom& getDrawableChordAtom(const ChordSymbolChar& inChar);

        static const DrawableChordAtom& getDrawableChordAtom(ChordSymbolCharValue inCharValue,
                                                             bool inIsSuperscript);

        static const std::map<const ChordSymbolChar, const DrawableChordAtom>& getDrawableChordAtoms();

    private:
        std::map<const ChordSymbolChar, const DrawableChordAtom> mMap;

    private:
        DrawableChordAtomFactory();
        static const DrawableChordAtomFactory& getInstance();
        void insert(const DrawableChordAtomParams& inParams);
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
