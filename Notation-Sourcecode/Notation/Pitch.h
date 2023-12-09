// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <map>
#include <string>

// Local
#include "AccidentalType.h"

namespace komp
{
    enum class ClefType;
    
    enum class PitchStep
    {
        C = 0,
        D = 1,
        E = 2,
        F = 3,
        G = 4,
        A = 5,
        B = 6
    };
    
    class Pitch
    {
    public:
        Pitch();
        Pitch(PitchStep inStep, int inOctave, int inAlter);

        // Calculates step, octave and alter from the given
        // slot, clef and midi-pitch
        Pitch(ClefType clef, int slot, int midiPitch);

        // Calculates step and octave from the given slot
        Pitch(ClefType clef, int slot);
        
        // Calculates step and octave and alter
        Pitch(ClefType clef, int slot, AccidentalType inAccidental);

        PitchStep step{PitchStep::C};
        int octave{4};
        int alter{0};

        // c = 0, c# = 1, Db = 1, D = 2, etc.
        int chromaticValue() const;
        
        int getMidiNote() const;
        int slot(ClefType clef) const;
        
        static constexpr const int MIDDLE_C_MIDI_VALUE = 60;
        static int midiNoteForSlot(int slot, ClefType clef);
        
        /// returns the pitch in 'scientific pitch notation' i.e.
        /// C4 for middle C, D5, Eb5, etc.
        std::string getString() const;
        
        /// parses the pitch from 'scientific pitch notation' i.e.
        /// C4 for middle C, D5, D#5, etc.
        bool setString(const std::string& inString);
        
        /// resets the pitch to an equivalent setting which makes the
        /// alter value as close to zero as possible
        void minimizeAlter();

        /// when passing through C raises the octave value.  ignores alter value.
        void incrementStepwise();
        
        /// when passing through C lowers the octave value.  ignores alter value.
        void decrementStepwise();
        
        /// not enharmonically safe, makes the pidi pitch one step higher while
        /// keeping the alter value as close to zero as possible
        void incrementSemitone();
        
        /// not enharmonically safe, makes the pidi pitch one step higher while
        /// keeping the alter value as close to zero as possible
        void decrementSemitone();

        /// for the given alter, return the appropriate AccidentalType;
        AccidentalType getAccidental() const;

    private:
        int slotDistanceFromMiddleC() const;
        
        // Take data about a komp note and convert to Pitch
        void setUsingKompNoteInfo(ClefType clef, int slot, int midiPitch);

        static PitchStep midiNoteToStep(int midiNote, int alter);
        
        static const std::map<PitchStep, int> sStepSemitoneMap;
        static const std::map<int, PitchStep> sSemitoneStepMap;
    };

    /// this is purely a C++ thing, 'less than' does not imply that the
    /// pitch is lower, it is just a convenient comparison operator for
    /// equality, sorting and other C++ purposes
    bool operator<(const Pitch& lhs, const Pitch& rhs);
    bool operator>(const Pitch& lhs, const Pitch& rhs);

    bool operator==(const Pitch& lhs, const Pitch& rhs);
    bool operator!=(const Pitch& lhs, const Pitch& rhs);

    bool operator<=(const Pitch& lhs, const Pitch& rhs);
    bool operator>=(const Pitch& lhs, const Pitch& rhs);

    std::ostream& operator<<(std::ostream& os, const Pitch& pitch);
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
