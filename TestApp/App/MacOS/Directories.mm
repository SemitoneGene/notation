// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Directories.h"

// OS X
#import <Cocoa/Cocoa.h>

namespace komp
{
    std::string
    getTestOutputDirectory()
    {
        NSString* subdirectoryName = @"komp-test-output";
        NSString* outputDirectory = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)objectAtIndex:0]stringByAppendingPathComponent:subdirectoryName];
        return std::string{[outputDirectory UTF8String]};
    }
    
    std::string
    getConfigDirectory()
    {
        NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString *subdirPath = [resourcePath stringByAppendingPathComponent:@"Config"];
        return std::string{[subdirPath UTF8String]};
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


/*
 -(NSString*)
 testOutputDirectory:(NSString*)inSubdirectoryName {
 return [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)objectAtIndex:0]stringByAppendingPathComponent:inSubdirectoryName];
 }
*/
