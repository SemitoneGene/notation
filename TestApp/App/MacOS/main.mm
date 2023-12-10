// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// OSX
#import <Cocoa/Cocoa.h>

// Local
#import "FontManager.h"
#import "Logging.h"

#ifdef DEBUG
#define KOMP_LOG_OUTPUT_LEVEL KOMP_LOG_VERBOSE
#else
#define KOMP_LOG_OUTPUT_LEVEL KOMP_LOG_INFO
#endif

int main(int argc, const char * argv[]) {
    
    auto& fontManager = komp::FontManager::instance();
    fontManager.configure();

    loguru::init(argc, argv);
    loguru::g_stderr_verbosity = 3;

    return NSApplicationMain(argc, argv);
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
