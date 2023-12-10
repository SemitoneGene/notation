// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#import "MxTest.h"

// OS X
#import <Cocoa/Cocoa.h>

// Notation
#import "Preferences.h"

// Local
#import "ScoreDocumentCocoa.h"
#import "Score.h"
#import "ScoreViewCocoa.h"

@interface MxTest ()

@end

@implementation MxTest

// MARK: - Public Functions


- (void)
runTests:(NSString*)outputDirectoryName {
    
    auto controller = [NSDocumentController sharedDocumentController];
    [controller
     closeAllDocumentsWithDelegate:nil
     didCloseAllSelector:nil contextInfo:nil];
    
    [self createOutputDirectoryIfNotExists:outputDirectoryName];
    [self deleteContentsOfTestOutputDirectory:outputDirectoryName];
    
    auto fileUrls = [self findAllXmlFiles];
    for( NSURL* fileUrl in fileUrls ) {

        @try {
            
            [self runMxTest:fileUrl testDirName:outputDirectoryName controller:controller];
            
        } @catch (NSException *exception) {
            
            [self writeErrorInfo:fileUrl testDirName:outputDirectoryName errorMessage:exception.reason];
            
        } @finally {


        }
    }
}


// MARK: - Private Functions - Top Level


- (void)
runMxTest:(NSURL*)inFileUrl testDirName:(NSString*)testDirName controller:(NSDocumentController*)inController {
    
    NSError* error;
    NSDocument* document = [inController makeDocumentWithContentsOfURL:inFileUrl ofType:@"MusicXML" error:&error];
    if (error) {
        @throw [NSException
                exceptionWithName:@"makeDocumentWithContentsOfURL exception"
                reason:error.localizedDescription
                userInfo:nil];
    } else {
        NSString* outputPdfFilePath = [self outputPdfFilePath:inFileUrl testDirName:testDirName];
        [self writeScoreDocumentToPdf:document pdfOutputFilePath:outputPdfFilePath];
        [document close];
    }
}


-(NSMutableArray<NSURL*>*)
findAllXmlFiles {
    
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *documentsPath = [resourcePath stringByAppendingPathComponent:@"musicxml"];
    
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSURL *directoryURL = [NSURL fileURLWithPath:documentsPath isDirectory:true];
    NSArray *keys = [NSArray arrayWithObject:NSURLIsDirectoryKey];
    
    NSDirectoryEnumerator *enumerator = [fileManager
                                         enumeratorAtURL:directoryURL
                                         includingPropertiesForKeys:keys
                                         options:0
                                         errorHandler:^(NSURL *url, NSError *error) {
                                             return YES;
                                         }];
    
    NSMutableArray<NSURL*> *fileUrls = [NSMutableArray new];
    
    for (NSURL *url in enumerator) {
        NSError *error;
        NSNumber *isDirectory = nil;
        if (! [url getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:&error]) {
            
        }
        else if (! [isDirectory boolValue]) {
            
            NSString *lastPath = [url lastPathComponent];
            NSString *fileExtension = [lastPath pathExtension];
            NSString *xmlString = @"xml";
            
            if ([xmlString isEqualToString:fileExtension] ) {
                
                // check if it is a known bad/failure file
                BOOL isTestPartsSpecialCases = [lastPath containsString:@"testPartsSpecialCases"];
                BOOL is41gPartNoId = [lastPath containsString:@"41g-PartNoId"];
                
                // check if it is an annoyingly large file
                BOOL isEzinar = [lastPath containsString:@"ezinar.xml"];
                BOOL isHomoSapiens = [lastPath containsString:@"logic01a_homoSapiens.xml"];
                BOOL isFreezing = [lastPath containsString:@"freezing.xml"];
                
                if (!isTestPartsSpecialCases &&
                   !is41gPartNoId &&
                   !isEzinar &&
                   !isHomoSapiens &&
                   !isFreezing ) {
                    [fileUrls addObject:url];
                }
            }
        }
    }
    return fileUrls;
}



-(void)
deleteContentsOfTestOutputDirectory:(NSString*)testDirName {
    
    NSFileManager *fileMgr = [NSFileManager defaultManager];
    NSArray *fileArray = [fileMgr contentsOfDirectoryAtPath:[self testOutputDirectory:testDirName] error:nil];
    for (NSString *filename in fileArray)  {
        
        [fileMgr removeItemAtPath:[[self testOutputDirectory:testDirName] stringByAppendingPathComponent:filename] error:NULL];
    }
    
}


-(void)
writeScoreDocumentToPdf:(NSDocument*)document pdfOutputFilePath:(NSString*)pdfOutputFilePath {

    // TODO: this function was broken by changes to score layout in January 2017
    
//    ScoreDocumentCocoa *scoreDocument = (ScoreDocumentCocoa*)document;
//    const auto pageLayout = scoreDocument.score->getLayout()->getPageLayout();
//    const CGRect pageRect = CGRectMake(0, 0, pageLayout.getPageWidth(), pageLayout.getPageHeight());
//    const auto& score = scoreDocument.score;
//    
//    CFURLRef url = CFURLCreateWithFileSystemPath (NULL,
//                                                  (__bridge CFStringRef)pdfOutputFilePath,
//                                                  kCFURLPOSIXPathStyle,
//                                                  0);
//    
//
//    CFMutableDictionaryRef myDictionary = CFDictionaryCreateMutable(NULL, 0,
//                                             &kCFTypeDictionaryKeyCallBacks,
//                                             &kCFTypeDictionaryValueCallBacks);
//    
//    
//    CFDictionarySetValue(myDictionary, kCGPDFContextTitle, CFSTR("Komp Score"));
//    CFDictionarySetValue(myDictionary, kCGPDFContextCreator, CFSTR("Maestro MJB"));
//    CGContextRef pdfContext = nullptr;
//    pdfContext = CGPDFContextCreateWithURL (url, &pageRect, myDictionary); // 5
//    
//    try {
//        for (const auto& page : score->getPages()) {
//        
//            CGPDFContextBeginPage(pdfContext, myDictionary);
//            
//            // Flip the context so the image has the correct orientation
//            CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, pageRect.size.height);
//            CGContextConcatCTM(pdfContext, flipVertical);
//            scoreDocument.score->drawPage(pdfContext, page, pageRect);
//            
//            CGPDFContextEndPage(pdfContext);
//        }
//    } catch(std::exception& e) {
//        @throw [NSException
//                exceptionWithName:@"score->drawPage() exception"
//                reason:[NSString stringWithUTF8String:e.what()]
//                userInfo:nil];
//    }
//    
//    // Clean up
//    CGContextRelease(pdfContext);
//    CFRelease(myDictionary);
//    CFRelease(url);
}


// MARK: - Private Functions - Directory and FileName String Building


-(NSString*)
testOutputDirectory:(NSString*)inSubdirectoryName {
    return [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)objectAtIndex:0]stringByAppendingPathComponent:inSubdirectoryName];
}


-(NSString*)
inputFileNameWithoutExtension:(NSURL*)inInputFileUrl {
    return [[inInputFileUrl lastPathComponent] stringByDeletingPathExtension];
}


-(NSString*)
outputFileName:(NSURL*)inInputFileUrl prefix:(NSString*)prefix postfix:(NSString*)postfix {
    NSMutableString *outFileName = [NSMutableString new];
    [outFileName appendString:prefix];
    [outFileName appendString:[self inputFileNameWithoutExtension:inInputFileUrl]];
    [outFileName appendString:postfix];
    return outFileName;
}


-(NSString*)
outputPdfFileName:(NSURL*)inInputFileUrl {
    return [self outputFileName:inInputFileUrl prefix:@"komp.output." postfix:@".pdf"];
}


-(NSString*)
outputPdfFilePath:(NSURL*)inInputFileUrl testDirName:(NSString*)testDirName {
    return [[self testOutputDirectory:testDirName] stringByAppendingPathComponent:[self outputPdfFileName:inInputFileUrl]];
}


-(NSString*)
outputErrorFileName:(NSURL*)inInputFileUrl {
    return [self outputFileName:inInputFileUrl prefix:@"komp.error." postfix:@".txt"];
}


-(NSString*)
outputErrorFilePath:(NSURL*)inInputFileUrl testDirName:(NSString*)testDirName {
    return [[self testOutputDirectory:testDirName] stringByAppendingPathComponent:[self outputErrorFileName:inInputFileUrl]];
}


-(void)
createOutputDirectoryIfNotExists:(NSString*)testDirName {
    
    NSString *stringPath = [self testOutputDirectory:testDirName];
    NSError *error = nil;
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:stringPath]) {
        
        [[NSFileManager defaultManager]
         createDirectoryAtPath:stringPath
         withIntermediateDirectories:NO
         attributes:nil
         error:&error];
    }
    
    if (error) {
        [NSException raise:@"Error creating the test output directory - " format:@"%@", error];
    }
}


// MARK: - Private Functions - Error Handling


-(void)
writeErrorInfo:(NSURL*)inInputFileUrl testDirName:(NSString*)testDirName errorMessage:(NSString*)errorMessage {
    
    NSLog(@"%@ - %@", inInputFileUrl, errorMessage);
    
    NSError *error;
    
    [errorMessage
     writeToFile:[self outputErrorFilePath:inInputFileUrl testDirName:testDirName]
     atomically:NO
     encoding:NSUTF8StringEncoding
     error:&error];
    
    if (error) {
        NSLog(@"Error occurred while writing to text file - %@", error.localizedDescription);
    }
}

@end

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
