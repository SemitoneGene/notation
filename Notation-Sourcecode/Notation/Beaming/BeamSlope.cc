// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Self
#include "BeamSlope.h"

// System
#include <numeric>
#include <algorithm>

// Local
#include "Forward.h"
#include "BeamSpecIntf.h"
#include "BeamedNoteIntf.h"
#include "BeamLineIntf.h"
#include "Note.h"
#include "Chord.h"
#include "Staff.h"

namespace komp
{
    struct BeamSlope::Data
    {
        Data(BeamSpecIntf& outBeamSpec)
        : beamSpec(outBeamSpec)
        , avgSlot(6)
        , maxDeltaY(2.0)
        , direction(0)
        , maxNote{nullptr}
        , minNote{nullptr}
        , leftNote{nullptr}
        , rightNote{nullptr}
        {
            
        }
        
        BeamSpecIntf& beamSpec;
        std::vector<int> slots;
        double avgSlot;
        Temporal* maxNote;
        Temporal* minNote;
        Temporal* leftNote;
        Temporal* rightNote;
        double maxDeltaY;
        char direction; // -1 downSlope, 0 flat, 1 upSlope
    };


    BeamStrategyIntfUPtr
    BeamSlope::clone() const
    {
        return std::make_unique<BeamSlope>(*this);
    }


    bool
    BeamSlope::applyStrategy(BeamSpecIntf& outBeamSpec) const
    {

        if (!outBeamSpec.getIsValid()
          || outBeamSpec.getBeamLineCount() <= 0
          || outBeamSpec.getBeamedNoteCount() <= 0) {
            KOMP_THROW("bad state cannot proceeed");
        }
        
        Data data{outBeamSpec};
        analyzeNotes(data);
        calculateYPositions(data);
        return true;
    }
    

    double
    BeamSlope::getMaxDeltaYSpaces(int numNotes) const
    {
        // derived from Elain Gould's rules
        switch (numNotes) {
            case 0: return 0.0;
            case 1: return 0.0;
            case 2: return 1.0;
            case 3: return 1.0;
            default: return 1.25;
        }
    }
    
    
    void
    BeamSlope::analyzeNotes(Data& ioData) const
    {
        const auto oldCount = ioData.beamSpec.getBeamedNoteCount();
        if (oldCount <= 0) {
            LOG_F(WARNING, "Empty beam note count");
            return;
        }
        
        int sum = 0;
        for (int i = 0; i < oldCount; ++i) {
            auto& note = ioData.beamSpec.getBeamedNote(i).getNote();
            const auto slot = note.getSlot();
            ioData.slots.emplace_back(slot);
            
            if (!ioData.maxNote || slot > ioData.maxNote->getSlot()) {
                ioData.maxNote = &note;
            }
            
            if (!ioData.minNote || slot < ioData.minNote->getSlot()) {
                ioData.minNote = &note;
            }
            
            if (i == 0) {
                ioData.leftNote = &note;
            } else if (i == oldCount - 1) {
                ioData.rightNote = &note;
            }
            sum += slot;
        }
        
        ioData.avgSlot = static_cast<double>(sum)
                       / static_cast<double>(oldCount);
        ioData.maxDeltaY = getMaxDeltaYSpaces(oldCount);

        const auto rightSlot = ioData.rightNote->getSlot();
        const auto leftSlot = ioData.leftNote->getSlot();
        
        if (rightSlot > leftSlot) {
            ioData.direction = 1;
        } else if (rightSlot < leftSlot) {
            ioData.direction = -1;
        }
    }
    
    
    void
    BeamSlope::calculateYPositions(Data& ioData) const
    {
        auto& mainBeam = ioData.beamSpec.getBeamLine(0);
        auto& noteA = mainBeam.getPointA().getItem();
        auto& noteB = mainBeam.getPointB().getItem();

        auto slotA = noteA.getSlot();
        auto slotB = noteB.getSlot();
        
        const auto slotSize = Staff::getSlotSize();
        
        if (noteA.getItemTraits().getItemType() == ItemType::Chord) {
            const auto& chord = noteA.as<Chord>();
            if (ioData.beamSpec.getBeamPlacement() == BeamPlacement::Above) {
                slotA = chord.getTopNote().getSlot();
            } else if (ioData.beamSpec.getBeamPlacement() == BeamPlacement::Below) {
                slotA = chord.getBottomNote().getSlot();
            }
        }
        
        if (noteB.getItemTraits().getItemType() == ItemType::Chord) {
            const auto& chord = noteB.as<Chord>();
            if (ioData.beamSpec.getBeamPlacement() == BeamPlacement::Above) {
                slotB = chord.getTopNote().getSlot();
            } else if (ioData.beamSpec.getBeamPlacement() == BeamPlacement::Below) {
                slotB = chord.getBottomNote().getSlot();
            }
        }
        
        const auto absDelta = std::abs(slotB - slotA);
        const auto minVal = std::min(ioData.maxDeltaY, static_cast<double>(absDelta));
        const auto yTraversalSlots = minVal * static_cast<double>(ioData.direction);
        const auto yTraversalPixels = yTraversalSlots * (-1.0 * slotSize);
        
        const auto yEndpointPixels = noteA.getScoreY() + yTraversalPixels;
        mainBeam.getPointA().setYSlots(0);
        mainBeam.getPointB().setScoreY(yEndpointPixels);
        
        for (int i = 1; i < ioData.beamSpec.getBeamLineCount(); ++i) {
            
            auto& beamLine = ioData.beamSpec.getBeamLine(i);
            
            auto& pointA = beamLine.getPointA();
            auto& pointB = beamLine.getPointB();
            
            const auto aX = pointA.getScoreX();
            const auto aY = mainBeam.getScoreYIntercept(aX);
            const auto bX = pointB.getScoreX();
            const auto bY = mainBeam.getScoreYIntercept(bX);
            
            pointA.setScoreY(aY);
            pointB.setScoreY(bY);
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
