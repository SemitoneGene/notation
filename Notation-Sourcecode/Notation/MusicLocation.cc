// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "MusicLocation.h"

// Local
#include "CurrentState.h"
#include "Clef.h"
#include "Forward.h"
#include "KeySignature.h"
#include "Measure.h"
#include "Note.h"
#include "Rest.h"
#include "Score.h"
#include "Track.h"
#include "Pitch.h"
#include "Staff.h"

namespace komp
{
    MusicLocation::MusicLocation(const CurrentState& inState)
    : mMeasure{inState.getMeasure()}
    , mTrack{inState.getTrack()}
    , mHand{inState.getHand()}
    , mVoice{inState.getVoice()}
    , mSlot{inState.getSlot()}
    , mTime{0}
    {
    }

    MusicLocation::MusicLocation(Measure& inMeasure,
                                 Track& inTrack,
                                 int inHand,
                                 int inVoice,
                                 int inSlot,
                                 double inTime)
    : mMeasure{inMeasure}
    , mTrack{inTrack}
    , mHand{inHand}
    , mVoice{inVoice}
    , mSlot{inSlot}
    , mTime{inTime}
    {
    }

    MusicLocation::MusicLocation(const MusicLocation& other)
    : mMeasure{other.getMeasure()}
    , mTrack{other.getTrack()}
    , mHand{other.getHand()}
    , mVoice{other.getVoice()}
    , mSlot{other.getSlot()}
    , mTime{other.getTime()}
    {
    }

    Measure&
    MusicLocation::getMeasure() const
    {
        return mMeasure;
    }


    Track&
    MusicLocation::getTrack() const
    {
        return mTrack;
    }


    int
    MusicLocation::getHand() const
    {
        return mHand;
    }


    int
    MusicLocation::getVoice() const
    {
        return mVoice;
    }


    int
    MusicLocation::getSlot() const
    {
        return mSlot;
    }


    double
    MusicLocation::getTime() const
    {
        return mTime;
    }


    int
    MusicLocation::getRoundedTime() const
    {
        return std::ceil(getTime() - 0.5);
    }


    Staff&
    MusicLocation::getStaff() const
    {
        const auto& system = mMeasure.get().getStaffSystem();
        KOMP_ASSERT(system);
        return system->getStaff(mTrack, mHand);
    }


    void
    MusicLocation::setMeasure(Measure& inValue)
    {
        mMeasure = inValue;
    }

    void
    MusicLocation::setTrack(Track& inValue)
    {
        mTrack = inValue;
    }


    void
    MusicLocation::setHand(int inValue)
    {
        mHand = inValue;
        //notifyObservers({ItemChangeType::Hand});
    }


    void
    MusicLocation::setVoice(int inValue)
    {
        mVoice = inValue;
        //notifyObservers({ItemChangeType::Voice});
    }


    void
    MusicLocation::setSlot(int inValue)
    {
        mSlot = inValue;
    }


    void
    MusicLocation::setTime(double inValue)
    {
        mTime = inValue;
    }


    ClefType
    MusicLocation::getClef() const
    {
        // Synopsis: Iterate backwards through the track hand looking for clefs
        // starting from the measure / time of our location.  Return the first
        // one found.  if none are found then return the default clef
        
        const auto& score = mMeasure.get().getScore();
        
        auto iter = mMeasure.get().iter();
        auto firstMeasure = score.getMeasure(0)->iter();
        
        auto isClef = [&](Item& inItem) {
            
            if (inItem.getItemTraits().getItemType() != ItemType::Clef) {
                return false;
            }
            
            const auto& itemLoc = inItem.getLocation();
            if (&itemLoc.getTrack() != &mTrack.get()) {
                return false;
            }
            
            if (!(itemLoc.getHand() == getHand())) {
                return false;
            }
            
            return true;
        };
        
        auto isClefBeforeMyTime = [&](Item& inItem) {
            if (!isClef(inItem)) {
                return false;
            }
            
            const auto& clef = inItem.as<Clef>();
            const double clefTime = clef.getTime();
            
            if (clefTime > getTime()) {
                return false;
            }
            
            return true;
        };
        
        bool isMyMeasure = true;
        
        for ( ; iter >= firstMeasure; --iter, isMyMeasure = false) {
            ClefRefVec clefs;
            
            if (isMyMeasure) {
                clefs = (*iter)->items<Clef>(mTrack, isClefBeforeMyTime);
            } else {
                clefs = (*iter)->items<Clef>(mTrack, isClef);
            }
            
            if (clefs.empty()) {
                continue;
            }
            
            const Clef& clef = *clefs.crbegin();
            return clef.type();
        }
        
        return ClefType::G;
    }


    KeySignatureType
    MusicLocation::getKeySignature() const
    {
        // Synopsis - iterate backwards through the track hand looking for clefs
        // starting from the measure / time of our location.  return the first
        // one found.  if none are found then return the Staff's clef
        
        const auto& myTrack = getTrack();
        const auto& score = mMeasure.get().getScore();
        
        auto iter = mMeasure.get().iter();
        auto firstMeasure = score.getMeasure(0)->iter();
        
        auto isKey = [&](Item& inItem) {
            
            if (inItem.getItemTraits().getItemType() != ItemType::KeySignature) {
                return false;
            }
            
            const auto& itemLoc = inItem.getLocation();
            if (&itemLoc.getTrack() != &myTrack) {
                return false;
            }
            
            if (!(itemLoc.getHand() == getHand())) {
                return false;
            }
            
            return true;
        };
        
        auto isKeyBeforeMyTime = [&](Item& inItem) {
            if (!isKey(inItem)) {
                return false;
            }
            
            const auto& key = inItem.as<KeySignature>();
            const double keyTime = key.getTime();
            
            if (keyTime > getTime()) {
                return false;
            }
            
            return true;
        };
        
        bool isMyMeasure = true;
        
        for ( ; iter >= firstMeasure; --iter, isMyMeasure = false) {
            std::vector<KeySignatureRef> keys;
            
            if (isMyMeasure) {
                keys = (*iter)->items<KeySignature>(mTrack, isKeyBeforeMyTime);
            } else {
                keys = (*iter)->items<KeySignature>(mTrack, isKey);
            }
            
            if (keys.empty()) {
                continue;
            }
            
            KeySignature& key = *keys.crbegin();
            return key.getKey().getValue();
        }
        
        return KeySignatureType::C;
    }


    AccidentalType
    MusicLocation::getAccidental() const
    {
        // find all earlier notes for this measure, track, hand and slot
        auto predicate = [&](Item& inItem) {
            
            const auto& itemLoc = inItem.getLocation();
            if (&itemLoc.getTrack() != &mTrack.get()) {
                return false;
            }
            
            if (itemLoc.getHand() != getHand()) {
                return false;
            }
            
            if (inItem.getTime() >= getTime()) {
                return false;
            }
            
            if (inItem.getItemTraits().getItemType() != ItemType::Note) {
                return false;
            }
            
            auto& note = inItem.as<Note>();
            if (note.getSlot() != getSlot()) {
                return false;
            }
            
            return true;
        };
        
        // look for and return the most recent, previous accidental for the
        // current measure, track, hand, time and slot
        auto previousNotesForThisSlot = mMeasure.get().items<Note>(mTrack,
                                                                   predicate);
        for (auto iter = previousNotesForThisSlot.crbegin();
             iter != previousNotesForThisSlot.crend(); ++iter)
        {
            const auto currentAccidental = (*iter).get().getAccidental();
            if (currentAccidental != AccidentalType::NONE) {
                return currentAccidental;
            }
        }
        
        // if we didn't find any accidentals for this slot, then return the
        // accidental that is specified by the key signature for this slot
        const auto keySignature = getKeySignature();
        auto keyAccidental = KeySignature::getAccidental(keySignature,
                                                         getClef(),
                                                         getSlot());
        return keyAccidental;
    }


    TemporalRefVec
    MusicLocation::getOccupants() const
    {
        // find all earlier notes for this measure, track, hand and slot
        auto predicate = [&](Item& inItem) {
            
            if (inItem.getRoundedTime() != this->getRoundedTime()) {
                return false;
            }
            
            const auto& itemLoc = inItem.getLocation();
            if (itemLoc.getHand() != getHand()) {
                return false;
            }
            
            if (itemLoc.getVoice() != getVoice()) {
                return false;
            }
            
            if (&itemLoc.getTrack() != &mTrack.get()) {
                return false;
            }
            
            if (!inItem.is<Temporal>()) {
                return false;
            }
            
            return true;
        };
        
        return mMeasure.get().items<Temporal>(mTrack, predicate);
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
