// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// OSX
#import <Cocoa/Cocoa.h>

#include "Forward.h"

namespace komp {
    class Score;
}
@class ScoreViewCocoa;


@interface ScoreDocumentCocoa : NSDocument

@property (nonatomic, weak) IBOutlet ScoreViewCocoa* scoreView;

- (void) saveToPDF:(NSString*)inPath;

- (komp::Score*) getScore;
- (void) setScore:(komp::ScoreUPtr)inScore;

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

