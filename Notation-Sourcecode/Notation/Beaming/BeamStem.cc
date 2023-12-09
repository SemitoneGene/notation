// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Self
#include "BeamStem.h"

// System
#include <algorithm>
#include <numeric>

// Local
#include "Forward.h"
#include "BeamSpecIntf.h"
#include "BeamedNoteIntf.h"
#include "BeamLineIntf.h"
#include "Chord.h"
#include "Note.h"
#include "Staff.h"

namespace komp
{
    /// The amount to back out of a stem length to prevent it
    /// from protruding through the beam. in pixels
    static constexpr const double STEM_BUTT_SLOP = 0.2;
    
    BeamStrategyIntfUPtr
    BeamStem::clone() const
    {
        return std::make_unique<BeamStem>(*this);
    }
    

    bool
    BeamStem::applyStrategy(BeamSpecIntf& outBeamSpec) const
    {
        
        if (outBeamSpec.getBeamLineCount() == 0 || !outBeamSpec.getIsValid()) {
            KOMP_THROW("bad state cannot proceed");
        }
        
        const auto& mainBeam = outBeamSpec.getBeamLine(0);

        int index = 0;
        int count = outBeamSpec.getBeamedNoteCount();

        for (const auto& beamedNote : outBeamSpec.getBeamedNotes()) {
            
            // hold on to the note ref for convenience
            auto& temporal = beamedNote->getNote();
            const auto stemInfo = temporal.getStemInfo();
            
            // Ask the note for its desired stem thickness
            beamedNote->setStemThickness(stemInfo.thickness);
            
            // Figure out approximately where the beam is in relation to the note to determine above or below
            const auto approxBeamY = mainBeam.getScoreYIntercept(temporal.getScoreX());
            const auto isAbove = approxBeamY < temporal.getScoreY();
            
            // now that we know whether the beam is above or below, get the stem's x-position
            // and exact y-position and use these to calculate the exact Y-intercept point
            const double stemRelativeX = isAbove ? stemInfo.xUp : stemInfo.xDown;
            const auto drawPoint = temporal.getDrawPoint();
            double stemScoreX = drawPoint.x + stemRelativeX;
            double exactYPoint = mainBeam.getScoreYIntercept(stemScoreX);

            // Extend a the stem to the far side of the beam instead
            // of stopping in the middle of the beam
            static constexpr const auto Offset = 0.5;
            const auto thickness = mainBeam.getThickness();
            
            if (isAbove) {
                exactYPoint -= Offset * thickness;
            } else {
                exactYPoint += Offset * thickness;
            }

            static constexpr const auto Magic = 0.33;
            
            // upon visual inspection, down stems are not as tight to the notehead as we want them, nudge them in.
            // Magic is a magic number. a larger number causes gaps where the stem does not connect to the beam,
            // a smaller number causes the stem to be too far from the notehead
            if (!isAbove) {
                stemScoreX += Magic * stemInfo.thickness;
            }

            // for stems other than the first and last, we need to back them off slightly to
            // avoid anti-aliasing poking through the beam (we call this stem butts).
            if (index != 0 && index != (count - 1)) {
                if (!isAbove) {
                    exactYPoint -= STEM_BUTT_SLOP;
                } else {
                    exactYPoint += STEM_BUTT_SLOP;
                }
            }

            // Set the stem's line end points
            auto& beamside = beamedNote->getStemBeamsidePoint();
            beamside.setScoreX(stemScoreX);
            beamside.setScoreY(exactYPoint);
            
            beamedNote->getStemNoteheadPoint().setScoreX(stemScoreX);
            
            double stemScoreY = 0.0;
            
            Note* note = nullptr;
            Chord* chord = nullptr;
            if (temporal.getItemTraits().getItemType() == ItemType::Note) {
                note = dynamic_cast<Note*>(&temporal);
                if (note) {
                    if (note->getIsChordMember()) {
                        chord = note->getChord();
                    }
                }
            }
            
            if (chord) {
                if (isAbove) {
                    stemScoreY = chord->getStemInfo().yUp;
                } else {
                    stemScoreY = chord->getStemInfo().yDown;
                }
            } else {
                if (isAbove) {
                    stemScoreY = stemInfo.yUp + drawPoint.y;
                } else {
                    stemScoreY = stemInfo.yDown + drawPoint.y;
                }
            }
            
            beamedNote->getStemNoteheadPoint().setScoreY(stemScoreY);

            // Tell the BeamIntf to draw the stem
            beamedNote->setDoDrawStem(true);

            ++index;
        }

        return true;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
