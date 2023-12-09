// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <cmath>
#include <cstring>
#include <memory>
#include <vector>

// Logging
#include "Logging.h"

// Kern
#include <kern/ForwardDeclare.h>
#include <kern/TargetOs.h>

namespace komp
{
using ItemID = uint64_t;
inline ItemID initializeItemID() { return std::numeric_limits<ItemID>::max(); }
inline int roundToInt(double inValue) { return static_cast<int>(std::ceil(inValue - 0.5)); }

enum class AccidentalType;
enum class ArpeggiateType;
enum class BarlineLocation;
enum class BarlineType;
enum class ClefType;
enum class MarkType;
enum class NoteheadType;
enum class PlacementType;
enum class StaffType;
enum class TemporalType;

template<typename FLOATING_POINT>
inline bool areFloatsEqual(FLOATING_POINT a, FLOATING_POINT b, FLOATING_POINT epsilon) {
    return std::abs(a - b) < epsilon;
}

template<typename FLOATING_POINT>
inline bool areFloatsEqual(FLOATING_POINT a, FLOATING_POINT b) {
    return std::abs(a - b) < static_cast<FLOATING_POINT>(0.000000001); // guess
}

template<typename T>
inline bool areVectorsEqual(const std::vector<T>& a,
                            const std::vector<T>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    
    for(size_t i = 0; i < a.size(); i++) {
        if (!(a[i] == b[i])) {
            return false;
        }
    }
    
    return true;
}

// Uncomment the following definition to see logging of why the drawable caches are being dirtied
// #define MATT_IS_INSANE

#ifdef MATT_IS_INSANE
#ifndef __FILENAME__
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#endif

#ifndef KOMP_FILE_AND_LINE
#define KOMP_FILE_AND_LINE                  \
std::string (                               \
std::string( __FILENAME__ )                 \
+ std::string(" (")                         \
+ std::string( std::to_string(__LINE__ ) )  \
+ std::string(") ")                         \
+ std::string( __FUNCTION__ ))
#endif

#define KOMP_SET_DIRTY mCache.setIsDirty(KOMP_FILE_AND_LINE);
#else
#define KOMP_SET_DIRTY mCache.setIsDirty();
#endif

KOMP_FORWARD_DECLARE(AttachedItem);
KOMP_FORWARD_DECLARE(BeamedNoteIntf);
KOMP_FORWARD_DECLARE(BeamHeightIntf);
KOMP_FORWARD_DECLARE(BeamIntf);
KOMP_FORWARD_DECLARE(BeamLineIntf);
KOMP_FORWARD_DECLARE(BeamSpecIntf);
KOMP_FORWARD_DECLARE(BeamStrategyIntf);
KOMP_FORWARD_DECLARE(BeamStrokeData);
KOMP_FORWARD_DECLARE(Chord);
KOMP_FORWARD_DECLARE(ChordSymbol);
KOMP_FORWARD_DECLARE(Clef);
KOMP_FORWARD_DECLARE(ClefData);
KOMP_FORWARD_DECLARE(Clipboard);
KOMP_FORWARD_DECLARE(Command);
KOMP_FORWARD_DECLARE(CommandIntf);
KOMP_FORWARD_DECLARE(CurrentState);
KOMP_FORWARD_DECLARE(Curve);
KOMP_FORWARD_DECLARE(Drawable);
KOMP_FORWARD_DECLARE(DrawableAccidental);
KOMP_FORWARD_DECLARE(DrawableBarline);
KOMP_FORWARD_DECLARE(DrawableBracket);
KOMP_FORWARD_DECLARE(DrawableContext);
KOMP_FORWARD_DECLARE(DrawableLayer);
KOMP_FORWARD_DECLARE(Ending);
KOMP_FORWARD_DECLARE(FileMediatorIntf);
KOMP_FORWARD_DECLARE(Item);
KOMP_FORWARD_DECLARE(ItemObserver);
KOMP_FORWARD_DECLARE(ItemRegistry);
KOMP_FORWARD_DECLARE(ItemTraits);
KOMP_FORWARD_DECLARE(KeySignature);
KOMP_FORWARD_DECLARE(LineSpanner);
KOMP_FORWARD_DECLARE(Lyric);
KOMP_FORWARD_DECLARE(Mark);
KOMP_FORWARD_DECLARE(MarkPlacement);
KOMP_FORWARD_DECLARE(Measure);
KOMP_FORWARD_DECLARE(MeasureNumberInfo);
KOMP_FORWARD_DECLARE(MeasureObserver);
KOMP_FORWARD_DECLARE(MIDIController);
KOMP_FORWARD_DECLARE(MIDIEvent);
KOMP_FORWARD_DECLARE(MusicLocation);
KOMP_FORWARD_DECLARE(Note);
KOMP_FORWARD_DECLARE(Octave);
KOMP_FORWARD_DECLARE(Page);
KOMP_FORWARD_DECLARE(Player);
KOMP_FORWARD_DECLARE(Rect);
KOMP_FORWARD_DECLARE(Rest);
KOMP_FORWARD_DECLARE(Score);
KOMP_FORWARD_DECLARE(ScoreInfo);
KOMP_FORWARD_DECLARE(ScoreLayout);
KOMP_FORWARD_DECLARE(ScoreLayoutInfo);
KOMP_FORWARD_DECLARE(ScoreObserver);
KOMP_FORWARD_DECLARE(Selection);
KOMP_FORWARD_DECLARE(NoteSpacerIntf);
KOMP_FORWARD_DECLARE(Spanner);
KOMP_FORWARD_DECLARE(Spatial);
KOMP_FORWARD_DECLARE(RawAccidental);
KOMP_FORWARD_DECLARE(RehearsalMark);
KOMP_FORWARD_DECLARE(RelativePoint);
KOMP_FORWARD_DECLARE(Staff);
KOMP_FORWARD_DECLARE(StaffSystem);
KOMP_FORWARD_DECLARE(SystemBracket);
KOMP_FORWARD_DECLARE(SystemBracketInfo);
KOMP_FORWARD_DECLARE(TempoMark);
KOMP_FORWARD_DECLARE(Temporal);
KOMP_FORWARD_DECLARE(Tie);
KOMP_FORWARD_DECLARE(Track);
KOMP_FORWARD_DECLARE(TrackInfo);
KOMP_FORWARD_DECLARE(TupletIntf);
KOMP_FORWARD_DECLARE(UndoManagerObserver);
KOMP_FORWARD_DECLARE(Wedge);
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
