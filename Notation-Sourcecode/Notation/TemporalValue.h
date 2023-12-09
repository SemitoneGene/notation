// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>

// Local
#include "Forward.h"
#include "TemporalType.h"
#include "Constants.h"
#include "UpDown.h"

namespace komp
{
    constexpr const double DEFAULT_TIME_BASE = TIME_BASE;
    
    class TemporalValue
    {
    public:

        static constexpr const TemporalType SMALLEST_DURATION = TemporalType::TEN_TWENTY_FOURTH;

        static constexpr const double WHOLE = DEFAULT_TIME_BASE * 4;
        static constexpr const double HALF = DEFAULT_TIME_BASE * 2;
        static constexpr const double QUARTER = DEFAULT_TIME_BASE;
        static constexpr const double EIGHTH = DEFAULT_TIME_BASE / 2;
        static constexpr const double SIXTEENTH = DEFAULT_TIME_BASE / 4;
        static constexpr const double THIRTY_SECOND = DEFAULT_TIME_BASE / 8;
        static constexpr const double SIXTY_FOURTH = DEFAULT_TIME_BASE / 16;
        static constexpr const double ONE_TWENTY_EIGHTH = DEFAULT_TIME_BASE / 32;
        static constexpr const double TWO_FIFTY_SIXTH = DEFAULT_TIME_BASE / 64;
        static constexpr const double FIVE_TWELFTH = DEFAULT_TIME_BASE / 128;
        static constexpr const double TEN_TWENTY_FOURTH = DEFAULT_TIME_BASE / 512;

        static const std::map<int, TemporalType> getClocksMap();

        // unsupported
        static constexpr const double MAXIMA = TemporalValue::WHOLE;
        static constexpr const double LONG = TemporalValue::WHOLE;
        static constexpr const double BREVE = TemporalValue::WHOLE;
        
        static inline constexpr double getValue(TemporalType temporalType)
        {
            switch (temporalType) {
                case TemporalType::WHOLE: return TemporalValue::WHOLE;
                case TemporalType::HALF: return TemporalValue::HALF;
                case TemporalType::QUARTER: return TemporalValue::QUARTER;
                case TemporalType::EIGHTH: return TemporalValue::EIGHTH;
                case TemporalType::SIXTEENTH: return TemporalValue::SIXTEENTH;
                case TemporalType::THIRTY_SECOND: return TemporalValue::THIRTY_SECOND;
                case TemporalType::SIXTY_FOURTH: return TemporalValue::SIXTY_FOURTH;
                case TemporalType::ONE_TWENTY_EIGHTH: return TemporalValue::ONE_TWENTY_EIGHTH;
                case TemporalType::TWO_FIFTY_SIXTH: return TemporalValue::TWO_FIFTY_SIXTH;
                case TemporalType::FIVE_TWELFTH: return TemporalValue::FIVE_TWELFTH;
                case TemporalType::TEN_TWENTY_FOURTH: return TemporalValue::TEN_TWENTY_FOURTH;
                default: return TemporalValue::QUARTER;
            }
        }


        static inline constexpr TemporalType halve(TemporalType temporalType)
        {
            switch (temporalType) {
                case TemporalType::WHOLE: return TemporalType::HALF;
                case TemporalType::HALF: return TemporalType::QUARTER;
                case TemporalType::QUARTER: return TemporalType::EIGHTH;
                case TemporalType::EIGHTH: return TemporalType::SIXTEENTH;
                case TemporalType::SIXTEENTH: return TemporalType::THIRTY_SECOND;
                case TemporalType::THIRTY_SECOND: return TemporalType::SIXTY_FOURTH;
                case TemporalType::SIXTY_FOURTH: return TemporalType::ONE_TWENTY_EIGHTH;
                case TemporalType::ONE_TWENTY_EIGHTH: return TemporalType::TWO_FIFTY_SIXTH;
                case TemporalType::TWO_FIFTY_SIXTH: return TemporalType::FIVE_TWELFTH;
                case TemporalType::FIVE_TWELFTH: return TemporalType::TEN_TWENTY_FOURTH;
                case TemporalType::TEN_TWENTY_FOURTH: return TemporalType::TEN_TWENTY_FOURTH;
                default: return TemporalType::QUARTER;
            }
        }


        static inline constexpr TemporalType augment(TemporalType temporalType)
        {
            switch (temporalType) {
                case TemporalType::WHOLE: return TemporalType::WHOLE;
                case TemporalType::HALF: return TemporalType::WHOLE;
                case TemporalType::QUARTER: return TemporalType::HALF;
                case TemporalType::EIGHTH: return TemporalType::QUARTER;
                case TemporalType::SIXTEENTH: return TemporalType::EIGHTH;
                case TemporalType::THIRTY_SECOND: return TemporalType::SIXTEENTH;
                case TemporalType::SIXTY_FOURTH: return TemporalType::THIRTY_SECOND;
                case TemporalType::ONE_TWENTY_EIGHTH: return TemporalType::SIXTY_FOURTH;
                case TemporalType::TWO_FIFTY_SIXTH: return TemporalType::ONE_TWENTY_EIGHTH;
                case TemporalType::FIVE_TWELFTH: return TemporalType::TWO_FIFTY_SIXTH;
                case TemporalType::TEN_TWENTY_FOURTH: return TemporalType::FIVE_TWELFTH;
                default: return TemporalType::QUARTER;
            }
        }

        static double getValue(TemporalType temporalType, int dots);
        
        /// the number of 'tails' or beams require to denote the duration
        static inline constexpr int getNumFlags(TemporalType temporalType)
        {
            switch (temporalType) {
                case TemporalType::WHOLE: return 0;
                case TemporalType::HALF: return 0;
                case TemporalType::QUARTER: return 0;
                case TemporalType::EIGHTH: return 1;
                case TemporalType::SIXTEENTH: return 2;
                case TemporalType::THIRTY_SECOND: return 3;
                case TemporalType::SIXTY_FOURTH: return 4;
                case TemporalType::ONE_TWENTY_EIGHTH: return 5;
                case TemporalType::TWO_FIFTY_SIXTH: return 6;
                case TemporalType::FIVE_TWELFTH: return 7;
                case TemporalType::TEN_TWENTY_FOURTH: return 8;
                default: return 0;
            }
        }

        /// given a number of flags, what kind of note is it? this doesn't
        /// work if the number of flags is 0 (always returns HALF)
        static inline constexpr TemporalType getTemporalTypeForNumFlags(int numFlags)
        {
            if (numFlags == 1) {
                return TemporalType::EIGHTH;
            }

            if (numFlags == 2) {
                return TemporalType::SIXTEENTH;
            }

            if (numFlags == 3) {
                return TemporalType::THIRTY_SECOND;
            }

            if (numFlags == 4) {
                return TemporalType::SIXTY_FOURTH;
            }

            if (numFlags == 5) {
                return TemporalType::ONE_TWENTY_EIGHTH;
            }

            if (numFlags == 6) {
                return TemporalType::TWO_FIFTY_SIXTH;
            }

            if (numFlags == 7) {
                return TemporalType::FIVE_TWELFTH;
            }

            if (numFlags == 8) {
                return TemporalType::TEN_TWENTY_FOURTH;
            }

            return TemporalType::HALF;
        }

        /// true for eighth note and shorter durations
        static inline constexpr bool getHasFlags(TemporalType temporalType)
        {
            switch (temporalType) {
                case TemporalType::WHOLE: return false;
                case TemporalType::HALF: return false;
                case TemporalType::QUARTER: return false;
                case TemporalType::EIGHTH: return true;
                case TemporalType::SIXTEENTH: return true;
                case TemporalType::THIRTY_SECOND: return true;
                case TemporalType::SIXTY_FOURTH: return true;
                case TemporalType::ONE_TWENTY_EIGHTH: return true;
                case TemporalType::TWO_FIFTY_SIXTH: return true;
                case TemporalType::FIVE_TWELFTH: return true;
                case TemporalType::TEN_TWENTY_FOURTH: return true;
                default: return false;
            }
        }

        /// true if the temporal type should have a stem (i.e. Quarter, Eighth, etc)
        /// and false if it should not (i.e. Whole, Breve, Long)
        static inline constexpr bool getHasStem(TemporalType inType)
        {
            return inType != TemporalType::WHOLE
                && inType != TemporalType::BREVE;
        }
        
        static inline constexpr TemporalType getTypeByTimeSignatureDenominator(int inTimeSignatureDenominator)
        {
            switch (inTimeSignatureDenominator) {
                case 1: return TemporalType::WHOLE;
                case 2: return TemporalType::HALF;
                case 4: return TemporalType::QUARTER;
                case 8: return TemporalType::EIGHTH;
                case 16: return TemporalType::SIXTEENTH;
                case 32: return TemporalType::THIRTY_SECOND;
                case 64: return TemporalType::SIXTY_FOURTH;
                case 128: return TemporalType::ONE_TWENTY_EIGHTH;
                case 256: return TemporalType::TWO_FIFTY_SIXTH;
                case 512: return TemporalType::FIVE_TWELFTH;
                case 1024: return TemporalType::TEN_TWENTY_FOURTH;
                default: return TemporalType::QUARTER;
            }
        }
        
        static inline constexpr int getTimeSignatureDenominatorByType(TemporalType inTemporalType)
        {
            switch (inTemporalType) {
                case TemporalType::WHOLE: return 1;
                case TemporalType::HALF: return 2;
                case TemporalType::QUARTER: return 4;
                case TemporalType::EIGHTH: return 8;
                case TemporalType::SIXTEENTH: return 16;
                case TemporalType::THIRTY_SECOND: return 32;
                case TemporalType::SIXTY_FOURTH: return 64;
                case TemporalType::ONE_TWENTY_EIGHTH: return 128;
                case TemporalType::TWO_FIFTY_SIXTH: return 256;
                case TemporalType::FIVE_TWELFTH: return 512;
                case TemporalType::TEN_TWENTY_FOURTH: return 1024;
                default: return 4;
            }
        }
        
        // This should work but does not -> getNumFlags(SMALLEST_DURATION);
        static constexpr const int MAX_NUM_FLAGS = 8;
        
        static inline bool getIsBeamable(TemporalType inTemporalType)
        {
            return getNumFlags(inTemporalType) > 0;
        }

        static inline TemporalFlagType getTemporalFlagType(TemporalType inType, UpDown inStemDirection)
        {
            TemporalFlagType result = TemporalFlagType::UNHANDLED;
            const bool isStemUp = inStemDirection == UpDown::UP;
            
            switch (inType)
            {
                case TemporalType::EIGHTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::EIGHTH_UP;
                    } else {
                        result = TemporalFlagType::EIGHTH_DOWN;
                    }
                    break;
                }
                    
                case TemporalType::SIXTEENTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::SIXTEENTH_UP;
                    } else {
                        result = TemporalFlagType::SIXTEENTH_DOWN;
                    }
                    break;
                }

                case TemporalType::THIRTY_SECOND:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::THIRTY_SECOND_UP;
                    } else {
                        result = TemporalFlagType::THIRTY_SECOND_DOWN;
                    }
                    break;
                }

                case TemporalType::SIXTY_FOURTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::SIXTY_FOURTH_UP;
                    } else {
                        result = TemporalFlagType::SIXTY_FOURTH_DOWN;
                    }
                    break;
                }

                case TemporalType::ONE_TWENTY_EIGHTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::ONE_TWENTY_EIGHTH_UP;
                    } else {
                        result = TemporalFlagType::ONE_TWENTY_EIGHTH_DOWN;
                    }
                    break;
                }
                    
                case TemporalType::TWO_FIFTY_SIXTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::TWO_FIFTY_SIXTH_UP;
                    } else {
                        result = TemporalFlagType::TWO_FIFTY_SIXTH_DOWN;
                    }
                    break;
                }

                case TemporalType::FIVE_TWELFTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::FIVE_TWELFTH_UP;
                    } else {
                        result = TemporalFlagType::FIVE_TWELFTH_DOWN;
                    }
                    break;
                }

                case TemporalType::TEN_TWENTY_FOURTH:
                {
                    if (isStemUp) {
                        result = TemporalFlagType::TEN_TWENTY_FOURTH_UP;
                    } else {
                        result = TemporalFlagType::TEN_TWENTY_FOURTH_DOWN;
                    }
                    break;
                }

                case TemporalType::BREVE:
                case TemporalType::WHOLE:
                case TemporalType::HALF:
                case TemporalType::QUARTER:
                    // These types will never be flagged
                    break;
                    
                default:
                {
                    LOG_F(ERROR, "Unhandled flag type");
                    break;
                }
            }
            
            return result;
        }

        static inline double getSpacesForDot(int inDots)
        {
            return static_cast<double>(inDots) * 0.5;
        }


        static inline double getSpacesForType(TemporalType inType)
        {
            switch (inType)
            {
                case TemporalType::BREVE:
                case TemporalType::WHOLE:
                    return 7.0;

                case TemporalType::HALF:
                    return 5.0;

                case TemporalType::QUARTER:
                    return 3.5;

                case TemporalType::EIGHTH:
                    return 2.5;

                case TemporalType::SIXTEENTH:
                case TemporalType::THIRTY_SECOND:
                case TemporalType::SIXTY_FOURTH:
                case TemporalType::ONE_TWENTY_EIGHTH:
                case TemporalType::TWO_FIFTY_SIXTH:
                case TemporalType::FIVE_TWELFTH:
                case TemporalType::TEN_TWENTY_FOURTH:
                    return 2.0;
                    
                default:
                {
                    LOG_F(WARNING, "Unahandled temporal type");
                    return 2.0;
                }
            }
        }

        static inline double getSpacesForTypeAndDots(TemporalType inType, int inDots)
        {
            const auto noteSpaces = getSpacesForType(inType);
            const auto dotSpaces = getSpacesForDot(inDots);
            return noteSpaces + dotSpaces;
        }
    };



} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
