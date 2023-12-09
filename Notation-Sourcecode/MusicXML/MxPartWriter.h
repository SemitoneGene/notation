// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "MxLocation.h"
#include "MxScoreWriter.h"
#include "Throw.h"

// Mx
#include "mx/api/PartData.h"

namespace komp
{
    class MxPartWriter
    {
    public:
        MxPartWriter(MxScoreWriter& mxScoreWriter, MxLocation inMxLocation);
        
        mx::api::PartData getPartData();
        
    private:
        MxScoreWriter& mMxScoreWriter;
        MxLocation mMxLocation;
        mx::api::PartData mOutPartData;
        
    private:
        void writePartHeader();
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
