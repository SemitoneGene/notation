// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordSymbolValue.h"

// System
#include <sstream>

// Local
#include "DrawableChordAtomFactory.h"
#include "Throw.h"

// Local Frameworks
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    static const komp::ChordSymbolChar UNKNOWN_CHAR{komp::ChordSymbolCharValue::UNKNOWN_CHORD_CHAR, false};
}

namespace komp
{

    class ChordSymbolValue::TempData
    {
    public:
        TempData(const ChordSymbolString& inString)
        : mCleanString{}
        , mRootString{}
        , mRoot{UNKNOWN_CHAR}
        , mRootAlter{UNKNOWN_CHAR}
        , mQualityChar{UNKNOWN_CHAR}
        , m2Number{UNKNOWN_CHAR}
        , m2Alter{UNKNOWN_CHAR}
        , m4Number{UNKNOWN_CHAR}
        , m4Alter{UNKNOWN_CHAR}
        , m5Number{UNKNOWN_CHAR}
        , m5Alter{UNKNOWN_CHAR}
        , m6Number{UNKNOWN_CHAR}
        , m6Alter{UNKNOWN_CHAR}
        , m7Number{UNKNOWN_CHAR}
        , m7Alter{UNKNOWN_CHAR}
        , m9Number{UNKNOWN_CHAR}
        , m9Alter{UNKNOWN_CHAR}
        , m11Number{UNKNOWN_CHAR}
        , m11Alter{UNKNOWN_CHAR}
        , m13Number{UNKNOWN_CHAR}
        , m13Alter{UNKNOWN_CHAR}
        , mBass{UNKNOWN_CHAR}
        , mBassAlter{UNKNOWN_CHAR}
        {
            for (const auto& ch : inString) {
                if (ch.chordSymbolCharValue != ChordSymbolCharValue::PARENS_LEFT &&
                    ch.chordSymbolCharValue != ChordSymbolCharValue::PARENS_RIGHT &&
                    ch.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR) {
                    mCleanString.push_back(ch);
                }

            }

            bool isRootFound = false;
            for (const auto& ch : mCleanString) {
                if (ch.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR) {
                    const auto& atom = DrawableChordAtomFactory::getDrawableChordAtom(ch);
                    if (atom.getChordSymbolCharType() == ChordSymbolCharType::ROOT) {
                        isRootFound = true;
                    }
                }
                if (isRootFound) {
                    mRootString.push_back(ch);
                }
            }
        }

        ChordSymbolString mCleanString;
        ChordSymbolString mRootString;
        ChordSymbolChar mRoot;
        ChordSymbolChar mRootAlter;
        ChordSymbolChar mQualityChar;
        ChordSymbolChar m2Number;
        ChordSymbolChar m2Alter;
        ChordSymbolChar m4Number;
        ChordSymbolChar m4Alter;
        ChordSymbolChar m5Number;
        ChordSymbolChar m5Alter;
        ChordSymbolChar m6Number;
        ChordSymbolChar m6Alter;
        ChordSymbolChar m7Number;
        ChordSymbolChar m7Alter;
        ChordSymbolChar m9Number;
        ChordSymbolChar m9Alter;
        ChordSymbolChar m11Number;
        ChordSymbolChar m11Alter;
        ChordSymbolChar m13Number;
        ChordSymbolChar m13Alter;
        ChordSymbolChar mBass;
        ChordSymbolChar mBassAlter;

        inline bool isRoot() const { return mRoot.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR; }
        inline bool isRootAlter() const { return mRootAlter != UNKNOWN_CHAR && DrawableChordAtomFactory::getDrawableChordAtom(mRootAlter).getChordSymbolCharType() == ChordSymbolCharType::ACCIDENTAL; }
        inline bool isBass() const { return mBass.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR; }
        inline bool isBassAlter() const { return mBassAlter.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR; }
        inline bool is2() const { return m2Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_2; }
        inline bool is4() const { return m4Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_4; }
        inline bool is5() const { return m5Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_5; }
        inline bool is6() const { return m6Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_6; }
        inline bool is7() const { return m7Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_7; }
        inline bool is9() const { return m9Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_9; }
        inline bool is11() const { return m11Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_11; }
        inline bool is13() const { return m13Number.chordSymbolCharValue == ChordSymbolCharValue::NUMBER_13; }

        inline bool isQuality() const
        {
            const bool isUnknown = mQualityChar == UNKNOWN_CHAR;

            if (isUnknown) {
                return false;
            }

            const auto& atom = DrawableChordAtomFactory::getDrawableChordAtom(mQualityChar);
            const bool isCorrect = atom.getChordSymbolCharType() == ChordSymbolCharType::QUALITY;
            return (!isUnknown) && isCorrect;
        }

        inline bool is9First() const
        {
            return !is6() && !is7() && is9();
        }

        inline bool is11First() const
        {
            return !is6() && !is7() && !is9() && is11();
        }

        inline bool is13First() const
        {
            return !is6() && !is7() && !is9() && !is11() && is13();
        }
    };

	ChordSymbolValue::ChordSymbolValue()
	: mString{}
    , mSemantics{}
	{

	}


    ChordSymbolValue::ChordSymbolValue(const ChordSymbolSemantics& inSemantics)
    : ChordSymbolValue{}
    {
        setSemantics(inSemantics);
    }


    ChordSymbolValue::ChordSymbolValue(const ChordSymbolString& inString)
    : ChordSymbolValue{}
    {
        setString(inString);
    }

// MARK: - Public

    const ChordSymbolString&
    ChordSymbolValue::getString() const
    {
        return mString;
    }


    void
    ChordSymbolValue::setString(const ChordSymbolString& inString)
    {
        mString = inString;
        TempData tempData{mString};
        separateStringParts(tempData);
        generateSemantics(tempData);
    }


    const ChordSymbolSemantics&
    ChordSymbolValue::getSemantics() const
    {
        return mSemantics;
    }


    void
    ChordSymbolValue::setSemantics(const ChordSymbolSemantics& inSemantics)
    {
        mSemantics = inSemantics;
        TempData tempData{ChordSymbolString{}};
        generateString(tempData);
    }


    std::string
    ChordSymbolValue::getJson() const
    {
        std::stringstream ss;
        ss << mString;
        return ss.str();
    }


    void
    ChordSymbolValue::parseJson(const std::string& inJson)
    {
        ChordSymbolString newValue;

        std::istringstream iss{ inJson };
        json root;
        iss >> root;


        for (auto iter = root.begin(); iter != root.end(); ++iter) {
            const auto& currentNode = *iter;
            const std::string chordSymbolCharName = currentNode["chordSymbolCharValue"];
            const std::string boolString = currentNode["isSuperscript"];
            bool isSuper = false;

            if (boolString == "true") {
                isSuper = true;
            } else if (boolString == "True") {
                isSuper = true;
            } else if (boolString == "TRUE") {
                isSuper = true;
            }

            const auto value = ChordSymbolCharValueFromString(chordSymbolCharName);
            const ChordSymbolChar chordSymbolChar{value, isSuper};
            newValue.push_back(chordSymbolChar);
        }

        this->setString( std::move( newValue ) );
    }


    int
    ChordSymbolValue::getFirstExtension() const
    {
        if (mSemantics.get2().isShown) {
            return 2;
        } else if (mSemantics.get4().isShown) {
            return 4;
        } else if (mSemantics.get6().isShown) {
            return 6;
        } else if (mSemantics.get7().isShown) {
            return 7;
        } else if (mSemantics.get9().isShown) {
            return 9;
        } else if (mSemantics.get11().isShown) {
            return 11;
        } else if (mSemantics.get13().isShown) {
            return 13;
        }

        return 0;
    }

	void
	ChordSymbolValue::generateString(TempData& ioTempData)
	{
        mString.clear();

        if (getSemantics().getHasRoot()) {
            const auto& rootPitch = getSemantics().getRoot();
            auto rootCharValue = ChordSymbolCharValue::ROOT_C;

            switch (rootPitch.step) {
                case PitchStep::C:
                    rootCharValue = ChordSymbolCharValue::ROOT_C;
                    break;

                case PitchStep::D:
                    rootCharValue = ChordSymbolCharValue::ROOT_D;
                    break;

                case PitchStep::E:
                    rootCharValue = ChordSymbolCharValue::ROOT_E;
                    break;

                case PitchStep::F:
                    rootCharValue = ChordSymbolCharValue::ROOT_F;
                    break;

                case PitchStep::G:
                    rootCharValue = ChordSymbolCharValue::ROOT_G;
                    break;

                case PitchStep::A:
                    rootCharValue = ChordSymbolCharValue::ROOT_A;
                    break;

                case PitchStep::B:
                    rootCharValue = ChordSymbolCharValue::ROOT_B;
                    break;

                default:
                    break;
            }

            mString.emplace_back(rootCharValue, false);

            if (rootPitch.alter != 0) {
                auto rootAlterChar = ChordSymbolCharValue::ACCIDENTAL_FLAT;

                if (rootPitch.alter <= -2) {
                    rootAlterChar = ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT;
                } else if (rootPitch.alter == -1) {
                    rootAlterChar = ChordSymbolCharValue::ACCIDENTAL_FLAT;
                } else if (rootPitch.alter == 1) {
                    rootAlterChar = ChordSymbolCharValue::ACCIDENTAL_SHARP;
                } else if (rootPitch.alter >= 2) {
                    rootAlterChar = ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP;
                }

                mString.emplace_back(rootAlterChar, false);
            }

            // handle quality
            if (getSemantics().getQualityIndicator() != ChordQualityIndicator::NONE &&
                getSemantics().getQualityIndicator() != ChordQualityIndicator::UNKNOWN) {

                const auto& symbolMap = DrawableChordAtomFactory::getDrawableChordAtoms();

                const auto filter = [&](const std::pair<const ChordSymbolChar, const DrawableChordAtom>& inPair) {
                    if (inPair.second.getChordSymbolCharType() != ChordSymbolCharType::QUALITY) {
                        return false;
                    }

                    if (inPair.first.isSuperscript) {
                        return false;
                    }

                    const auto& semantics = getSemantics();
                    const auto& indicator = semantics.getQualityIndicator();
                    const auto& quality = semantics.getQuality();

                    if (ChordSymbolMaps::getQuality(inPair.first.chordSymbolCharValue) != quality) {
                        return false;
                    }

                    if (ChordSymbolMaps::getQualityIndicator(inPair.first.chordSymbolCharValue) != indicator) {
                        return false;
                    }

                    return true;
                };

                const auto foundSymbol = std::find_if(symbolMap.cbegin(), symbolMap.cend(), filter);

                if (foundSymbol != symbolMap.cend()) {
                    mString.emplace_back(foundSymbol->first);
                }
            }

            // handle extensions

            auto ext = getSemantics().get2();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_2, true);
            }

            ext = getSemantics().get4();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_4, true);
            }

            ext = getSemantics().get5();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_5, true);
            }

            ext = getSemantics().get6();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            }

            ext = getSemantics().get7();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            }

            ext = getSemantics().get9();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            }

            ext = getSemantics().get11();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            }

            ext = getSemantics().get13();
            if (ext.isShown) {
                if (ext.alter != ChordExtensionAlter::NONE &&
                    ext.alter != ChordExtensionAlter::UNKNOWN) {
                    const auto cval = ChordSymbolMaps::getAlterCharValue(ext.alter);
                    mString.emplace_back(cval, true);
                }
                mString.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            }

            // handle bass
            if (getSemantics().getHasBass()) {

                mString.emplace_back(ChordSymbolCharValue::SLASH, false);

                const auto& bassPitch = getSemantics().getBass();
                auto bassCharValue = ChordSymbolCharValue::ROOT_C;

                switch (bassPitch.step) {
                    case PitchStep::C:
                        bassCharValue = ChordSymbolCharValue::ROOT_C;
                        break;

                    case PitchStep::D:
                        bassCharValue = ChordSymbolCharValue::ROOT_D;
                        break;

                    case PitchStep::E:
                        bassCharValue = ChordSymbolCharValue::ROOT_E;
                        break;

                    case PitchStep::F:
                        bassCharValue = ChordSymbolCharValue::ROOT_F;
                        break;

                    case PitchStep::G:
                        bassCharValue = ChordSymbolCharValue::ROOT_G;
                        break;

                    case PitchStep::A:
                        bassCharValue = ChordSymbolCharValue::ROOT_A;
                        break;

                    case PitchStep::B:
                        bassCharValue = ChordSymbolCharValue::ROOT_B;
                        break;
                        
                    default:
                        break;
                }
                
                mString.emplace_back(bassCharValue, false);

                if (bassPitch.alter != 0) {
                    auto bassAlterChar = ChordSymbolCharValue::ACCIDENTAL_FLAT;

                    if (bassPitch.alter <= -2) {
                        bassAlterChar = ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT;
                    } else if (bassPitch.alter == -1) {
                        bassAlterChar = ChordSymbolCharValue::ACCIDENTAL_FLAT;
                    } else if (bassPitch.alter == 1) {
                        bassAlterChar = ChordSymbolCharValue::ACCIDENTAL_SHARP;
                    } else if (bassPitch.alter >= 2) {
                        bassAlterChar = ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP;
                    }

                    mString.emplace_back(bassAlterChar, false);
                }
            }
        }
    }


    void
    ChordSymbolValue::generateSemantics(TempData& ioTempData)
    {
        invalidateSemantics();

        if (getString().empty()) {
            return;
        }

        if (ioTempData.mRoot == UNKNOWN_CHAR) {
            return;
        }

        setRootPitch(ioTempData);
        setBassPitch(ioTempData);
        setQualityAndExtensions(ioTempData);
    }


    void
    ChordSymbolValue::invalidateSemantics()
    {
        mSemantics.invalidate();
    }


    void
    ChordSymbolValue::separateStringParts(TempData& ioTempData)
    {
        using atoms = DrawableChordAtomFactory;
        const auto& rootStr = ioTempData.mRootString;
        const auto& cleanStr = ioTempData.mCleanString;
        const bool isRootInsanelyPlaced = cleanStr.size() != rootStr.size();
        const auto& extStr = isRootInsanelyPlaced ? rootStr : cleanStr;

        if (!rootStr.empty()) {
            const auto& rootAtom = atoms::getDrawableChordAtom(rootStr.front());
            if (rootAtom.getChordSymbolCharType() == ChordSymbolCharType::ROOT) {
                ioTempData.mRoot = rootAtom.getChordSymbolChar();
                if (rootStr.size() > 1) {
                    const auto& alterAtom = atoms::getDrawableChordAtom(rootStr.at(1));
                    if (alterAtom.getChordSymbolCharType() == ChordSymbolCharType::ACCIDENTAL) {

                        // complicated check to see if this accidental actually belongs to the
                        // number that follows it and does not belong to the root.

                        const bool isOnlyRoot = rootStr.size() == 2;
                        const bool isSuperscript = alterAtom.getChordSymbolChar().isSuperscript;
                        bool isFollowedBySeven = false;

                        if (!isOnlyRoot && isSuperscript) {
                            const auto& followingChar = atoms::getDrawableChordAtom(rootStr.at(2));
                            const bool isNumber = followingChar.getChordSymbolCharType() == ChordSymbolCharType::EXTENSION;
                            if (isNumber) {
                                isFollowedBySeven = followingChar.getChordSymbolChar().chordSymbolCharValue == ChordSymbolCharValue::NUMBER_7;
                            }
                        }

                        const bool isRootTheOwner = (isOnlyRoot) || (!isSuperscript) || (isFollowedBySeven);
                        if (isRootTheOwner) {
                            ioTempData.mRootAlter = alterAtom.getChordSymbolChar();
                        }
                    }
                }
            }
        }

        if (extStr.size() >= 2) {
            const auto& secondChar = extStr.at(1);
            const auto& secondAtom = atoms::getDrawableChordAtom(secondChar);

            if (secondAtom.getChordSymbolCharType() == ChordSymbolCharType::QUALITY) {
                ioTempData.mQualityChar = secondChar;

            } else if (extStr.size() >= 3) {

                const auto& thirdChar = extStr.at(2);
                const auto& thirdAtom = atoms::getDrawableChordAtom(thirdChar);

                if (thirdAtom.getChordSymbolCharType() == ChordSymbolCharType::QUALITY) {
                    ioTempData.mQualityChar = thirdAtom.getChordSymbolChar();
                }
            }
        }
        
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_2, ioTempData.m2Alter, ioTempData.m2Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_4, ioTempData.m4Alter, ioTempData.m4Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_5, ioTempData.m5Alter, ioTempData.m5Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_6, ioTempData.m6Alter, ioTempData.m6Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_7, ioTempData.m7Alter, ioTempData.m7Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_9, ioTempData.m9Alter, ioTempData.m9Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_11, ioTempData.m11Alter, ioTempData.m11Number);
        isCharValuePresent(extStr, ChordSymbolCharValue::NUMBER_13, ioTempData.m13Alter, ioTempData.m13Number);

        const auto findSlash = [&](const ChordSymbolChar& inChar) {
            const auto result = inChar.chordSymbolCharValue == ChordSymbolCharValue::SLASH;
            return result;
        };

        const auto slashIter = std::find_if(cleanStr.cbegin(), cleanStr.cend(), findSlash);
        if (slashIter != cleanStr.cend()) {
            const auto bassIter = slashIter + 1;
            if (bassIter != cleanStr.cend()) {
                const auto& bassChar = *bassIter;
                if (bassChar.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR) {
                    const auto bassCharType = DrawableChordAtomFactory::getDrawableChordAtom(bassChar).getChordSymbolCharType();
                    if (bassCharType == ChordSymbolCharType::ROOT) {
                        ioTempData.mBass = bassChar;
                        const auto bassAlterIter = bassIter + 1;
                        if (bassAlterIter != cleanStr.cend()) {
                            const auto bassAlterChar = *bassAlterIter;
                            if (bassAlterChar.chordSymbolCharValue != ChordSymbolCharValue::UNKNOWN_CHORD_CHAR) {
                                const auto& bassAlterAtom = DrawableChordAtomFactory::getDrawableChordAtom(bassAlterChar);
                                const auto isAlter = bassAlterAtom.getChordSymbolCharType() == ChordSymbolCharType::ACCIDENTAL;
                                if (isAlter) {
                                    ioTempData.mBassAlter = bassAlterChar;
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    void
    ChordSymbolValue::setRootPitch(const TempData& inTempData)
    {
        Pitch rootPitch;
        rootPitch.octave = 4;
        rootPitch.step = convertRootValue(inTempData.mRoot.chordSymbolCharValue);

        if (inTempData.isRootAlter()) {
            rootPitch.alter = alterValue(inTempData.mRootAlter);
        }

        setLetterOnlyMajorChord(rootPitch);
    }


    void
    ChordSymbolValue::setBassPitch(const TempData& inTempData)
    {
        if (!inTempData.isBass()) {
            mSemantics.setHasBass(false);
            mSemantics.setBass(Pitch{PitchStep::C, 0, 4});
            return;
        }

        Pitch bassPitch;
        bassPitch.octave = 4;
        bassPitch.step = convertRootValue(inTempData.mBass.chordSymbolCharValue);

        if (inTempData.isBassAlter()) {
            bassPitch.alter = alterValue(inTempData.mBassAlter);
        }

        mSemantics.setHasBass(true);
        mSemantics.setBass(bassPitch);
    }


    void
    ChordSymbolValue::setQualityAndExtensions(const TempData& inTempData)
    {
        if (!inTempData.isRoot()) {
            invalidateSemantics();
            return;
        }


        switch (inTempData.mQualityChar.chordSymbolCharValue) {

            case ChordSymbolCharValue::UNKNOWN_CHORD_CHAR:
            {
                setTriad(ChordQuality::MAJOR, ChordQualityIndicator::NONE);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is7() || inTempData.is9First() || inTempData.is11First() || inTempData.is13First()) {
                    mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE});
                }

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_AUG_SYMBOL:
            case ChordSymbolCharValue::QUALITY_AUG_TEXT:
            {
                auto indicator = ChordQualityIndicator::TEXT;

                if (ChordSymbolCharValue::QUALITY_AUG_SYMBOL == inTempData.mQualityChar.chordSymbolCharValue) {
                    indicator = ChordQualityIndicator::SYMBOLIC;
                }

                setTriad(ChordQuality::AUGMENTED, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is7() || inTempData.is9First() || inTempData.is11First() || inTempData.is13First()) {
                    mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE});
                }

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_DIM_SYMBOL:
            case ChordSymbolCharValue::QUALITY_DIM_TEXT:
            {
                auto indicator = ChordQualityIndicator::TEXT;

                if (ChordSymbolCharValue::QUALITY_DIM_SYMBOL == inTempData.mQualityChar.chordSymbolCharValue) {
                    indicator = ChordQualityIndicator::SYMBOLIC;
                }

                setTriad(ChordQuality::DIMINISHED, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MINOR));
                }

                if (inTempData.is7() || inTempData.is9First() || inTempData.is11First() || inTempData.is13First()) {
                    mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::DIMINISHED, ChordExtensionAlter::NONE});
                }

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL:
            {
                auto indicator = ChordQualityIndicator::SYMBOLIC;

                setTriad(ChordQuality::HALF_DIMINISHED, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MINOR));
                }

                if (inTempData.is7() || inTempData.is9First() || inTempData.is11First() || inTempData.is13First()) {
                    mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE});
                }

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_LOWERCASE_M:
            case ChordSymbolCharValue::QUALITY_MIN_TEXT:
            case ChordSymbolCharValue::QUALITY_MINOR_SYMBOL:
            {
                auto indicator = ChordQualityIndicator::TEXT;

                if (ChordSymbolCharValue::QUALITY_LOWERCASE_M == inTempData.mQualityChar.chordSymbolCharValue) {
                    indicator = ChordQualityIndicator::LETTER;
                } else if (ChordSymbolCharValue::QUALITY_MINOR_SYMBOL == inTempData.mQualityChar.chordSymbolCharValue) {
                    indicator = ChordQualityIndicator::SYMBOLIC;
                }

                setTriad(ChordQuality::MINOR, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is7() || inTempData.is9First() || inTempData.is11First() || inTempData.is13First()) {
                    mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MINOR, ChordExtensionAlter::NONE});
                }

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_UPPERCASE_M:
            case ChordSymbolCharValue::QUALITY_MAJ_TEXT:
            {
                auto indicator = ChordQualityIndicator::TEXT;

                if (ChordSymbolCharValue::QUALITY_UPPERCASE_M == inTempData.mQualityChar.chordSymbolCharValue) {
                    indicator = ChordQualityIndicator::LETTER;
                }
                
                setTriad(ChordQuality::MAJOR, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is7() || inTempData.is9First() || inTempData.is11First() || inTempData.is13First()) {
                    mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE});
                }

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL:
            {
                auto indicator = ChordQualityIndicator::SYMBOLIC;

                setTriad(ChordQuality::MAJOR, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                // triangle always implies major seventh
                mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE});

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;

            case ChordSymbolCharValue::QUALITY_SUS_TEXT:
            {
                auto indicator = ChordQualityIndicator::TEXT;

                setTriad(ChordQuality::SUS4, indicator);

                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }
                if (inTempData.is2()) {
                    mSemantics.set2(getVisibleExt(inTempData.m2Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is4()) {
                    mSemantics.set4(getVisibleExt(inTempData.m4Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is5()) {
                    setVisible5(inTempData);
                }

                if (inTempData.is6()) {
                    mSemantics.set6(getVisibleExt(inTempData.m6Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                // triangle always implies major seventh
                mSemantics.set7(ChordSymbolExtension{inTempData.is7(), ChordExtensionQuality::MAJOR, ChordExtensionAlter::NONE});

                if (inTempData.is9()) {
                    mSemantics.set9(getVisibleExt(inTempData.m9Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }

                if (inTempData.is11()) {
                    mSemantics.set11(getVisibleExt(inTempData.m11Alter.chordSymbolCharValue, ChordExtensionQuality::PERFECT));
                }

                if (inTempData.is13()) {
                    mSemantics.set13(getVisibleExt(inTempData.m13Alter.chordSymbolCharValue, ChordExtensionQuality::MAJOR));
                }
            }
                break;
                
            default:
            {
                KOMP_THROW("bad chord symbol quality value");
            }
                break;
        }
    }


    PitchStep
    ChordSymbolValue::convertRootValue(const ChordSymbolCharValue inChar)
    {
        PitchStep result = PitchStep::A;

        switch (inChar) {
            case ChordSymbolCharValue::ROOT_A: result = PitchStep::A; break;
            case ChordSymbolCharValue::ROOT_B: result = PitchStep::B; break;
            case ChordSymbolCharValue::ROOT_C: result = PitchStep::C; break;
            case ChordSymbolCharValue::ROOT_D: result = PitchStep::D; break;
            case ChordSymbolCharValue::ROOT_E: result = PitchStep::E; break;
            case ChordSymbolCharValue::ROOT_F: result = PitchStep::F; break;
            case ChordSymbolCharValue::ROOT_G: result = PitchStep::G; break;
            default: KOMP_THROW( "invalid root char" );
        }

        return result;
    }


    int
    ChordSymbolValue::alterValue(const ChordSymbolChar& inChar)
    {
        if (inChar.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT) {
            return -2;
        } else if (inChar.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_FLAT) {
            return -1;
        } else if (inChar.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_NATURAL) {
            return 0;
        } else if (inChar.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_SHARP) {
            return 1;
        } else if (inChar.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP) {
            return 2;
        }

        return 0;
    }


    void
    ChordSymbolValue::setLetterOnlyMajorChord(const Pitch& inRoot)
    {
        mSemantics.setHasRoot(true);
        mSemantics.setRoot(inRoot);
        mSemantics.setQuality(ChordQuality::MAJOR);
        mSemantics.setQualityIndicator(ChordQualityIndicator::NONE);
        mSemantics.set5(ChordSymbolExtension{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE});
    }


    bool
    ChordSymbolValue::isCharValuePresent(const ChordSymbolString& inCleanString, const ChordSymbolCharValue inValue, ChordSymbolChar& outPrecedingChar, ChordSymbolChar& outChar)
    {
        outPrecedingChar = UNKNOWN_CHAR;
        outChar = UNKNOWN_CHAR;

        const auto find = [&](const ChordSymbolChar& inChar) {
            return inChar.chordSymbolCharValue == inValue;
        };

        const auto iter = std::find_if(inCleanString.cbegin(), inCleanString.cend(), find);
        const bool isFound = iter != inCleanString.cend();

        if (isFound) {

            outChar = *iter;

            if (iter != inCleanString.cbegin()) {
                outPrecedingChar = *(iter - 1);
            }
        }

        return isFound;
    }


    void
    ChordSymbolValue::setTriad(ChordQuality inQuality, ChordQualityIndicator inIndicator)
    {
        if (inQuality == ChordQuality::MAJOR ||
            inQuality == ChordQuality::MINOR) {
            mSemantics.setQuality(inQuality);
            mSemantics.setQualityIndicator(inIndicator);
            mSemantics.set5(ChordSymbolExtension{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE});
        } else if (inQuality == ChordQuality::DIMINISHED ||
                   inQuality == ChordQuality::HALF_DIMINISHED) {
            mSemantics.setQuality(inQuality);
            mSemantics.setQualityIndicator(inIndicator);
            mSemantics.set5(ChordSymbolExtension{false, ChordExtensionQuality::DIMINISHED, ChordExtensionAlter::NONE});
        } else if (inQuality == ChordQuality::AUGMENTED) {
            mSemantics.setQuality(inQuality);
            mSemantics.setQualityIndicator(inIndicator);
            mSemantics.set5(ChordSymbolExtension{false, ChordExtensionQuality::AUGMENTED, ChordExtensionAlter::NONE});
        } else if (inQuality == ChordQuality::SUS4) {
            mSemantics.setQuality(inQuality);
            mSemantics.setQualityIndicator(inIndicator);
            mSemantics.set5(ChordSymbolExtension{false, ChordExtensionQuality::PERFECT, ChordExtensionAlter::NONE});
        }
    }


    ChordExtensionQuality
    ChordSymbolValue::charToExtQuality(ChordSymbolCharValue inCharValue)
    {
        switch (inCharValue)
        {
            case ChordSymbolCharValue::ACCIDENTAL_FLAT: return ChordExtensionQuality::DIMINISHED;
            case ChordSymbolCharValue::ACCIDENTAL_SHARP: return ChordExtensionQuality::AUGMENTED;
            case ChordSymbolCharValue::ACCIDENTAL_NATURAL: return ChordExtensionQuality::MAJOR;
            case ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT: return ChordExtensionQuality::DOUBLE_DIMINISHED;
            case ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP: return ChordExtensionQuality::DOUBLE_AUGMENTED;
            default: break;
        }
        return ChordExtensionQuality::UNKNOWN;
    }


    ChordExtensionAlter
    ChordSymbolValue::charToExtAlter(ChordSymbolCharValue inCharValue)
    {
        switch (inCharValue)
        {
            case ChordSymbolCharValue::ACCIDENTAL_FLAT: return ChordExtensionAlter::FLAT;
            case ChordSymbolCharValue::ACCIDENTAL_SHARP: return ChordExtensionAlter::SHARP;
            case ChordSymbolCharValue::ACCIDENTAL_NATURAL: return ChordExtensionAlter::NONE;
            case ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT: return ChordExtensionAlter::DOUBLE_FLAT;
            case ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP: return ChordExtensionAlter::DOUBLE_SHARP;
            default: break;
        }
        return ChordExtensionAlter::NONE;
    }


    void
    ChordSymbolValue::setVisible5(const TempData& inTempData)
    {
        if (!inTempData.is5()) {
            return;
        }

        auto alter = ChordExtensionAlter::NONE;
        auto quality = ChordExtensionQuality::PERFECT;

        if (inTempData.m5Alter.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT) {
            alter = ChordExtensionAlter::DOUBLE_FLAT;
            quality = ChordExtensionQuality::DOUBLE_DIMINISHED;
        } else if (inTempData.m5Alter.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_FLAT) {
            alter = ChordExtensionAlter::FLAT;
            quality = ChordExtensionQuality::DIMINISHED;
        } else if (inTempData.m5Alter.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_NATURAL) {
            alter = ChordExtensionAlter::NONE;
            quality = ChordExtensionQuality::PERFECT;
        } else if (inTempData.m5Alter.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_SHARP) {
            alter = ChordExtensionAlter::SHARP;
            quality = ChordExtensionQuality::AUGMENTED;
        } else if (inTempData.m5Alter.chordSymbolCharValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP) {
            alter = ChordExtensionAlter::DOUBLE_SHARP;
            quality = ChordExtensionQuality::DOUBLE_AUGMENTED;
        }

        mSemantics.set5(ChordSymbolExtension{true, quality, alter});
    }


    ChordSymbolExtension
    ChordSymbolValue::getVisibleExt(ChordSymbolCharValue inValue, ChordExtensionQuality inDefaultExtQuality)
    {
        auto alter = ChordExtensionAlter::NONE;
        auto quality = inDefaultExtQuality;

        if (inDefaultExtQuality == ChordExtensionQuality::PERFECT) {
            if (inValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT) {
                alter = ChordExtensionAlter::DOUBLE_FLAT;
                quality = ChordExtensionQuality::DOUBLE_DIMINISHED;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_FLAT) {
                alter = ChordExtensionAlter::FLAT;
                quality = ChordExtensionQuality::DIMINISHED;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_NATURAL) {
                alter = ChordExtensionAlter::NONE;
                quality = inDefaultExtQuality;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_SHARP) {
                alter = ChordExtensionAlter::SHARP;
                quality = ChordExtensionQuality::AUGMENTED;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP) {
                alter = ChordExtensionAlter::DOUBLE_SHARP;
                quality = ChordExtensionQuality::DOUBLE_AUGMENTED;
            }
        } else {
            if (inValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT) {
                alter = ChordExtensionAlter::DOUBLE_FLAT;
                quality = ChordExtensionQuality::DIMINISHED;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_FLAT) {
                alter = ChordExtensionAlter::FLAT;
                quality = ChordExtensionQuality::MINOR;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_NATURAL) {
                alter = ChordExtensionAlter::NONE;
                quality = inDefaultExtQuality;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_SHARP) {
                alter = ChordExtensionAlter::SHARP;
                quality = ChordExtensionQuality::AUGMENTED;
            } else if (inValue == ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP) {
                alter = ChordExtensionAlter::DOUBLE_SHARP;
                quality = ChordExtensionQuality::DOUBLE_AUGMENTED;
            }
        }

        return ChordSymbolExtension{true, quality, alter};
    }


    bool operator<(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight)
    {
        if (inLeft.getString() < inRight.getString()) {
            return true;
        } else if (inLeft.getString() > inRight.getString()) {
            return false;
        }

        if (inLeft.getSemantics() < inRight.getSemantics()) {
            return true;
        } else if (inLeft.getSemantics() > inRight.getSemantics()) {
            return false;
        }

        return false;
    }


    bool operator>(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight)
    {
        return inRight < inLeft;
    }


    bool operator==(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight)
    {
        return (!(inLeft < inRight)) && (!(inLeft > inRight));
    }


    bool operator!=(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight)
    {
        return !(inLeft == inRight);
    }


    bool operator<=(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight)
    {
        return (inLeft < inRight) || (inLeft == inRight);
    }


    bool operator>=(const ChordSymbolValue& inLeft, const ChordSymbolValue& inRight)
    {
        return (inLeft > inRight) || (inLeft == inRight);
    }


    std::ostream& operator<<(std::ostream& os, const ChordSymbolValue& inValue)
    {
        os << inValue.getString();
        return os;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
