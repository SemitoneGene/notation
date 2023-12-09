// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

static constexpr const int MIDDLE_SLOT = 6;
static constexpr const double PIXELS_PER_INCH = 72;
static constexpr const double MILLIMETERS_PER_INCH = 25.4;

/// the minimum width of a measure's note area in pixels
static constexpr const double MINIMUM_MEASURE_NOTESPACE = 64.0;

/// Number of hands allowed in a track
static constexpr const int MAX_HANDS = 8;

/// Number of voices allowed in a track
static constexpr const int MAX_VOICES = 8;

/// Number of dots allowed on a rest or note (arbitrary)
static constexpr const int MAX_DOTS = 2;

/// Maximum value to be involved with a tuplet (arbitrary) for
/// example 19:3 or 10:19 is fine, but 20:x or x:20 is not allowed
static constexpr const int MAX_TUPLET_COUNT = 19;

/// Maximum number of tuplets allowed to affect a single temporal
/// (i.e. the maximum amount of nesting)
static constexpr const int MAX_TUPLET_NESTING = 1;

/// Lowest pitch is 35 pixels below base
static constexpr const int BASE_YPOS = 35;

/// Clocks per quarter note
static constexpr const double TIME_BASE = 240;

/// Range chosen arbitrarily
static constexpr const double MIN_TEMPO = 10;
static constexpr const double MAX_TEMPO = 400;
static constexpr const double DEFAULT_TEMPO = 100;

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
