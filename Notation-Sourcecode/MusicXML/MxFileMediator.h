// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMediatorIntf.h"

// System
#include <memory>

namespace komp
{
    class MxFileMediator : public FileMediatorIntf
    {
    public:
        komp::ScoreUPtr loadFile(std::string_view inPath) override;
        komp::ScoreUPtr loadFile(std::string_view inPath, const InstrumentConfig& inConfig) override;
        komp::ScoreUPtr loadStream(std::istream& inDataStream) override;
        komp::ScoreUPtr loadStream(std::istream& inDataStream, const InstrumentConfig& inConfig) override;
        FileSaveResult saveFile(const FileSaveParams& inParams, std::string_view filePath) override;
        FileSaveResult saveStream(const FileSaveParams& inParams, std::ostream& outDataStream) override;

    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
