// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    enum class TemporalType
    {
        BREVE,
        WHOLE,
        HALF,
        QUARTER,
        EIGHTH,
        SIXTEENTH,
        THIRTY_SECOND,
        SIXTY_FOURTH,
        ONE_TWENTY_EIGHTH,
        TWO_FIFTY_SIXTH,
        FIVE_TWELFTH,
        TEN_TWENTY_FOURTH,
        UNHANDLED
    };
    
    enum class TemporalFlagType
    {
        EIGHTH_UP,
        SIXTEENTH_UP,
        THIRTY_SECOND_UP,
        SIXTY_FOURTH_UP,
        ONE_TWENTY_EIGHTH_UP,
        TWO_FIFTY_SIXTH_UP,
        FIVE_TWELFTH_UP,
        TEN_TWENTY_FOURTH_UP,
        EIGHTH_DOWN,
        SIXTEENTH_DOWN,
        THIRTY_SECOND_DOWN,
        SIXTY_FOURTH_DOWN,
        ONE_TWENTY_EIGHTH_DOWN,
        TWO_FIFTY_SIXTH_DOWN,
        FIVE_TWELFTH_DOWN,
        TEN_TWENTY_FOURTH_DOWN,
        UNHANDLED
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
