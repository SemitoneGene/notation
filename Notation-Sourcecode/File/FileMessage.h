// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp
#include "Throw.h"

// System
#include <list>
#include <ostream>
#include <sstream>

namespace komp
{
    class FileMessage;
    using FileMessages = std::list<FileMessage>;

    enum class FileMessageSeverity
    {
        Fatal,
        Error,
        Warning,
        Informational
    };
    
    // comment out the next line to stop logging file messages to the console
#define KOMP_LOG_FILE_MESSAGES
#ifdef KOMP_LOG_FILE_MESSAGES
    #define KOMP_FILE_LOG LOG_F(WARNING, "%s",outMessages.back().toString().c_str());
#else
    #define KOMP_FILE_LOG
#endif

    // convenience macros for adding messages to a FileMessages list 'outMessages'
    #define KOMP_FILE_FATAL(strMessage) outMessages.emplace_back(strMessage,FileMessageSeverity::Fatal,std::string{__FILENAME__},__LINE__); KOMP_FILE_LOG;
    #define KOMP_FILE_ERROR(strMessage) outMessages.emplace_back(strMessage,FileMessageSeverity::Error,std::string{__FILENAME__},__LINE__); KOMP_FILE_LOG;
    #define KOMP_FILE_WARN(strMessage) outMessages.emplace_back(strMessage,FileMessageSeverity::Warning,std::string{__FILENAME__},__LINE__); KOMP_FILE_LOG;
    #define KOMP_FILE_INFO(strMessage) outMessages.emplace_back(strMessage,FileMessageSeverity::Informational,std::string{__FILENAME__},__LINE__); KOMP_FILE_LOG;
    
    inline std::string fileMessageSeverityStr(FileMessageSeverity severity)
    {
        switch(severity) {
            case FileMessageSeverity::Fatal: return "Fatal";
            case FileMessageSeverity::Error: return "Error";
            case FileMessageSeverity::Warning: return "Warning";
            case FileMessageSeverity::Informational: return "Informational";
            default: throw std::runtime_error("unknown severity value");
        }
    }


    inline int fileMessageSeverityInt(FileMessageSeverity severity) {
        switch(severity) {
            case FileMessageSeverity::Fatal: return 0;
            case FileMessageSeverity::Error: return 1;
            case FileMessageSeverity::Warning: return 2;
            case FileMessageSeverity::Informational: return 3;
            default: throw std::runtime_error( "unknown severity value");
        }
    }

    struct FileMessage
    {
        std::string message;
        FileMessageSeverity severity;
        std::string sourcecodeFile;
        int sourcecodeLine;

        FileMessage()
        : message{}
        , severity{FileMessageSeverity::Informational}
        , sourcecodeFile{}
        , sourcecodeLine{-1}
        {

        }

        FileMessage(std::string inMessage,
                    FileMessageSeverity inSeverity,
                    std::string inSourcecodeFile,
                    int inSourcecodeLine)
        : message{inMessage}
        , severity{inSeverity}
        , sourcecodeFile{inSourcecodeFile}
        , sourcecodeLine{inSourcecodeLine}
        {

        }

        inline void toStream(std::ostream& os) const
        {
            os << "FileMessage: (" << fileMessageSeverityStr(severity) << ") "
               << message << " - "
               << sourcecodeFile << "("
               << sourcecodeLine << ")";
        }

        inline std::string toString() const
        {
            std::stringstream ss;
            toStream(ss);
            return ss.str();
        }
    };
    
    inline std::ostream& operator<<(std::ostream& os, const FileMessage& fileMessage)
    {
        fileMessage.toStream( os );
        return os;
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
