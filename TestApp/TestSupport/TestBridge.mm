// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#import "TestBridge.h"

// Local
#import "TestController.h"

@interface TestBridge()

{
    komp::TestController _testController;
}

@end

@implementation TestBridge

- (void)
runTests:(bool)excludeSlowTests {
    _testController = komp::TestController{};
    _testController.runTests(excludeSlowTests);
    
    const std::string message = _testController.getTestRunResultMessage();
    NSString* nsmsg = [NSString stringWithUTF8String:message.c_str()];
    //std::cout << std::endl << message << std::endl;
    NSAlert* alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:nsmsg];
    
    if (_testController.getFailureCount() > 0 || _testController.getErrorCount() > 0) {
        [alert setInformativeText:@"Please fix the broken tests!"];
    } else {
        [alert setInformativeText:@"Success!"];
    }
    
    [alert setAlertStyle:NSAlertStyleWarning];
    alert.accessoryView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 1000, 0)];
    [alert runModal];
}

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
