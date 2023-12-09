// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#if TARGET_OS_IPHONE
#elif TARGET_IPHONE_SIMULATOR
#else

@interface NSBezierPath (Cocoa)

- (void) addQuadCurveToPoint:(CGPoint)endPoint controlPoint:(CGPoint)controlPoint;
- (CGPathRef) CGPath;

@end

#endif

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
