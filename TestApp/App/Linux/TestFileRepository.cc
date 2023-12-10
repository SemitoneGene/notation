// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TestSupport/TestFileRepository.h"

// System
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>


// Local
#include "FileMediatorFactory.h"
#include "Score.h"
#include "Throw.h"


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

static const std::string FilesPath = "/Users/gragan/inprogress/semitone/komp/Resources/Documents/musicxml";

void
TestFileRepository::initializeFileList()
{
    if (std::filesystem::exists(FilesPath) && std::filesystem::is_directory(FilesPath)) {
        
        for (auto const& entry: std::filesystem::recursive_directory_iterator(FilesPath)) {
            
            if (std::filesystem::is_regular_file(entry)
                && entry.path().extension() == ".xml") {
                
                const auto fileName = entry.path().filename().string();
                const auto sub = entry.path().parent_path().filename().string() + "/";
                                
                const auto iter = myMxLoadFailureFiles.find(fileName);
                const auto isLoadFailureExpected = iter != myMxLoadFailureFiles.cend();
                myTestFiles.emplace_back(FilesPath +  + "/",
                                         sub,
                                         fileName,
                                         isLoadFailureExpected);
            }
        }
    } else {
        std::cerr << FilesPath << " is not a valid directory." << std::endl;
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
