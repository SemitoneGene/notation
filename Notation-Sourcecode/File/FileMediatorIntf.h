// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <istream>
#include <map>
#include <string>

// Local
#include "FileSaveResult.h"
#include "Forward.h"
#include "StringViewComparator.h"

namespace komp
{
    class InstrumentConfig;

    /// any key value data that you want to save with the file
    /// can be added to the properties map.  Some filetypes
    /// like MIDI will not support this, but MusicXML does
    struct FileSaveParams
    {
        FileSaveParams() = delete;
        
        FileSaveParams(Score& inScore)
        : score(inScore)
        {
        }
        
        Score& score;
        std::map<std::string, std::string, StringViewComparator> properties;
    };

    class FileMediatorIntf
    {
    public:
        virtual ~FileMediatorIntf() {}
        
        virtual ScoreUPtr loadFile(std::string_view inPath) = 0;
        virtual ScoreUPtr loadFile(std::string_view inPath, const InstrumentConfig& inConfig) = 0;
        virtual ScoreUPtr loadStream(std::istream& inDataStream) = 0;
        virtual ScoreUPtr loadStream(std::istream& inDataStream, const InstrumentConfig& inConfig) = 0;
        virtual FileSaveResult saveFile(const FileSaveParams& inParams, std::string_view filePath) = 0;
        virtual FileSaveResult saveStream(const FileSaveParams& inParams, std::ostream& outDataStream) = 0;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
