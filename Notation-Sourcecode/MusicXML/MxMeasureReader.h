// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "MxScoreReader.h"
#include "MxLocation.h"
#include "Temporal.h"

// Mx
#include "mx/api/ScoreData.h"

namespace komp
{
    class MxMeasureReader
    {
    public:
        
        MxMeasureReader(MxScoreReader& mxScore);
        
        void importMeasures();

    private:
        MxScoreReader& mMxScoreReader;
        MxLocation mCurrentLocation;

    private:
        
        void importClefs(const std::vector<mx::api::ClefData>& clefs);
        void importPartData(const mx::api::PartData& inPartData);
        void importMeasureData(const mx::api::MeasureData& inMeasureData, bool& outIsBeamActive);
        void importStaffData(const mx::api::StaffData& inStaffData, bool& outIsBeamActive);
        void importVoiceData(const mx::api::VoiceData& inVoiceData, bool& outIsBeamActive);
        void importNotes(const std::vector<mx::api::NoteData>& inNotes, bool& outIsBeamActive);
        void importMeter(const mx::api::MeasureData& inMeasureData);
        void importMetronomes(const mx::api::TempoData& inTempoData);
        void importKeys(const mx::api::MeasureData& inMeasureData);
        void importBarlines(const mx::api::MeasureData& inMeasureData);
        void importDirections(const mx::api::DirectionData& inDirectionData, const std::map<int, mx::api::VoiceData>& inVoices);
        void importDirectionMark(const mx::api::MarkData& mark, int inKompTickPosition, mx::api::Placement inMxPlacement);
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
