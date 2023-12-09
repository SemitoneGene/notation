// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "ChordHead.h"

namespace komp
{
    class ChordNote;
    using ChordNotes = std::vector<ChordNote>;

    enum class RightLeft : bool
    {
        LEFT,
        RIGHT
    };

    class ChordNote
    {
    public:
        ChordNote(Note& inNote);
        
        Note& getNote() const;
        ChordHead chordHead;
        
        void update();
        
    private:
        NoteRef mNote;
    };
    
    /// less than operator for sorting chorded notes by
    /// slot ascending, midiPitch ascending, x ascending, address ascending
    bool operator<(const ChordNote& lhs, const ChordNote& rhs);
    bool operator>(const ChordNote& lhs, const ChordNote& rhs);
    bool operator==(const ChordNote& lhs, const ChordNote& rhs);
    bool operator!=(const ChordNote& lhs, const ChordNote& rhs);
    bool operator<=(const ChordNote& lhs, const ChordNote& rhs);
    bool operator>=(const ChordNote& lhs, const ChordNote& rhs);
    
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

