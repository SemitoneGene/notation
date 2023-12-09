// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "FileMessage.h"
#include "FileMediatorIntf.h"
#include "MxLocation.h"
#include "MxConverter.h"
#include "MxCurveStack.h"
#include "StringViewComparator.h"

// Mx
#include "mx/api/ScoreData.h"


namespace komp
{
    class Score;
    
    class MxScoreWriter
    {
    public:
        MxScoreWriter(const FileSaveParams& inParams,
                      FileMessages& outFileMessages);

        // main routine - parses the komp score and writes its data to an
        // mx::api::ScoreData object which is then returned. During the
        // execution is will write messages to outFileMessages
        mx::api::ScoreData getScoreData();

        Score& getScore();
        FileMessages& getFileMessages();
        std::list<WedgeRef>& getWedges();
        std::list<EndingRef>& getEndings();

        MxCurveStack& getTieStack();
        MxCurveStack& getSlurStack();

        /// i.e. musicxml <millimeters>xx</millimeters>
        double getMillimeters() const;

        /// i.e. musicxml <tenths>xx</tenths>
        double getTenths() const;

        double getSpaceSize() const;

        double measureRelativeXToMeasureDefaultX(double inMeasureRelativeX,
                                                 const Measure& inMeasure) const;

        double unscaledScoreYToDefaultY(double inUnscaledScoreY,
                                        const Staff& inStaff) const;

    private:
        Score& mScore;
        FileMessages& mOutFileMessages;
        std::unique_ptr<MxLocation> mMxLocationPtr;
        std::list<WedgeRef> mWedges;
        std::list<EndingRef> mEndings;

    private:
        void initializeMxLocation();
        MxConverterParams setEncodingInfo(mx::api::ScoreData& outScore);
        MxCurveStack mTieStack;
        MxCurveStack mSlurStack;
        double mMillimeters; // musicxml
        double mTenths;      // musicxml
        double mStaffSpaceSizePixels;
        std::map<std::string, std::string, StringViewComparator> mProperties;
        void addClientProperties(mx::api::ScoreData& ioScoreData);
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
