// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>

// Komp
#include "BarlineType.h"
#include "Forward.h"
#include "KeySignatureType.h"
#include "MxMark.h"
#include "Octave.h"
#include "SoundID.h"
#include "StemDirection.h"

// Mx
#include "mx/api/ScoreData.h"

namespace komp
{
enum class AccidentalType;
enum class ArpeggiateType;
enum class ClefSymbolType;
enum class ClefType;
enum class PitchStep;
enum class TemporalType;
enum class TimeSignatureType;
enum class BeamChunk;

using MxClefSymbolMap = std::map<const ClefSymbolType, const mx::api::ClefSymbol>;
using MxAccidental = std::map<const komp::AccidentalType, const mx::api::Accidental>;
using MxNoteheadTypeMap = std::map<const komp::NoteheadType, const mx::api::Notehead>;
using MxPitchStepMap = std::map<const komp::PitchStep, const mx::api::Step>;
using MxTemporalTypeMap = std::map<const TemporalType, const mx::api::DurationName>;
using MxTimeSignatureTypeMap = std::map<const komp::TimeSignatureType, const mx::api::TimeSignatureSymbol>;
using MxOttavaMap = std::map<const komp::OctaveType, const mx::api::OttavaType>;
using MxBeamChunkMap = std::map<const komp::BeamChunk, const mx::api::Beam>;
using MxSoundMap = std::map<const komp::SoundID, const mx::api::SoundID>;
using MxLeftBarlineMap = std::map<const komp::BarlineType, const mx::api::BarlineType>;
using MxRightBarlineMap = std::map<const komp::BarlineType, const mx::api::BarlineType>;

struct MxConverterParams
{
    MxConverterParams();
    
    /// sets the values to match the the inScore
    MxConverterParams(Score& inScore);
    
    long double tenths;
    long double millimeters;
    long double pixelsPerInch;
};

class MxConverter
{
public:
    MxConverter(MxConverterParams params);
    
    static constexpr const long double MILLIMETERS_PER_INCH = 25.4;
    
    long double convertTenthsToPixels(long double mxTenths) const;
    long double convertPixelsToTenths(long double pixels) const;
    int convertTenthsToRoundedPixels(long double mxTenths) const;
    int convertPixelsToRoundedTenths(long double pixels) const;
    
    long double getTenthsPerMillimeter() const;
    long double getTenthsPerInch() const;
    long double getPixelsPerTenth() const;
    long double getPixelsPerInch() const;
    
    int convertDefaultYToSlot(double inDefaultY) const;
    double convertSlotToDefaultY(int inSlot) const;
    
    static const MxMarkSet& getMarks();
    static komp::MarkType convertMark(mx::api::MarkType value, mx::api::Placement placement);
    static mx::api::MarkType convertMark(komp::MarkType value, mx::api::Placement placement);
    static komp::MarkType convertMark(mx::api::MarkType value);
    static mx::api::MarkType convertMark(komp::MarkType value);
    
    static komp::ArpeggiateType convertArpeggiate(mx::api::MarkType value);

    static const MxClefSymbolMap& getClefs();
    static ClefSymbolType convertClefSymbol(mx::api::ClefSymbol value);
    static mx::api::ClefSymbol convertClefSymbol(ClefSymbolType value);
    static komp::ClefType convertClefType(const mx::api::ClefData& clefData);
    
    static const MxAccidental& getAccidentals();
    static const komp::AccidentalType convertAccidental(mx::api::Accidental value);
    static const mx::api::Accidental convertAccidental(komp::AccidentalType value);
    
    static const MxPitchStepMap& getPitches();
    static const PitchStep convertPitchStep(mx::api::Step value);
    static const mx::api::Step convertPitchStep(PitchStep value);
    
    static const MxTemporalTypeMap& getTemporalTypes();
    static const TemporalType convertTemporalType(mx::api::DurationName value);
    static const mx::api::DurationName convertTemporalType(TemporalType value);
    
    static const MxNoteheadTypeMap& getNoteheadTypes();
    static const NoteheadType convertNoteheadType(mx::api::Notehead value);
    static const mx::api::Notehead convertNoteheadType(NoteheadType value);
    
    static const StemDirection convertStemDirection(mx::api::Stem value);
    static const mx::api::Stem convertStemDirection(StemDirection value);
    
    static const MxTimeSignatureTypeMap& getTimeSignatureTypes();
    static const TimeSignatureType convertTimeSignatureType(mx::api::TimeSignatureSymbol value);
    static const mx::api::TimeSignatureSymbol convertTimeSignatureType(TimeSignatureType value);
    
    static KeySignatureValue convertKey(const mx::api::KeyData& inKey);
    static mx::api::KeyData convertKey(const KeySignatureValue& inKey);
    
    static const MxOttavaMap& getOttavas();
    static const OctaveType convertOttava(mx::api::OttavaType value);
    static const mx::api::OttavaType convertOttava(OctaveType value);
    
    static const MxBeamChunkMap& getBeamChunkMap();
    static const BeamChunk convertBeamChunk(mx::api::Beam value);
    static const mx::api::Beam convertBeamChunk(BeamChunk value);
    
    static const MxSoundMap& getSoundMap();
    static const komp::SoundID convertSoundID(mx::api::SoundID value);
    static const mx::api::SoundID convertSoundID(komp::SoundID value);
    
    static const MxLeftBarlineMap& getLeftBarlineMap();
    static const komp::BarlineType convertLeftBarline(mx::api::BarlineType inValue);
    static const mx::api::BarlineType convertLeftBarline(komp::BarlineType inValue);
    
    static const MxRightBarlineMap& getRightBarlineMap();
    static const komp::BarlineType convertRightBarline(mx::api::BarlineType inValue);
    static const mx::api::BarlineType convertRightBarline(komp::BarlineType inValue);
    
private:
    const long double mMxMillimeters;
    const long double mMxTenths;
    const long double mPixelsPerInch;
    
private:
    static const MxMarkSet sMarks;
    static const MxClefSymbolMap sClefs;
    static const MxAccidental sAccidentals;
    static const MxPitchStepMap sPitches;
    static const MxTemporalTypeMap sTemporalTypes;
    static const MxNoteheadTypeMap sNoteheadTypes;
    static const MxTimeSignatureTypeMap sTimeSignatureTypes;
    static const MxOttavaMap sOttavaMap;
    static const MxBeamChunkMap sBeamChunkMap;
    static const MxSoundMap sSoundMap;
    static const MxLeftBarlineMap sLeftBarlineMap;
    static const MxRightBarlineMap sRightBarlineMap;
};
}

