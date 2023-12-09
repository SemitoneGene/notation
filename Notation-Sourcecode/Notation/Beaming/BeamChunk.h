// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

namespace komp
{
    /// \brief  describes the bit of beam associated with a note
    ///
    /// \discussion for a given note, describes in a declarative
    /// way the beam segments as they relate to this note. If we
    /// iterate accross the notes and look at their BeamChunks,
    /// we can figure out what lines need to be drawn.
    ///
	enum class BeamChunk
	{
		Unknown,     ///< uninitialized or error state
		Begin,       ///< a non-fractional beam line begins on this note
		Continue,    ///< a non-fractional beam line continues through this note
		End,         ///< a non-fractional beam line ends on this note
		BrokenRight, ///< a fractional beam extends from this note to the right (forward)
		BrokenLeft,  ///< a fractional beam extends from this note to the left (backward)
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
