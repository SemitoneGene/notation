// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Local
#include "ChordSymbolEnums.h"

namespace komp
{
    ChordSymbolMaps::ChordSymbolMaps()
    : mQualityMap{}
    {
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_AUG_TEXT, ChordQuality::AUGMENTED);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_AUG_SYMBOL, ChordQuality::AUGMENTED);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_DIM_TEXT, ChordQuality::DIMINISHED);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_DIM_SYMBOL, ChordQuality::DIMINISHED);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL, ChordQuality::DIMINISHED);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_MAJ_TEXT, ChordQuality::MAJOR);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, ChordQuality::MAJOR);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_UPPERCASE_M, ChordQuality::MAJOR);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_MIN_TEXT, ChordQuality::MINOR);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, ChordQuality::MINOR);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_LOWERCASE_M, ChordQuality::MINOR);
        mQualityMap.emplace(ChordSymbolCharValue::QUALITY_SUS_TEXT, ChordQuality::SUS4);

        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_AUG_TEXT, ChordQualityIndicator::TEXT);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_AUG_SYMBOL, ChordQualityIndicator::SYMBOLIC);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_DIM_TEXT, ChordQualityIndicator::TEXT);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_DIM_SYMBOL, ChordQualityIndicator::SYMBOLIC);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL, ChordQualityIndicator::SYMBOLIC);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_MAJ_TEXT, ChordQualityIndicator::TEXT);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL, ChordQualityIndicator::SYMBOLIC);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_UPPERCASE_M, ChordQualityIndicator::LETTER);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_MIN_TEXT, ChordQualityIndicator::TEXT);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_MINOR_SYMBOL, ChordQualityIndicator::SYMBOLIC);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_LOWERCASE_M, ChordQualityIndicator::LETTER);
        mQualityIndicatorMap.emplace(ChordSymbolCharValue::QUALITY_SUS_TEXT, ChordQualityIndicator::TEXT);

    }


    ChordQuality
    ChordSymbolMaps::getQuality(ChordSymbolCharValue inChar)
    {
        const auto& inst = getInstance();
        const auto& map = inst.mQualityMap;
        const auto iter = map.find(inChar);

        if (iter == map.cend()) {
            return ChordQuality::UNKNOWN;
        }

        return iter->second;
    }


    ChordQualityIndicator
    ChordSymbolMaps::getQualityIndicator(ChordSymbolCharValue inChar)
    {
        const auto& inst = getInstance();
        const auto& map = inst.mQualityIndicatorMap;
        const auto iter = map.find(inChar);

        if (iter == map.cend()) {
            return ChordQualityIndicator::UNKNOWN;
        }

        return iter->second;
    }


    int
    ChordSymbolMaps::getInt(ChordExtensionAlter inAlter)
    {
        switch (inAlter) {
            case ChordExtensionAlter::DOUBLE_FLAT: return -2;
            case ChordExtensionAlter::DOUBLE_SHARP: return 2;
            case ChordExtensionAlter::FLAT: return -1;
            case ChordExtensionAlter::SHARP: return 1;
            case ChordExtensionAlter::UNKNOWN: return 0;
            case ChordExtensionAlter::NONE: return 0;
            default:
                break;
        }

        return 0;
    }


    ChordExtensionAlter
    ChordSymbolMaps::getChordExtensionAlter(int inAlterValue)
    {
        if (inAlterValue <= -2) {
            return ChordExtensionAlter::DOUBLE_FLAT;
        } else if (inAlterValue == -1) {
            return ChordExtensionAlter::FLAT;
        } else if (inAlterValue == 0) {
            return ChordExtensionAlter::NONE;
        } else if (inAlterValue == 1) {
            return ChordExtensionAlter::SHARP;
        } else if (inAlterValue >= 2) {
            return ChordExtensionAlter::DOUBLE_SHARP;
        }

        return ChordExtensionAlter::UNKNOWN;
    }


    ChordSymbolCharValue
    ChordSymbolMaps::getAlterCharValue(ChordExtensionAlter inAlter)
    {
        switch (inAlter) {
            case ChordExtensionAlter::DOUBLE_FLAT: return ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT;
            case ChordExtensionAlter::DOUBLE_SHARP: return ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP;
            case ChordExtensionAlter::FLAT: return ChordSymbolCharValue::ACCIDENTAL_FLAT;
            case ChordExtensionAlter::SHARP: return ChordSymbolCharValue::ACCIDENTAL_SHARP;
            case ChordExtensionAlter::UNKNOWN: return ChordSymbolCharValue::ACCIDENTAL_NATURAL;
            case ChordExtensionAlter::NONE: return ChordSymbolCharValue::ACCIDENTAL_NATURAL;
            default:
                break;
        }

        return ChordSymbolCharValue::ACCIDENTAL_NATURAL;
    }


    const ChordSymbolMaps&
    ChordSymbolMaps::getInstance()
    {
        static const ChordSymbolMaps theInstance;
        return theInstance;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
