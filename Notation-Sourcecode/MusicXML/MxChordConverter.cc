// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxChordConverter.h"

// Local
#include "MxConverter.h"
#include "Throw.h"
#include "kern/StringDistance.h"

namespace
{
    static constexpr const char* const KOMP_CHORD_DATA_NAME = "komp-chord-string";
}

namespace komp
{
    struct MxKindKey
    {
        mx::api::ChordKind kind;
        bool isSymbol;
        bool operator<(const MxKindKey& inOther) const
        {
            if (this->kind < inOther.kind) {
                return true;
            } else if (this->kind > inOther.kind) {
                return false;
            }

            if (this->isSymbol < inOther.isSymbol) {
                return true;
            } else if (this->isSymbol > inOther.isSymbol) {
                return false;
            }

            return false;
        }

        MxKindKey(mx::api::ChordKind inKind, bool inIsSymbol)
        : kind(inKind)
        , isSymbol(inIsSymbol)
        {

        }
    };


    class MxChordConverter::MxKindMap
    {
    public:
        static std::string getText(mx::api::ChordKind inKind, bool inSymbol)
        {
            const auto& instance = getInstance();
            const auto& map = instance.mMap;
            const auto iter = map.find(MxKindKey{inKind, inSymbol});

            if (iter != map.cend()) {
                return iter->second;
            }

            return "";
        }


    private:
        static const MxKindMap& getInstance() { static const MxKindMap sInstancce; return sInstancce; }
        std::map<const MxKindKey, const std::string> mMap;

        MxKindMap()
        : mMap{}
        {
            mMap.emplace(MxKindKey{mx::api::ChordKind::major, false}, std::string{""});
            mMap.emplace(MxKindKey{mx::api::ChordKind::major, true}, std::string{""});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minor, false}, std::string{CHORD_SYMBOL_STRING_MIN_TEXT});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minor, true}, std::string{""});
            mMap.emplace(MxKindKey{mx::api::ChordKind::augmented, false}, std::string{CHORD_SYMBOL_STRING_AUG_TEXT});
            mMap.emplace(MxKindKey{mx::api::ChordKind::augmented, true}, std::string{""});
            mMap.emplace(MxKindKey{mx::api::ChordKind::diminished, false}, std::string{CHORD_SYMBOL_STRING_DIM_TEXT});
            mMap.emplace(MxKindKey{mx::api::ChordKind::diminished, true}, std::string{""});
            mMap.emplace(MxKindKey{mx::api::ChordKind::dominant, false}, std::string{"7"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::majorSeventh, false}, std::string{std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"7"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::majorSeventh, true}, std::string{"7"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minorSeventh, false}, std::string{std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"7"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minorSeventh, true}, std::string{"7"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::diminishedSeventh, true}, std::string{"7"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::diminishedSeventh, false}, std::string{std::string{CHORD_SYMBOL_STRING_DIM_TEXT} + std::string{"7"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::augmentedSeventh, false}, std::string{std::string{CHORD_SYMBOL_STRING_AUG_TEXT} + std::string{"7"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::augmentedSeventh, true}, std::string{"7"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::halfDiminished, true}, std::string{"7"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::majorSixth, false}, std::string{std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"6"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::majorSixth, true}, std::string{"6"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minorSixth, false}, std::string{std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"6"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minorSixth, true}, std::string{"6"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::dominantNinth, false}, std::string{"9"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::majorNinth, false}, std::string{std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"9"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::majorNinth, true}, std::string{"9"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minorNinth, false}, std::string{std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"9"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minorNinth, true}, std::string{"9"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::dominant11Th, false}, std::string{"11"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::major11Th, false}, std::string{std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"11"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::major11Th, true}, std::string{"11"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minor11Th, false}, std::string{std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"11"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minor11Th, true}, std::string{"11"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::dominant13Th, false}, std::string{"13"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::major13Th, false}, std::string{std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"13"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::major13Th, true}, std::string{"13"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minor13Th, false}, std::string{std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"13"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::minor13Th, true}, std::string{"13"});
            mMap.emplace(MxKindKey{mx::api::ChordKind::suspendedSecond, false}, std::string{std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"2"}});
            mMap.emplace(MxKindKey{mx::api::ChordKind::suspendedFourth, false}, std::string{std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"}});
        }
    };

    class MxChordConverter::MxMapKey
    {
    public:
        mx::api::ChordKind kind;
        bool useSymbol;
        std::string text;

        MxMapKey()
        : kind{mx::api::ChordKind::unspecified}
        , useSymbol{false}
        , text{}
        {

        }
    };

    bool operator<(const MxChordConverter::MxMapKey& inLeft, const MxChordConverter::MxMapKey& inRight)
    {
        if (inLeft.kind < inRight.kind) {
            return true;
        } else if (inLeft.kind > inRight.kind) {
            return false;
        }

        if (inLeft.useSymbol < inRight.useSymbol) {
            return true;
        } else if (inLeft.useSymbol > inRight.useSymbol) {
            return false;
        }

        if (inLeft.text < inRight.text) {
            return true;
        } else if (inLeft.text > inRight.text) {
            return false;
        }
        return false;
    }

    using MxChordMap = std::map<MxChordConverter::MxMapKey, ChordSymbolValue>;
    using MxMapPair = std::pair<MxChordConverter::MxMapKey, ChordSymbolValue>;

    class MxChordConverter::MapProvider
    {
    public:
        static const MxChordMap& getMap()
        {
            const auto& theInstance = getInstance();
            const auto& theMap = theInstance.mMap;
            return theMap;
        }

    private:
        MxChordMap mMap;
        static const MxChordConverter::MapProvider& getInstance()
        {
            static const MxChordConverter::MapProvider instance{};
            return instance;
        }

        MapProvider()
        {
            MxMapKey mxVal;
            ChordSymbolValue kompVal;
            ChordSymbolString str;
            ChordSymbolSemantics semantics;
            ChordSymbolExtension extension;

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
            kompVal = ChordSymbolValue{str};

            mxVal.kind = mx::api::ChordKind::major;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = CHORD_SYMBOL_STRING_MAJ_TEXT;
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = CHORD_SYMBOL_STRING_MAJ_LETTER;
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MIN_TEXT, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = CHORD_SYMBOL_STRING_MIN_TEXT;
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = CHORD_SYMBOL_STRING_MIN_LETTER;
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_AUG_TEXT, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::augmented;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = CHORD_SYMBOL_STRING_AUG_TEXT;
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_AUG_SYMBOL, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::augmented;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_DIM_TEXT, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::diminished;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = CHORD_SYMBOL_STRING_DIM_TEXT;
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_DIM_SYMBOL, false);
            semantics = kompVal.getSemantics();
//            extension.quality = ChordExtensionQuality::DIMINISHED;
//            extension.alter = ChordExtensionAlter::NONE;
//            extension.isShown = false;
//            semantics.set7(extension);
//            kompVal.setSemantics(semantics);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::diminished;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::dominant;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "7";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"7"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"7"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "7";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"7"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MIN_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"7"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "7";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_DIM_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::diminishedSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "7";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_DIM_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::diminishedSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_DIM_TEXT} + std::string{"7"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_AUG_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::augmentedSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "7";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_AUG_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::augmentedSeventh;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_AUG_TEXT} + std::string{"7"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_7, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::halfDiminished;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "7";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL, false);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::halfDiminished;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "";
            mMap.emplace(mxVal, kompVal);

            // MARK: - 6

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT, true);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"6"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M, true);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"6"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, true);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "6";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "6";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MIN_TEXT, true);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"6"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, true);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "6";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M, true);
            str.emplace_back(ChordSymbolCharValue::NUMBER_6, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorSixth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"6"};
            mMap.emplace(mxVal, kompVal);

            // MARK: - 9

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::dominantNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "9";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "9";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"9"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::majorNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"9"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"9"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MIN_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"9"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_9, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minorNinth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "9";
            mMap.emplace(mxVal, kompVal);

            // MARK: - 11

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::dominant11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "11";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "11";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"11"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"11"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"11"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MIN_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"11"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_11, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor11Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "11";
            mMap.emplace(mxVal, kompVal);

            // MARK: - 13

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::dominant13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "13";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "13";
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_UPPERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"13"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MAJ_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::major13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"13"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_LOWERCASE_M, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"13"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MIN_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"13"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_13, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::minor13Th;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = "13";
            mMap.emplace(mxVal, kompVal);

            // MARK: - Sus

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_SUS_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_4, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::suspendedFourth;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};
            mMap.emplace(mxVal, kompVal);

            str.clear();
            str.emplace_back(ChordSymbolCharValue::ROOT_C, false);
            str.emplace_back(ChordSymbolCharValue::QUALITY_SUS_TEXT, false);
            str.emplace_back(ChordSymbolCharValue::NUMBER_2, true);
            kompVal = ChordSymbolValue{str};
            mxVal.kind = mx::api::ChordKind::suspendedSecond;
            mxVal.useSymbol = kompVal.getSemantics().getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
            mxVal.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"2"};
            mMap.emplace(mxVal, kompVal);
        }
    };


    mx::api::ChordData
    MxChordConverter::convert(const ChordSymbolValue& inValue)
    {
        const auto& semantics = inValue.getSemantics();
        const auto& map = MapProvider::getMap();

        const auto& rootPitch = semantics.getRoot();
        mx::api::ChordData chordData;

        // put the komp data into a processing instruction
        mx::api::MiscData kompChordData{};
        kompChordData.name = KOMP_CHORD_DATA_NAME;
        kompChordData.data = inValue.getJson();
        chordData.miscData.push_back(kompChordData);

        // set the root and bass notes
        chordData.root = MxConverter::convertPitchStep(rootPitch.step);
        chordData.rootAlter = rootPitch.alter;

        if (semantics.getHasBass()) {
            const auto bassPitch = semantics.getBass();
            chordData.bass = MxConverter::convertPitchStep(bassPitch.step);
            chordData.bassAlter = bassPitch.alter;
        }

        if (!semantics.getIsValid()) {
            return chordData;
        }

        const auto firstExt = inValue.getFirstExtension();
        const bool isSymbol = semantics.getQualityIndicator() == ChordQualityIndicator::SYMBOLIC;
        const bool isLetter = semantics.getQualityIndicator() == ChordQualityIndicator::LETTER;
        const bool isText = semantics.getQualityIndicator() == ChordQualityIndicator::TEXT;
        const bool isNone = !isSymbol && !isLetter && !isText;

        MxMapKey lookupKey;
        lookupKey.kind = mx::api::ChordKind::other;
        lookupKey.text = "";
        lookupKey.useSymbol = false;

        if ((firstExt == 0) ||
            (((firstExt == 2) || (firstExt == 4)) && (semantics.getQuality() != ChordQuality::SUS4))) {
            if (semantics.getQuality() == ChordQuality::MAJOR) {
                lookupKey.kind = mx::api::ChordKind::major;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isLetter) {
                    lookupKey.text = CHORD_SYMBOL_STRING_MAJ_LETTER;
                } else if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_MAJ_TEXT;
                } else if (isNone) {
                    lookupKey.text = "";
                }

            } else if (semantics.getQuality() == ChordQuality::MINOR) {
                lookupKey.kind = mx::api::ChordKind::minor;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isLetter) {
                    lookupKey.text = CHORD_SYMBOL_STRING_MIN_LETTER;
                } else if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_MIN_TEXT;
                } else if (isNone) {
                    lookupKey.text = "";
                }

            } else if (semantics.getQuality() == ChordQuality::AUGMENTED) {
                lookupKey.kind = mx::api::ChordKind::augmented;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = isSymbol ? "" : CHORD_SYMBOL_STRING_AUG_TEXT;

            } else if (semantics.getQuality() == ChordQuality::DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::diminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = isSymbol ? "" : CHORD_SYMBOL_STRING_DIM_TEXT;


            } else if (semantics.getQuality() == ChordQuality::HALF_DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::halfDiminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";

            } else if (semantics.getQuality() == ChordQuality::SUS4) {
                lookupKey.kind = mx::api::ChordKind::suspendedFourth;
                lookupKey.useSymbol = false;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};

            } else if (semantics.getQuality() == ChordQuality::UNKNOWN) {
                lookupKey.kind = mx::api::ChordKind::major;
                lookupKey.useSymbol = false;
                lookupKey.text = "";
            }
        } else if ((firstExt == 2) && (semantics.getQuality() == ChordQuality::SUS4)) {
            lookupKey.kind = mx::api::ChordKind::suspendedSecond;
            lookupKey.useSymbol = false;
            lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"2"};

        } else if ((firstExt == 4) && (semantics.getQuality() == ChordQuality::SUS4)) {
            lookupKey.kind = mx::api::ChordKind::suspendedFourth;
            lookupKey.useSymbol = false;
            lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};

        } else if (firstExt == 6) {
            if (semantics.getQuality() == ChordQuality::MAJOR) {
                lookupKey.kind = mx::api::ChordKind::majorSixth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "6";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"6"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"6"};
                } else if (isNone) {
                    lookupKey.text = "6";
                }

            } else if (semantics.getQuality() == ChordQuality::MINOR) {
                lookupKey.kind = mx::api::ChordKind::minorSixth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "6";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"6"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"6"};
                }

            } else if (semantics.getQuality() == ChordQuality::AUGMENTED) {
                lookupKey.kind = mx::api::ChordKind::augmented;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_AUG_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::diminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_DIM_TEXT;
                }


            } else if (semantics.getQuality() == ChordQuality::HALF_DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::halfDiminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";

            } else if (semantics.getQuality() == ChordQuality::SUS4) {
                lookupKey.kind = mx::api::ChordKind::suspendedFourth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};
                if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};
                }

            } else if (semantics.getQuality() == ChordQuality::UNKNOWN) {
                lookupKey.kind = mx::api::ChordKind::majorSixth;
                lookupKey.useSymbol = false;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"6"};
            }
        } else if (firstExt == 7) {
            if (semantics.getQuality() == ChordQuality::MAJOR && semantics.getQualityIndicator() == ChordQualityIndicator::NONE) {
                lookupKey.kind = mx::api::ChordKind::dominant;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "7";

            } else if (semantics.getQuality() == ChordQuality::MAJOR) {
                lookupKey.kind = mx::api::ChordKind::majorSeventh;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "7";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"7"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"7"};
                }

            } else if (semantics.getQuality() == ChordQuality::MINOR) {
                lookupKey.kind = mx::api::ChordKind::minorSeventh;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "7";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"7"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"7"};
                }

            } else if (semantics.getQuality() == ChordQuality::AUGMENTED) {
                lookupKey.kind = mx::api::ChordKind::augmentedSeventh;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "7";
                if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_AUG_TEXT} + std::string{"7"};
                }

            } else if (semantics.getQuality() == ChordQuality::DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::diminishedSeventh;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "7";
                if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_DIM_TEXT} + std::string{"7"};
                }

            } else if (semantics.getQuality() == ChordQuality::HALF_DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::halfDiminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "7";

            } else if (semantics.getQuality() == ChordQuality::SUS4) {
                lookupKey.kind = mx::api::ChordKind::suspendedFourth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};

            } else if (semantics.getQuality() == ChordQuality::UNKNOWN) {
                lookupKey.kind = mx::api::ChordKind::dominant;
                lookupKey.useSymbol = false;
                lookupKey.text = "7";
            }
        } else if (firstExt == 9) {
            if (semantics.getQuality() == ChordQuality::MAJOR && semantics.getQualityIndicator() == ChordQualityIndicator::NONE) {
                lookupKey.kind = mx::api::ChordKind::dominantNinth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "9";

            } else if (semantics.getQuality() == ChordQuality::MAJOR) {
                lookupKey.kind = mx::api::ChordKind::majorNinth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "9";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"9"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"9"};
                }

            } else if (semantics.getQuality() == ChordQuality::MINOR) {
                lookupKey.kind = mx::api::ChordKind::minorNinth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "9";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"9"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"9"};
                }

            } else if (semantics.getQuality() == ChordQuality::AUGMENTED) {
                lookupKey.kind = mx::api::ChordKind::augmented;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_AUG_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::diminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_DIM_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::HALF_DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::halfDiminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";

            } else if (semantics.getQuality() == ChordQuality::SUS4) {
                lookupKey.kind = mx::api::ChordKind::suspendedFourth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};

            } else if (semantics.getQuality() == ChordQuality::UNKNOWN) {
                lookupKey.kind = mx::api::ChordKind::dominantNinth;
                lookupKey.useSymbol = false;
                lookupKey.text = "9";
            }
        } else if (firstExt == 11) {
            if (semantics.getQuality() == ChordQuality::MAJOR && semantics.getQualityIndicator() == ChordQualityIndicator::NONE) {
                lookupKey.kind = mx::api::ChordKind::dominant11Th;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "11";

            } else if (semantics.getQuality() == ChordQuality::MAJOR) {
                lookupKey.kind = mx::api::ChordKind::major11Th;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "11";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"11"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"11"};
                }

            } else if (semantics.getQuality() == ChordQuality::MINOR) {
                lookupKey.kind = mx::api::ChordKind::minor11Th;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "11";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"11"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"11"};
                }

            } else if (semantics.getQuality() == ChordQuality::AUGMENTED) {
                lookupKey.kind = mx::api::ChordKind::augmented;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_AUG_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::diminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_DIM_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::HALF_DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::halfDiminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";

            } else if (semantics.getQuality() == ChordQuality::SUS4) {
                lookupKey.kind = mx::api::ChordKind::suspendedFourth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};

            } else if (semantics.getQuality() == ChordQuality::UNKNOWN) {
                lookupKey.kind = mx::api::ChordKind::dominant11Th;
                lookupKey.useSymbol = false;
                lookupKey.text = "11";
            }
        } else if (firstExt == 13) {
            if (semantics.getQuality() == ChordQuality::MAJOR && semantics.getQualityIndicator() == ChordQualityIndicator::NONE) {
                lookupKey.kind = mx::api::ChordKind::dominant13Th;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "13";

            } else if (semantics.getQuality() == ChordQuality::MAJOR) {
                lookupKey.kind = mx::api::ChordKind::major13Th;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "13";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_LETTER} + std::string{"13"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MAJ_TEXT} + std::string{"13"};
                }

            } else if (semantics.getQuality() == ChordQuality::MINOR) {
                lookupKey.kind = mx::api::ChordKind::minor13Th;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "13";
                if (isLetter) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_LETTER} + std::string{"13"};
                } else if (isText) {
                    lookupKey.text = std::string{CHORD_SYMBOL_STRING_MIN_TEXT} + std::string{"13"};
                }

            } else if (semantics.getQuality() == ChordQuality::AUGMENTED) {
                lookupKey.kind = mx::api::ChordKind::augmented;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_AUG_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::diminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";
                if (isText) {
                    lookupKey.text = CHORD_SYMBOL_STRING_DIM_TEXT;
                }

            } else if (semantics.getQuality() == ChordQuality::HALF_DIMINISHED) {
                lookupKey.kind = mx::api::ChordKind::halfDiminished;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = "";

            } else if (semantics.getQuality() == ChordQuality::SUS4) {
                lookupKey.kind = mx::api::ChordKind::suspendedFourth;
                lookupKey.useSymbol = isSymbol;
                lookupKey.text = std::string{CHORD_SYMBOL_STRING_SUS_TEXT} + std::string{"4"};

            } else if (semantics.getQuality() == ChordQuality::UNKNOWN) {
                lookupKey.kind = mx::api::ChordKind::dominant13Th;
                lookupKey.useSymbol = false;
                lookupKey.text = "13";
            }
        }

        chordData.chordKind = lookupKey.kind;
        chordData.useSymbols = lookupKey.useSymbol ? mx::api::Bool::yes : mx::api::Bool::unspecified;
        chordData.text = lookupKey.text;

//        auto filter = [&](const MxMapPair& inPair) {
//            const auto& inMxValue = inPair.first;
//            if (inMxValue.kind != lookupKey.kind) {
//                return false;
//            }
//
//            if (inMxValue.useSymbol != lookupKey.useSymbol) {
//                return false;
//            }
//
//            if (inMxValue.text.size() != lookupKey.text.size()) {
//                return false;
//            }
//
//            if (inMxValue.text.size() <= 1) {
//                return inMxValue.text == lookupKey.text;
//            } else {
//                auto left = inMxValue.text;
//                auto right = inMxValue.text;
//                std::transform(left.begin(), left.end(), left.begin(), ::toupper);
//                std::transform(right.begin(), right.end(), right.begin(), ::toupper);
//                return left == right;
//            }
//
//            return false;
//        };

        const auto iter = map.find(lookupKey); //std::find_if(map.cbegin(), map.cend(), filter);
        KOMP_ASSERT(iter != map.cend());
        const auto prototype = iter->second;

        const auto& proto2 = prototype.getSemantics().get2();
        const auto& inVal2 = inValue.getSemantics().get2();
        addExtension(mx::api::ExtensionNumber::second, proto2, inVal2, chordData);

        const auto& proto4 = prototype.getSemantics().get4();
        const auto& inVal4 = inValue.getSemantics().get4();
        addExtension(mx::api::ExtensionNumber::fourth, proto4, inVal4, chordData);

        const auto& proto5 = prototype.getSemantics().get5();
        const auto& inVal5 = inValue.getSemantics().get5();
        addExtension(mx::api::ExtensionNumber::fifth, proto5, inVal5, chordData);

        const auto& proto6 = prototype.getSemantics().get6();
        const auto& inVal6 = inValue.getSemantics().get6();
        addExtension(mx::api::ExtensionNumber::sixth, proto6, inVal6, chordData);

        const auto& proto7 = prototype.getSemantics().get7();
        const auto& inVal7 = inValue.getSemantics().get7();
        addExtension(mx::api::ExtensionNumber::seventh, proto7, inVal7, chordData);

        const auto& proto9 = prototype.getSemantics().get9();
        const auto& inVal9 = inValue.getSemantics().get9();
        addExtension(mx::api::ExtensionNumber::ninth, proto9, inVal9, chordData);

        const auto& proto11 = prototype.getSemantics().get11();
        const auto& inVal11 = inValue.getSemantics().get11();
        addExtension(mx::api::ExtensionNumber::eleventh, proto11, inVal11, chordData);

        const auto& proto13 = prototype.getSemantics().get13();
        const auto& inVal13 = inValue.getSemantics().get13();
        addExtension(mx::api::ExtensionNumber::thirteenth, proto13, inVal13, chordData);

        return chordData;
    }


    ChordSymbolValue
    MxChordConverter::convert(const mx::api::ChordData& inValue)
    {
        if (inValue.miscData.size() == 1) {
            const auto& misc = inValue.miscData.front();
            if (misc.name == KOMP_CHORD_DATA_NAME) {
                return convertFromKompData(inValue);
            }
        }

        return convertFromMusicXml(inValue);
    }

    ChordSymbolValue
    MxChordConverter::convertFromKompData(const mx::api::ChordData& inValue)
    {
        try {

            const auto& misc = inValue.miscData.front();
            ChordSymbolValue result;
            result.parseJson(misc.data);
            return result;

        } catch(std::exception& e) {

            LOG_F(ERROR,  "exception occurred trying to parse komp chord symbol data: %s", e.what() );
            return convertFromMusicXml(inValue);

        } catch(...) {

            LOG_F(ERROR,  "unknown exception occurred trying to parse komp chord symbol data" );
            return convertFromMusicXml(inValue);

        }
    }


    ChordSymbolValue
    MxChordConverter::convertFromMusicXml(const mx::api::ChordData& inValue)
    {
        ChordSymbolValue outValue{};
        MxMapKey mapKey;
        mapKey.kind = inValue.chordKind;
        mapKey.useSymbol = inValue.useSymbols == mx::api::Bool::yes;
        mapKey.text = inValue.text;

        const auto exactMatchIter = MapProvider::getMap().find(mapKey);
        const bool isExactMatch = exactMatchIter != MapProvider::getMap().cend();
        bool isFuzzyMatch = false;
        ChordSymbolSemantics semantics{};

        if (!isExactMatch && mapKey.text.size() > 1) {
            const auto& sourceMap = MapProvider::getMap();
            std::vector<MxMapPair> kindMatches{};
            std::copy_if(sourceMap.cbegin(), sourceMap.cend(), std::back_inserter(kindMatches),
                         [&](const MxMapPair& inPair) {
                             return inPair.first.kind == mapKey.kind && inPair.first.text.size() > 1;
                         });

            if (kindMatches.size() > 0) {
                std::vector<std::pair<double, std::string>> distances;

                for (const auto& pair : kindMatches) {
                    if (pair.first.text.size() > 1) {
                        const auto distance = kern::getDistance(pair.first.text, mapKey.text);
                        distances.emplace_back(distance, pair.first.text);
                    }
                }

                if (distances.size() > 0) {
                    std::ranges::sort(distances,
                                      [&](std::pair<double, std::string>& inLeft, std::pair<double, std::string>& inRight) {
                        return inLeft.first < inRight.first;
                    });

                    const auto& bestMatch = distances.front();
                    isFuzzyMatch = true;
                    mapKey.text = bestMatch.second;
                }
            }
        }

        if (isExactMatch) {
            semantics = exactMatchIter->second.getSemantics();
        } else if (isFuzzyMatch && (MapProvider::getMap().find(mapKey) != MapProvider::getMap().end())) {
            const auto iter = MapProvider::getMap().find(mapKey);
            KOMP_ASSERT(iter != MapProvider::getMap().end());
            semantics = iter->second.getSemantics();

        } else {
            if (inValue.chordKind == mx::api::ChordKind::majorMinor ||
                inValue.chordKind == mx::api::ChordKind::neapolitan ||
                inValue.chordKind == mx::api::ChordKind::italian ||
                inValue.chordKind == mx::api::ChordKind::french ||
                inValue.chordKind == mx::api::ChordKind::german ||
                inValue.chordKind == mx::api::ChordKind::pedal ||
                inValue.chordKind == mx::api::ChordKind::power ||
                inValue.chordKind == mx::api::ChordKind::tristan ||
                inValue.chordKind == mx::api::ChordKind::other ||
                inValue.chordKind == mx::api::ChordKind::none) {
                LOG_F(INFO, "unsupported chord type - creating a major chord instead");
                mapKey.kind = mx::api::ChordKind::major;
            }

            if (inValue.chordKind == mx::api::ChordKind::dominant ||
                inValue.chordKind == mx::api::ChordKind::dominantNinth ||
                inValue.chordKind == mx::api::ChordKind::dominant11Th ||
                inValue.chordKind == mx::api::ChordKind::dominant13Th ||
                inValue.chordKind == mx::api::ChordKind::suspendedSecond ||
                inValue.chordKind == mx::api::ChordKind::suspendedFourth) {
                mapKey.useSymbol = false;
            }

            if (inValue.chordKind == mx::api::ChordKind::halfDiminished) {
                mapKey.useSymbol = true;
            }

            mapKey.text = MxKindMap::getText(mapKey.kind, mapKey.useSymbol);
            const auto& valuesMap = MapProvider::getMap();
            const auto iter = valuesMap.find(mapKey);

            if (iter == valuesMap.cend()) {
                KOMP_THROW("code before this line is broken");
            }
            
            const auto& prototype = iter->second;
            semantics = prototype.getSemantics();
        }


        // set the root
        Pitch rootPitch{};
        rootPitch.step = MxConverter::convertPitchStep(inValue.root);
        rootPitch.alter = inValue.rootAlter;
        semantics.setRoot(rootPitch);

        // set the bass
        if (inValue.bass != mx::api::Step::unspecified) {
            Pitch bassPitch{};
            bassPitch.step = MxConverter::convertPitchStep(inValue.bass);
            bassPitch.alter = inValue.bassAlter;
            semantics.setHasBass(true);
            semantics.setBass(bassPitch);
        }

        // handle extensions
        for (const auto& ext : inValue.extensions) {
            using ConstChordSymbolSemantics = const ChordSymbolSemantics;
            using ConstChordSymbolExtensionRef = const ChordSymbolExtension&;
            using GetterFunction = ConstChordSymbolExtensionRef (ConstChordSymbolSemantics::*)() const;
            GetterFunction getter = nullptr;
            using SetterFunction = void (ChordSymbolSemantics::*)(const ChordSymbolExtension&);
            SetterFunction setter = nullptr;
            bool isBeyondSeven = false;

            switch (ext.extensionNumber) {
                case mx::api::ExtensionNumber::second:
                {
                    getter = &ChordSymbolSemantics::get2;
                    setter = &ChordSymbolSemantics::set2;
                }
                    break;
                case mx::api::ExtensionNumber::fourth:
                {
                    getter = &ChordSymbolSemantics::get4;
                    setter = &ChordSymbolSemantics::set4;
                }
                    break;
                case mx::api::ExtensionNumber::fifth:
                {
                    getter = &ChordSymbolSemantics::get5;
                    setter = &ChordSymbolSemantics::set5;
                }
                    break;

                case mx::api::ExtensionNumber::sixth:
                {
                    getter = &ChordSymbolSemantics::get6;
                    setter = &ChordSymbolSemantics::set6;
                }
                    break;

                case mx::api::ExtensionNumber::seventh:
                {
                    getter = &ChordSymbolSemantics::get7;
                    setter = &ChordSymbolSemantics::set7;
                }
                    break;

                case mx::api::ExtensionNumber::ninth:
                {
                    getter = &ChordSymbolSemantics::get9;
                    setter = &ChordSymbolSemantics::set9;
                    isBeyondSeven = true;
                }
                    break;

                case mx::api::ExtensionNumber::eleventh:
                {
                    getter = &ChordSymbolSemantics::get11;
                    setter = &ChordSymbolSemantics::set11;
                    isBeyondSeven = true;
                }
                    break;

                case mx::api::ExtensionNumber::thirteenth:
                {
                    getter = &ChordSymbolSemantics::get13;
                    setter = &ChordSymbolSemantics::set13;
                    isBeyondSeven = true;
                }
                    break;

                case mx::api::ExtensionNumber::first:
                case mx::api::ExtensionNumber::third:
                default:
                    break;
            }

            if (getter == nullptr || setter == nullptr) {
                continue;
            }


            auto kompExt = ((semantics).*(getter))();
//            const bool isPerfectIntervalType = ext.extensionNumber == mx::api::ExtensionNumber::fourth ||
//                                               ext.extensionNumber == mx::api::ExtensionNumber::fifth ||
//                                               ext.extensionNumber == mx::api::ExtensionNumber::eleventh ||
//                                               ext.extensionNumber == mx::api::ExtensionNumber::first;

            if (ext.extensionType == mx::api::ExtensionType::add ||
                ext.extensionType == mx::api::ExtensionType::alter) {
                kompExt.isShown = true;
            } else if (ext.extensionType == mx::api::ExtensionType::remove) {
                kompExt.isShown = false;
            }

            const auto alterInt = ext.getAlterValue();

            auto defaultQuality = ChordExtensionQuality::UNKNOWN;
            auto plusOneQuality = ChordExtensionQuality::UNKNOWN;
            auto plusTwoQuality = ChordExtensionQuality::UNKNOWN;
            auto minusOneQuality = ChordExtensionQuality::UNKNOWN;
            auto minusTwoQuality = ChordExtensionQuality::UNKNOWN;

            const auto chordQuality = semantics.getQuality();
            const bool isMajor = (chordQuality == ChordQuality::MAJOR) || (chordQuality == ChordQuality::SUS4) || (chordQuality == ChordQuality::UNKNOWN);
            const bool isMinor = chordQuality == ChordQuality::MINOR;
            const bool isDim = (chordQuality == ChordQuality::DIMINISHED) || (chordQuality == ChordQuality::HALF_DIMINISHED);
            const bool isAug = chordQuality == ChordQuality::AUGMENTED;
            const bool isDominant = isMajor && ( (semantics.getQualityIndicator() == ChordQualityIndicator::NONE) || (semantics.getQualityIndicator() == ChordQualityIndicator::UNKNOWN) );
//            const bool isMajSev = isMajor && !isDominant;

            if (ext.extensionNumber == mx::api::ExtensionNumber::second) {
                minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                minusOneQuality = ChordExtensionQuality::MINOR;
                defaultQuality = ChordExtensionQuality::MAJOR;
                plusOneQuality = ChordExtensionQuality::AUGMENTED;
                plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
            } else if ((ext.extensionNumber == mx::api::ExtensionNumber::fifth) ||
                       (ext.extensionNumber == mx::api::ExtensionNumber::fourth) ||
                       (ext.extensionNumber == mx::api::ExtensionNumber::eleventh)) {
                minusTwoQuality = ChordExtensionQuality::DOUBLE_DIMINISHED;
                minusOneQuality = ChordExtensionQuality::DIMINISHED;
                defaultQuality = ChordExtensionQuality::PERFECT;
                plusOneQuality = ChordExtensionQuality::AUGMENTED;
                plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
            } else if (ext.extensionNumber == mx::api::ExtensionNumber::sixth) {
                if (isMajor || isAug) {
                    minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                    minusOneQuality = ChordExtensionQuality::MINOR;
                    defaultQuality = ChordExtensionQuality::MAJOR;
                    plusOneQuality = ChordExtensionQuality::AUGMENTED;
                    plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
                } else if (isMinor || isDim) {
                    minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                    minusOneQuality = ChordExtensionQuality::MINOR;
                    defaultQuality = ChordExtensionQuality::MINOR;
                    plusOneQuality = ChordExtensionQuality::MAJOR;
                    plusTwoQuality = ChordExtensionQuality::AUGMENTED;
                } else {
                    minusTwoQuality = ChordExtensionQuality::DOUBLE_DIMINISHED;
                    minusOneQuality = ChordExtensionQuality::MINOR;
                    defaultQuality = ChordExtensionQuality::MAJOR;
                    plusOneQuality = ChordExtensionQuality::AUGMENTED;
                    plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
                }
            } else if (ext.extensionNumber == mx::api::ExtensionNumber::seventh) {

                if (isDominant || isMinor || isAug || (chordQuality == ChordQuality::HALF_DIMINISHED)) {
                    minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                    minusOneQuality = ChordExtensionQuality::MINOR;
                    defaultQuality = ChordExtensionQuality::MINOR;
                    plusOneQuality = ChordExtensionQuality::AUGMENTED;
                    plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
                } else if (chordQuality == ChordQuality::DIMINISHED) {
                    minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                    minusOneQuality = ChordExtensionQuality::MINOR;
                    defaultQuality = ChordExtensionQuality::DIMINISHED;
                    plusOneQuality = ChordExtensionQuality::AUGMENTED;
                    plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
                } else {
                    minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                    minusOneQuality = ChordExtensionQuality::MINOR;
                    defaultQuality = ChordExtensionQuality::MAJOR;
                    plusOneQuality = ChordExtensionQuality::AUGMENTED;
                    plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
                }
            } else if ((ext.extensionNumber == mx::api::ExtensionNumber::ninth) ||
                       (ext.extensionNumber == mx::api::ExtensionNumber::thirteenth)){
                minusTwoQuality = ChordExtensionQuality::DIMINISHED;
                minusOneQuality = ChordExtensionQuality::MINOR;
                defaultQuality = ChordExtensionQuality::MAJOR;
                plusOneQuality = ChordExtensionQuality::AUGMENTED;
                plusTwoQuality = ChordExtensionQuality::DOUBLE_AUGMENTED;
            } else {
                LOG_F(ERROR, "unrecognized chord extension");
            }

            KOMP_ASSERT(defaultQuality != ChordExtensionQuality::UNKNOWN);
            KOMP_ASSERT(plusOneQuality != ChordExtensionQuality::UNKNOWN);
            KOMP_ASSERT(plusTwoQuality != ChordExtensionQuality::UNKNOWN);
            KOMP_ASSERT(minusOneQuality != ChordExtensionQuality::UNKNOWN);
            KOMP_ASSERT(minusTwoQuality != ChordExtensionQuality::UNKNOWN);

            if (alterInt <= -2) {

                kompExt.alter = ChordExtensionAlter::DOUBLE_FLAT;
                kompExt.quality = minusTwoQuality;

//                if (isPerfectIntervalType) {
//                    kompExt.quality = ChordExtensionQuality::DOUBLE_DIMINISHED;
//                } else {
//                    kompExt.quality = ChordExtensionQuality::DIMINISHED;
//                }

            } else if (alterInt == -1) {

                kompExt.alter = ChordExtensionAlter::FLAT;
                kompExt.quality = minusOneQuality;

//                if (isPerfectIntervalType) {
//                    kompExt.quality = ChordExtensionQuality::DIMINISHED;
//                } else {
//                    kompExt.quality = ChordExtensionQuality::MINOR;
//                }

            } else if (alterInt == 0) {

                kompExt.alter = ChordExtensionAlter::NONE;
                kompExt.quality = defaultQuality;

//                if (isPerfectIntervalType) {
//                    kompExt.quality = ChordExtensionQuality::PERFECT;
//                } else {
//                    kompExt.quality = ChordExtensionQuality::MAJOR;
//                }

            } else if (alterInt == 1) {

                kompExt.alter = ChordExtensionAlter::SHARP;
                kompExt.quality = plusOneQuality;
//                kompExt.quality = ChordExtensionQuality::AUGMENTED;

            } else if (alterInt >= 2) {

                kompExt.alter = ChordExtensionAlter::DOUBLE_SHARP;
                kompExt.quality = plusTwoQuality;
//                kompExt.quality = ChordExtensionQuality::DOUBLE_AUGMENTED;
            }

            ((semantics).*(setter))(kompExt);

            if (isBeyondSeven) {
                const auto quality = semantics.getQuality();
                const bool isQFullyDiminished = quality == ChordQuality::DIMINISHED;
                const bool isQMajor = quality == ChordQuality::MAJOR;
                const bool isQualityShown = (semantics.getQualityIndicator() != ChordQualityIndicator::NONE) &&
                                            (semantics.getQualityIndicator() != ChordQualityIndicator::UNKNOWN);

                if (!semantics.get6().isShown) {
                    if ((!semantics.get7().isShown) &&
                        (semantics.get7().quality == ChordExtensionQuality::UNKNOWN) &&
                        (semantics.get7().alter == ChordExtensionAlter::UNKNOWN)) {
                        if (isQFullyDiminished) {
                            ChordSymbolExtension sev;
                            sev.quality = ChordExtensionQuality::DIMINISHED;
                            sev.alter = ChordExtensionAlter::NONE;
                            sev.isShown = false;
                            semantics.set7(sev);
                        } else if (isQMajor && isQualityShown) {
                            ChordSymbolExtension sev;
                            sev.quality = ChordExtensionQuality::MAJOR;
                            sev.alter = ChordExtensionAlter::NONE;
                            sev.isShown = false;
                            semantics.set7(sev);
                        } else {
                            ChordSymbolExtension sev;
                            sev.quality = ChordExtensionQuality::MINOR;
                            sev.alter = ChordExtensionAlter::NONE;
                            sev.isShown = false;
                            semantics.set7(sev);
                        }
                    }
                }
            }
        }

        outValue.setSemantics(semantics);
        return outValue;
    }


    void
    MxChordConverter::addExtension(mx::api::ExtensionNumber inExtensionNumber,
                                   const ChordSymbolExtension& inPrototype,
                                   const ChordSymbolExtension& inActual,
                                   mx::api::ChordData& outChordData)
    {
        if (!inPrototype.isShown && !inActual.isShown) {
            return;
        }

        if (inPrototype == inActual) {
            return;
        }


        if (inPrototype.isShown && !inActual.isShown) {
            mx::api::Extension ext;
            ext.extensionNumber = inExtensionNumber;
            ext.extensionType = mx::api::ExtensionType::remove;
            ext.setAlterValue(inActual.getAlterValue());
            ext.printObject = mx::api::Bool::no;
            outChordData.extensions.push_back(ext);
        } else if (!inPrototype.isShown && inActual.isShown) {
            mx::api::Extension ext;
            ext.extensionNumber = inExtensionNumber;
            ext.extensionType = mx::api::ExtensionType::add;
            ext.setAlterValue(inActual.getAlterValue());
            ext.printObject = mx::api::Bool::yes;
            outChordData.extensions.push_back(ext);
        } else if (inPrototype.isShown && inActual.isShown) {
            mx::api::Extension ext;
            ext.extensionNumber = inExtensionNumber;
            ext.extensionType = mx::api::ExtensionType::alter;
            ext.setAlterValue(inActual.getAlterValue());
            ext.printObject = mx::api::Bool::yes;
            outChordData.extensions.push_back(ext);
        } else {
            LOG_F(ERROR, "should not hit this");
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
