// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "FileSaveResult.h"

namespace komp
{
    FileSaveResult::FileSaveResult(FileMessages&& inFileMessages, FileSaveStatus inStatus)
    : mFileMessages(std::move(inFileMessages))
    , mStatus(inStatus)
    {
        
    }
    
    const FileMessages&
    FileSaveResult::getFileMessages() const
    {
        return mFileMessages;
    }
    
    FileSaveStatus
    FileSaveResult::getStatus() const
    {
        return mStatus;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
