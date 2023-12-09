// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxNoteReader.h"

// System
#include <cmath>

// Local
#include "Clef.h"
#include "FontTheme.h"
#include "Mark.h"
#include "Measure.h"
#include "MxConverter.h"
#include "MxDynamicsReader.h"
#include "Note.h"
#include "NoteheadType.h"
#include "BeamFactory.h"
#include "Pitch.h"
#include "Rest.h"
#include "Score.h"
#include "TemporalValue.h"
#include "Track.h"
#include "Unused.h"


namespace komp
{
MxNoteReader::MxNoteReader(MxLocation inLocation,
                           mx::api::NoteData inNoteData,
                           MxScoreReader& mxScore)
: mLocation{inLocation}
, mNoteData{inNoteData}
, mMxScoreReader{mxScore}
{
    KOMP_ASSERT(inLocation.mxTickPosition == inNoteData.tickTimePosition);
}

const MxLocation&
MxNoteReader::location() const
{
    return mLocation;
}

const mx::api::NoteData&
MxNoteReader::noteData() const
{
    return mNoteData;
}

int
MxNoteReader::slot(Score& inScore) const
{
    auto& measure = inScore.getMeasure(mLocation.getKompMeasureIndex());
    auto& track = inScore.getTracks().at(mLocation.partIndex);
    const MusicLocation location(*measure.get(),
                                 *track.get(),
                                 mLocation.handIndex,
                                 mLocation.voiceIndex,
                                 6,
                                 mLocation.getKompTickPosition());
    const auto clef = location.getClef();
    Pitch pitch {
        MxConverter::convertPitchStep(mNoteData.pitchData.step),
        mNoteData.pitchData.octave,
        mNoteData.pitchData.alter};
    
    return pitch.slot(clef);
}

int
MxNoteReader::midiPitch() const
{
    Pitch pitch{
        MxConverter::convertPitchStep(mNoteData.pitchData.step),
        mNoteData.pitchData.octave,
        mNoteData.pitchData.alter};
    
    int pitchVal = pitch.getMidiNote();
    
    if (pitchVal < 0) {
        pitchVal = 0;
    }
    
    if (pitchVal > 127) {
        pitchVal = 127;
    }
    
    return pitchVal;
}

int
MxNoteReader::temporalValue() const
{
    return TemporalValue::getValue(kompTemporalType());
}


Pitch
MxNoteReader::getPitch() const
{
    const auto& pitchData = noteData().pitchData;
    auto theStep = MxConverter::convertPitchStep(pitchData.step);
    return Pitch{theStep, pitchData.octave, pitchData.alter};
}

bool
MxNoteReader::isGrace() const
{
    return mNoteData.noteType == mx::api::NoteType::grace;
}

TemporalType
MxNoteReader::kompTemporalType() const
{
    return MxConverter::convertTemporalType(mNoteData.durationData.durationName);
}

NoteheadType
MxNoteReader::kompNoteheadType() const
{
    return MxConverter::convertNoteheadType(mNoteData.notehead);
}

StemDirection
MxNoteReader::kompStemDirection() const
{
    return MxConverter::convertStemDirection(mNoteData.stem);
}

void
MxNoteReader::importNote(komp::Score& outScore,
                         bool& outIsBeamActive,
                         ChordInfo& outCurrentChord,
                         bool& outIsTupletOccurring) const
{
    auto& outMessages = mMxScoreReader.getFileMessages();
    const auto& loc = location();
    const int measureIndex = loc.getKompMeasureIndex();
    KOMP_ASSERT(measureIndex >= 0);
    KOMP_ASSERT(measureIndex < static_cast<int>(outScore.getMeasureCount()));
    const auto& measure = outScore.getMeasure(measureIndex);
    
    auto& reg = outScore.getItemRegistry();
    
    // Use dynamic casting and branching to set notes and rests below
    Temporal* temporal = nullptr;
    Rest* rest = nullptr;
    Note* note = nullptr;
    
    const auto tickPosition = loc.getKompTickPosition();
    const auto isRest = mNoteData.isRest;
    auto& track = outScore.getTracks().at(loc.partIndex);
    auto& measureRef = *measure.get();
    auto& trackRef = *track.get();
    
    const MusicLocation mloc(measureRef,
                             trackRef,
                             loc.handIndex,
                             loc.voiceIndex,
                             slot(outScore));
    
    if (isRest) {
        const auto temporalType = mNoteData.isMeasureRest
        ? TemporalType::WHOLE : kompTemporalType();
        rest = &reg.createItem<Rest>(mloc, temporalType);
        temporal = dynamic_cast<Temporal*>(rest);
    } else {
        note = &reg.createItem<Note>(mloc,
                                     kompTemporalType(),
                                     kompNoteheadType(),
                                     getPitch(),
                                     isGrace());
        temporal = dynamic_cast<Temporal*>(note);
    }
    
    const auto dots = mNoteData.durationData.durationDots;
    temporal->setDuration(temporal->getDuration().setDots(dots));
    
    KOMP_ASSERT(temporal != nullptr);
    
    if (note) {
        if (noteData().pitchData.accidental != mx::api::Accidental::none) {
            const auto acc = noteData().pitchData.accidental;
            const auto kompAccidental = MxConverter::convertAccidental(acc);
            if (kompAccidental == komp::AccidentalType::NONE) {
                KOMP_FILE_ERROR("unsupported accidental type");
            } else {
                note->setAccidental(kompAccidental);
            }
        }
        
        const auto isRaw = hasMiscField("raw");
        
        if (isRaw) {
            note->setIsRaw(true);
        } else {
            const auto isStemFixed = hasMiscField("stem-direction-locked");
            StemPolicy stemPolicy;
            stemPolicy.policy = isStemFixed ? Policy::SPECIFIED : Policy::AUTOMATIC;
            stemPolicy.direction = StemDirection::UNSPECIFIED;
            
            // TODO: How do we deal with multiple voices and stem direction?
            // ----->>> Take a look at MxScoreReader::calculateStemDirections()
            const auto& data = noteData();
            if (data.stem == mx::api::Stem::up) {
                stemPolicy.direction = StemDirection::UP;
            } else if (data.stem == mx::api::Stem::down) {
                stemPolicy.direction = StemDirection::DOWN;
            }
            note->setStemPolicy(stemPolicy);
        }
    }
    
    // Add the temporal before adding marks and dynamics as they may need a valid measure
    temporal->setIsQuiescent(false);
    measure->insertItem(*temporal, tickPosition);
    
    const auto& data = noteData();
    
    if (note) {
        if (data.isChord) {
            if (!outCurrentChord.isChordInProgress) {
                allocateChord(outCurrentChord);
                outCurrentChord.notes.push_back(*note);
            } else if (outCurrentChord.mxLocation != mLocation) {
                deallocateChord(outCurrentChord, *measure.get());
                allocateChord(outCurrentChord);
                outCurrentChord.notes.push_back(*note);
            } else {
                outCurrentChord.notes.push_back(*note);
            }
        } else {
            if (outCurrentChord.isChordInProgress) {
                deallocateChord(outCurrentChord, *measure.get());
            }
        }
        
        importBeams(*note, outIsBeamActive);
    }
    
    for (const auto& mxMark : data.noteAttachmentData.marks) {
        if (mx::api::isMarkDynamic(mxMark.markType)) {
            MxDynamicsReader dynamicsReader{mMxScoreReader, mxMark, mLocation};
            if (temporal->is<Temporal>()) {
                dynamicsReader.insertDynamic(*temporal);
            } else {
                KOMP_FILE_WARN("Unable to find temporal to add dynamic.");
            }
        } else {
            auto kompMark = MarkType::NONE;
            const auto& smuflMarkMap = FontTheme::instance().SmuflMarkMap;
            
            switch (mxMark.markType)
            {
            case mx::api::MarkType::otherArticulation:
            case mx::api::MarkType::otherOrnament:
            case mx::api::MarkType::otherTechnical:
                {
                    const auto smufl = mxMark.name;
                    const auto iter = smuflMarkMap.find(smufl);
                    if (iter != smuflMarkMap.cend()) {
                        kompMark = iter->second;
                    }
                    break;
                }
                
            case mx::api::MarkType::arpeggiate:
            case mx::api::MarkType::arpeggiateUp:
            case mx::api::MarkType::arpeggiateDown:
            case mx::api::MarkType::nonArpeggiate:
                continue;
                
            default:
                {
                    kompMark = MxConverter::convertMark(mxMark.markType);
                    break;
                }
            }
            
            auto placement = PlacementType::AUTOMATIC;
            if (mxMark.positionData.placement == mx::api::Placement::above) {
                placement = PlacementType::ABOVE_ITEM;
            } else if (mxMark.positionData.placement == mx::api::Placement::below) {
                placement = PlacementType::BELOW_ITEM;
            }
            
            if (kompMark == MarkType::NONE) {
                KOMP_FILE_WARN("Mark import failed");
                continue;
            } else {
                temporal->getAttachments().addMark(reg,
                                                   *temporal,
                                                   kompMark,
                                                   placement);
            }
            
            Mark& markPtr = temporal->getAttachments().getMarks().back();
            
            if (mxMark.positionData.isDefaultXSpecified && mMxScoreReader.getIsScalingFound()) {
                const auto spaceSize = markPtr.getSpaceSize();
                const auto defaultX = mxMark.positionData.defaultX;
                const auto xSpaces = defaultX / 10;
                const auto xPixels = xSpaces * spaceSize;
                markPtr.getRelativePoint().setX(xPixels);
            }
            
            if (mxMark.positionData.isDefaultYSpecified && mMxScoreReader.getIsScalingFound()) {
                const auto spaceSize = markPtr.getSpaceSize();
                const auto staffY = markPtr.getLocation().getStaff().getScoreY();
                const auto defaultY = mxMark.positionData.defaultY;
                const auto ySpaces = defaultY / 10;
                const auto yPixels = ySpaces * spaceSize * -1;
                const auto markY = staffY + yPixels;
                markPtr.getRelativePoint().setScoreY(markY);
            }
        }
    }
    
    auto& mxSpannerCollector = mMxScoreReader.getSpannerCollector();
    
    // TODO: refactor curves and ties
    // TODO: support reading curve bezier details
    for (const auto& start : data.noteAttachmentData.curveStarts) {
        const bool isSlur = start.curveType == mx::api::CurveType::slur;
        const bool isTie = start.curveType == mx::api::CurveType::tie;
        auto mxSpannerPtr = std::make_unique<MxNoteSpanner>(start.numberLevel,
                                                            *temporal);
        
        auto placement = PlacementType::AUTOMATIC;
        
        if (start.curvePoints.positionData.placement == mx::api::Placement::above) {
            placement = PlacementType::ABOVE_ITEM;
        } else if (start.curvePoints.positionData.placement == mx::api::Placement::below) {
            placement = PlacementType::BELOW_ITEM;
        } else {
            if (start.curveOrientation == mx::api::CurveOrientation::overhand) {
                placement = PlacementType::ABOVE_ITEM;
            } else if (start.curveOrientation == mx::api::CurveOrientation::underhand) {
                placement = PlacementType::BELOW_ITEM;
            }
        }
        
        mxSpannerPtr->setPlacementType(placement);
        
        if (isSlur && !mxSpannerCollector.slurMgr.push(start.numberLevel,
                                                       std::move(mxSpannerPtr))) {
            KOMP_FILE_WARN( "invalid slur start" );
        }
        
        if (isTie) {
            mxSpannerCollector.ties.startTie(mNoteData, *temporal);
        }
    }
    
    for (const auto& stop : data.noteAttachmentData.curveStops) {
        const auto isSlur = stop.curveType == mx::api::CurveType::slur;
        const auto isTie = stop.curveType == mx::api::CurveType::tie;
        
        if (isSlur) {
            if (!mxSpannerCollector.slurMgr.completeSpanner(stop.numberLevel,
                                                            *temporal)) {
                KOMP_FILE_WARN( "invalid slur stop" );
            }
        } else if (isTie) {
            mxSpannerCollector.ties.finishTie(mNoteData, *temporal);
        }
    }
    
    for (const auto& start : data.noteAttachmentData.tupletStarts) {
        mxSpannerCollector.tuplets.startTuplet(start, mNoteData, *temporal);
        outIsTupletOccurring = true;
    }
    
    if (outIsTupletOccurring) {
        mxSpannerCollector.tuplets.addToCurrentTuplet(*temporal);
    }
    
    for (const auto& stop : data.noteAttachmentData.tupletStops) {
        mxSpannerCollector.tuplets.stopTuplet(stop, *temporal);
        outIsTupletOccurring = false;
    }
    
    if (note) {
        note->deducePitch();
        note->placeMarks();
    }
    
    mMxScoreReader.noteMxNoteMap.emplace(temporal, mNoteData);
    
} // ths is the end of the massive crappy function MxNoteReader::importNote


void
MxNoteReader::importBeams(Note& inNote, bool& outIsBeamActive) const
{
    const auto& data = noteData();
    if (data.beams.empty()) {
        return;
    }
    
    const auto& beamZero = data.beams.at(0);
    const bool isBeamStart = (beamZero == mx::api::Beam::begin);
    const bool isBeamStop = (beamZero == mx::api::Beam::end);
    
    if (isBeamStart) {
        if (outIsBeamActive) {
            LOG_F(ERROR, "illogical beam start detected");
        }
        startBeam(outIsBeamActive);
    }
    
    BeamFactory beamFactory;
    auto beamedNote = beamFactory.createBeamedNote(inNote);
    int b = 0;
    int mx = static_cast<int>(data.beams.size());
    int k = static_cast<int>(beamedNote->getCount());
    for ( ; (b < mx && b < k); ++b) {
        auto mxChunk = data.beams.at(static_cast<size_t>(b));
        
        beamedNote->setIsBroken(b, false);
        if (mxChunk == mx::api::Beam::forwardBroken ||
            mxChunk == mx::api::Beam::end) {
            beamedNote->setIsBroken(b, true);
        }
        
        beamedNote->setBeamChunk(b, MxConverter::convertBeamChunk(mxChunk));
    }
    
    auto& beamMgr = mMxScoreReader.getSpannerCollector().beamMgr;
    if (beamMgr.getIsSpannerStackEmpty()) {
        LOG_F(ERROR, "illogical beaming in MusicXML file detected");
        outIsBeamActive = false;
        return;
    }
    
    auto& beamMgrPayload = *beamMgr.peek();
    auto& beamSpec = beamMgrPayload.getStart();
    parseBeamData(*beamedNote);
    if (!beamSpec->addBeamedNote(*beamedNote)) {
        LOG_F(ERROR, "beamSpec->addBeamedNote(*beamedNote) returned false");
        return;
    }
    
    if (isBeamStart) {
        beamMgrPayload.setStemPolicy(inNote.getStemPolicy());
    }
    
    if (isBeamStop) {
        if (!outIsBeamActive) {
            LOG_F(ERROR, "illogical beam stop detected");
            return;
        }
        stopBeam(outIsBeamActive);
    }
}

void
MxNoteReader::startBeam(bool& outIsBeamActive) const
{
    if (outIsBeamActive) {
        auto& outMessages = mMxScoreReader.getFileMessages();
        KOMP_FILE_ERROR("illogical beam start detected");
        return;
    }
    
    outIsBeamActive = true;
    auto& mxSpannerCollector = mMxScoreReader.getSpannerCollector();
    auto& beamMgr = mxSpannerCollector.beamMgr;
    BeamFactory beamFactory;
    auto beamSpec = beamFactory.createBeamSpec();
    auto beamSpanner = std::make_unique<MxBeamSpanner>(std::move(beamSpec));
    
    if (!beamMgr.push(std::move(beamSpanner))) {
        auto& outMessages = mMxScoreReader.getFileMessages();
        KOMP_FILE_ERROR("illogical beam start detected");
    }
}

void
MxNoteReader::parseBeamData(BeamedNoteIntf& outBeamedNote) const
{
    if (outBeamedNote.getCount() <= 0) {
        return;
    }
    outBeamedNote.setIsBroken(0, false);
    
    for (int i = 1; i < outBeamedNote.getCount() && i < static_cast<int>(mNoteData.beams.size()); ++i) {
        const auto mxBeamInfo = mNoteData.beams.at(i);
        switch (mxBeamInfo) {
            case mx::api::Beam::begin: {
                outBeamedNote.setIsBroken(i, false);
                break;
            }
            case mx::api::Beam::end: {
                outBeamedNote.setIsBroken(i, true);
                break;
            }
            case mx::api::Beam::forwardBroken: {
                outBeamedNote.setIsBroken(i, true);
                break;
            }
            case mx::api::Beam::backwardBroken: {
                outBeamedNote.setIsBroken(i, false);
                break;
            }
            case mx::api::Beam::unspecified: {
                outBeamedNote.setIsBroken(i, false);
                break;
            }
            default:
                break;
        }
    }
}

void
MxNoteReader::stopBeam(bool& outIsBeamActive) const
{
    if (!outIsBeamActive) {
        auto& outMessages = mMxScoreReader.getFileMessages();
        KOMP_FILE_ERROR("illogical beam end detected");
        return;
    }
    
    outIsBeamActive = false;
    auto& mxSpannerCollector = mMxScoreReader.getSpannerCollector();
    auto& beamMgr = mxSpannerCollector.beamMgr;
    auto& beamSpec = beamMgr.peek()->getStart();
    
    // completeSpanner should really be a void function here.
    // we are only calling beamSpec->clone() to satisfy the signature
    // of this function but the value being passed to completeSpanner
    // is completely ignored and discarded in the case of beams.
    // TODO: find a way to do this without being obtuse
    auto clone = beamSpec->clone();
    if (!beamMgr.completeSpanner(clone)) {
        auto& outMessages = mMxScoreReader.getFileMessages();
        KOMP_FILE_ERROR("illogical beam end detected");
    }
}


void
MxNoteReader::allocateChord(ChordInfo& outChord) const
{
    auto arpeggiate = mx::api::MarkType::unspecified;
    for (const auto& mxMark : mNoteData.noteAttachmentData.marks) {
        if (mx::api::isMarkArpeggiate(mxMark.markType)
         || mx::api::isMarkNonArpeggiate(mxMark.markType)) {
            arpeggiate = mxMark.markType;
            break;
        }
    }

    outChord.notes.clear();
    outChord.mxLocation = mLocation;
    outChord.arpeggiate = arpeggiate;
    outChord.isChordInProgress = true;
}


void
MxNoteReader::deallocateChord(ChordInfo& outChord,
                              Measure& inMeasure) const
{
    if (outChord.notes.size() < 2) {
        LOG_F(WARNING, "a chord with less than 2 notes was encountered and no chord object was created");
        outChord = ChordInfo();
        outChord.isChordInProgress = false;
        return;
    }
    
    auto& score = mMxScoreReader.getScore();
    const auto& measure = score.getMeasure(mLocation.getKompMeasureIndex());
    const auto& track = score.getTracks().at(mLocation.partIndex);
    const MusicLocation loc(*measure.get(),
                            *track.get(),
                            mLocation.handIndex,
                            mLocation.voiceIndex);
    auto& reg = score.getItemRegistry();
    auto& chord = reg.createItem<Chord>(loc, outChord.notes);
    
    const auto arpeggiate = komp::MxConverter::convertArpeggiate(outChord.arpeggiate);
    chord.setArpeggiate(arpeggiate);

    chord.setIsQuiescent(false);
    inMeasure.insertItem(chord);
    mMxScoreReader.noteMxNoteMap.emplace(&chord, mNoteData);
    outChord = ChordInfo();
    outChord.isChordInProgress = false;
}


bool
MxNoteReader::hasMiscField(std::string miscField) const
{
    return std::find(mNoteData.miscData.cbegin(),
                     mNoteData.miscData.cend(),
                     miscField) != mNoteData.miscData.cend();
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
