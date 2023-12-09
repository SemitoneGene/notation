// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


// Self
#include "BeamChunks.h"

// System

// Local
#include "BeamSpecIntf.h"
#include "BeamChunk.h"

namespace komp
{
    
    struct BeamChunks::LocalData
    {
        BeamSpecIntf& beamSpec;       ///< the beam spec that we are working with
        int maxLevel;                 ///< out of all the notes, what is the max beam level found
        int currentLevel;             ///< the current beam level under consideration
        int currentNote;              ///< the index of the beamed note currently under consideration
        bool isCurrentNoteFirst;      ///< is the current note the first one in the group of beamed notes
        bool isCurrentNoteLast;       ///< is the current note the last one in the group of beamed notes
        bool doesPreviousHaveMyLevel; ///< does the previous note have as many beam levels as the current note
        bool doesNextHaveMyLevel;     ///< does the next note have as many beam levels as the current note
        bool isPreviousBroken;        ///< is the previous note's 'isBroken' flag set for the same level as the current note
        int beamedNoteCount;          ///< the number of notes in this beam spec
        
        
        LocalData(BeamSpecIntf& outBeamSpec)
        : beamSpec(outBeamSpec)
        , maxLevel(outBeamSpec.getMaxLevel())
        , currentLevel(0)
        , currentNote(0)
        , isCurrentNoteFirst(true)
        , isCurrentNoteLast(false)
        , doesPreviousHaveMyLevel(false)
        , doesNextHaveMyLevel(false)
        , isPreviousBroken(true)
        , beamedNoteCount(outBeamSpec.getBeamedNoteCount())
        {
            
        }
    };
    
    
    BeamStrategyIntfUPtr
    BeamChunks::clone() const
    {
        return std::make_unique<BeamChunks>(*this);
    }


    bool
    BeamChunks::applyStrategy(BeamSpecIntf& outBeamSpec) const
    {
        if (!isStrategyRequired(outBeamSpec)) {
            return true;
        }
        LocalData data(outBeamSpec);
        return strategyImpl(data);
    }
    
    
    bool
    BeamChunks::isStrategyRequired(BeamSpecIntf& outBeamSpec) const
    {
        for (const auto& beamedNote : outBeamSpec.getBeamedNotes()) {
            for (const auto& beamChunk : beamedNote->getBeamChunks()) {
                if (beamChunk == BeamChunk::Unknown) {
                    return true;
                }
            }
        }
        return false;
    }
    
    
    bool
    BeamChunks::strategyImpl(LocalData& data) const
    {
        data.currentLevel = data.maxLevel;
        for (data.currentLevel = data.maxLevel; data.currentLevel >= 1; --data.currentLevel) {

            data.doesPreviousHaveMyLevel = false;
            data.doesNextHaveMyLevel = false;
            data.isPreviousBroken = true;
            
            for (data.currentNote = 0; data.currentNote < data.beamedNoteCount; ++data.currentNote) {
                
                if (data.currentLevel >= data.beamSpec.getBeamedNote(data.currentNote).getCount()) {
                    continue;
                }
                
                data.isCurrentNoteFirst = (data.currentNote == 0);
                data.isCurrentNoteLast = (data.currentNote == (data.beamedNoteCount - 1));
                setDoesPreviousHaveMyLevel(data);
                setDoesNextHaveMyLevel(data);
                setIsPreviousNoteBroken(data);
                setBeamChunk(data);
            }
        }
        
        // the main beam should be unbroken all the way accross
        for (data.currentNote = 0; data.currentNote < data.beamedNoteCount; ++data.currentNote) {
            data.isCurrentNoteFirst = (data.currentNote == 0);
            data.isCurrentNoteLast = (data.currentNote == (data.beamedNoteCount - 1));
            auto& beamedNote = data.beamSpec.getBeamedNote(data.currentNote);
            
            if (data.isCurrentNoteFirst) {

                beamedNote.setIsBroken(0, false);
                beamedNote.setBeamChunk(0, BeamChunk::Begin);
            
            } else if (data.isCurrentNoteLast) {
                
                beamedNote.setIsBroken(0, true);
                beamedNote.setBeamChunk(0, BeamChunk::End);
                
            } else {
                
                beamedNote.setIsBroken(0, false);
                beamedNote.setBeamChunk(0, BeamChunk::Continue);
                
            }
        }
        
        return true;
    }
    
    
    void
    BeamChunks::setDoesPreviousHaveMyLevel(LocalData& data) const
    {
        if (data.isCurrentNoteFirst) {
            data.doesPreviousHaveMyLevel = false;
            return;
        }
        
        const auto& currentBeamedNote = data.beamSpec.getBeamedNote(data.currentNote);
        const auto& previousBeamedNote = data.beamSpec.getBeamedNote(data.currentNote - 1);
        data.doesPreviousHaveMyLevel = previousBeamedNote.getCount() >= currentBeamedNote.getCount();
    }
    
    
    void
    BeamChunks::setDoesNextHaveMyLevel(LocalData& data) const
    {
        if (data.isCurrentNoteLast) {
            data.doesNextHaveMyLevel = false;
            return;
        }
        
        const auto& currentBeamedNote = data.beamSpec.getBeamedNote(data.currentNote);
        const auto& nextBeamedNote = data.beamSpec.getBeamedNote(data.currentNote + 1);
        data.doesNextHaveMyLevel = nextBeamedNote.getCount() >= currentBeamedNote.getCount();
    }
    
    
    void
    BeamChunks::setIsPreviousNoteBroken(LocalData& data) const
    {
        if (data.isCurrentNoteFirst) {
            data.isPreviousBroken = false;
            return;
        }
        
        if (!data.doesPreviousHaveMyLevel) {
            data.isPreviousBroken = false;
            return;
        }
        
        const auto& previousBeamedNote = data.beamSpec.getBeamedNote(data.currentNote - 1);
        data.isPreviousBroken = previousBeamedNote.getIsBroken(data.currentLevel);
    }
    
    
    void
    BeamChunks::setBeamChunk(LocalData& data) const
    {
        const auto& beamedNote = data.beamSpec.getBeamedNotes().at(data.currentNote);
        
        if (data.isCurrentNoteFirst) {
            
            if (!data.doesNextHaveMyLevel || beamedNote->getIsBroken(data.currentLevel)) {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::BrokenRight);
            } else {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::Begin);
            }
            
        } else if (data.isCurrentNoteLast) {

            if (!data.doesPreviousHaveMyLevel || data.isPreviousBroken) {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::BrokenLeft);
            } else {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::End);
            }
            
        } else if (data.doesPreviousHaveMyLevel && data.doesNextHaveMyLevel) {
            
            setBeamChunkIfPreviousHasNextHas(data);
            
        } else if (!data.doesPreviousHaveMyLevel && data.doesNextHaveMyLevel) {
            
            setBeamChunkIfPreviousDoesNotHaveNextHas(data);
            
        } else if (data.doesPreviousHaveMyLevel && !data.doesNextHaveMyLevel) {
        
            setBeamChunkIfPreviousHasNextDoesNotHave(data);
            
        } else { // if (!data.doesPreviousHaveMyLevel && !data.doesNextHaveMyLevel)
        
            setBeamChunkIfPreviousDoesNotHaveNextDoesNotHave(data);
        }
    }

    
    void
    BeamChunks::setBeamChunkIfPreviousHasNextDoesNotHave(LocalData& data) const
    {
        if (!data.doesPreviousHaveMyLevel) {
            return;
        }
        
        if (data.doesNextHaveMyLevel) {
            return;
        }
        
        const auto& beamedNote = data.beamSpec.getBeamedNotes().at(data.currentNote);

        // current note must be 'broken' because next note does not have this level
        beamedNote->setIsBroken(data.currentLevel, true);
        
        if (data.isPreviousBroken) {

            beamedNote->setBeamChunk(data.currentLevel, BeamChunk::BrokenRight);
            
        } else {
            
            beamedNote->setBeamChunk(data.currentLevel, BeamChunk::End);
        }
    }
    
    
    void
    BeamChunks::setBeamChunkIfPreviousDoesNotHaveNextDoesNotHave(LocalData& data) const
    {
        if (data.doesPreviousHaveMyLevel) {
            return;
        }
        
        if (data.doesNextHaveMyLevel) {
            return;
        }
        
        const auto& beamedNote = data.beamSpec.getBeamedNotes().at(data.currentNote);

        // current note must be 'broken' because next note does not have this level
        beamedNote->setIsBroken(data.currentLevel, true);
        beamedNote->setBeamChunk(data.currentLevel, BeamChunk::BrokenRight);
    }
    
    
    void
    BeamChunks::setBeamChunkIfPreviousHasNextHas(LocalData& data) const
    {
        if (!data.doesPreviousHaveMyLevel) {
            return;
        }
        
        if (!data.doesNextHaveMyLevel) {
            return;
        }
        
        const auto& beamedNote = data.beamSpec.getBeamedNotes().at(data.currentNote);
        
        if (beamedNote->getIsBroken(data.currentLevel)) {
            if (data.isPreviousBroken) {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::BrokenRight);
            } else {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::End);
            }
        } else {
            if (data.isPreviousBroken) {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::Begin);
            } else {
                beamedNote->setBeamChunk(data.currentLevel, BeamChunk::Continue);
            }
        }
    }
    
    
    void
    BeamChunks::setBeamChunkIfPreviousDoesNotHaveNextHas(LocalData& data) const
    {
        if (data.doesPreviousHaveMyLevel) {
            return;
        }
        
        if (!data.doesNextHaveMyLevel) {
            return;
        }
        
        const auto& beamedNote = data.beamSpec.getBeamedNotes().at(data.currentNote);
        
        if (beamedNote->getIsBroken(data.currentLevel)) {
            beamedNote->setBeamChunk(data.currentLevel, BeamChunk::BrokenRight);
        } else {
            beamedNote->setBeamChunk(data.currentLevel, BeamChunk::Begin);
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
