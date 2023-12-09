// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "NoteheadType.h"
#include "TemporalType.h"

namespace komp
{
    /// \brief  values needed when determining how a stem should be
    /// created for a note, i.e. where to attach it, it's thickness
    ///
    class StemInfo
    {
    public:
        /// The position where a stem should attach to a notehead,
        /// in pixels, relative to the temporal's
        /// getScoreX, when the stem is up.
        double xUp;

        /// The position where a stem should attach to a notehead,
        /// in pixels, relative to the temporal's
        /// getScoreX, when the stem is down.
        double xDown;

        /// The position where a stem should attach to a notehead,
        /// in pixels, relative to the temporal's
        /// getScoreY, when the stem is up.
        double yUp;

        /// The position where a stem should attach to a notehead,
        /// in pixels, relative to the temporal's
        /// getScoreY, when the stem is down.
        double yDown;

        /// The thickness of the stem
        double thickness;

        /// The slot of the temporal that the stem is attached to.
        /// in the case of a chord this is the slot of the note
        /// that is at the oppiset end of stem direction, i.e.
        /// the bottom notehead if the stem is up
        int attachSlot;

        /// Same as attach slot, but precise for any additionsl y
        // offset from the exact slot location if the stem is up
        double attachSlotPreciseUp;

        /// Same as attach slot, but precise for any additionsl y
        // offset from the exact slot location if the stem is down
        double attachSlotPreciseDown;

        /// The slot of the temporal where the stem leaves its
        /// temporals.  For example, in the case of a single
        /// notehead, this value will be the same as attachSlot.
        /// In the case of a chord, this value will be the
        /// note that is furthest from the attachSlot.
        int escapeSlot;

        /// Same as escape slot, but precise for any additionsl y
        // offset from the exact slot location if the stem is up
        double escapeSlotPreciseUp;

        /// Same as escape slot, but precise for any additionsl y
        // offset from the exact slot location if the stem is down
        double escapeSlotPreciseDown;

        /// The desired default length, in staff spaces, for the
        /// stem if no other rules or considerations exist.
        double idealLength;
        static constexpr const double DEFAULT_IDEAL_LENGTH = 3.5;

        /// The minimum length for the stem in staff spaces.
        double minLength;
        static constexpr const double DEFAULT_MIN_LENGTH = 2.5;

        /// The maximum length for the stem in staff spaces.
        double maxLength;
        static constexpr const double DEFAULT_MAX_LENGTH = 256.0;

        /// The staff where the Temporal resides
        Staff* staff;

        /// The temporal type that is being stemmed
        TemporalType temporalType;
        
        /// will the stem have a flag on it
        bool hasFlag;

        /// The font size to use when drawing the note flags
        double flagsFontSize;

        /// The Temporal's score y
        double temporalY;

        /// in some cases a Temporal may defer ownership of a stem to
        /// another object, for example a beamed note will allow the beam
        /// to own the Stem and its policy. isStemOwner will be set to
        /// false in such a case.
        bool isStemOwner;

        StemInfo()
        : xUp{0.0}
        , xDown{0.0}
        , yUp{0.0}
        , yDown{0.0}
        , thickness{0.0}
        , attachSlot{0}
        , attachSlotPreciseUp{0.0}
        , attachSlotPreciseDown{0.0}
        , escapeSlot{0}
        , escapeSlotPreciseUp{0.0}
        , escapeSlotPreciseDown{0.0}
        , idealLength{DEFAULT_IDEAL_LENGTH}
        , minLength{DEFAULT_MIN_LENGTH}
        , maxLength{DEFAULT_MAX_LENGTH}
        , staff{nullptr}
        , temporalType{TemporalType::QUARTER}
        , hasFlag{false}
        , flagsFontSize{0.0}
        , temporalY{0.0}
        , isStemOwner{true}
        {
            
        }
    };

    // Staff* is ignored
    inline bool operator==( const StemInfo& a, const StemInfo& b ) {
        return areFloatsEqual(a.xUp, b.xUp) &&
            areFloatsEqual( a.xDown, b.xDown ) &&
            areFloatsEqual( a.yUp, b.yUp ) &&
            areFloatsEqual( a.yDown, b.yDown ) &&
            areFloatsEqual( a.thickness, b.thickness ) &&
            a.attachSlot == b.attachSlot &&
            areFloatsEqual( a.attachSlotPreciseUp, b.attachSlotPreciseUp ) &&
            areFloatsEqual( a.attachSlotPreciseDown, b.attachSlotPreciseDown ) &&
            a.escapeSlot == b.escapeSlot &&
            areFloatsEqual( a.escapeSlotPreciseUp, b.escapeSlotPreciseUp ) &&
            areFloatsEqual( a.escapeSlotPreciseDown, b.escapeSlotPreciseDown ) &&
            areFloatsEqual( a.idealLength, b.idealLength ) &&
            areFloatsEqual( a.minLength, b.minLength ) &&
            areFloatsEqual( a.maxLength, b.maxLength ) &&
            a.temporalType == b.temporalType &&
            a.hasFlag == b.hasFlag &&
            areFloatsEqual( a.flagsFontSize, b.flagsFontSize ) &&
            areFloatsEqual( a.temporalY, b.temporalY ) &&
            a.isStemOwner == b.isStemOwner;
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
