// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "TemporalType.h"

namespace komp
{
/// Previously we were getting the stem thickness from the Engraver, but the
/// Engraver "scales" its return values by some weirdness that came from
/// MusicXML.  So for now we will just return this const. pixels.
static constexpr const double STEM_THICKNESS = 0.5;

/// by default, during playback, Temporals will sustain for this
/// percentage of their mathematical total duration
static constexpr const double DEFAULT_DURATION_PERCENTAGE = 0.95;

/// distance between the notehead frame.right and the duration dot
/// frame.left in staff spaces
static constexpr const double SPACES_BEFORE_FIRST_DOT = 0.25;

/// distance between dots when there is more than one duration dot
/// in staff spaces
static constexpr const double SPACES_BETWEEN_DOTS = 0.125;

/// distance between the right side of a flat and the left side of
/// the notehead
static constexpr const double ACCIDENTAL_FLAT_SPACES = 0.39;

/// distance between the right side of a double-flat and the left side
/// of the notehead
static constexpr const double ACCIDENTAL_DOUBLE_FLAT_SPACES = 0.59;

/// distance between the right side of a sharp accidental and the left
/// side of the notehead
static constexpr const double ACCIDENTAL_SHARP_SPACES = 0.49;

/// distance between the right side of a double-sharp accidental and the
/// left side of the notehead
static constexpr const double ACCIDENTAL_DOUBLE_SHARP_SPACES = 0.46;

/// distance between the right side of a natural accidental and the
/// left side of the notehead
static constexpr const double ACCIDENTAL_NATURAL_SPACES = 0.40;

/// Distance between the left side of a note or accidental and the right side of the arpeggiate
static constexpr const double ARPEGGIATE_SPACE = 2.6;

/// the minimum amount of space allowed between the left of the
/// current Temporals's frame and the right of the previous
/// Temporal's frame.  in staff spaces
static constexpr const double MINIMUM_TEMPORAL_SPACES_LEFT = 0.1;

/// the minimum amount of space allowed between the right of the
/// current Temporals's frame and the left of the next Temporal's
/// frame.  in staff spaces
static constexpr const double MINIMUM_TEMPORAL_SPACES_RIGHT = 0.5;

/// the temporal type of a raw (stemless) note
static constexpr const TemporalType RAW_TEMPORAL_TYPE = TemporalType::SIXTY_FOURTH;

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
