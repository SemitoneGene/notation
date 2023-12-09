// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include <string>

namespace komp
{
#define KOMP_ENUM_CLASS_NAME ChordType

#define KOMP_ENUM_MEMBERS( KOMP_ENUM_VALUE ) \
KOMP_ENUM_VALUE( UNKNOWN                   ) \
KOMP_ENUM_VALUE( MAJOR                     ) \
KOMP_ENUM_VALUE( MINOR                     ) \
KOMP_ENUM_VALUE( AUGMENTED                 ) \
KOMP_ENUM_VALUE( DIMINISHED                ) \
KOMP_ENUM_VALUE( DOMINANT                  ) \
KOMP_ENUM_VALUE( MAJOR_SEVENTH             ) \
KOMP_ENUM_VALUE( MINOR_SEVENTH             ) \
KOMP_ENUM_VALUE( DIMINISHED_SEVENTH        ) \
KOMP_ENUM_VALUE( AUGMENTED_SEVENTH         ) \
KOMP_ENUM_VALUE( HALF_DIMINISHED           ) \
KOMP_ENUM_VALUE( MAJOR_MINOR               ) \
KOMP_ENUM_VALUE( MAJOR_SIXTH               ) \
KOMP_ENUM_VALUE( MINOR_SIXTH               ) \
KOMP_ENUM_VALUE( DOMINANT_NINTH            ) \
KOMP_ENUM_VALUE( MAJOR_NINTH               ) \
KOMP_ENUM_VALUE( MINOR_NINTH               ) \
KOMP_ENUM_VALUE( DOMINANT_11TH             ) \
KOMP_ENUM_VALUE( MAJOR_11TH                ) \
KOMP_ENUM_VALUE( MINOR_11TH                ) \
KOMP_ENUM_VALUE( DOMINANT_13TH             ) \
KOMP_ENUM_VALUE( MAJOR_13TH                ) \
KOMP_ENUM_VALUE( MINOR_13TH                ) \
KOMP_ENUM_VALUE( SUSPENDED_SECOND          ) \
KOMP_ENUM_VALUE( SUSPENDED_FOURTH          ) \
KOMP_ENUM_VALUE( NEAPOLITAN                ) \
KOMP_ENUM_VALUE( ITALIAN                   ) \
KOMP_ENUM_VALUE( FRENCH                    ) \
KOMP_ENUM_VALUE( GERMAN                    ) \
KOMP_ENUM_VALUE( PEDAL                     ) \
KOMP_ENUM_VALUE( POWER                     ) \
KOMP_ENUM_VALUE( TRISTAN                   ) \
KOMP_ENUM_VALUE( OTHER                     ) \
KOMP_ENUM_VALUE( NONE                      ) \

#include "kern/Enum.def"
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
