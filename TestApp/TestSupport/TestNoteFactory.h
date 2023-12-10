// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Komp
#include "Clef.h"
#include "Forward.h"
#include "Note.h"
#include "NoteheadType.h"
#include "Pitch.h"
#include "TemporalType.h"
#include "TemporalValue.h"

namespace komp
{
struct TestNote
{
    bool isRest{false};
    std::vector<komp::Pitch> pitches;
    komp::TemporalType temporalType{komp::TemporalType::EIGHTH};
    int dots{0};
};

struct TestNoteFactory
{
    Score& score;
    int trackIndex;
    int handIndex;
    int voiceIndex;
    int measureIndex;
    double time;
    komp::TemporalType temporalType;
    int dots;
    komp::Pitch pitch;
    komp::ClefType clefType;
    bool autoAdvanceMeasure;
    bool autoAddMeasures;
    bool autoReconcilePitchAfterInsert;
    komp::AccidentalType accidental;
    komp::MarkType mark;
    komp::PlacementType placement;
    
    inline double getDuration() const
    {
        double durTime = komp::TemporalValue::getValue(temporalType);
        double dotVal = durTime / 2;
        for (int i = 0; i < dots; ++i) {
            durTime += dotVal;
            dotVal /= 2;
        }
        return durTime;
    }
    
    inline void advancePosition()
    {
        const double durTime = getDuration();
        time += durTime;
        if (autoAdvanceMeasure) {
            if (time >= mCurrentClocksPerMeasure && mCurrentClocksPerMeasure > 0) {
                time = 0;
                ++measureIndex;
            }
        }
    }
    
    inline void setEntryPositionBasedOnMeasureContents()
    {
        using namespace komp;
        const auto& measure = score.getMeasure(measureIndex);
        const auto& track = score.getTracks().at(trackIndex);
        
        auto filter = [&](Item& inFilterItem) {
            
            if (!inFilterItem.is<Temporal>()) {
                return false;
            }
            
            const auto& loc = inFilterItem.getLocation();
            return &loc.getTrack() == track.get()
            && loc.getHand() == handIndex
            && loc.getVoice() == voiceIndex;
        };
        
        const auto temporals = measure->items<Temporal>(*track.get(), filter);
        const auto iter = temporals.crbegin();
        if (iter == temporals.crend()) {
            time = 0.0;
            return;
        }
        const Temporal& temporal = *iter;
        time = temporal.getTime();
        time += temporal.getDuration().getClocks();
    }
    
    inline komp::Note& insertNote()
    {
        using namespace komp;
        
        if (autoAddMeasures) {
            if (measureIndex > static_cast<int>(score.getMeasureCount()) - 1) {
                score.insertMeasure(static_cast<size_t>(measureIndex));
                score.layout();
            }
        }
        
        auto& measure = score.getMeasure(measureIndex);
        auto& track = score.getTracks().at(trackIndex);
        const auto slot = pitch.slot(clefType);
        
        const MusicLocation loc(*measure.get(),
                                *track.get(),
                                handIndex,
                                voiceIndex,
                                slot);
        auto& note = score.getItemRegistry().createItem<Note>(loc,
                                                              temporalType,
                                                              NoteheadType::NORMAL,
                                                              pitch);
        note.setTime(time);
        note.setDuration(note.getDuration().setDots(dots));
        note.setIsQuiescent(false);
        measure->insertItem(note, time);
        mCurrentClocksPerMeasure = measure->getClocks();
        
        if (autoReconcilePitchAfterInsert) {
            note.deducePitch();
        }
        
        if (accidental != AccidentalType::NONE) {
            note.setAccidental(accidental);
        }

        if (mark != MarkType::NONE) {
            note.getAttachments().addMark(score.getItemRegistry(),
                                          note,
                                          mark,
                                          placement);
        }

        return note;
    }
    
    /// \brief Adds the given number of steps to the PitchStep, wraps around if it goes out-of-bounds
    inline void addSteps(int inNumSteps)
    {
        int value = static_cast<int>(this->pitch.step);
        value += inNumSteps;
        bool isNegative = value < 0;
        
        if (isNegative) {
            value *= -1;
        }
        
        value %= 7;
        
        if (isNegative) {
            value *= -1;
        }
        
        this->pitch.step = static_cast<komp::PitchStep>(value);
    }
    
    TestNoteFactory() = delete;
    
    TestNoteFactory(Score& inScore)
    : score(inScore)
    , trackIndex(0)
    , handIndex(0)
    , voiceIndex(0)
    , measureIndex(0)
    , time(0)
    , temporalType(komp::TemporalType::QUARTER)
    , dots(0)
    , pitch()
    , clefType(komp::ClefType::G)
    , mCurrentClocksPerMeasure(0)
    , autoAdvanceMeasure(true)
    , autoAddMeasures(false)
    , autoReconcilePitchAfterInsert(false)
    , accidental(komp::AccidentalType::NONE)
    , mark(komp::MarkType::NONE)
    , placement{komp::PlacementType::AUTOMATIC}
    {
        
    }
    
private:
    double mCurrentClocksPerMeasure;
};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
