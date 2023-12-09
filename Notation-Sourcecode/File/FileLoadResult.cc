// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FileLoadResult.h"

// Local
#include "Score.h"
#include "Track.h"

namespace komp
{
    FileLoadResult::FileLoadResult(ScoreUPtr inScore, FileLoadStatus inStatus)
    : mScore(std::move(inScore))
    , mFileMessages()
    , mStatus(inStatus)
    , mIsKompFile(false)
    , mVersion(0, 0, 0)
    {

    }
    
    const FileMessages&
    FileLoadResult::getFileMessages() const
    {
        return mFileMessages;
    }
    
    void
    FileLoadResult::setFileMessages(FileMessages&& messages)
    {
        mFileMessages = std::move(messages);
    }
    
    bool
    FileLoadResult::getIsKompFile() const
    {
        return mIsKompFile;
    }
    
    void
    FileLoadResult::setIsKompFile(bool inValue)
    {
        mIsKompFile = inValue;
    }
    
    const Version&
    FileLoadResult::getVersion() const
    {
        return mVersion;
    }
    
    void
    FileLoadResult::setVersion(Version inValue)
    {
        mVersion = std::move(inValue);
    }
    
    FileLoadStatus
    FileLoadResult::getStatus() const
    {
        if (!mScore) {
            return FileLoadStatus::Failure;
        }
        
        return mStatus;
    }


    bool
    FileLoadResult::getProperty(std::string_view inPropertyName, double& outValue) const
    {
        const auto iter = mProperties.find(inPropertyName);

        if (iter == mProperties.cend()) {
            return false;
        }

        const auto& value = iter->second;

        try {
            const double dval = std::stod(value);
            outValue = dval;
            return true;
        } catch (...) {
            return false;
        }

        return false;
    }



    bool
    FileLoadResult::getProperty(std::string_view inPropertyName, int& outValue) const
    {
        const auto iter = mProperties.find(inPropertyName);

        if (iter == mProperties.cend()) {
            return false;
        }

        const auto& value = iter->second;

        try {
            const int ival = std::stoi(value);
            outValue = ival;
            return true;
        } catch (...) {
            return false;
        }
        
        return false;
    }



    bool
    FileLoadResult::getProperty(std::string_view inPropertyName, std::string& outValue) const
    {
        const auto iter = mProperties.find(inPropertyName);

        if (iter == mProperties.cend()) {
            return false;
        }

        outValue = iter->second;
        return true;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
