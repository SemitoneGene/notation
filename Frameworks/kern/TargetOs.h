// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#define KOMP_EXPORT __attribute__((visibility("default")))

#if KOMP_BUILD_APPLE 

#include "TargetConditionals.h"

#if TARGET_OS_IPHONE
    #define KompBezierPath UIBezierPath
    #define KompColor UIColor
    #define KompFont UIFont
    #define KompImage UIImage
#elif TARGET_OS_MAC
    #define KompBezierPath NSBezierPath
    #define KompColor NSColor
    #define KompFont NSFont
    #define KompImage NSImage
#else
    #error Only TARGET_OS_MAC and TARGET_OS_IPHONE are supported.
#endif

#endif

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
