// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#if TARGET_OS_IPHONE
#elif TARGET_IPHONE_SIMULATOR
#else
// OSX
#import <Cocoa/Cocoa.h>

// Self
#import "NotationNSBezierPath+Cocoa.h"

@implementation NSBezierPath (Cocoa)

- (void)
addQuadCurveToPoint:(CGPoint)endPoint controlPoint:(CGPoint)controlPoint {
    
    CGPoint QP0 = self.currentPoint;
    CGPoint QP2 = endPoint;
    CGPoint CP3 = QP2;
    CGPoint QP1 = controlPoint;
    
    CGPoint CP1 = CGPointMake(
                              //  QP0   +   2   / 3    * (QP1   - QP0  )
                              QP0.x + ((2.0 / 3.0) * (QP1.x - QP0.x)), QP0.y + ((2.0 / 3.0) * (QP1.y - QP0.y)));
    
    CGPoint CP2 = CGPointMake(
                              //  QP2   +  2   / 3    * (QP1   - QP2)
                              QP2.x + (2.0 / 3.0) * (QP1.x - QP2.x), QP2.y + (2.0 / 3.0) * (QP1.y - QP2.y));
    
    [self curveToPoint:CP3 controlPoint1:CP1 controlPoint2:CP2];
}

- (CGPathRef)
CGPath {

    const long numElements = [self elementCount];
    if (numElements <= 0) {
        return NULL;
    }
    
    CGPoint points[3];
    BOOL didClosePath = YES;
    
    CGMutablePathRef path = CGPathCreateMutable();

    for (int i = 0; i < numElements; i++) {
        switch ([self elementAtIndex:i associatedPoints:points])
        {
            case NSBezierPathElementMoveTo:
                CGPathMoveToPoint(path, NULL, points[0].x, points[0].y);
                break;
                                
            case NSBezierPathElementLineTo:
                CGPathAddLineToPoint(path, NULL, points[0].x, points[0].y);
                didClosePath = NO;
                break;
                
            case NSBezierPathElementCubicCurveTo:
                CGPathAddCurveToPoint(path, NULL, points[0].x, points[0].y,
                                      points[1].x, points[1].y,
                                      points[2].x, points[2].y);
                didClosePath = NO;
                break;
                
            case NSBezierPathElementClosePath:
                CGPathCloseSubpath(path);
                didClosePath = YES;
                break;
            
        default:
            break;
        }
    }
    
    if (!didClosePath) {
        // Be sure the path is closed or Quartz may not do valid hit detection.
        //CGPathCloseSubpath(path);
    }
    
    CGPathRef result = CGPathCreateCopy(path);
    CGPathRelease(path);
    return result;
}

@end

#endif // !TARGET_OS_IPHONE || !TARGET_IPHONE_SIMULATOR

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
