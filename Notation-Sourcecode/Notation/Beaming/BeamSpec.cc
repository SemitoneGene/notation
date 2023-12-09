// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeamSpec.h"

// Local
#include "BeamLine.h"
#include "BeamLineCreator.h"
#include "BeamSlope.h"
#include "BeamStem.h"
#include "BeamHeight.h"
#include "BeamChunks.h"
#include "Note.h"
#include "Rest.h"
#include "Score.h"
#include "TemporalValue.h"

namespace komp
{
BeamSpec::BeamSpec()
: mStemPolicy(StemDirection::NONE, Policy::AUTOMATIC)
{
    mBeamStrategies.emplace_back(std::make_unique<BeamChunks>());
    mBeamStrategies.emplace_back(std::make_unique<BeamLineCreator>());
    mBeamStrategies.emplace_back(std::make_unique<BeamSlope>());
    mBeamStrategies.emplace_back(std::make_unique<BeamHeight>());
    mBeamStrategies.emplace_back(std::make_unique<BeamStem>());
}

BeamSpec::~BeamSpec()
{
}

BeamSpec::BeamSpec(const BeamSpec& other)
: mStemPolicy(other.mStemPolicy)
{
    for (const auto& beamedNote : other.mBeamedNotes) {
        mBeamedNotes.emplace_back(beamedNote->clone());
    }
    
    for (const auto& beamLine : other.mBeamLines) {
        mBeamLines.emplace_back(beamLine->clone());
    }
    
    for (const auto& beamStrategy : other.mBeamStrategies) {
        mBeamStrategies.emplace_back(beamStrategy->clone());
    }
}

BeamSpec::BeamSpec(BeamSpec&& other) = default;

BeamSpec&
BeamSpec::operator=(const BeamSpec& other)
{
    mBeamedNotes.clear();
    mBeamLines.clear();
    mBeamStrategies.clear();
    
    for (const auto& beamedNote : other.mBeamedNotes) {
        mBeamedNotes.emplace_back(beamedNote->clone());
    }
    
    for (const auto& beamLine : other.mBeamLines) {
        mBeamLines.emplace_back(beamLine->clone());
    }
    
    for (const auto& beamedNote : other.mBeamedNotes) {
        mBeamedNotes.emplace_back(beamedNote->clone());
    }
    
    mStemPolicy = other.mStemPolicy;
    return *this;
}

BeamSpec& BeamSpec::operator=(BeamSpec&& other) = default;

BeamSpecIntfUPtr
BeamSpec::clone() const
{
    return std::make_unique<BeamSpec>(*this);
}


Measure&
BeamSpec::getMeasure() const
{
    KOMP_ASSERT(getBeamedNoteCount() > 0);
    return getBeamedNote(0).getNote().getLocation().getMeasure();
}


void
BeamSpec::replaceChordedNotes()
{
    bool isChordMemberFound = false;
    
    for (const auto& beamedNote : mBeamedNotes) {
        if (beamedNote->getNote().getIsChordMember()) {
            isChordMemberFound = true;
            break;
        }
    }
    
    if (!isChordMemberFound) {
        return;
    }
    
    BeamedNoteIntfUPtrVec newBeamedNotes;
    
    for (const auto& beamedNote : mBeamedNotes) {
        
        auto temporal = &beamedNote->getNote();
        if (temporal->getIsChordMember()) {
            const auto chord = temporal->getChord();
            temporal = chord;
        }
        
        const auto compare = [&](const BeamedNoteIntfUPtr& inFilterItem) {
            return &inFilterItem->getNote() == temporal;
        };
        
        // Is the temporal already represented in the vector of beamed notes?
        const auto exists = [&](const BeamedNoteIntfUPtrVec& inBeamedNotes,
                                Temporal* inTemporal) {
            auto bnBegin = std::cbegin(inBeamedNotes);
            const auto bnEnd = std::cend(inBeamedNotes);
            const auto iter = std::find_if(bnBegin, bnEnd, compare);
            return iter != bnEnd;
        };
        
        if (!exists(newBeamedNotes, temporal)) {
            newBeamedNotes.emplace_back(beamedNote->clone(*temporal));
        }
    }
    
    mBeamedNotes = std::move(newBeamedNotes);
}


bool
BeamSpec::getIsValid() const
{
    if (getBeamedNoteCount() <= 0) {
        return false;
    }
    return true;
}


BeamPlacement
BeamSpec::getBeamPlacement() const
{
    if (mStemPolicy.getIsStemUp()) {
        return BeamPlacement::Above;
    } else {
        return BeamPlacement::Below;
    }
}


Policy
BeamSpec::getBeamPlacementPolicy() const
{
    return mStemPolicy.policy;
}


bool
BeamSpec::calculateBeamLines(StemPolicy& ioStemPolicy)
{
    const bool isSuccess = calculateBeamLinesImpl(ioStemPolicy);
    return isSuccess;
}


int
BeamSpec::getBeamLineCount() const
{
    return static_cast<int>(mBeamLines.size());
}


const BeamLineIntf&
BeamSpec::getBeamLine(int inIndex) const
{
    KOMP_ASSERT(inIndex >= 0);
    KOMP_ASSERT(inIndex < getBeamLineCount());
    return *mBeamLines.at(inIndex);
}


BeamLineIntf&
BeamSpec::getBeamLine(int inIndex)
{
    KOMP_ASSERT(inIndex >= 0);
    KOMP_ASSERT(inIndex < getBeamLineCount());
    return *mBeamLines.at(inIndex);
}


void
BeamSpec::removeBeamLine(int inIndex)
{
    KOMP_ASSERT(inIndex >= 0);
    KOMP_ASSERT(inIndex < getBeamLineCount());
    const auto& iter = mBeamLines.cbegin() + inIndex;
    mBeamLines.erase(iter);
}


void
BeamSpec::addBeamLine(const BeamLineIntf& inBeamLine)
{
    mBeamLines.emplace_back(inBeamLine.clone());
}


void
BeamSpec::clearBeamLines()
{
    mBeamLines.clear();
}


const BeamLineIntfUPtrVec&
BeamSpec::getBeamLines()
{
    return mBeamLines;
}


int
BeamSpec::getMaxLevel() const
{
    int maxLevel = -1;
    
    // if we have no beam lines then calculate from the beamed note collection
    if (mBeamLines.size() == 0) {
        for (const auto& beamedNote : mBeamedNotes) {
            if (beamedNote->getCount() - 1 > maxLevel) {
                maxLevel = beamedNote->getCount() - 1;
            }
        }
        return maxLevel;
    }
    
    // if we do have beam lines, use them instead for the calculation
    for (const auto& beamLine : mBeamLines) {
        if (beamLine->getLevel() > maxLevel) {
            maxLevel = beamLine->getLevel();
        }
    }
    return maxLevel;
}


int
BeamSpec::getBeamedNoteCount() const
{
    return static_cast<int>(mBeamedNotes.size());
}


const BeamedNoteIntf&
BeamSpec::getBeamedNote(int inIndex) const
{
    KOMP_ASSERT(inIndex >= 0);
    KOMP_ASSERT(inIndex < getBeamedNoteCount());
    return *mBeamedNotes.at(inIndex);
}


BeamedNoteIntf&
BeamSpec::getBeamedNote(int inIndex)
{
    KOMP_ASSERT(inIndex >= 0);
    KOMP_ASSERT(inIndex < getBeamedNoteCount());
    return *mBeamedNotes.at(inIndex);
}


bool
BeamSpec::addBeamedNote(const BeamedNoteIntf& inBeamedNote)
{
    const auto itemType = inBeamedNote.getNote().getItemTraits().getItemType();
    
    if (itemType == ItemType::Rest) {
        // currently, rest participation in beams is unsupported
        LOG_F(ERROR, "Rest participation in beams is unsupported");
        return false;
    }
    
    if (itemType == ItemType::Note) {
        // If the beamed note contains a note that is part of a chord,
        // the Chord object should be added instead of the note object
        auto& note = inBeamedNote.getNote().as<Note>();
        if (note.getIsChordMember()) {
            LOG_F(ERROR, "Cannot add chorded note to beam");
            return false;
        }
    }
        
    // Ensure all temporals in the collection are unique
    const auto beamedNoteID = inBeamedNote.getNote().getItemID();

    auto predicate = [&](const BeamedNoteIntfUPtr& findItem) {
        const auto findID = findItem->getNote().getItemID();
        return findID == beamedNoteID;
    };
    
    auto iter = std::find_if(mBeamedNotes.cbegin(),
                             mBeamedNotes.cend(),
                             predicate);
    if (iter != mBeamedNotes.cend()) {
        LOG_F(ERROR, "Note already in beamed note list");
        return false;
    }
    
    // Ensure that the temporal type is beamable
    const auto type = inBeamedNote.getNote().getDuration().getTemporalType();
    if (!TemporalValue::getIsBeamable(type)) {
        LOG_F(ERROR, "Temporal is not beamable: %d", type);
        return false;
    }
    
    mBeamedNotes.emplace_back(inBeamedNote.clone());
    return true;
}


bool
BeamSpec::removeBeamedNote(const Note& inNote)
{
    auto iter = mBeamedNotes.cbegin();
    auto end = mBeamedNotes.cend();
    for( ; iter != end; ++iter) {
        if (&(*iter)->getNote() == &inNote) {
            mBeamedNotes.erase(iter);
            clearState();
            return true;
        }
    }
    return false;
}


const BeamedNoteIntfUPtrVec&
BeamSpec::getBeamedNotes()
{
    return mBeamedNotes;
}


void
BeamSpec::addStrategy(BeamStrategyIntfUPtr&& inStrategy)
{
    mBeamStrategies.emplace_back(std::move(inStrategy));
}


void
BeamSpec::clearStrategies()
{
    mBeamStrategies.clear();
}


Point
BeamSpec::getDrawPoint() const
{
    if (mBeamedNotes.empty() ) {
        return POINT_ZERO;
    }
    
    const auto& first = mBeamedNotes.front();
    const auto& note = first->getNote();
    return note.getDrawPoint();
}


bool
BeamSpec::calculateBeamLinesImpl(StemPolicy& ioStemPolicy)
{
    replaceChordedNotes();
    sortBeamedNotes();
    mBeamLines.clear();
    mStemPolicy = choosePlacement(ioStemPolicy);
    
    for (const auto& beamStrategy : mBeamStrategies) {
        if (!beamStrategy->applyStrategy(*this)) {
            LOG_F(ERROR, "applyStrategy failed");
            return false;
        }
    }
    
    if (getBeamLineCount() == 0) {
        return false;
    }
    
    if (ioStemPolicy.getIsAutomatic()) {
        ioStemPolicy.direction = mStemPolicy.direction;
    }
    
    return true;
}


StemPolicy
BeamSpec::choosePlacement(StemPolicy inStemPolicy)
{
    if (inStemPolicy.getIsSpecified()) {
        if (inStemPolicy.getIsStemUp() || inStemPolicy.getIsStemDown()) {
            return inStemPolicy;
        }
    }
    
    if (mBeamedNotes.size() < 2) {
        return inStemPolicy;
    }
    
    int slotSum = 0;
    
    for (const auto& bn : mBeamedNotes) {
        slotSum += bn->getNote().getSlot();
    }
    
    const double slotAvg = static_cast<double>(slotSum)
    / static_cast<double>(mBeamedNotes.size());
    if (slotAvg > 6.01) {
        return StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC};
    } else {
        return StemPolicy{StemDirection::UP, Policy::AUTOMATIC};
    }
}


void
BeamSpec::clearState()
{
    mBeamLines.clear();
    clearBeamChunks();
}


void
BeamSpec::clearBeamChunks()
{
    for (auto& bn : mBeamedNotes) {
        for (int i = 0; i < bn->getCount(); ++i) {
            bn->setBeamChunk(i, BeamChunk::Unknown);
        }
    }
}


void
BeamSpec::sortBeamedNotes()
{
    auto predicate = [&](const BeamedNoteIntfUPtr& lhs,
                         const BeamedNoteIntfUPtr& rhs) {
        
        const auto& l = lhs->getNote();
        const auto& r = rhs->getNote();
        const auto lmeasure = l.getLocation().getMeasure().getIndex();
        const auto rmeasure = r.getLocation().getMeasure().getIndex();
        
        if (lmeasure < rmeasure) {
            return true;
        } else if (lmeasure > rmeasure) {
            return false;
        }
        
        if (l.getTime() < r.getTime()) {
            return true;
        } else if (l.getTime() > r.getTime()) {
            return false;
        }
        
        const auto& leftLoc = l.getLocation();
        const auto& rightLoc = r.getLocation();
        
        if (leftLoc.getVoice() < rightLoc.getVoice()) {
            return true;
        } else if (leftLoc.getVoice() > rightLoc.getVoice()) {
            return false;
        }
        
        return lhs.get() < rhs.get();
    };
    
    std::ranges::sort(mBeamedNotes, predicate);
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
