// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Self
#include "BeamLineCreator.h"

// Local
#include "BeamSpecIntf.h"
#include "BeamLine.h"
#include "Note.h"
#include "Staff.h"
#include "Temporal.h"

namespace komp
{
    
    struct BeamLineCreator::LocalData
    {
        BeamSpecIntf& beamSpec;
        int currentBeamLevel;
        int maxBeamLevel;
        const BeamedNoteIntf* startNoteP;
        const BeamedNoteIntf* endNoteP;
        
        LocalData(BeamSpecIntf& outBeamSpec)
        : beamSpec(outBeamSpec)
        , currentBeamLevel(-1)
        , maxBeamLevel(-1)
        , startNoteP(nullptr)
        , endNoteP(nullptr)
        {
            
        }
    };
    
 
    BeamStrategyIntfUPtr
    BeamLineCreator::clone() const
    {
        return std::make_unique<BeamLineCreator>(*this);
    }
    

    bool
    BeamLineCreator::applyStrategy(BeamSpecIntf& outBeamSpec) const
    {
        LocalData data{outBeamSpec};
        data.beamSpec.clearBeamLines();
        data.maxBeamLevel = data.beamSpec.getMaxLevel();
        
        for (data.currentBeamLevel = 0;
             data.currentBeamLevel <= data.maxBeamLevel;
             ++data.currentBeamLevel) {
            calculateBeamLinesAtLevel(data);
        }
        
        if (data.beamSpec.getBeamLineCount() == 0) {
            LOG_F(ERROR, "No beam lines were calculated");
            return false;
        }
        
        adjustEndpointBeamXValues(data);
        return true;
    }
    
    
    void
    BeamLineCreator::calculateBeamLinesAtLevel(LocalData& data) const
    {
        data.startNoteP = nullptr;
        
        for (const auto& beamedNote : data.beamSpec.getBeamedNotes()) {
            
            if (data.currentBeamLevel >= beamedNote->getCount()) {
                continue;
            }
            
            const auto chunkType = beamedNote->getBeamChunk(data.currentBeamLevel);
            
            if (chunkType == BeamChunk::Begin) {
            
                data.startNoteP = beamedNote.get();
                data.endNoteP = nullptr;
            
            } else if (chunkType == BeamChunk::End) {
                
                // create a complete line so long as we
                // have already encountered a start chunk
                if (data.startNoteP) {
                    
                    data.endNoteP = beamedNote.get();
                    createUnbrokenLine(data);
                }
                
            } else if (chunkType == BeamChunk::BrokenLeft) {

                // set the pointers
                data.startNoteP = beamedNote.get();
                data.endNoteP = beamedNote.get();
                
                // create the fractional beam
                createBrokenLeft(data);
            
            } else if (chunkType == BeamChunk::BrokenRight) {

                // set the pointers
                data.startNoteP = beamedNote.get();
                data.endNoteP = beamedNote.get();
                
                // create the fractional beam
                createBrokenRight(data);
            }
        }
    }
    
    
    void
    BeamLineCreator::createUnbrokenLine(LocalData& data) const
    {
        auto pair = findBeamsideNotes(data);
        BeamLine beamLine{*pair.first, *pair.second};
        const auto spacing = Staff::getSpacing();
        const auto beamThickness = spacing / 2.0;
        beamLine.setThickness(beamThickness);
        beamLine.setLevel(data.currentBeamLevel);
        beamLine.setIsAbove(data.beamSpec.getBeamPlacement() == BeamPlacement::Above);
        beamLine.getPointA().setX(0);
        beamLine.getPointA().setYSlots(0);
        data.beamSpec.addBeamLine(beamLine);
        data.startNoteP = nullptr;
        data.endNoteP = nullptr;
    }
    
    
    void
    BeamLineCreator::createBrokenRight(LocalData& data) const
    {
        data.endNoteP = data.startNoteP;
        auto pair = findBeamsideNotes(data);
        BeamLine beamLine{*pair.first, *pair.second};
        const double spacing = Staff::getSpacing();
        const double beamThickness = spacing / 2.0;
        beamLine.setThickness(beamThickness);
        beamLine.setLevel(data.currentBeamLevel);
        beamLine.getPointB().addX(FRACTIONAL_BEAM_LENGTH);
        beamLine.setIsAbove(data.beamSpec.getBeamPlacement() == BeamPlacement::Above);
        data.beamSpec.addBeamLine(beamLine);
        data.startNoteP = nullptr;
        data.endNoteP = nullptr;
    }

    
    void
    BeamLineCreator::createBrokenLeft(LocalData& data) const
    {
        data.endNoteP = data.startNoteP;
        auto pair = findBeamsideNotes(data);
        BeamLine beamLine{*pair.first, *pair.second};
        const double spacing = Staff::getSpacing();
        const double beamThickness = spacing / 2.0;
        beamLine.setThickness(beamThickness);
        beamLine.setLevel(data.currentBeamLevel);
        beamLine.getPointA().addX(-1 * FRACTIONAL_BEAM_LENGTH);
        beamLine.setIsAbove(data.beamSpec.getBeamPlacement() == BeamPlacement::Above);
        data.beamSpec.addBeamLine(beamLine);
        data.startNoteP = nullptr;
        data.endNoteP = nullptr;
    }
    
    
    void
    BeamLineCreator::adjustEndpointBeamXValues(LocalData& data) const
    {
        for (auto& beamLine : data.beamSpec.getBeamLines()) {
            auto& a = beamLine->getNoteA();
            auto& b = beamLine->getNoteB();
            const auto scoreX1 = findStemX(a, data.beamSpec.getBeamPlacement());
            const auto scoreX2 = findStemX(b, data.beamSpec.getBeamPlacement());
            const auto currentX1 = a.getScoreX();
            const auto currentX2 = b.getScoreX();
            const auto deltaX1 = scoreX1 - currentX1;
            const auto deltaX2 = scoreX2 - currentX2;
            beamLine->getPointA().addX(deltaX1);
            beamLine->getPointB().addX(deltaX2);
        }
    }
    
    
    double
    BeamLineCreator::findStemX(Temporal& inTemporal,
                                  BeamPlacement inBeamPlacement) const
    {
        if (inTemporal.getItemTraits().getItemType() == ItemType::Note) {
            const auto& note = inTemporal.as<Note>();
            if (note.getIsChordMember()) {
                auto chord = note.getChord();
                const auto& bottomNote = chord->getBottomNote();
                const auto& stemInfo = chord->getStemInfo();
                if (inBeamPlacement == BeamPlacement::Above) {
                    return stemInfo.xUp + bottomNote.getScoreX();
                } else {
                    return stemInfo.xDown + bottomNote.getScoreX();
                }
            }
        }
        
        const auto& stemInfo = inTemporal.getStemInfo();
        if (inBeamPlacement == BeamPlacement::Above) {
            return stemInfo.xUp + inTemporal.getScoreX();
        } else {
            return stemInfo.xDown + inTemporal.getScoreX();
        }
    }
    
    
    std::pair<Temporal*, Temporal*>
    BeamLineCreator::findBeamsideNotes(LocalData& data) const
    {
        auto& startNote = data.startNoteP->getNote();
        auto& endNote = data.endNoteP->getNote();
        
        Temporal* first = &startNote;
        Temporal* second = &endNote;
        
        if (startNote.getItemTraits().getItemType() == ItemType::Chord) {
            const auto& chord = startNote.as<Chord>();
            if (data.beamSpec.getBeamPlacement() == BeamPlacement::Above) {
                first = &chord.getTopNote();
            } else {
                first = &chord.getBottomNote();
            }
        }
        
        if (endNote.getItemTraits().getItemType() == ItemType::Chord) {
            const auto& chord = endNote.as<Chord>();
            if (data.beamSpec.getBeamPlacement() == BeamPlacement::Above) {
                second = &chord.getTopNote();
            } else {
                second = &chord.getBottomNote();
            }
        }
        
        return std::make_pair(first, second);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
