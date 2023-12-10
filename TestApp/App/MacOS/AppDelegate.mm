// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#import "AppDelegate.h"

// Notation
#import "FileMediatorFactory.h"
#import "FontManager.h"
#import "Score.h"

// Testing
#import "MxTest.h"
#import "TestBridge.h"
#import "TestMenuItem.h"
#import "TestScores.h"

// Local
#import "ScoreDocumentCocoa.h"
#import "ScoreDocumentController.h"

@interface AppDelegate()

@end


@implementation AppDelegate

- (void)
applicationWillFinishLaunching:(NSNotification *)notification {
    
    (void)[[ScoreDocumentController alloc] init];
    
    komp::FontManager::instance();
}

// MARK: - Menu Items

- (IBAction)
runMxTests:(id)sender {
    
    MxTest* mxTest = [MxTest new];
    [mxTest runTests:@"komp-test-output"];
}

- (IBAction)
runTests:(id)sender {
    
    TestBridge* testBridge = [TestBridge new];
    [testBridge runTests:false];
}

- (IBAction)
runFastTests:(id)sender {
    
    TestBridge* testBridge = [TestBridge new];
    [testBridge runTests:true];
}

- (IBAction)
runMxRoundtripTest:(id)sender {
    
    // user interaction to select a file
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setFloatingPanel:YES];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowsMultipleSelection:NO];

    NSArray* fileTypes = [[NSArray alloc] initWithObjects:@"xml", @"XML", nil];
    panel.allowedContentTypes = fileTypes;

    [panel beginWithCompletionHandler:^(NSInteger result){

        if (result == NSModalResponseOK) {
            
            // load and display the selected file
            NSURL* url = [[panel URLs] objectAtIndex:0];
            NSDocumentController* sharedController = [NSDocumentController sharedDocumentController];
            [sharedController openDocumentWithContentsOfURL:url display:YES completionHandler:^(NSDocument* document, BOOL documentWasAlreadyOpen, NSError* error) {
            
                // create a save path - will be in
                // /Users/you/Library/Containers/com.semtitone.audio.KompOSX/Data/Documents/komp-test-output
                NSString *fileNameWithoutExtension = [[url lastPathComponent] stringByDeletingPathExtension];
                NSString *directory = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)objectAtIndex:0]stringByAppendingPathComponent:@"komp-test-output"];
                NSMutableString* filePath = [NSMutableString stringWithString:directory];
                [filePath appendString:@"/"];
                [filePath appendString:fileNameWithoutExtension];
                [filePath appendString:@".saved.xml"];
                
                // save the file using the C++ FileMediator object
                std::string newUrlCppString = [filePath UTF8String];
                auto fileMediator = komp::FileMediatorFactory::create(komp::FileMediatorType::MusicXML);
                ScoreDocumentCocoa* scoreDoc = (ScoreDocumentCocoa*)document;
                komp::FileSaveParams fparams{*[scoreDoc getScore]};
                fileMediator->saveFile(fparams, newUrlCppString);
                
                // copy the original file to the outdir for convenience
                NSMutableString* originalCopyPath = [NSMutableString stringWithString:@"file://"];
                [originalCopyPath appendString:directory];
                [originalCopyPath appendString:@"/"];
                [originalCopyPath appendString:fileNameWithoutExtension];
                [originalCopyPath appendString:@".original.xml"];
                NSURL* orignalCopyUrl = [NSURL URLWithString:originalCopyPath];
                //if ( [[NSFileManager defaultManager] isReadableFileAtPath:[url absoluteString]] ) {
                NSError* copyError;
                [[NSFileManager defaultManager] copyItemAtURL:url toURL:orignalCopyUrl error:&copyError];
                
                // TODO: reopen the saved file for side-by-side comparison once
                // file-saving code is further developed.  for now i will be
                // be diffing the xml code instead
                // NSURL* savedUrl = [NSURL URLWithString:filePath];
                
            }]; // end openDocumentWithContentsOfURL

        } // end if (result == NSFileHandlingPanelOKButton)

    }]; // end beginWithCompletionHandler

} // end runMxRoundtripTest


- (IBAction)
displayNamedScores:(id)sender {
    TestMenuItem* testMenuItem = (TestMenuItem*)sender;
    if (testMenuItem == nil) {
        throw std::runtime_error( "displayNamedScores received an improper NSMenuItem" );
    }
    const std::string testName = [testMenuItem.name UTF8String];
    auto scores = komp::TestScores::create(testName);
    
    for (auto& score : scores) {
        if (!score) {
            throw std::runtime_error( "displayNamedScores received a null score from TestScores::create" );
        }
        ScoreDocumentCocoa* scoreDoc =
        (ScoreDocumentCocoa*)[[NSDocumentController sharedDocumentController]
                              openUntitledDocumentAndDisplay:YES error:nil];
        [scoreDoc setScore:std::move(score)];
        [scoreDoc showWindows];
    }    
}

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
