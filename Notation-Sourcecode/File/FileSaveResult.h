// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "Forward.h"
#include "Version.h"

namespace komp
{
    enum class FileSaveStatus
    {
        Suceess,
        Failure
    };
    
    class FileSaveResult
    {
    public:
        FileSaveResult(FileMessages&& inFileMessages, FileSaveStatus inStatus);

        const FileMessages& getFileMessages() const;
        FileSaveStatus getStatus() const;
        
    private:
        FileMessages mFileMessages;
        FileSaveStatus mStatus;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
