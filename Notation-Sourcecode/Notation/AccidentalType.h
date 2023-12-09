// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class AccidentalType
    {
        NONE = 0,
        SHARP,
        FLAT,
        NATURAL,
        DOUBLE_SHARP,
        DOUBLE_FLAT,
        SHARP_SHARP,
        NATURAL_SHARP,
        NATURAL_FLAT,
        QUARTER_FLAT,
        QUARTER_SHARP,
        TRIPLE_SHARP,
        TRIPLE_FLAT,
        PAREN_LEFT,
        PAREN_RIGHT
    };
    
    class AccidentalInfo
    {
    public:
        
        /// returns true if the AccidentalType is supported and
        /// recognized as an accidental that will alter a midi pitch
        inline static bool isPitchAltering(AccidentalType inType) {
            if (inType == AccidentalType::TRIPLE_FLAT ||
                inType == AccidentalType::DOUBLE_FLAT ||
                inType == AccidentalType::FLAT ||
                inType == AccidentalType::NATURAL ||
                inType == AccidentalType::SHARP ||
                inType == AccidentalType::DOUBLE_SHARP ||
                inType == AccidentalType::TRIPLE_SHARP) {
                return true;
            }
            return false;
        }
        
        /// if the AccidentalType isPitchAltering, will return
        /// the alter amount. e.g. 0 for natural, -1 for flat
        inline static int getPitchAlter(AccidentalType inType) {
            
            if (inType == AccidentalType::TRIPLE_FLAT) {
                return -3;
            } else if (inType == AccidentalType::DOUBLE_FLAT) {
                return -2;
            } else if (inType == AccidentalType::FLAT) {
                return -1;
            } else if (inType == AccidentalType::NATURAL) {
                return 0;
            } else if (inType == AccidentalType::SHARP) {
                return 1;
            } else if (inType == AccidentalType::DOUBLE_SHARP) {
                return 2;
            } else if (inType == AccidentalType::TRIPLE_SHARP) {
                return 3;
            }
            
            return 0;
        }

        /// if the alter value is within the acceptable range (-3 to 3)
        /// this function will return the appropriate accidental type
        /// to represent the alter value.
        inline static AccidentalType getAccidentalType(int inAlterValue) {

            if (inAlterValue == -3) {
                return AccidentalType::TRIPLE_FLAT;
            } else if (inAlterValue == -2) {
                return AccidentalType::DOUBLE_FLAT;
            } else if (inAlterValue == -1) {
                return AccidentalType::FLAT;
            } else if (inAlterValue == 0) {
                return AccidentalType::NATURAL;
            } else if (inAlterValue == 1) {
                return AccidentalType::SHARP;
            } else if (inAlterValue == 2) {
                return AccidentalType::DOUBLE_SHARP;
            } else if (inAlterValue == 3) {
                return AccidentalType::TRIPLE_SHARP;
            }

            return AccidentalType::NONE;
        }
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
