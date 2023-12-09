// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Duration.h"
#include "Pitch.h"
#include "TemporalType.h"

namespace komp
{
    /// \brief  Information to convert a Temporal to a MIDI event.
    ///
    /// \discussion PlaybackNote delivers the information and functions
    /// needed to convert Temporals into MIDI events.  This will
    /// prevent us from creating too many functions on Temporals
    /// solely for the purpose of MIDI translation. Instead we will
    /// attempt to deliver all the needed info in a single payload
    /// with the PlaybackNote class.
    ///
    class PlaybackNote
    {
    public:
        PlaybackNote();

        /// a PlaybackNote is considered silent if it is not intended to be
        /// heard in playback.  For example, a Rest (or any other slient)
        /// Temporal can return a silent PlaybackNote to indicate that it
        /// should not be represented in the midi data.  This allows us
        /// to provide a consistent interface for Temporals even though
        /// rests are outliers in that they do not affect the playback.
        /// If a PlaybackNote returns getIsSilent() == false, but returns
        /// a velocit/dynamic of 0, this means that the note is intended
        /// to be included in the MIDI playback data, but that it should
        /// have a velocity of zero.  Another place that we will see
        /// true is in the case of a Temporal that has an incoming tie.
        bool getIsSilent() const;

        /// the ticks per quarter note this object's time functions assume
        /// i.e. if the notation engine's Score is operating with a
        /// TimeBase of 240, then this function will return 240
        double getTimeBase() const;

        /// the start time of the note, relative the the beginning of the
        /// Measure, denominated by getTimeBase()
        double getStartTime() const;

        /// the length of this note, denominated by getTimeBase(). This
        /// will be adjusted to account for an outgoing Tie chain if one
        /// exists.  This will also be adjusted 
        double getTimeDuration() const;

        /// the stop time of the note, relative the the beginning of the
        /// Measure, denominated by getTimeBase()
        double getStopTime() const;

        /// the Pitch of this PlaybackNote (which can return MIDI pitch)
        const Pitch& getPitch() const;

        /// this is the on velocity denominated as a percentage and
        /// constrained to the range 0.0 to 1.0.  See getIsSilent() for
        /// more information regarding the meaning of 0.0.
        double getLoudness() const;

        /// this is the off velocity denominated as a percentage and
        /// constrained to the range 0.0 to 1.0.
        double getOffLoudness() const;

        /// converts getLoudness to MIDI's 0 - 127 velocity value
        int getVelocity() const;

        /// converts getOffLoudness to MIDI's 0 - 127 velocity value
        int getOffVelocity() const;

        ///@{
        /// See getters for descriptions
        void setIsSilent(bool inValue);
        void setTimeBase(double inTimeBase);
        void setStartTime(double inTime);
        void setPitch(Pitch inPitch);
        void addDuration(Duration inDuration);
        void setLoudness(double inValue);
        void setOffLoudness(double inValue);
        void setVelocity(int inValue);
        void setOffVelocity(int inValue);
        ///@}

    private:
        bool mIsSilent;
        double mStartTime;
        Pitch mPitch;
        std::vector<Duration> mDurations; /// size > 1 in case of tie chains
        double mLoudness;
        double mOffLoudness;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
