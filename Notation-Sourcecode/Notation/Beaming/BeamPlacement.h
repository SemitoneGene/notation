// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    /// describes how the beam is placed in relation to its notes
    enum class BeamPlacement
    {
        Unknown,         ///< Bad or uninitialized state
        Above,           ///< All of the beam lines are above all of the beamed notes
        Below,           ///< All of the beam lines are below all of the beamed notes
        MixedAboveBelow, ///< The beam is above the first note, but below the last note.
        MixedBelowAbove, ///< The beam is below the first note, but above the last note.
        MixedAboveAbove, ///< The beam is above both the first and last note, but one or more of the middle notes is in the other direction
        MixedBelowBelow, ///< The beam is below both the first and last note, but one or more of the middle notes is in the other direction
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
