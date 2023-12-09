// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordSymbolSemantics.h"

// Local
#include "Throw.h"

namespace komp
{
	ChordSymbolSemantics::ChordSymbolSemantics()
    : mHasRoot{false}
    , mRoot{PitchStep::C, 4, 0}
    , mQuality{ChordQuality::UNKNOWN}
    , mQualityIndicator{ChordQualityIndicator::UNKNOWN}
    , mHasBass{false}
    , mBass{PitchStep::C, 4, 0}
    , mExt7{}
    , mExt9{}
    , mExt11{}
    , mExt13{}
	{

	}

// MARK: - Public

    bool
    ChordSymbolSemantics::getIsValid() const
    {
        return isSemanticallyValid();
    }


    void
    ChordSymbolSemantics::invalidate()
    {
        *this = ChordSymbolSemantics{};
    }
    

    bool
    ChordSymbolSemantics::getHasRoot() const
    {
        return mHasRoot;
    }
    

    void
    ChordSymbolSemantics::setHasRoot(bool inValue)
    {
        mHasRoot = inValue;
    }
    

    Pitch
    ChordSymbolSemantics::getRoot() const
    {
        return mRoot;
    }
    

    void
    ChordSymbolSemantics::setRoot(Pitch inPitch)
    {
        inPitch.octave = 4;
        mRoot = std::move(inPitch);
    }
    

    ChordQuality
    ChordSymbolSemantics::getQuality() const
    {
        return mQuality;
    }
    

    void
    ChordSymbolSemantics::setQuality(ChordQuality inQuality)
    {
        mQuality = inQuality;
    }


    ChordQualityIndicator
    ChordSymbolSemantics::getQualityIndicator() const
    {
        return mQualityIndicator;
    }


    void
    ChordSymbolSemantics::setQualityIndicator(ChordQualityIndicator inIndicator)
    {
        mQualityIndicator = inIndicator;
    }


    bool
    ChordSymbolSemantics::getHasBass() const
    {
        return mHasBass;
    }
    

    void
    ChordSymbolSemantics::setHasBass(bool inValue)
    {
        mHasBass = inValue;
    }
    

    Pitch
    ChordSymbolSemantics::getBass() const
    {
        return mBass;
    }


    void
    ChordSymbolSemantics::setBass(Pitch inPitch)
    {
        inPitch.octave = 4;
        mBass = inPitch;
    }


    const ChordSymbolExtension&
    ChordSymbolSemantics::get2() const
    {
        return mExt2;
    }


    void
    ChordSymbolSemantics::set2(const ChordSymbolExtension& inValue)
    {
        mExt2 = inValue;
    }


    const ChordSymbolExtension&
    ChordSymbolSemantics::get4() const
    {
        return mExt4;
    }


    void
    ChordSymbolSemantics::set4(const ChordSymbolExtension& inValue)
    {
        mExt4 = inValue;
    }


    const ChordSymbolExtension&
    ChordSymbolSemantics::get5() const
    {
        return mExt5;
    }


    void
    ChordSymbolSemantics::set5(const ChordSymbolExtension& inValue)
    {
        mExt5 = inValue;
    }


    const ChordSymbolExtension&
    ChordSymbolSemantics::get6() const
    {
        return mExt6;
    }


    void
    ChordSymbolSemantics::set6(const ChordSymbolExtension& inValue)
    {
        mExt6 = inValue;
    }
    

    const ChordSymbolExtension&
    ChordSymbolSemantics::get7() const
    {
        return mExt7;
    }


    void
    ChordSymbolSemantics::set7(const ChordSymbolExtension& inValue)
    {
        mExt7 = inValue;
    }

    const ChordSymbolExtension&
    ChordSymbolSemantics::get9() const
    {
        return mExt9;
    }


    void
    ChordSymbolSemantics::set9(const ChordSymbolExtension& inValue)
    {
        mExt9 = inValue;
    }


    const ChordSymbolExtension&
    ChordSymbolSemantics::get11() const
    {
        return mExt11;
    }


    void
    ChordSymbolSemantics::set11(const ChordSymbolExtension& inValue)
    {
        if (inValue.quality == ChordExtensionQuality::MAJOR) {
            KOMP_THROW("invalid value, use 'PERFECT' instead of 'MAJOR' for 11ths");
        }
        mExt11 = inValue;
    }


    const ChordSymbolExtension&
    ChordSymbolSemantics::get13() const
    {
        return mExt13;
    }


    void
    ChordSymbolSemantics::set13(const ChordSymbolExtension& inValue)
    {
        if (inValue.quality == ChordExtensionQuality::PERFECT) {
            KOMP_THROW("invalid value, use 'MAJOR' instead of 'PERFECT' for 13ths");
        }
        mExt13 = inValue;
    }


    bool
    ChordSymbolSemantics::getIsAnyExtensionVisible() const
    {
        if (get2().isShown) {
            return false;
        } else if (get4().isShown) {
            return false;
        } else if (get5().isShown) {
            return false;
        } else if (get6().isShown) {
            return false;
        } else if (get7().isShown) {
            return false;
        } else if (get9().isShown) {
            return false;
        } else if (get11().isShown) {
            return false;
        } else if (get13().isShown) {
            return false;
        }

        return true;
    }
    
	bool
	ChordSymbolSemantics::isSemanticallyValid() const
	{
        if (!getHasRoot()) {
            return false;
        }

        if (mRoot.alter < -2 || mRoot.alter > 2) {
            return false;
        }

        if (getQuality() == ChordQuality::UNKNOWN) {
            return false;
        }

        if (getQuality() == ChordQuality::HALF_DIMINISHED) {

            if (getQualityIndicator() != ChordQualityIndicator::SYMBOLIC) {
                return false;
            }

            if (get7().isShown) {

                if (get7().alter != ChordExtensionAlter::NONE) {
                    return false;
                }
            }
        }

        if (get7().isShown) {

            if (get7().alter == ChordExtensionAlter::UNKNOWN) {
                return false;
            }

            if (get7().quality == ChordExtensionQuality::UNKNOWN) {
                return false;
            }
        }

        if (get9().isShown) {

            if (get9().alter == ChordExtensionAlter::UNKNOWN) {
                return false;
            }

            if (get9().quality == ChordExtensionQuality::UNKNOWN) {
                return false;
            }
        }

        if (get11().isShown) {

            if (get11().alter == ChordExtensionAlter::UNKNOWN) {
                return false;
            }

            if (get11().quality == ChordExtensionQuality::UNKNOWN) {
                return false;
            }
        }

        if (get13().isShown) {

            if (get13().alter == ChordExtensionAlter::UNKNOWN) {
                return false;
            }

            if (get13().quality == ChordExtensionQuality::UNKNOWN) {
                return false;
            }
        }

        return true;
	}


    bool operator<(const ChordSymbolSemantics& inLeft, const ChordSymbolSemantics& inRight)
    {
        #define CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE( MEMBER_NAME ) \
        if ( static_cast<int>( inLeft.MEMBER_NAME ) < static_cast<int>( inRight.MEMBER_NAME ) ) \
        { \
            return true; \
        } \
        else if ( static_cast<int>( inLeft.MEMBER_NAME ) > static_cast<int>( inRight.MEMBER_NAME ) ) \
        { \
            return false; \
        } \

        #define CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE_WITHOUT_CAST( MEMBER_NAME ) \
        if ( inLeft.MEMBER_NAME < inRight.MEMBER_NAME ) \
        { \
            return true; \
        } \
        else if ( inLeft.MEMBER_NAME > inRight.MEMBER_NAME ) \
        { \
            return false; \
        } \

        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE( getHasRoot() );

        if (inLeft.getHasRoot() || inRight.getHasRoot() ) {
            if (inLeft.getRoot() < inRight.getRoot()) {
                return true;
            } else if (inLeft.getRoot() > inRight.getRoot()) {
                return false;
            }
        }

        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE( getQuality() );
        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE( getQualityIndicator() );
        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE( getHasBass() );

        if (inLeft.getHasBass() || inRight.getHasBass() ) {
            if (inLeft.getBass() < inRight.getBass()) {
                return true;
            } else if (inLeft.getBass() > inRight.getBass()) {
                return false;
            }
        }

        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE_WITHOUT_CAST( get7() );
        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE_WITHOUT_CAST( get9() );
        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE_WITHOUT_CAST( get11() );
        CHORD_SYMBOL_SEMANTICS_MEMBER_COMPARE_WITHOUT_CAST( get13() );
        return false;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
