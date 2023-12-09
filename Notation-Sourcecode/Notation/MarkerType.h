// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class MarkerType
    {
        /// For testing purposes only, MarkerType NONE should have the value of
        /// zero, and the rest of the enum values should not be specified. Do
        /// not rely on the integer values in production code, but I need to
        /// write a test which loops through all mark types.

        NONE = 0,
        CODA,
        CODA_SQUARE,
        DA_CAPO,
        DAL_SEGNO,
        SEGNO,

        /// this value is here for test purposes only. please do not use it in
        /// production code.  always leave this as the last enum value
        TOTAL_COUNT_MARKER_TYPES ///< the number of mark types in the enum
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

