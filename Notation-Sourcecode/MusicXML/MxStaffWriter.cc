// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "MxStaffWriter.h"
#include "MxNoteWriter.h"
#include "MxConverter.h"

// Komp
#include "ChordSymbol.h"
#include "Clef.h"
#include "MxChordConverter.h"
#include "Note.h"
#include "Rest.h"
#include "RoundPlaces.h"
#include "Score.h"
#include "Staff.h"
#include "TempoMark.h"
#include "Temporal.h"
#include "Track.h"
#include "Wedge.h"

namespace komp
{
    MxStaffWriter::MxStaffWriter(const ItemRefVec& inSortedItems,
                                 MxLocation inMxLocation,
                                 MxScoreWriter& inMxScoreWriter)
    : mItems(inSortedItems)
    , mMxLocation(inMxLocation)
    , mMxScoreWriter(inMxScoreWriter)
    , mOutStaffData()
    , mConverter(inMxScoreWriter.getScore())
    {

    }
    
    mx::api::StaffData
    MxStaffWriter::getStaffData()
    {
        mOutStaffData = mx::api::StaffData{};

        auto& score = mMxScoreWriter.getScore();
        const auto& track = score.getTracks().at(mMxLocation.partIndex);

        int tupletNumberLevel = 0;

        for (Item& item : mItems) {
            const auto& loc = item.getLocation();
            if (&loc.getTrack() == track.get()
             && loc.getHand() == mMxLocation.handIndex) {
                dispatchItem(item, tupletNumberLevel);
            }
        }

        closeoutWedges();
        addStartingClefIfAbsent();
        mx::api::StaffData temp = std::move(mOutStaffData);
        mOutStaffData = mx::api::StaffData{};
        return temp;
    }
    
    void
    MxStaffWriter::dispatchItem(Item& item, int& tupletNumberLevel)
    {
        const auto itemType = item.getItemTraits().getItemType();
        if (itemType == ItemType::Chord) {
        
            KOMP_THROW("don't try to export chords");

        } else if (itemType == ItemType::Rest || itemType == ItemType::Note) {
            auto& temporal = item.as<Temporal>();
            
            const auto& temporalLoc = temporal.getLocation();
            const auto& measure = temporalLoc.getMeasure();
            
            // Check to see if there are concurrent notes, if so, it is part of a chord
            auto concurrentNotesFilter = [&](Item& inItem) {
                
                const auto& loc = inItem.getLocation();

                if (&loc.getTrack() != &temporalLoc.getTrack()) {
                    return false;
                }
                
                if (loc.getHand() != temporalLoc.getHand()) {
                    return false;
                }
                
                if (loc.getVoice() != temporalLoc.getVoice()) {
                    return false;
                }
                
                if (!inItem.is<Temporal>()) {
                    return false;
                }
                
                if (inItem.getRoundedTime() != temporal.getRoundedTime()) {
                    return false;
                }
                
                if (&inItem == &temporal) {
                    return false;
                }
                
                return true;
            };
            
            auto concurrentNotes = measure.items<Note>(temporalLoc.getTrack(),
                                                       concurrentNotesFilter);
            bool isPartOfChord = concurrentNotes.size() > 0;
            
            if (temporal.getItemTraits().getItemType() == ItemType::Note) {
                auto& note = temporal.as<Note>();
                isPartOfChord |= note.getIsChordMember();
            }
            
            if (isPartOfChord && !temporal.getIsChordMember()) {
                // If the note is concurrent with any other notes on the same voice,
                // it cannot be represented in MusicXML.  Also, if the note has been
                // properly added to a komp Chord object, then we handle it elsewhere
                LOG_F(ERROR, "simultaneous notes in the same voice must be chorded to be represented in MusicXML");
                return;
            }
            
            MxNoteWriter noteWriter {
                temporal,
                mMxLocation,
                mMxScoreWriter,
                isPartOfChord,
                mProcessedChords
            };
            const auto& noteData = noteWriter.getNoteData(tupletNumberLevel, mOutStaffData);

            // TODO: this is part of a hack to support Dorico and Sibelius requirement that multi-stave
            // parts use different voices for each staff.  The NoteWriter object is where the hack actually
            // takes place, here we are just using the NoteWriter's voice number.
            mOutStaffData.voices[noteData.userRequestedVoiceNumber - 1].notes.emplace_back(noteData);

            if (!noteData.isChord) {
                mMxLocation.mxTickPosition += noteData.durationData.durationTimeTicks;
            }
        } else if (item.is<Clef>()) {
            const auto& clef = item.as<Clef>();
            mx::api::ClefData clefData = makeClefData(clef.type(),
                                                      mMxLocation.handIndex,
                                                      mMxLocation.mxTickPosition);
            mOutStaffData.clefs.emplace_back(std::move(clefData));
        } else if (item.getItemTraits().getItemType() == ItemType::ChordSymbol) {
            const auto& chordSymbol = item.as<ChordSymbol>();
            handleChordSymbol(chordSymbol);
        } else if (item.is<TempoMark>()) {
            const auto& tempo = item.as<TempoMark>();
            const auto& tempoValue = tempo.getTempoValue();
            mx::api::TempoData mxTempo{};
            mxTempo.tempoType = mx::api::TempoType::beatsPerMinute;
            mxTempo.beatsPerMinute.durationName = MxConverter::convertTemporalType(tempoValue.getBeatType());
            mxTempo.beatsPerMinute.dots = tempoValue.getBeatTypeDots();
            mxTempo.beatsPerMinute.beatsPerMinute = tempoValue.getBpm();
            mxTempo.tickTime = mMxLocation.mxTickPosition;
            mx::api::DirectionData direction{};
            direction.tempos.emplace_back(std::move(mxTempo));
            mOutStaffData.directions.emplace_back(std::move(direction));
        } else if (item.is<Wedge>()) {
            handleWedgeItem(item.as<Wedge>());
        }
        closeoutWedges();
    }
    
    void
    MxStaffWriter::addStartingClefIfAbsent()
    {
        if (mMxLocation.mxMeasureIndex != 0) {
            return;
        }
        
        if (mOutStaffData.clefs.size() > 0) {
            if (mOutStaffData.clefs.at(0).tickTimePosition == 0) {
                // a starting clef exists, abort
                return;
            }
        }
        
        const auto& track = mMxScoreWriter.getScore().getTracks().at(mMxLocation.partIndex);
        const auto clefType = track->getClefType(mMxLocation.handIndex);
        const auto clefData = makeClefData(clefType, mMxLocation.handIndex, 0);
        mOutStaffData.clefs.emplace(mOutStaffData.clefs.begin(), std::move(clefData));
    }
    
    mx::api::ClefData
    MxStaffWriter::makeClefData(ClefType clefType, int handIndex, int time)
    {
        const auto& clefInfo = Clef::clefInfo(clefType);
        mx::api::ClefData clefData;
        clefData.line = clefInfo.delineationLine;
        clefData.tickTimePosition = time;
        clefData.symbol = MxConverter::convertClefSymbol(clefInfo.clefSymbolInfo.type);
        clefData.octaveChange = clefInfo.octaveDisplacement;
        return clefData;
    }
    
    void
    MxStaffWriter::handleWedgeItem(Wedge& inWedge)
    {
        mMxScoreWriter.getWedges().push_front(inWedge);
        mOutStaffData.directions.emplace_back();
        auto& direction = mOutStaffData.directions.back();
        direction.wedgeStarts.emplace_back();

        auto directionLocation = mMxLocation;
        directionLocation.mxTickPosition = inWedge.getRoundedTime();
        direction.tickTimePosition = komp::roundToInt(inWedge.getRoundedTime());
        const auto& measure = inWedge.getLocation().getMeasure();
        const auto currentTimeConvertedToX = measure.positionForTime(mMxLocation.getKompTickPosition());
        const auto pixelOffset = inWedge.getX() - currentTimeConvertedToX;
        
        auto& wedgeStart = direction.wedgeStarts.back();
        wedgeStart.positionData.isDefaultXSpecified = true;
        wedgeStart.positionData.defaultX = komp::roundPlaces(mConverter.convertPixelsToTenths(pixelOffset), 5);
        wedgeStart.positionData.isDefaultYSpecified = true;
        if (inWedge.getWedgeType() == WedgeType::CRESCENDO) {
            wedgeStart.wedgeType = mx::api::WedgeType::crescendo;
        } else if (inWedge.getWedgeType() == WedgeType::DIMINUENDO) {
            wedgeStart.wedgeType = mx::api::WedgeType::diminuendo;
        }
        const auto defaultY = mConverter.convertSlotToDefaultY(inWedge.getSlot());
        wedgeStart.positionData.defaultY = komp::roundPlaces(defaultY, 5);
    }
    
    void
    MxStaffWriter::closeoutWedges()
    {
        auto& wedges = mMxScoreWriter.getWedges();

        for (auto w = wedges.begin(); w != wedges.end(); ++w) {
            
            const Wedge& wedge = *w;
            const auto wedgeEndTime = wedge.getEndTime();
            const auto& measure = wedge.getEndMeasure();
            KOMP_ASSERT(measure);
            
            if (measure->getIndex() != mMxLocation.getKompMeasureIndex()) {
                continue;
            }
            
            if (mMxLocation.getKompTickPosition() - wedgeEndTime < mMxLocation.getKompTicksPerQuarter()) {
                mOutStaffData.directions.emplace_back();
                auto& direction = mOutStaffData.directions.back();
                direction.wedgeStops.emplace_back();
                direction.tickTimePosition = wedgeEndTime;
                const auto currentTimeConvertedToX = measure->positionForTime(wedgeEndTime);
                const auto pixelOffset = wedge.getEndX() - currentTimeConvertedToX;
                
                auto& wedgeStop = direction.wedgeStops.back();
                wedgeStop.positionData.isDefaultXSpecified = true;
                wedgeStop.positionData.defaultX = komp::roundPlaces(mConverter.convertPixelsToTenths(pixelOffset), 5);

                w = wedges.erase(w);
                --w;
            }
        }
    }

    void
    MxStaffWriter::handleChordSymbol(const ChordSymbol& inChordSymbol)
    {
        mx::api::DirectionData directionData;
        directionData.tickTimePosition =  inChordSymbol.getTime() / mMxLocation.getTickConversionRatio();
        mx::api::ChordData chordData = MxChordConverter::convert(inChordSymbol.getChordSymbolValue());
        const auto& staff = inChordSymbol.getLocation().getStaff();

        if (!inChordSymbol.getIsTimeLocked()) {
            chordData.positionData.isDefaultXSpecified = true;
            const auto x = inChordSymbol.getX();
            const auto& measure = inChordSymbol.getLocation().getMeasure();
            chordData.positionData.defaultX = mMxScoreWriter.measureRelativeXToMeasureDefaultX(x,
                                                                                               measure);
        }
        const auto unscaledScoreY = inChordSymbol.getScoreY();
        chordData.positionData.isDefaultYSpecified = true;
        chordData.positionData.defaultY = mMxScoreWriter.unscaledScoreYToDefaultY(unscaledScoreY, staff);

        directionData.chords.push_back(chordData);
        mOutStaffData.directions.push_back(directionData);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
