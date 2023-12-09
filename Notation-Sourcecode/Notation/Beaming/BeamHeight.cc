// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Self
#include "BeamHeight.h"

// System
#include <numeric>
#include <algorithm>

// Local
#include "Forward.h"
#include "BeamSpecIntf.h"
#include "BeamedNoteIntf.h"
#include "BeamLineIntf.h"
#include "Note.h"
#include "Staff.h"
#include "Chord.h"

namespace komp
{
    static constexpr const double DOUBLES_SLOP = 0.00001;
    
    BeamStrategyIntfUPtr
    BeamHeight::clone() const
    {
        return std::make_unique<BeamHeight>(*this);
    }
    
    
    bool
    BeamHeight::applyStrategy(BeamSpecIntf& outBeamSpec) const
    {
        if (outBeamSpec.getBeamLineCount() < 1) {
            KOMP_THROW("no beam lines");
        }
        
        const int maxLevel = outBeamSpec.getMaxLevel();
        const int mainBeamDistance = calculateStemLength(maxLevel);
        const bool isAbove = outBeamSpec.getBeamPlacement() == BeamPlacement::Above;
        
        for (int i = 0; i < outBeamSpec.getBeamLineCount(); ++i) {
            
            auto& beamLine = outBeamSpec.getBeamLine(i);
            const int level = beamLine.getLevel();
            double slotDistance = mainBeamDistance - (level * BEAM_SPACING * 2);

            if (!isAbove) {
                slotDistance *= -1;
            }
            
            beamLine.getPointA().addYSlots(slotDistance);
            beamLine.getPointB().addYSlots(slotDistance);
        }
        
        for (const auto& beamLine : outBeamSpec.getBeamLines()) {

            double lineX1 = beamLine->getPointA().getScoreX();
            double lineX2 = beamLine->getPointB().getScoreX();
            
            // TODO: I don't understand why this is happening
            if (lineX1 > lineX2) {
                std::swap(lineX1, lineX2);
            }
            
            for (const auto& beamedNote : outBeamSpec.getBeamedNotes()) {
                const auto& note = beamedNote->getNote();
                const double noteX = note.getScoreX();
                const auto stemInfo = note.getStemInfo();
                const double noteStemX = noteX + (isAbove ? stemInfo.xUp : stemInfo.xDown);
                
                // skip this note if it is not under the beam
                if (noteStemX + DOUBLES_SLOP < lineX1 || noteStemX - DOUBLES_SLOP > lineX2) {
                    continue;
                }
                double minimumDistance = MINIMUM_DISTANCE_SPACES * Staff::getSpacing();
                
                if (isAbove) {
                    minimumDistance *= -1;
                }
                
                const auto& clearanceTemporal = getClearanceTemporal(*beamedNote, outBeamSpec);
                const double actualDistance = beamLine->getScoreYIntercept(noteX) - clearanceTemporal.getScoreY();
                
                const bool doAdjustAbove = isAbove && actualDistance > minimumDistance;
                const bool doAdjustBelow = !isAbove && actualDistance < minimumDistance;
                const bool doAdjust = doAdjustAbove || doAdjustBelow;
                
                if (!doAdjust) {
                    continue;
                }
                
                const double adjustmentPixels = minimumDistance - actualDistance;
                
                for (const auto& innerLoopBeamLine : outBeamSpec.getBeamLines()) {
                    innerLoopBeamLine->getPointA().addYPixels(adjustmentPixels);
                    innerLoopBeamLine->getPointB().addYPixels(adjustmentPixels);
                }
            }
        }
        return true;
    }
    
    
    double
    BeamHeight::calculateStemLength(int inMaxLevel) const
    {
        const double slotsPerSpace = 2.0;
        const double idealStemLength = IDEAL_STEM_LENGTH_SPACES * slotsPerSpace;
        
        if (inMaxLevel <= 1) {
            return idealStemLength;
        }
        
        const int extraBeams = inMaxLevel - 1;
        const double extraLength = static_cast<double>(extraBeams) * BEAM_SPACING * slotsPerSpace;
        
        return idealStemLength + extraLength;
    }
    
    
    Temporal&
    BeamHeight::getClearanceTemporal(const BeamedNoteIntf& inBeamedNote,
                                        const BeamSpecIntf& inBeamSpec) const
    {
        return getClearanceTemporal(inBeamedNote.getNote(),
                                    inBeamSpec);
    }
    
    
    Temporal&
    BeamHeight::getClearanceTemporal(Temporal& inTemporal,
                                        const BeamSpecIntf& inBeamSpec) const
    {
        const auto isAbove = inBeamSpec.getBeamPlacement() == BeamPlacement::Above;
        
        if (inTemporal.getItemTraits().getItemType() == ItemType::Chord) {
            auto& clearanceChord = inTemporal.as<Chord>();
            if (isAbove) {
                return clearanceChord.getTopNote();
            } else {
                return clearanceChord.getBottomNote();
            }
        }
        return inTemporal;
    }
    
    
    Temporal&
    BeamHeight::getClearanceTemporal(Item& inItem,
                                        const BeamSpecIntf& inBeamSpec) const
    {
        KOMP_ASSERT(inItem.is<Temporal>());
        auto& temporal = inItem.as<Temporal>();
        return getClearanceTemporal(temporal, inBeamSpec);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
