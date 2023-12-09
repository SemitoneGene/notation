// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MxScoreWriter.h"

// Mx
#include "mx/api/NoteData.h"

namespace komp
{
    class Note;
    class Rest;
    class Temporal;
    
    class MxNoteWriter
    {
    public:
        MxNoteWriter(Temporal& inTemporal,
                     MxLocation inMxLocation,
                     MxScoreWriter& inMxScoreWriter,
                     bool isPartOfChord,
                     std::set<Chord*>& inProcessedChords);
        
        mx::api::NoteData getNoteData(int& tupletNumberLevel,
                                      mx::api::StaffData& outStaffData);
        
    private:
        Temporal& mTemporal;
        MxLocation mMxLocation;
        MxScoreWriter& mMxScoreWriter;
        mx::api::NoteData mOutNoteData;
        bool mIsPartOfChord;        
        std::set<Chord*>& mProcessedChords;
        
    private:
        void writeRestData(const Rest& rest);
        void writeNoteData(const Note& note);
        std::vector<mx::api::Beam> parseBeam(const Temporal& temporal);
        mx::api::PositionData createPositionData(const Mark& inMark,
                                                 mx::api::Placement inPlacement);
        void handleMarks(const Temporal& inTemporal,
                         const MarkRefVec& inMarks,
                         mx::api::StaffData& outStaffData);
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
