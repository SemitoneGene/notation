// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "ChordSymbolEnums.h"
#include "ChordSymbolExtension.h"
#include "Pitch.h"

namespace komp
{

    /// \brief  Represents the Semantic Meaning of a Chord Symbol
    ///
    /// \discussion Represents the chord symbol as closely while allowing for the client to create
    /// any (potentially illogical) string of ChordSymbolAtoms.  If the user's desired chord symbol
    /// atoms can represent a valid structure, then ChordSymbolSemantics will approximate it.
    ///
	class ChordSymbolSemantics
	{
	public:
		ChordSymbolSemantics();
        
    public:
        /// returns true if the internal state represents a semantically
        /// comprehensible chord symbol
        bool getIsValid() const;

        /// resets the internal state to to the same invalid state that
        /// a newly created ChordSemantics object has.
        void invalidate();

        bool getHasRoot() const;
        void setHasRoot(bool inValue);
        Pitch getRoot() const;
        void setRoot(Pitch inPitch);

        ChordQuality getQuality() const;
        void setQuality(ChordQuality inQuality);
        ChordQualityIndicator getQualityIndicator() const;
        void setQualityIndicator(ChordQualityIndicator inIndicator);
        bool getIsQualityShortened() const;
        void setIsQualityShortened(bool inValue);

        bool getHasBass() const;
        void setHasBass(bool inValue);
        Pitch getBass() const;
        void setBass(Pitch inPitch);

        const ChordSymbolExtension& get2() const;
        void set2(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get4() const;
        void set4(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get5() const;
        void set5(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get6() const;
        void set6(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get7() const;
        void set7(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get9() const;
        void set9(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get11() const;
        void set11(const ChordSymbolExtension& inValue);

        const ChordSymbolExtension& get13() const;
        void set13(const ChordSymbolExtension& inValue);

        bool getIsAnyExtensionVisible() const;

    private:
        bool mHasRoot;
    	Pitch mRoot;
        ChordQuality mQuality;
        ChordQualityIndicator mQualityIndicator;
        bool mHasBass;
        Pitch mBass;

        ChordSymbolExtension mExt2;
        ChordSymbolExtension mExt4;
        ChordSymbolExtension mExt5;
        ChordSymbolExtension mExt6;
        ChordSymbolExtension mExt7;
        ChordSymbolExtension mExt9;
        ChordSymbolExtension mExt11;
        ChordSymbolExtension mExt13;

    private:
    	bool isSemanticallyValid() const;
	};

    bool operator<(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight);

    inline bool operator>(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight)
    {
        return inRight < inLeft;
    }

    inline bool operator==(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight)
    {
        return (!(inLeft < inRight)) && (!(inLeft > inRight));
    }

    inline bool operator!=(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight)
    {
        return !(inLeft == inRight);
    }

    inline bool operator<=(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight)
    {
        return (inLeft < inRight) || (inLeft == inRight);
    }

    inline bool operator>=(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight)
    {
        return (inLeft > inRight) || (inLeft == inRight);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
