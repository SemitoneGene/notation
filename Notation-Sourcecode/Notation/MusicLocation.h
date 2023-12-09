// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "AccidentalType.h"
#include "ClefType.h"
#include "Forward.h"
#include "KeySignatureType.h"

namespace komp
{    
    /// \brief  Describes the Location of a Score Item
    ///
	/// \discussion Describes the exact insertion point, in musical terms for an
    /// item in the score.  Namely, a score item is located using Score, Track,
    /// Hand, Voice, Slot and Time*.  Also provides lookups for information about
    /// the described music location, such as the active clef key, etc.
	///
	class MusicLocation
	{
    public:
        MusicLocation() = delete;
        MusicLocation(const CurrentState& inState);
        MusicLocation(Measure& inMeasure,
                      Track& inTrack,
                      int inHand = 0,
                      int inVoice = 0,
                      int inSlot = 0,
                      double inTime = 0.0);
        MusicLocation(const MusicLocation& other);
        
        Measure& getMeasure() const;
        Track& getTrack() const;
        int getHand() const;
        int getVoice() const;
        int getSlot() const;
        double getTime() const;
        int getRoundedTime() const;
        Staff& getStaff() const;
        
        void setMeasure(Measure& inValue);
        void setTrack(Track& inValue);
        void setHand(int inHand);
        void setVoice(int inValue);
        void setSlot(int inValue);
        void setTime(double inValue);
        
        /// Calculates and returns the active clef for the MusicLocation
        ClefType getClef() const;
        
        /// Calculates and returns the active key signature for the MusicLocation
        KeySignatureType getKeySignature() const;
        
        /// Calculates the currently operative accidental for the current slot
        /// taking into consideration the key signature and any previously existing
        /// notes with accidentals on the same slot in the measure
        AccidentalType getAccidental() const;
        
        /// Returns the temporals that occupy the same measure, track,
        /// hand, voice and time.  i.e. return the temporals that are
        /// vertically timewise aligned in the same voice.
        TemporalRefVec getOccupants() const;
        
    private:
        MeasureRef mMeasure;
        TrackRef mTrack;
        int mHand;
        int mVoice;
        int mSlot;
        double mTime;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
