// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class MarkCategory
    {
        NONE,
        ACCIDENTAL,
        ARTICULATION,
        DYNAMIC,
        EXPRESSION,
        FERMATA,
        MARKER,
        ORNAMENT,
        TEMPO,
        TECHNIQUE,
        UNKNOWN
    };
    
    enum class MarkType
    {
        /// For testing purposes only, MarkType NONE should have the value of
        /// zero, and the rest of the enum values should not be specified. Do
        /// not rely on the integer values in production code, but I need to
        /// write a test which loops through all mark types.
        NONE = 0,
        ACCENT_ABOVE,
        ACCENT_BELOW,
        ACCENT_STACCATO_ABOVE,
        ACCENT_STACCATO_BELOW,
        BRASS_DOIT,
        BRASS_FALLOFF,
        BRASS_LIFT,
        BRASS_MUTE_CLOSED,
        BRASS_PLOP,
        BRASS_SCOOP,
        BREATH_MARK,
        CAESURA,
        DOT,
        DYNAMIC_F,
        DYNAMIC_FF,
        DYNAMIC_FFF,
        DYNAMIC_FFFF,
        DYNAMIC_FFFFF,
        DYNAMIC_FFFFFF,
        DYNAMIC_FP,
        DYNAMIC_FZ,
        DYNAMIC_MF,
        DYNAMIC_MP,
        DYNAMIC_NIENTE,
        DYNAMIC_OTHER,
        DYNAMIC_P,
        DYNAMIC_PP,
        DYNAMIC_PPP,
        DYNAMIC_PPPP,
        DYNAMIC_PPPPP,
        DYNAMIC_PPPPPP,
        DYNAMIC_RF,
        DYNAMIC_RFZ,
        DYNAMIC_S,
        DYNAMIC_SF,
        DYNAMIC_SFFZ,
        DYNAMIC_SFP,
        DYNAMIC_SFPP,
        DYNAMIC_SFZ,
        DYNAMIC_UNKNOWN,
        EXPRESSION_ACCELERANDO,
        EXPRESSION_CRESCENDO,
        EXPRESSION_DECRESCENDO,
        EXPRESSION_DIM,
        EXPRESSION_LV,
        EXPRESSION_RITARDANDO,
        FERMATA,
        FERMATA_INVERTED,
        FERMATA_LONG_ABOVE,
        FERMATA_LONG_BELOW,
        FERMATA_SHORT_ABOVE,
        FERMATA_SHORT_BELOW,
        FLAT,
        GRACE_SLASH_DOWN,
        GRACE_SLASH_UP,
        MARCATO_ABOVE,
        MARCATO_BELOW,
        MARCATO_STACCATO_ABOVE,
        MARCATO_STACCATO_BELOW,
        MARCATO_TENUTO_ABOVE,
        MARCATO_TENUTO_BELOW,
        MORDENT,
        MORDENT_INVERTED,
        NATURAL,
        PAREN_LEFT,
        PAREN_RIGHT,
        PEDAL,
        PEDAL_UP,
        SHARP,
        STACCATISSIMO_ABOVE,
        STACCATISSIMO_BELOW,
        STACCATO_ABOVE,
        STACCATO_BELOW,
        STRESS_ABOVE,
        STRESS_BELOW,
        STRINGS_BOW_DOWN,
        STRINGS_BOW_UP,
        STRINGS_HARMONIC,
        STRINGS_HARMONIC_HALF,
        STRINGS_OPEN,
        TENUTO_ABOVE,
        TENUTO_ACCENT_ABOVE,
        TENUTO_ACCENT_BELOW,
        TENUTO_BELOW,
        TENUTO_STACCATO_ABOVE,
        TENUTO_STACCATO_BELOW,
        TREMOLO_ONE,
        TREMOLO_TWO,
        TREMOLO_THREE,
        TREMOLO_FOUR,
        TREMOLO_FIVE,
        TRILL,
        TURN,
        TURN_INVERTED,
        TURN_SLASH,
        UNSTRESS_ABOVE,
        UNSTRESS_BELOW,

        /// this value is here for test purposes only. please do not use it in
        /// production code.  always leave this as the last enum value
        TOTAL_COUNT_MARK_TYPES ///< the number of mark types in the enum
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

