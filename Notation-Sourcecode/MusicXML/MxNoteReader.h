// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "FileMessage.h"
#include "Forward.h"
#include "MxLocation.h"
#include "MxScoreReader.h"
#include "Pitch.h"
#include "StemDirection.h"

// Mx
#include "mx/api/NoteData.h"

namespace komp
{
    enum class TemporalType;
    class Score;

    struct ChordInfo
    {
        NoteRefVec notes;
        MxLocation mxLocation{MxLocationParameters{}};
        bool isChordInProgress{false};
        mx::api::MarkType arpeggiate{mx::api::MarkType::unspecified};
        
        ChordInfo()
        {
        }
        
        ChordInfo(const NoteRefVec& inNotes, const MxLocation& inMxLocation)
        : notes{inNotes}
        , mxLocation{inMxLocation}
        {

        }
    };

    class MxNoteReader
    {
    public:
        MxNoteReader(MxLocation inLocation,
                     mx::api::NoteData noteData,
                     MxScoreReader& mxScore);

        const MxLocation& location() const;
        const mx::api::NoteData& noteData() const;

        int slot(Score& score) const;
        int midiPitch() const;
        int temporalValue() const;
        Pitch getPitch() const;
        bool isGrace() const;
        
        TemporalType kompTemporalType() const;
        NoteheadType kompNoteheadType() const;
        StemDirection kompStemDirection() const;

        void importNote(komp::Score& outScore,
                        bool& outIsBeamActive,
                        ChordInfo& outCurrentChord,
                        bool& outIsTupletOccurring) const;
    
    private:
        void importBeams(Note& inNote, bool& outIsBeamActive) const;
        void startBeam(bool& outIsBeamActive) const;
        void parseBeamData(BeamedNoteIntf& outBeamedNote) const;
        void stopBeam(bool& outIsBeamActive) const;
        void allocateChord(ChordInfo& outChord) const;
        void deallocateChord(ChordInfo& outChord, Measure& inMeasure) const;        
        bool hasMiscField(std::string miscField) const;

    private:
        MxLocation mLocation;
        mx::api::NoteData mNoteData;
        MxScoreReader& mMxScoreReader;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
