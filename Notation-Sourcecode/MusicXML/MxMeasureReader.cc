// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MxMeasureReader.h"

// komp
#include "ChordSymbol.h"
#include "Marker.h"
#include "Measure.h"
#include "MxChordConverter.h"
#include "MxClef.h"
#include "MxConverter.h"
#include "MxDynamicsReader.h"
#include "MxNoteReader.h"
#include "Note.h"
#include "RehearsalMark.h"
#include "Rest.h"
#include "Score.h"
#include "TempoMark.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Track.h"
#include "Unused.h"

// Mx
#include "mx/api/ClefData.h"

namespace komp
{
    
    MxMeasureReader::MxMeasureReader(MxScoreReader& mxScore)
    : mMxScoreReader(mxScore)
    , mCurrentLocation{mMxScoreReader.createMxLocationObject()}
    {
        
    }

    void
    MxMeasureReader::importMeasures()
    {
        const auto& score = mMxScoreReader.getScore();
        KOMP_ASSERT(score.getMeasureCount() == mCurrentLocation.getTotalKompMeasures());
        KOMP_ASSERT(score.getTracks().size() >= mMxScoreReader.getScoreData().parts.size());
        mCurrentLocation.partIndex = 0;
        for (const auto& partData : mMxScoreReader.getScoreData().parts) {
            importPartData(partData);
            ++mCurrentLocation.partIndex;
        }
    }
    
    void
    MxMeasureReader::importPartData(const mx::api::PartData& inPartData)
    {
        bool isBeamActive = false;
        mCurrentLocation.mxMeasureIndex = 0;
        for (const auto& measureData : inPartData.measures) {

            if (mMxScoreReader.getIsScalingFound()) {
                const double widthTenths = static_cast<double>(measureData.width);
                const double widthSpaces = widthTenths / 10.0;
                const double width = widthSpaces * mMxScoreReader.getSpaceSize();
                mMxScoreReader.measureWidthMap[mCurrentLocation.getKompMeasureIndex()] = width;
            }

            if (mCurrentLocation.getKompMeasureIndex() < 0) {
                ++mCurrentLocation.mxMeasureIndex;
                continue;
            }

            importMeasureData(measureData, isBeamActive);
            ++mCurrentLocation.mxMeasureIndex;
        }
    }

    void
    MxMeasureReader::importMeasureData(const mx::api::MeasureData& inMeasureData,
                                       bool& outIsBeamActive)
    {
        mCurrentLocation.handIndex = 0;
        importMeter(inMeasureData);
        importKeys(inMeasureData);
        importBarlines(inMeasureData);
        for (const auto& staffData : inMeasureData.staves) {
            importStaffData(staffData, outIsBeamActive);
            ++mCurrentLocation.handIndex;
        }
    }
    
    void
    MxMeasureReader::importStaffData(const mx::api::StaffData& inStaffData,
                                     bool& outIsBeamActive)
    {
        mCurrentLocation.voiceIndex = 0;
        importClefs(inStaffData.clefs);

        // TODO: this is part of a hack to support the way that other applications read and write
        // voice tags.  Many will write 'voice 2' for the second staff of a multi-staff part whereas
        // Komp wants these to be written to voice 1.  This could be an issue for multiple-voices.
        bool doRewriteVoiceToZero = inStaffData.voices.size() == 1;
        
        for (const auto& dir : inStaffData.directions) {
            for( const auto& tempo : dir.tempos ) {
                importMetronomes( tempo );
            }
        }
        
        for (const auto& voiceMapPair : inStaffData.voices) {
            
            if (mCurrentLocation.voiceIndex > MAX_HANDS) {
                continue;
            }
            const auto voice = doRewriteVoiceToZero ? 0 : voiceMapPair.first;
            mCurrentLocation.voiceIndex = voice;
            importVoiceData(voiceMapPair.second, outIsBeamActive);
        }
        
        for (const auto& direction : inStaffData.directions) {
            importDirections( direction, inStaffData.voices );
        }
    }

    void
    MxMeasureReader::importVoiceData(const mx::api::VoiceData& inVoiceData,
                                     bool& outIsBeamActive)
    {
        importNotes(inVoiceData.notes, outIsBeamActive);
    }

    void
    MxMeasureReader::importClefs(const std::vector<mx::api::ClefData>& clefs)
    {
        for (const auto& clef : clefs) {
            mCurrentLocation.mxTickPosition = clef.tickTimePosition;
            MxClef clefImporter(clef, mCurrentLocation);
            clefImporter.insertClef(mMxScoreReader.getScore(),
                                    mMxScoreReader.getFileMessages());
        }
    }

    void
    MxMeasureReader::importNotes(const std::vector<mx::api::NoteData>& inNotes,
                                 bool& outIsBeamActive)
    {
        bool isTupletOccurring = false;
        auto loc = mMxScoreReader.createMxLocationObject();
        ChordInfo currentChord{NoteRefVec{}, std::move(loc)};
        
        auto& score = mMxScoreReader.getScore();
        
        for (const auto& noteData : inNotes) {
            mCurrentLocation.mxTickPosition = noteData.tickTimePosition;
            auto mxNoteReader = MxNoteReader{mCurrentLocation, noteData, mMxScoreReader};
            mxNoteReader.importNote(score, outIsBeamActive, currentChord, isTupletOccurring);
        }
        
        if (currentChord.isChordInProgress) {
            auto kompMeasureIndex = mCurrentLocation.getKompMeasureIndex();
            if (kompMeasureIndex >= 0 && kompMeasureIndex < score.getMeasureCount()) {
                const auto& measure = score.getMeasure(mCurrentLocation.getKompMeasureIndex());
                KOMP_ASSERT(measure);

                if (currentChord.notes.size() > 1) {
                    const auto& track = score.getTracks().at(mCurrentLocation.partIndex);
                    const auto hand = currentChord.notes.front().get().getLocation().getHand();
                    const MusicLocation loc(*measure.get(), *track.get(), hand);
                    auto& chord = score.getItemRegistry().createItem<Chord>(loc,
                                                                           currentChord.notes);
                    const auto arpeggiate = komp::MxConverter::convertArpeggiate(currentChord.arpeggiate);
                    chord.setArpeggiate(arpeggiate);
                    measure->insertItem(chord);
                    chord.setIsQuiescent(false);

                    currentChord = ChordInfo();
                    currentChord.isChordInProgress = false;

                    if (inNotes.size() > 1) {
                        mMxScoreReader.noteMxNoteMap.emplace(&chord, inNotes.back() );
                    }
                } else {
                    LOG_F(INFO, "cannot create a chord with less than 2 notes");
                }
            }
        }
    }

    void
    MxMeasureReader::importMeter(const mx::api::MeasureData& inMeasureData)
    {
        mCurrentLocation.mxTickPosition = 0;
        const int kompMeasureIndex = mCurrentLocation.getKompMeasureIndex();
        auto& measure = mMxScoreReader.getScore().getMeasure(kompMeasureIndex);
        const bool isFirstMeasure = (kompMeasureIndex == 0);
        const auto& time = inMeasureData.timeSignature;

        measure->setMeter(time.beats, time.beatType);

        switch (time.symbol) {
            case mx::api::TimeSignatureSymbol::unspecified:
                measure->setTimeSignatureType(TimeSignatureType::NUMERIC);
                break;
            case mx::api::TimeSignatureSymbol::common:
                measure->setTimeSignatureType(TimeSignatureType::COMMON);
                break;
            case mx::api::TimeSignatureSymbol::cut:
                measure->setTimeSignatureType(TimeSignatureType::CUT);
                break;
            default:
                measure->setTimeSignatureType(TimeSignatureType::NUMERIC);
                break;
        }

        if (time.display == mx::api::Bool::no) {
            measure->setTimeSignatureType(TimeSignatureType::NUMERIC);
            measure->setMeterVisible(false);
        } else if (!isFirstMeasure && time.isImplicit) {
            measure->setTimeSignatureType(TimeSignatureType::NUMERIC);
            measure->setMeterVisible(false);
        } else {
            measure->setMeterVisible(true);
        }
    }

    void
    MxMeasureReader::importMetronomes(const mx::api::TempoData& inTempoData)
    {
        const int kompMeasureIndex = mCurrentLocation.getKompMeasureIndex();
        const int kompTickTime = mCurrentLocation.getKompTickPosition();
        auto& score = mMxScoreReader.getScore();
        const auto& measure = score.getMeasure(kompMeasureIndex);
        const auto& track = score.getTracks().at(mCurrentLocation.partIndex);

        auto temporalType = TemporalType::QUARTER;
        int dots = 0;
        int bpm = 120;

        if (inTempoData.tempoType == mx::api::TempoType::beatsPerMinute ) {
            temporalType = MxConverter::convertTemporalType(inTempoData.beatsPerMinute.durationName);
            dots = inTempoData.beatsPerMinute.dots;
            bpm = inTempoData.beatsPerMinute.beatsPerMinute;
        }
        
        const TempoValue tempoValue{static_cast<double>(bpm), temporalType, dots};
        const auto tempoMarks = measure->items<TempoMark>(*track.get());
        
        // Locate existing tempo at location
        bool makeTempo = true;
        
        if (kompMeasureIndex == 0) {
            for (const auto& tempoMark : tempoMarks) {
                if (tempoMark.get().getTime() == kompTickTime) {
                    // The mark was created when importing the default score info
                    tempoMark.get().setTempoValue(tempoValue);
                    makeTempo = false;
                    break;
                }
            }
        }
        
        if (makeTempo) {
            auto& reg = score.getItemRegistry();
            const MusicLocation loc(*measure.get(), *track.get());
            auto& tempoMark = reg.createItem<TempoMark>(loc, tempoValue);
            tempoMark.setIsQuiescent(false);
            measure->insertItem(tempoMark, kompTickTime);
        }
    }
    
    void
    MxMeasureReader::importKeys(const mx::api::MeasureData& inMeasureData)
    {
        const auto& score = mMxScoreReader.getScore();
        const auto& measure = score.getMeasure(mCurrentLocation.getKompMeasureIndex());
        KOMP_ASSERT(measure);
        auto& track = score.getTracks().at(mCurrentLocation.partIndex);
        KOMP_ASSERT(track);
        
        for(const auto& keyData : inMeasureData.keys) {
            
            const auto kompKey = MxConverter::convertKey(keyData);

            // if staff index is negative, apply the key signature to all hands in the track
            int firstHandIndex = 0;
            int lastHandIndex = track->getHands() - 1;
            
            // if staff index is valid, then apply the key signature to only the given hand
            if (keyData.staffIndex > 0 && keyData.staffIndex <= lastHandIndex) {
                firstHandIndex = keyData.staffIndex;
                lastHandIndex = keyData.staffIndex;
            }
            
            for (int h = firstHandIndex; h <= lastHandIndex; ++h) {
                measure->insertKeySignature(*track.get(),             // track
                                            h,                        // hand
                                            kompKey);                 // key signature value
            }
        }
    }
    
    void
    MxMeasureReader::importBarlines(const mx::api::MeasureData& inMeasureData)
    {
        const auto& measure = mMxScoreReader.getScore().getMeasure(mCurrentLocation.getKompMeasureIndex());
        KOMP_ASSERT(measure);
        
        const auto& barlines = inMeasureData.barlines;
        for (const auto& barline : barlines) {

            bool isLeftBarline = false;

            if (barline.location == mx::api::HorizontalAlignment::left) {
                isLeftBarline = true;
            } else if (barline.location == mx::api::HorizontalAlignment::right) {
                isLeftBarline = false;
            } else {
                if (barline.tickTimePosition == 0) {
                    isLeftBarline = true;
                } else {
                    isLeftBarline = false;
                }
            }

            const auto leftBarlineType = MxConverter::convertLeftBarline(barline.barlineType);
            const auto rightBarlineType = MxConverter::convertRightBarline(barline.barlineType);

            if (isLeftBarline &&
                leftBarlineType != BarlineType::UNKNOWN) {
                if (barline.repeat) {
                    measure->setLeftBarline(BarlineType::LEFT_REPEAT);
                } else {
                    measure->setLeftBarline(leftBarlineType);
                }
            }

            if (!isLeftBarline &&
                rightBarlineType != BarlineType::UNKNOWN) {
                if (barline.repeat) {
                    measure->setRightBarline(BarlineType::RIGHT_REPEAT);
                } else {
                    measure->setRightBarline(rightBarlineType);
                }
            }
            
            // Push endings onto a stack to deal with later
            if (barline.endingType != mx::api::EndingType::none) {
                auto& mxSpannerCollector = mMxScoreReader.getSpannerCollector();
                auto& tracks = mMxScoreReader.getScore().getTracks();
                const auto& track = tracks.at(mCurrentLocation.partIndex);
                mxSpannerCollector.endings.addEnding(*measure.get(),
                                                     *track.get(),
                                                     barline.endingType,
                                                     barline.endingNumber);
            }
        }
    }

    void
    MxMeasureReader::importDirections(const mx::api::DirectionData& inDirectionData,
                                      const std::map<int, mx::api::VoiceData>& inVoices)
    {
        mCurrentLocation.mxTickPosition = inDirectionData.tickTimePosition;
        const int kompMeasureIndex = mCurrentLocation.getKompMeasureIndex();
        const int kompTickLocation = mCurrentLocation.getKompTickPosition();
        TimePoint timePoint;
        auto& score = mMxScoreReader.getScore();
        timePoint.measure = score.getMeasure(kompMeasureIndex).get();
        KOMP_ASSERT(timePoint.measure);
        timePoint.track = score.getTracks().at(mCurrentLocation.partIndex).get();
        timePoint.hand = mCurrentLocation.handIndex;
        timePoint.time = kompTickLocation;
        
        auto& outMessages = mMxScoreReader.getFileMessages();
        auto& spanners = mMxScoreReader.getSpannerCollector();
        
        for (const auto& mark : inDirectionData.marks) {
            importDirectionMark(mark, kompTickLocation, inDirectionData.placement);
        }

        auto tempLocation = mCurrentLocation;
        tempLocation.mxTickPosition = inDirectionData.tickTimePosition;
        timePoint.time = tempLocation.getKompTickPosition();
        
        for (const auto& wedgeStart : inDirectionData.wedgeStarts) {
            auto& theScore = mMxScoreReader.getScore();
            auto theLocation = mCurrentLocation;
            theLocation.mxTickPosition = inDirectionData.tickTimePosition;
            spanners.wedges.startWedge(theScore,
                                       wedgeStart,
                                       inDirectionData.placement,
                                       mx::api::NoteData{},
                                       theLocation);
        }

        for (const auto& wedgeStop : inDirectionData.wedgeStops) {
            auto theLocation = mCurrentLocation;
            theLocation.mxTickPosition = inDirectionData.tickTimePosition;
            spanners.wedges.stopWedge(wedgeStop, theLocation);
        }

        for (const auto& ottavaStart : inDirectionData.ottavaStarts) {
            MxOttavaParams oParams;
            oParams.numberLevel = -1; //ottavaStart.numberLevel; // TODO: this seems to be missing from Mx
            oParams.timePoint = timePoint;
            oParams.octaveType = MxConverter::convertOttava(ottavaStart.ottavaType);
            
            MxOttavaPtr mxOttavaPtr{new MxOttava{oParams}};
            
            if (!spanners.ottavaMgr.push(-1, std::move(mxOttavaPtr))) {
                KOMP_FILE_WARN("invalid ottava start");
            }
        }

        for (const auto& ottavaStop : inDirectionData.ottavaStops) {
            if (!spanners.ottavaMgr.completeSpanner(ottavaStop.numberLevel, timePoint)) {
                KOMP_FILE_WARN("invalid ottava stop");
            }
        }

        const auto& tracks = score.getTracks();
        
        if (!inDirectionData.chords.empty()) {

            auto& track = tracks.at(mCurrentLocation.partIndex);
            
            const auto& measure = score.getMeasure(mCurrentLocation.getKompMeasureIndex());

            const double beatUnit = measure->getBeatUnit();
            const int beatsPerBar = measure->getBeats();
            const auto temporalType = TemporalValue::getTypeByTimeSignatureDenominator(static_cast<int>(beatUnit));
            const auto ticksPerBeat = TemporalValue::getValue(temporalType);

            auto& factory = score.getCommandFactory();

            for (const auto& chord : inDirectionData.chords) {
                ChordSymbolParams cparams(*track.get());
                cparams.hand = mCurrentLocation.handIndex;
                cparams.value = MxChordConverter::convert(chord);
                cparams.time = komp::roundToInt(static_cast<double>(mCurrentLocation.getTickConversionRatio())
                                                * static_cast<double>(inDirectionData.tickTimePosition));
                const auto& staff = measure->getStaffSystem()->getStaff(cparams.track,
                                                                        cparams.hand);
                
                // TODO: think more about chord symbols that have an x-position that is not in-line with their time
                // for now this is too problematic
                //            if (chord.positionData.isDefaultXSpecified && mMxScoreReader.getIsScalingFound()) {
                //                cparams.isXDifferentFromTime = true;
                //                cparams.measureRelativeX = mMxScoreReader.measureDefaultXToMeasureX(chord.positionData.defaultX, staff, measure.get());
                //            }
                
                if (chord.positionData.isDefaultYSpecified && mMxScoreReader.getIsScalingFound()) {
                    cparams.isSlotSpecified = true;
                    const double unscaledScoreY = mMxScoreReader.defaultYToUnscaledScoreY(chord.positionData.defaultY, staff);
                    const auto slot = staff.pixelToSlot(unscaledScoreY);
                    cparams.slot = slot;
                }
                
                // quantize the chord symbol to the nearest 'beat'
                double nearestAbove = 0.0;
                double nearestBelow = 0.0;
                
                for (int i = 0; i < beatsPerBar + 1; ++i) {
                    nearestAbove += ticksPerBeat;
                    
                    if (nearestAbove >= static_cast<double>(cparams.time)) {
                        break;
                    }
                    
                    nearestBelow = nearestAbove;
                }
                
                const auto distanceAbove = nearestAbove - static_cast<double>(cparams.time);
                const auto distanceBelow = static_cast<double>(cparams.time) - nearestBelow;

                if (std::abs(distanceAbove) < std::abs(distanceBelow)) {
                    cparams.time = nearestAbove;
                } else {
                    cparams.time = nearestBelow;
                }

                if (std::abs(distanceAbove) < std::abs(distanceBelow)) {
                    cparams.time = nearestAbove;
                } else {
                    cparams.time = nearestBelow;
                }
                
                auto chordSymbolCommand = factory.insertChordSymbolCommand(*measure.get(),
                                                                           cparams);
                chordSymbolCommand->execute();
            }
        }
        
        if (!inDirectionData.segnos.empty()) {
            const auto& track = score.getTracks().at(mCurrentLocation.partIndex);
            for (const auto& segno : inDirectionData.segnos) {
                const MusicLocation loc(*timePoint.measure, *track.get());
                auto& reg = score.getItemRegistry();
                auto& segnoMarker = reg.createItem<Marker>(loc, MarkerParams{
                    MarkerType::SEGNO,
                    segno.positionData.defaultX,
                    segno.positionData.defaultY
                });
                timePoint.measure->insertItem(segnoMarker);
            }
        }

        if (!inDirectionData.codas.empty()) {
            const auto& track = score.getTracks().at(mCurrentLocation.partIndex);
            for (const auto& coda : inDirectionData.codas) {
                const MusicLocation loc(*timePoint.measure, *track.get());
                auto& reg = score.getItemRegistry();
                auto& codaMark = reg.createItem<Marker>(loc, MarkerParams{
                    MarkerType::CODA,
                    coda.positionData.defaultX,
                    coda.positionData.defaultY
                });
                timePoint.measure->insertItem(codaMark);
            }
        }

        
        if (!inDirectionData.rehearsals.empty()) {
            const auto& track = score.getTracks().at(mCurrentLocation.partIndex);
            for (const auto& rehearsal : inDirectionData.rehearsals) {
                const MusicLocation loc(*timePoint.measure, *track.get());
                auto& reg = score.getItemRegistry();
                auto& rehearsalMark = reg.createItem<RehearsalMark>(loc, RehearsalParams{
                    rehearsal.text,
                    rehearsal.positionData.defaultX,
                    rehearsal.positionData.defaultY
                });
                timePoint.measure->insertItem(rehearsalMark);
            }
        }
        
        // push stupid directions onto a stack to deal with later
        const auto hasMarks = inDirectionData.marks.size() > 0;
        const auto hasWords = inDirectionData.words.size() > 0;
        
        if (hasMarks || hasWords) {
            MxDirection savedDirection;
            savedDirection.direction = inDirectionData;
            savedDirection.voiceMap = inVoices;
            savedDirection.direction.ottavaStarts.clear();
            savedDirection.direction.ottavaStops.clear();
            savedDirection.direction.tempos.clear();
            savedDirection.direction.wedgeStarts.clear();
            savedDirection.direction.wedgeStops.clear();
            savedDirection.direction.chords.clear();
            savedDirection.location = mCurrentLocation;
            savedDirection.location.mxTickPosition = inDirectionData.tickTimePosition;
            mMxScoreReader.addDirection(std::move(savedDirection));
        }
    }

    void
    MxMeasureReader::importDirectionMark(const mx::api::MarkData& mark, int inKompTickPosition, mx::api::Placement inMxPlacement)
    {
        MxDynamicsReader reader{mMxScoreReader, mark, mCurrentLocation, inMxPlacement};
        reader.insertDynamicOnNearestTemporalIfPossible();
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
