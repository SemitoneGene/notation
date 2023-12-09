// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxNoteWriter.h"

// Local
#include "BeamIntf.h"
#include "Chord.h"
#include "Clef.h"
#include "FontTheme.h"
#include "Mark.h"
#include "Measure.h"
#include "MxConverter.h"
#include "Note.h"
#include "Pitch.h"
#include "Rest.h"
#include "RoundPlaces.h"
#include "Score.h"
#include "Slur.h"
#include "Temporal.h"
#include "Tie.h"
#include "TupletIntf.h"

namespace komp
{
    MxNoteWriter::MxNoteWriter(Temporal& inTemporal,
                               MxLocation inMxLocation,
                               MxScoreWriter& inMxScoreWriter,
                               bool isPartOfChord,
                               std::set<Chord*>& inProcessedChords)
    : mTemporal(inTemporal)
    , mMxLocation(inMxLocation)
    , mMxScoreWriter(inMxScoreWriter)
    , mOutNoteData()
    , mIsPartOfChord(isPartOfChord)
    , mProcessedChords{inProcessedChords}
    {
        
    }
    
    mx::api::NoteData MxNoteWriter::getNoteData(int& tupletNumberLevel,
                                                mx::api::StaffData& outStaffData)
    {
        mOutNoteData = mx::api::NoteData{};
        
        const auto itemType = mTemporal.getItemTraits().getItemType();
        if (itemType == ItemType::Rest) {
            auto& rest = mTemporal.as<Rest>();
            writeRestData(rest);
        } else if (itemType == ItemType::Note) {
            auto& note = mTemporal.as<Note>();
            writeNoteData(note);
        }
        
        mOutNoteData.tickTimePosition = mTemporal.getTime();
        mOutNoteData.durationData.durationName = MxConverter::convertTemporalType(mTemporal.getDuration().getTemporalType());
        mOutNoteData.durationData.durationTimeTicks = mTemporal.getDuration().getClocks();

        mOutNoteData.positionData.isDefaultXSpecified = true;
        const auto measureRelativeX = mTemporal.getX();
        const auto& measure = mMxScoreWriter.getScore().getMeasure(mMxLocation.getKompMeasureIndex());
        const auto defaultX = mMxScoreWriter.measureRelativeXToMeasureDefaultX(measureRelativeX,
                                                                               *measure.get());
        mOutNoteData.positionData.defaultX = defaultX;

        // TODO: This is a hack to interop with Dorico and Sibelius.  When Komp
        // is modified to support multiple voices, this will no longer work.
        mOutNoteData.userRequestedVoiceNumber = mMxLocation.voiceIndex + 1 + mMxLocation.handIndex;

        mOutNoteData.durationData.durationDots = mTemporal.getDuration().getDots();

        const auto kompDuration = mTemporal.getDuration();
        const auto kompTuplets = mTemporal.getTuplets();
        if (kompDuration.getTupletRatios().size() > 0) {
            if (kompTuplets.size() > 0) {
                const TupletIntf& tuplet = kompTuplets.at(0);
                const auto tupletRatio = tuplet.getTupletRatio();
                const auto actual = tupletRatio.getActual();
                const auto normal = tupletRatio.getNormal();
                const bool isTupletStart = mTemporal.getIsTupletStart();
                const bool isTupletStop = mTemporal.getIsTupletStop();

                if (isTupletStart) {

                    ++tupletNumberLevel;
                    KOMP_ASSERT(tupletNumberLevel >= 1);
                    mx::api::TupletStart start;
                    start.numberLevel = tupletNumberLevel;
                    start.actualNumber = actual.getCount();
                    start.actualDots = actual.getDots();
                    start.actualDurationName = MxConverter::convertTemporalType(actual.getTemporalType());

                    start.normalNumber = normal.getCount();
                    start.normalDots = normal.getDots();
                    start.normalDurationName = MxConverter::convertTemporalType(normal.getTemporalType());
                    mOutNoteData.noteAttachmentData.tupletStarts.emplace_back(start);
                }

                mOutNoteData.durationData.timeModificationActualNotes = actual.getCount();
                mOutNoteData.durationData.timeModificationNormalNotes = normal.getCount();
                mOutNoteData.durationData.timeModificationNormalType = MxConverter::convertTemporalType(normal.getTemporalType());
                mOutNoteData.durationData.timeModificationNormalTypeDots = normal.getDots();

                if (isTupletStop) {
                    KOMP_ASSERT(tupletNumberLevel >= 1);
                    mx::api::TupletStop stop;
                    stop.numberLevel = tupletNumberLevel;
                    mOutNoteData.noteAttachmentData.tupletStops.emplace_back(stop);
                    --tupletNumberLevel;
                }
            }
        }

        handleMarks(mTemporal, mTemporal.getAttachments().getMarks(), outStaffData);
        KOMP_ASSERT(mTemporal.getIsChordMember() == mIsPartOfChord);
        mOutNoteData.isChord = mIsPartOfChord;

        const auto& attachments = mTemporal.getAttachments();

        const auto incomingCurves = attachments.getIncomingCurves();
        const auto outgoingCurves = attachments.getOutgoingCurves();

        std::vector<CurveRef> incomingTies;
        std::vector<CurveRef> outgoingTies;
        std::vector<CurveRef> incomingSlurs;
        std::vector<CurveRef> outgoingSlurs;

        const auto tieFilter = [&](std::vector<CurveRef>::value_type inFilterCurve) {
            return inFilterCurve.get().getItemTraits().getItemType() == ItemType::Tie;
        };

        const auto slurFilter = [&](std::vector<CurveRef>::value_type inFilterCurve) {
            return inFilterCurve.get().getItemTraits().getItemType() == ItemType::Slur;
        };

        std::copy_if(incomingCurves.cbegin(), incomingCurves.cend(), std::back_inserter(incomingTies), tieFilter);
        std::copy_if(outgoingCurves.cbegin(), outgoingCurves.cend(), std::back_inserter(outgoingTies), tieFilter);
        std::copy_if(incomingCurves.cbegin(), incomingCurves.cend(), std::back_inserter(incomingSlurs), slurFilter);
        std::copy_if(outgoingCurves.cbegin(), outgoingCurves.cend(), std::back_inserter(outgoingSlurs), slurFilter);

        if (!outgoingTies.empty()) {
            const auto& outgoingTie = outgoingTies.front();
            const auto numberLevel = mMxScoreWriter.getTieStack().push(outgoingTie);
            mx::api::CurveStart mxCurve{mx::api::CurveType::tie};
            mxCurve.numberLevel = numberLevel;
            mOutNoteData.noteAttachmentData.curveStarts.emplace_back(std::move(mxCurve));
            mOutNoteData.isTieStart = true;
        }

        for (const auto& slur : outgoingSlurs) {
            const auto numberLevel = mMxScoreWriter.getSlurStack().push(slur);
            mx::api::CurveStart mxCurve{mx::api::CurveType::slur};
            mxCurve.numberLevel = numberLevel;
            mOutNoteData.noteAttachmentData.curveStarts.emplace_back(std::move(mxCurve));
        }

        if (!incomingTies.empty()) {
            const auto& incomingTie = incomingTies.front();
            const auto numberLevel = mMxScoreWriter.getTieStack().pop(incomingTie);
            mx::api::CurveStop mxCurve{mx::api::CurveType::tie};
            mxCurve.numberLevel = numberLevel;
            mOutNoteData.noteAttachmentData.curveStops.emplace_back(std::move(mxCurve));
            mOutNoteData.isTieStop = true;
        }

        for (const auto& slur : incomingSlurs) {
            const auto numberLevel = mMxScoreWriter.getSlurStack().pop(slur);
            mx::api::CurveStop mxCurve{mx::api::CurveType::slur};
            mxCurve.numberLevel = numberLevel;
            mOutNoteData.noteAttachmentData.curveStops.emplace_back(std::move(mxCurve));
        }

        mx::api::NoteData temp = std::move(mOutNoteData);
        mOutNoteData = mx::api::NoteData{};
        return temp;
    }
    
    
    void MxNoteWriter::writeRestData(const Rest& rest)
    {
        mOutNoteData.isRest = true;
    }
    
    void MxNoteWriter::writeNoteData(const Note& inNote)
    {
        mOutNoteData.notehead = MxConverter::convertNoteheadType(inNote.getNoteheadType());
        
        const auto pitch = inNote.getPitch();
        mOutNoteData.pitchData.step = MxConverter::convertPitchStep(pitch.step);
        mOutNoteData.pitchData.octave = pitch.octave;
        mOutNoteData.pitchData.alter = pitch.alter;
        mOutNoteData.pitchData.accidental = MxConverter::convertAccidental(inNote.getAccidental());
        auto parentChord = inNote.getChord();
        bool isBottomChordNote = false;

        if (parentChord) {
            const auto& chord = parentChord->as<Chord>();
            isBottomChordNote = chord.getBottomNote().getItemID() == inNote.getItemID();
        }

        if (parentChord && isBottomChordNote) {
            mOutNoteData.beams = parseBeam(*parentChord);
        } else {
            mOutNoteData.beams = parseBeam(inNote);
        }

        if (inNote.getIsRaw()) {
            mOutNoteData.stem = mx::api::Stem::none;
            mOutNoteData.miscData.push_back("raw");
        } else if (inNote.getStemPolicy().getIsStemUp()) {
            mOutNoteData.stem = mx::api::Stem::up;
            if (inNote.getStemPolicy().getIsSpecified()) {
                mOutNoteData.miscData.push_back("stem-direction-locked");
            }
        } else if (inNote.getStemPolicy().getIsStemDown()) {
            mOutNoteData.stem = mx::api::Stem::down;
            if (inNote.getStemPolicy().getIsSpecified()) {
                mOutNoteData.miscData.push_back("stem-direction-locked");
            }
        }
    }
    
    std::vector<mx::api::Beam>
    MxNoteWriter::parseBeam(const Temporal& inTemporal)
    {
        std::vector<mx::api::Beam> beams;
        
        if (!inTemporal.getBeam()) {
            return beams;
        }
        
        const auto& beamPtr = inTemporal.getBeam();
        const auto& beamedNotes = beamPtr->getBeamedNotes();
        BeamedNoteIntf* beamedNote = nullptr;
        
        for (const auto& currentBeamedNote : beamedNotes) {
            if (&currentBeamedNote->getNote() == &inTemporal) {
                beamedNote = currentBeamedNote.get();
                break;
            }
        }
        
        if (!beamedNote) {
            return beams;
        }
        
        const auto& beamChunks = beamedNote->getBeamChunks();
        
        for (const auto& beamChunk : beamChunks) {
            beams.emplace_back(MxConverter::convertBeamChunk(beamChunk));
        }
        
        return beams;
    }



    mx::api::PositionData
    MxNoteWriter::createPositionData(const Mark& inMark, mx::api::Placement inPlacement)
    {
        mx::api::PositionData positionData;
        positionData.placement = inPlacement;
        positionData.isDefaultXSpecified = true;
        positionData.isDefaultYSpecified = true;

        const auto& staff = inMark.getLocation().getStaff();

        // TODO: this will technically be wrong if/when we have staff sizes that differ
        const auto spaceSize = Staff::getSpacing();

        const auto staffY = staff.getScoreY();
        const auto markY = inMark.getScoreY();
        const auto noteX = inMark.getRelativePoint().getItem().getScoreX();
        const auto markX = inMark.getScoreX();
        const auto xOffset = markX - noteX;
        const auto yOffset = markY - staffY;
        const auto xTenths = (xOffset / spaceSize) * 10.0;      // assumes <tenths>40</tenths>
        const auto yTenths = (yOffset / spaceSize) * 10.0 * -1; // assumes <tensth>40</tenths>
        positionData.defaultX = komp::roundPlaces(xTenths, 5);
        positionData.defaultY = komp::roundPlaces(yTenths, 5);
        return positionData;
    }


    void
    MxNoteWriter::handleMarks(const Temporal& inTemporal,
                              const MarkRefVec& inMarks,
                              mx::api::StaffData& outStaffData)
{
        auto marksToWrite = inMarks;
        
        if (mIsPartOfChord) {
            // Grab all of the marks on the chord and move them to the note.
            // We must do this as MusicXML does not have a concept of marks
            // attached to a chord. We attach marks to a chord as it enables
            // our mark layout routines.
            KOMP_ASSERT(inMarks.empty());
            const auto chord = inTemporal.getChord();
            if (!mProcessedChords.contains(chord)) {
                marksToWrite = chord->getAttachments().getMarks();
                mProcessedChords.insert(chord);
            }
            
            const auto arpeggiate = chord->getArpeggiate();
            if (arpeggiate != komp::ArpeggiateType::NONE) {
                
                // Save appregiate as MusicXML 'notations'
                auto mxType = mx::api::MarkType::unspecified;
                
                switch (arpeggiate)
                {
                case komp::ArpeggiateType::STANDARD:
                    mxType = mx::api::MarkType::arpeggiate;
                    break;

                case komp::ArpeggiateType::DOWN:
                    mxType = mx::api::MarkType::arpeggiateDown;
                    break;

                case komp::ArpeggiateType::UP:
                    mxType = mx::api::MarkType::arpeggiateUp;
                    break;

                case komp::ArpeggiateType::NON:
                    mxType = mx::api::MarkType::nonArpeggiate;
                    break;
                    
                default:
                    break;
                }
                    
                auto mxMarkData = mx::api::MarkData{mxType};
                mxMarkData.tickTimePosition = mOutNoteData.tickTimePosition;
                mOutNoteData.noteAttachmentData.marks.push_back(mxMarkData);
            }
        }
        
        if (marksToWrite.size() == 0) {
            return;
        }
        
        for (const Mark& mark : marksToWrite) {
            
            const auto markType = mark.getMarkData().getMarkType();
            const auto isArticulation = mark.getMarkData().is(MarkCategory::ARTICULATION);
            const auto isExpression = mark.getMarkData().is(MarkCategory::EXPRESSION);
            const auto isTechnical = mark.getMarkData().is(MarkCategory::TECHNIQUE);
            
            const auto isPedal = markType == MarkType::PEDAL || markType == MarkType::PEDAL_UP;
            const auto isNiente = markType == MarkType::DYNAMIC_NIENTE;
            const auto isAccentStaccato = (markType == MarkType::ACCENT_STACCATO_ABOVE) || (markType == MarkType::ACCENT_STACCATO_BELOW);
            const auto isMarcatoTenuto = (markType == MarkType::MARCATO_TENUTO_ABOVE) || (markType == MarkType::MARCATO_TENUTO_BELOW);
            const auto isMarcatoStaccato = (markType == MarkType::MARCATO_STACCATO_ABOVE) || (markType == MarkType::MARCATO_STACCATO_BELOW);
            const auto isTenutoAccent = (markType == MarkType::TENUTO_ACCENT_ABOVE) || (markType == MarkType::TENUTO_ACCENT_BELOW);

            const auto isSpecialSmufl = isNiente ||
                       isAccentStaccato ||
                       isMarcatoTenuto ||
                       isMarcatoStaccato ||
                       isTenutoAccent ||
                       (markType == MarkType::BRASS_MUTE_CLOSED);

            // certain marks must be represented as 'directions' instead of 'notations'
            // in MusicXML and they require different handling especially for placement
            const auto isDirection = isPedal;

            // some marks do not have a strongly-typed MusicXML enum, for these we will
            // put a smufl name into the MusicXML 'other' field
            std::string specialSmufl = "";

            // establish a variable to hold the MusicXML mark type. this must be changed
            // to something other than unspecified or else we will throw later in the func
            auto mxType = mx::api::MarkType::unspecified;

            // determine if the mark is above or below the staff
            const auto& staff = mark.getLocation().getStaff();
            const auto markUnscaledScoreY = mark.getScoreY();

            // automagically ascertain if the flippage should be above/below
            auto pivotY = staff.getScoreMiddle();
            auto& markParent = mark.getRelativePoint().getItem();
            if (markParent.is<Temporal>()) {
                auto t = dynamic_cast<Temporal*>(&markParent);
                if (t->getIsChordMember()) {
                    t = t->getChord();
                }
                auto frames = t->getFrames();
                pivotY = frames.head.getCenter().y;
            }


            const auto isAbove = markUnscaledScoreY < pivotY;
            const auto mxPlacement = isAbove ? mx::api::Placement::above : mx::api::Placement::below;


            if (isPedal) {
                if (markType == MarkType::PEDAL) {
                    mxType = mx::api::MarkType::pedal;
                } else if (markType == MarkType::PEDAL_UP) {
                    mxType = mx::api::MarkType::damp;
                }
            } else if (isNiente) {
                mxType = mx::api::MarkType::otherDynamics;
                specialSmufl = "dynamicNiente";
            } else if (isArticulation && isSpecialSmufl) {
                mxType = mx::api::MarkType::otherArticulation;
                specialSmufl = FontTheme::instance().MarkInfo.at(markType).getString();
            } else if (isTechnical && isSpecialSmufl) {
                mxType = mx::api::MarkType::otherTechnical;
                specialSmufl = FontTheme::instance().MarkInfo.at(markType).getString();
            } else {
                mxType = MxConverter::convertMark(mark.getMarkType());
            }

            // prevent a 'mark didn't save' bug by puking instead
            if (!isExpression) {
                KOMP_ASSERT( mxType != mx::api::MarkType::unspecified );
            }

            auto mxMarkData = mx::api::MarkData{mxPlacement, mxType};

            if (!specialSmufl.empty()) {
                // TODO: SMUFLFIX - make sure this is working ok after smufl map removal
                mxMarkData.name = specialSmufl;
            }


            if (isDirection && !isExpression) {
                // handle marks that must be saved as MusicXML 'directions'
                mx::api::DirectionData directionData;
                directionData.placement = mxPlacement;
                directionData.voice = mMxLocation.voiceIndex + 1;
                directionData.isStaffValueSpecified = true;
                directionData.tickTimePosition = inTemporal.getRoundedTime();
                mxMarkData.tickTimePosition = directionData.tickTimePosition;
                mxMarkData.positionData.placement = mxPlacement;
                const auto defaultY = mMxScoreWriter.unscaledScoreYToDefaultY(markUnscaledScoreY,
                                                                              staff);
                mxMarkData.positionData.isDefaultYSpecified = true;
                mxMarkData.positionData.defaultY = komp::roundPlaces(defaultY, 5);
                const auto defaultX = mMxScoreWriter.measureRelativeXToMeasureDefaultX(mark.getX(),
                                                                                       mark.getLocation().getMeasure());
                mxMarkData.positionData.isDefaultXSpecified = true;
                mxMarkData.positionData.defaultX = komp::roundPlaces(defaultX, 5);
                directionData.marks.push_back(mxMarkData);
                outStaffData.directions.push_back(directionData);
            } else if (isExpression) {
                mx::api::DirectionData directionData;
                directionData.placement = mxPlacement;
                directionData.voice = mMxLocation.voiceIndex + 1;
                directionData.isStaffValueSpecified = true;
                directionData.tickTimePosition = inTemporal.getRoundedTime();
                mx::api::WordsData wordsData;
                wordsData.text = mark.getMarkData().getString();
                wordsData.fontData.style = mx::api::FontStyle::italic;
                wordsData.positionData.placement = mxPlacement;
                const auto defaultY = mMxScoreWriter.unscaledScoreYToDefaultY(markUnscaledScoreY,
                                                                              staff);
                wordsData.positionData.isDefaultYSpecified = true;
                wordsData.positionData.defaultY = komp::roundPlaces(defaultY, 5);
                const auto defaultX = mMxScoreWriter.measureRelativeXToMeasureDefaultX(mark.getX(),
                                                                                       mark.getLocation().getMeasure());
                wordsData.positionData.isDefaultXSpecified = true;
                wordsData.positionData.defaultX = komp::roundPlaces(defaultX, 5);
                directionData.words.push_back(wordsData);
                outStaffData.directions.push_back(directionData);
            } else {

                // handle marks that can be saved as MusicXML 'notations'
                mxMarkData.positionData = createPositionData(mark, mxPlacement);
                mxMarkData.positionData.placement = mxPlacement;
                mxMarkData.tickTimePosition = mOutNoteData.tickTimePosition;
                mOutNoteData.noteAttachmentData.marks.push_back(mxMarkData);
            }
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
