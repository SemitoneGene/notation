// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// OSX
#import <Cocoa/Cocoa.h>

namespace komp {
class Score;
}

@interface ScoreViewCocoa : NSView

- (void) setNeedsDisplay;

- (void) zoomIn;
- (void) zoomOut;
- (void) resetZoom;
- (CGFloat) getZoomScale;

- (void) setScore:(komp::Score*)inScore;

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
