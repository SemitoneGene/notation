// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Geometry.h"

namespace komp
{
    struct NoteFrames
    {
        /// Returns a rect around the notehead only. for a rest, the head frame
        /// is the rest glyph. for a chord, the head frame is the bounding rect
        /// of all of the chord's noteheads
        Rect head;
        Rect accidental;
        Rect dots;
        Rect stem;
        Rect flags;
        Rect techniques;
        Rect arpeggiate;
        Rect drawableLegerLines;

        /// returns a rect around the notehead(s), accidental(s) and dot(s)
        inline Rect getCompleteHeadFrame() const
        {
            Rect result;
            result.initializeOrUnion(head);
            result.initializeOrUnion(accidental);
            result.initializeOrUnion(dots);
            return result;
        }

        /// returns a rect around everything (the entire temporal)
        inline Rect getCompleteFrame() const
        {
            Rect result;
            result.initializeOrUnion(head);
            result.initializeOrUnion(accidental);
            result.initializeOrUnion(dots);
            result.initializeOrUnion(stem);
            result.initializeOrUnion(flags);
            result.initializeOrUnion(techniques);
            result.initializeOrUnion(drawableLegerLines);
            result.initializeOrUnion(arpeggiate);
            return result;
        }
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
