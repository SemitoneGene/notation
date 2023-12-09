// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "Forward.h"
#include "StringViewComparator.h"
#include "Version.h"


#include <string>
#include <map>

namespace komp
{
    enum class FileLoadStatus
    {
        Suceess,
        Failure
    };
    
    class FileLoadResult
    {
    public:
        FileLoadResult(ScoreUPtr inScore, FileLoadStatus inStatus);
        
        const FileMessages& getFileMessages() const;
        void setFileMessages(FileMessages&& messages);
        
        /// was the file created by komp
        bool getIsKompFile() const;
        void setIsKompFile(bool inValue);
        
        /// the version number found in the komp file
        /// or 0.0.0 if the file was not from komp
        const Version& getVersion() const;
        void setVersion(Version inValue);
        
        /// if getScore() is null then status will always return Failure
        /// otherwise status will set by the FileMediatorb
        FileLoadStatus getStatus() const;

        /// get any supported key-value pair that has been saved in the file.
        /// returns true if the retreival was successful, false means unsuccessful
        /// outValue is the value of the property being retreived
        bool getProperty(std::string_view inPropertyName, double& outValue) const;

        /// get any supported key-value pair that has been saved in the file.
        /// returns true if the retreival was successful, false means unsuccessful
        /// outValue is the value of the property being retreived
        bool getProperty(std::string_view inPropertyName, int& outValue) const;

        /// get any supported key-value pair that has been saved in the file.
        /// returns true if the retreival was successful, false means unsuccessful
        /// outValue is the value of the property being retreived
        bool getProperty(std::string_view inPropertyName, std::string& outValue) const;

    private:
        ScoreUPtr mScore;
        FileMessages mFileMessages;
        FileLoadStatus mStatus;
        bool mIsKompFile;
        Version mVersion;
        std::map<std::string, std::string, StringViewComparator> mProperties;
    };
    
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
