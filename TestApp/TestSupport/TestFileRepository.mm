// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TestSupport/TestFileRepository.h"

// Local
#include "FileMediatorFactory.h"
#include "Score.h"
#include "Throw.h"

// OSX
#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>

namespace komp
{
    const TestFiles&
    TestFileRepository::getAllFiles()
    {
        return getFiles();
    }
    
    TestFiles
    TestFileRepository::getNonExcludedFiles()
    {
        TestFiles outFiles;
        const auto& excluded = getInstance().myExcludedFiles;
        for (const auto& file : getAllFiles()) {
            if (excluded.find(std::string(file.getFilename())) == excluded.cend()) {
                outFiles.push_back(file);
            }
        }
        return outFiles;
    }
    
    komp::ScoreUPtr
    TestFileRepository::loadScore(std::string fileName)
    {
        auto finder = [&](const TestFile& file) {
            return file.getFilename() == fileName;
        };
        
        auto iter = std::find_if(getAllFiles().cbegin(), getAllFiles().cend(), finder);
        KOMP_ASSERT(iter != getAllFiles().cend());
        auto fileMediator = komp::FileMediatorFactory::create(komp::FileMediatorType::MusicXML);
        return fileMediator->loadFile(iter->getFullPath());
    }


    komp::ScoreUPtr
    TestFileRepository::loadScore(std::string fileName, const InstrumentConfig config)
    {
        auto finder = [&](const TestFile& file) {
            return file.getFilename() == fileName;
        };

        auto iter = std::find_if(getAllFiles().cbegin(), getAllFiles().cend(), finder);
        KOMP_ASSERT(iter != getAllFiles().cend());
        auto fileMediator = komp::FileMediatorFactory::create(komp::FileMediatorType::MusicXML);
        return fileMediator->loadFile(iter->getFullPath(), config);
    }
    
    TestFileRepository::TestFileRepository()
    {
        myMxLoadFailureFiles = expectedMxLoadFailureFiles;
        myExcludedFiles = excludedFiles;
        initializeFileList();
    }
    
    void TestFileRepository::initializeFileList()
    {
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* documentsPath = [resourcePath stringByAppendingPathComponent:@"musicxml"];
        
        
        NSFileManager* fileManager = [NSFileManager defaultManager];
        NSURL* directoryURL = [NSURL fileURLWithPath:documentsPath isDirectory:true];
        NSArray* keys = [NSArray arrayWithObject:NSURLIsDirectoryKey];
        
        NSDirectoryEnumerator* enumerator = [fileManager
                                             enumeratorAtURL:directoryURL
                                             includingPropertiesForKeys:keys
                                             options:0
                                             errorHandler:^(NSURL *url, NSError *error) {
                                                 return YES;
                                             }];
        
        NSMutableArray<NSURL*>* fileUrls = [NSMutableArray new];
        
        for (NSURL *url in enumerator) {
            NSError *error;
            NSNumber *isDirectory = nil;
            if (! [url getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:&error]) {
                
            }
            else if (! [isDirectory boolValue]) {
                
                NSString* lastPath = [url lastPathComponent];
                NSString* fileExtension = [lastPath pathExtension];
                NSString* xmlString = @"xml";
                
                if ([xmlString isEqualToString:fileExtension] ) {
                    [fileUrls addObject:url];
                }
            }
        }
        
        for (NSURL* url in fileUrls) {
            std::string mainDir = [documentsPath UTF8String];
            mainDir += "/";
            NSString* subDirPath = [[url absoluteString] stringByDeletingLastPathComponent];
            NSString* subDir = [subDirPath lastPathComponent];
            std::string sub = [subDir UTF8String];
            sub += "/";
            std::string fileName = [[url lastPathComponent] UTF8String];
            const auto iter = myMxLoadFailureFiles.find( fileName );
            bool isLoadFailureExpected = iter != myMxLoadFailureFiles.cend();
            myTestFiles.emplace_back(mainDir, sub, fileName, isLoadFailureExpected);
        }
    }
    
    const TestFileRepository& TestFileRepository::getInstance()
    {
        static TestFileRepository instance;
        return instance;
    }
    
    const TestFiles& TestFileRepository::getFiles()
    {
        return getInstance().myTestFiles;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
