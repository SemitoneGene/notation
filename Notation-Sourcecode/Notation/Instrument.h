// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>
#include <string>
#include <vector>

// Local
#include "ClefType.h"
#include "GroupBracketType.h"
#include "Pitch.h"
#include "SoundID.h"
#include "StaffType.h"

namespace komp
{
    /// \brief Represents a musical instrument and its metadata
    class Instrument
    {
    public:        
        Instrument();

        /// The instrument's displayed name in the score
        std::string name;
                
        /// The instrument's displayed abbreviation in the score
        std::string abbreviation;
        
        /// Instrument's family name for aggregation, e.g.
        /// Woodwinds, Brass, Strings, etc.
        std::string family;
                
        /// numStaves is determined by the size of the Clefs
        /// collection. most will be 1, but e.g. Piano is 2
        int getNumStaves() const;
        
        /// The default clef for each staff. most instruments will
        /// have only one, but piano (for example) will have 2
        std::vector<ClefType> startingClefs;
        
        /// The collection of clefs that is considered appropriate
        /// for use with this type of instrument. for example viola
        /// will have [C, G], Violin [G], Cello [F, C_LINE4, G]
        std::vector<ClefType> allowableClefs;
        
        /// The clef at which the instrument notation is displayed at
        ClefType transposingClef{ClefType::UNKNOWN};
        
        /// The clef at which the instrument notation  is played at
        ClefType concertClef{ClefType::UNKNOWN};

        /// The chromatic transposition steps from middle C
        int transposition{0};
        
        /// For most signle staff-instruments this will be 'None'
        /// but for grand staff instruments it will be 'PianoBrace'
        GroupBracketType instrumentBracket{GroupBracketType::None};

        /// get the lowest note on the instrument, in concert pitch
        const Pitch& getRangeLow() const;

        /// alter the lowest note (concert pitch)
        Pitch& getRangeLow();
        
        /// get the highest note on the instrument, in concert pitch
        const Pitch& getRangeHigh() const;
        
        /// alter the highest note (concert pitch)
        Pitch& getRangeHigh();

        /// get the interop, semantic sound identifier, i.e. MusicXML <instrument-sound>
        SoundID getSoundID() const;

        /// set the interop, semantic sound identifier, i.e. MusicXML <instrument-sound>
        void setSoundID(SoundID inSoundID);
        
        /// Apply a localization to the instrument info
        void applyLocalization(std::string_view inName,
                               std::string_view inAbbreviation,
                               std::string_view inFamily);

        /// Get the staff type
        StaffType getStaffType() const;

        /// Set the staff type
        void setStaffType(StaffType inStaffType);

        /// Get the number of staff lines in the staff
        int getStaffLines() const;

        /// Set the number of staff lines in the staff
        void setStaffLines(int inLineCount);
        
    private:
        Pitch mRangeLow{PitchStep::A, -1, 0};
        Pitch mRangeHigh{PitchStep::C, 9, 0};
        SoundID mSoundID{SoundID::KEYBOARD_PIANO};
        StaffType mStaffType{StaffType::NOTATION};
        int mStaffLines{5};
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
