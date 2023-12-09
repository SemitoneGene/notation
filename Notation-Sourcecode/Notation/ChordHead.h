// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "StemDirection.h"
#include "Geometry.h"

namespace komp
{
    enum class ChordSide
    {
        Unknown, ///< The notehead's position is undetermined
        Left,    ///< The notehead is to the left of the stem
        Right    ///< The notehead is to the right of the stem
    };

    class ChordHead;
    using ChordHeads = std::vector<ChordHead>;
    using ChordHeadPtr = ChordHead*;
    using ChordHeadPtrs = std::vector<ChordHeadPtr>;

    class ChordHead
    {
    public:
        ChordHead(int inSlot, ItemID inItemID);
        ItemID getItemID() const;
        int slot;
        ChordSide side;
        bool hasAccidental;
        Rect accidentalFrame;
        double accidentalOffset;
        Rect noteheadFrame;
        Point noteDrawPoint;
        Point chordDrawPoint;
        std::vector<Rect> noteheadFrames;

        /// calculates the side of the stem that the chord heads should be on
        /// result - outChordHeads members have ChordSide updated, also sorts
        static void calculateHeadSides(ChordHeadPtrs& outChordHeads,
                                       StemDirection inStemDirection);

        /// calculates the side of the stem that the chord heads should be on
        /// result - outChordHeads members have ChordSide updated, also sorts
        static void calculateHeadSidesNotPtr(ChordHeads& outChordHeads,
                                             StemDirection inStemDirection);

        /// assumes that the accidentals are in their normal single-note positions'
        /// and shuffles them outward to avoid collision
        static void calculateAccidentalPositionsNotPtr(ChordHeads& outChordHeads);

        /// assumes that the accidentals are in their normal single-note positions'
        /// and shuffles them outward to avoid collision
        static void calculateAccidentalPositions(ChordHeadPtrs& outChordHeads);

        static void sortHeads(ChordHeads& outChordHeads);
        static StemDirection calculateStemDirection(const ChordHeadPtrs& inChordHeads);
        static ChordSide oppositeSide(const ChordSide& inSide);

    private:
        static void moveAccidental(double inAmount, ChordHead& outChordHead);

    private:
        ItemID mItemID;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
