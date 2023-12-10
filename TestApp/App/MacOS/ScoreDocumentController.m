// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#import "ScoreDocumentController.h"

@implementation ScoreDocumentController

- (id)
openUntitledDocumentAndDisplay:(BOOL)displayDocument error:(NSError **)outError {
    
    if ([[self documents] count] > 0) {
        return [super openUntitledDocumentAndDisplay:displayDocument error:outError];
    } else {
        if (outError) {
            *outError = [NSError errorWithDomain:NSCocoaErrorDomain code:NSUserCancelledError userInfo:nil];
        }
        return nil;
    }
}

@end
