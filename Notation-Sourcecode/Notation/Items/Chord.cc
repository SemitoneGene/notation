// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Chord.h"

// System
#include <iomanip>
#include <sstream>
#include <string>

// Local
#include "BeamIntf.h"
#include "ChordHead.h"
#include "DrawableContext.h"
#include "DrawableNoteParts.h"
#include "Engraver.h"
#include "FontManager.h"
#include "ItemImpl.h"
#include "Mark.h"
#include "MarkImpl.h"
#include "Note.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "TemporalConstants.h"
#include "TemporalImpl.h"
#include "TemporalValue.h"
#include "TupletIntf.h"

namespace komp
{
Chord::Chord(const MusicLocation& inLocation,
             const NoteRefVec& inNotes)
: mAttachments{*this}
, mSpaceTimePoint{*this}
, mConstructorNotes{inNotes}
, mLocation(inLocation)
{
    if (mConstructorNotes.size() < 2) {
        KOMP_THROW("A chord must have at least 2 notes when it is constructed.");
    }
    
    TemporalImpl::initializeStemPolicy(*this, mStemPolicy);
}

const ItemTraits&
Chord::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Chord);
}


ItemUPtr
Chord::getMemento() const
{
    return std::make_unique<Chord>(*this);
}


void
Chord::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    setIsQuiescent(true);
    const auto itemP = &inItem;
    const Chord* downcastedP = dynamic_cast<const Chord*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    update();
    setIsQuiescent(wasQuiescent);
}

void
Chord::update()
{
    updateChordLayout();
    
    updateArpeggiate();
    
    if (!mNotes.empty()) {
        notifyObservers({ItemChangeType::Updated});
    }
}

void
Chord::itemIDChanged()
{
    completeConstruction();
    mSpaceTimePoint.setItem(*this);
    mIsRegistered = true;
}

void
Chord::itemIDInvalidated()
{
    mIsRegistered = false;
}

void
Chord::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
Chord::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
Chord::notifyObservers(const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        mDeferredChanges.insert(inChanges.begin(), inChanges.end());
        return;
    }

    ItemImpl::notifyObservers(*this,
                              mObservers,
                              std::move(inChanges),
                              mObserverSpinCount);
    
    MarkImpl::placeMarks(*this);
}


void
Chord::observedItemChanged(const Item& observedItem, const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        return;
    }
    
    if (!mIsObservingNotes) {
        return;
    }
    
    const auto& note = findNote(observedItem);    
    if (!note) {
        return;
    }
    
    const auto slotChanged = ItemObserver::contains(inChanges, ItemChangeType::Slot);
    
    bool doUpdate = false;
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Beam);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Duration);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::StemDirection);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::TemporalType);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Updated);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::X);
    doUpdate |= slotChanged;

    if (!doUpdate) {
        return;
    }
    
    const auto wasObserving = mIsObservingNotes;
    unobserveNotes();
    sort();
    const auto& currentDuration = getDuration();
    const auto& incomingDuration = note->getDuration();
    
    if (!incomingDuration.equals(currentDuration)) {
        setDuration(incomingDuration);
    }
    
    if (slotChanged) {
        std::ranges::sort(mNotes,
                          [&](const ChordNote& left,
                              const ChordNote& right) {
            return left.getNote().getSlot() < right.getNote().getSlot();
        });
        
        for (auto& note : mNotes) {
            const auto& chordNote = note.getNote();
            note.chordHead.slot = chordNote.getSlot();
        }
    }

    if (doUpdate) {
        for (auto& note : mNotes) {
            note.update();
        }
    }
    
    updateInternal();
    
    if (wasObserving) {
        observeNotes();
    }
    
    notifyObservers({ItemChangeType::X, ItemChangeType::Slot});
}


bool
Chord::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
Chord::setIsQuiescent(bool inValue)
{
    if (mIsQuiescent == inValue) {
        return;
    }
    
    mIsQuiescent = inValue;
    
    if (mDeferredChanges.empty()) {
        return;
    }
    
    if (!getIsQuiescent()) {
        ItemImpl::notifyObservers(*this,
                                  mObservers,
                                  mDeferredChanges,
                                  mObserverSpinCount);
        mDeferredChanges.clear();
    }
}


Rect
Chord::getFrame() const
{
    const auto& frames = getFrames();
    const auto& result = frames.getCompleteFrame();
    return result;
}


void
Chord::createDrawable()
{
    ItemImpl::createDrawable<DrawableNoteParts>(*this);
}

Region
Chord::getRegion() const
{
#ifdef KOMP_BUILD_APPLE
    auto path = Region(CGPathCreateMutable());
#elif KOMP_BUILD_LINUX
    auto path = Region{};
#endif
    
    for (const auto& chordNote : mNotes) {
        const auto& note = chordNote.getNote();
        const auto headFrame = note.getFrames().getCompleteHeadFrame();
        path.addRect(headFrame);
    }
    
    if (getIsBeamed()) {
        
        const auto beam = getBeam();
        const auto& beamedNotes = beam->getBeamedNotes();
        
        for (const auto& beamedNote : beamedNotes) {
            if (&beamedNote->getNote() == this) {
                
                // FIXME: This is a fudge to get beam strokes to work
                constexpr double MAKE_IT_WORK = 2.0;
                
                const auto& beamsidePoint = beamedNote->getStemBeamsidePoint();
                const auto& noteheadPoint = beamedNote->getStemNoteheadPoint();
                const auto stemThickness = beamedNote->getStemThickness();
                
                const auto x1 = beamsidePoint.getScoreX() - MAKE_IT_WORK;
                const auto x2 = noteheadPoint.getScoreX() + stemThickness + MAKE_IT_WORK;
                
                auto y1 = beamsidePoint.getScoreY();
                auto y2 = noteheadPoint.getScoreY();
                if (y1 > y2) {
                    std::swap(y1, y2);
                }
                y1 -= MAKE_IT_WORK;
                y1 += MAKE_IT_WORK;
                const Rect stemRect(y1, x1, y2, x2);
                path.addRect(stemRect);
            }
        }
        
    } else if (getStemPolicy().getHasStem()) {
        
        // Get the start and end pixel positions of the stemline
        const auto stemRect = getStemFrame();
        path.addRect(stemRect);
    }
    
    return path;
}


Point
Chord::getAnchor() const
{
    return getFrame().getCenter();
}


bool
Chord::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}


void
Chord::inserted()
{
    completeConstruction();
    
    if (mNotes.size() < 2) {
        KOMP_THROW("A chord must have at least two notes to exist within a measure.");
    }
    
    mIsInserted = true;
    mIsObservingNotes = true;
    updateChordLayout();
    
    notifyObservers({ItemChangeType::Inserted});
}


void
Chord::removed()
{
    completeConstruction();
    mIsInserted = false;
    
    notifyObservers({ItemChangeType::Removed});
}

void
Chord::removeSelf()
{
    ItemImpl::removeSelf(*this);
}


const MusicLocation&
Chord::getLocation() const
{
    return mLocation;
}


double
Chord::getX() const
{
    return mSpaceTimePoint.getMeasureRelativeX();
}


void
Chord::setX(double inValue)
{
    if (inValue == getX()) {
        return;
    }
    
    const auto currentX = getX();
    const auto deltaX = inValue - currentX;
    const auto wasObserving = mIsObservingNotes;
    unobserveNotes();
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        const auto noteCurrentX = note.getX();
        const auto noteNewX = noteCurrentX + deltaX;
        note.setX(noteNewX);
    }
    
    mSpaceTimePoint.setX(inValue);
    
    if (wasObserving) {
        observeNotes();
    }
    
    notifyObservers({ItemChangeType::X});
}


double
Chord::getScoreX() const
{
    return mSpaceTimePoint.getScoreX();
}


int
Chord::getSlot() const
{
    if (mNotes.empty()) {
        return 0;
    }
    
    const auto& topNote = getTopNote();
    return topNote.getSlot();
}


void
Chord::setSlot(int inValue)
{
    const auto chordSlot = getSlot();
    auto delta = inValue - chordSlot;
    
    if (delta == 0) {
        return;
    }
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        const auto noteSlot = note.getSlot();
        note.setSlot(noteSlot + delta);
    }
    mSpaceTimePoint.setSlot(getSlot());
}


double
Chord::getScoreY() const
{
    if (mNotes.empty()) {
        return 0.0;
    }
    
    const auto& top = getTopNote();
    return top.getScoreY();
}


Point
Chord::getDrawPoint() const
{
    Point p;
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}

double
Chord::getTime() const
{
    return mSpaceTimePoint.getTime();
}


int
Chord::getRoundedTime() const
{
    return ItemImpl::getRoundedTime(*this);
}


void
Chord::setTime(double inValue)
{
    if (inValue == getTime()) {
        return;
    }

    bool wasObserving = mIsObservingNotes;
    unobserveNotes();
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.setTime(inValue);
    }
    
    mSpaceTimePoint.setTime(inValue);
    
    if (wasObserving) {
        observeNotes();
    }
}


double
Chord::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
Chord::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}

bool
Chord::getIsNote() const
{
    return false;
}


bool
Chord::getIsRest() const
{
    return false;
}


bool
Chord::getIsChord() const
{
    return true;
}


bool
Chord::getIsChordMember() const
{
    return false;
}


Chord*
Chord::getChord() const
{
    return nullptr;
}


void
Chord::setChord(Chord* inChord)
{
}


void
Chord::clearChord()
{
}


bool
Chord::getIsLayoutItem() const
{
    return true;
}


Pitch
Chord::getPitch() const
{
    // TODO: decide what happens
    return Pitch{};
}


unsigned short
Chord::getGlyph() const
{
    // TODO: decide what happens
    return 0;
}


Duration
Chord::getDuration() const
{
    return mDuration;
}


void
Chord::setDuration(Duration inDuration)
{
    if (inDuration.equals(getDuration())) {
        return;
    }

    const bool wasObserving = mIsObservingNotes;
    unobserveNotes();
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.setDuration(inDuration);
    }
    mDuration = inDuration;
    updateInternal();
    if (wasObserving) {
        observeNotes();
    }
}


bool
Chord::getIsRaw() const
{
    return mIsRaw;
}


void
Chord::setIsRaw(bool inValue)
{
    if (inValue == mIsRaw) {
        return;
    }
    
    mIsRaw = inValue;
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.setIsRaw(inValue);
    }
    
    if (!inValue) {
        setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
    } else {
        setStemPolicy(StemPolicy{StemDirection::NONE, Policy::AUTOMATIC});
    }
}


StemPolicy
Chord::getStemPolicy() const
{
    const auto& beam = getBeam();
    if (beam) {
        return beam->getStemPolicy();
    }
    
    // TODO: We are having an issue with stem direction being set to UP
    // instead of NONE when isRaw.  Don't have time to fix it right
    // now so will force the return to be correct here.
    auto result = mStemPolicy;
    
    if (getIsRaw()) {
        result.direction = StemDirection::NONE;
    }
    
    return result;
}


void
Chord::setStemPolicy(StemPolicy inPolicy)
{
    TemporalImpl::setStemPolicy(*this, inPolicy, mStemPolicy);
    updateDrawables();
    notifyObservers({ItemChangeType::StemDirection});
}

StemInfo
Chord::getStemInfo() const
{
    if (mNotes.size() == 0) {
        StemInfo result;
        result.staff = &getLocation().getStaff();
        KOMP_ASSERT(result.staff);
        return result;
    }
    
    const auto isUp = getStemPolicy().getIsStemUp();
    
    const auto& topNote = getTopNote();
    const auto& bottomNote = getBottomNote();
    const auto escapeNote = isUp ? topNote : bottomNote;
    const auto attachNote = isUp ? bottomNote : topNote;
    
    auto escapeInfo = escapeNote.getStemInfo();
    auto attachInfo = attachNote.getStemInfo();
    
    const auto attachNoteOffsetX = attachNote.getX() - getX();
    const auto attachNoteOffsetY = attachNote.getScoreY() - getScoreY();
    const auto escapeNoteOffsetX = escapeNote.getX() - getX();
    const auto escapeNoteOffsetY = escapeNote.getScoreY() - getScoreY();
    
    attachInfo.xUp += attachNoteOffsetX;
    attachInfo.xDown += attachNoteOffsetX;
    escapeInfo.xUp += escapeNoteOffsetX;
    escapeInfo.xDown += escapeNoteOffsetX;
    attachInfo.yUp += attachNoteOffsetY;
    attachInfo.yDown += attachNoteOffsetY;
    escapeInfo.yUp += escapeNoteOffsetY;
    escapeInfo.yDown += escapeNoteOffsetY;
    
    StemInfo result = attachInfo;
    result.escapeSlot = escapeInfo.escapeSlot;
    result.escapeSlotPreciseUp = escapeInfo.escapeSlotPreciseUp;
    result.escapeSlotPreciseDown = escapeInfo.escapeSlotPreciseDown;
    result.temporalY = getScoreY();
    result.isStemOwner = !getBeam();
    KOMP_ASSERT(result.staff);
    
    return result;
}


PlaybackNote
Chord::getPlaybackNote() const
{
    return TemporalImpl::getPlaybackNote(*this);
}


Attachments&
Chord::getAttachments()
{
    completeConstruction();
    return mAttachments;
}


const Attachments&
Chord::getAttachments() const
{
    return mAttachments;
}


bool
Chord::getIsBeamed() const
{
    return getBeam() != nullptr;
}


BeamIntf*
Chord::getBeam() const
{
    return mBeam;
}


void
Chord::setBeam(BeamIntf& inBeam)
{
    completeConstruction();
    if (&inBeam == mBeam) {
        return;
    }
    
    mBeam = &inBeam;
    updateChordLayout();
    notifyObservers({ItemChangeType::Beam});
}


void
Chord::clearBeam()
{
    mBeam = nullptr;
    updateChordLayout();
    notifyObservers({ItemChangeType::Beam});
}


bool
Chord::getIsTupleted() const
{
    return TemporalImpl::getIsTupleted(mTuplets);
}


TupletIntfRefVec
Chord::getTuplets() const
{
    return TemporalImpl::getTuplets(mTuplets);
}


bool
Chord::addTuplet(TupletIntf& inTuplet)
{
    completeConstruction();
    const bool wasObserving = mIsObservingNotes;
    unobserveNotes();
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.addTuplet(inTuplet);
    }
    
    auto predicate = [&](const TupletIntf& vectorTuplet) {
        return &vectorTuplet == &inTuplet;
    };
    
    const auto iter = std::find_if(mTuplets.cbegin(), mTuplets.cend(), predicate);
    if (iter != mTuplets.cend()) {
        return false;
    }
    
    mTuplets.emplace_back(inTuplet);
    
    auto duration = getDuration();
    duration.addTupletRatio(inTuplet.getTupletRatio());
    setDuration(duration);
    
    updateInternal();
    
    if (wasObserving) {
        observeNotes();
    }
    
    return true;
}


bool
Chord::removeTuplet(TupletIntf& inTuplet)
{
    completeConstruction();
    TemporalImpl::removeTuplet(*this, mTuplets, inTuplet);
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.removeTuplet(inTuplet);
    }
    
    return true;
}


NoteFrames
Chord::getFrames() const
{
    NoteFrames result;
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        const auto current = note.getFrames();
        result.head.initializeOrUnion(current.head);
        result.dots.initializeOrUnion(current.dots);
        result.accidental.initializeOrUnion(current.accidental);
    }
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    const auto drawPoint = getDrawPoint();
    
    if (getIsBeamed()) {
        const auto beamStemFrame = getBeam()->getStemFrame(*this);
        result.stem.initializeOrUnion(beamStemFrame);
    } else {
        const auto stemFrame = drawable->getStemFrame(drawPoint);
        result.stem.initializeOrUnion(stemFrame);
    }
    
    const auto flagFrame = drawable->getNoteFlagsFrame(drawPoint);
    result.flags.initializeOrUnion(flagFrame);
    
    const auto arpeggiateFrame = drawable->getArpeggiateFrame(drawPoint);
    result.arpeggiate.initializeOrUnion(arpeggiateFrame);
    
    return result;
}


bool
Chord::getIsTupletStart() const
{
    // by definition, for the MusicXML a chord can never be the trigger
    // to 'start' a tuplet
    return false;
}


bool
Chord::getIsTupletStop() const
{
    // by definition, for the MusicXML a chord can never be the trigger
    // to 'stop' a tuplet
    return false;
}


bool
Chord::getIsGrace() const
{
    return false;
}

std::vector<NoteRef>
Chord::getNotes() const
{
    std::vector<NoteRef> notes;
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        notes.emplace_back(note);
    }
    return notes;
}


bool
Chord::addNote(Note& inNote)
{
    completeConstruction();
    return addNoteImpl(inNote);
}


void
Chord::clearNotes()
{
    completeConstruction();
    
    if (mNotes.empty()) {
        return;
    }
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.clearChord();
    }
    
    mNotes.clear();
}


bool
Chord::removeNote(const Note& inNote)
{
    completeConstruction();
    
    for (auto it = mNotes.cbegin(); it != mNotes.cend(); ++it) {
        const auto& chordNote = *it;
        auto& note = chordNote.getNote();
        if (&note == &inNote) {
            
            if (mIsInserted && mNotes.size() <= 2) {
                KOMP_THROW("A chord cannot exist in a measure without at least two notes.");
            }
            
            note.clearChord();
            mNotes.erase(it);
            return true;
        }
    }
    
    sort();
    return false;
}


Note&
Chord::getTopNote() const
{
    KOMP_ASSERT(!mNotes.empty());
    auto iter = mNotes.crbegin();
    return iter->getNote();
}


Note&
Chord::getBottomNote() const
{
    KOMP_ASSERT(!mNotes.empty());
    auto iter = mNotes.cbegin();
    return iter->getNote();
}


bool
Chord::exists(const Note& inNote) const
{
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        if (&note == &inNote) {
            return true;
        }
    }
    
    return false;
}


void
Chord::updateChordLayout()
{
    if (getIsQuiescent()) {
        return;
    }
    
    const bool wasObserving = mIsObservingNotes;
    unobserveNotes();
    updateInternal();
    if (wasObserving) {
        observeNotes();
    }
}


Rect
Chord::getStemFrame() const
{
    const auto frames = getFrames();
    return frames.stem;
}


ChordMemento
Chord::getChordMemento() const
{
    ItemUPtrVec noteMementos;
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        noteMementos.emplace_back(note.getMemento());
    }
    
    auto memento = getMemento();
    return ChordMemento { std::move(noteMementos), std::move(memento) };
}


bool
Chord::getIsMember(const Note& inNote) const
{
    for (const auto& chordNote : mNotes) {
        const auto& note = chordNote.getNote();
        if (&inNote == &note) {
            return true;
        }
    }
    return false;
}


bool
Chord::getIsFirst(const Note& inNote) const
{
    return inNote.getItemID() == getBottomNote().getItemID();
}


bool
Chord::getIsLast(const Note& inNote) const
{
    return inNote.getItemID() == getTopNote().getItemID();
}


void
Chord::registerNotes()
{
    if (mNotes.empty()) {
        return;
    }
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.setChord(this);
    }
}


void
Chord::unregisterNotes()
{
    if (mNotes.empty()) {
        return;
    }
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.clearChord();
    }
}


double
Chord::getMeasureDataX() const
{
    const auto& measure = getLocation().getMeasure();
    return measure.getDataX();
}


Note*
Chord::findNote(const Item& inItem) const
{
    auto finder = [&](const ChordNote& inChordNote) {
        const auto& note = inChordNote.getNote();
        return &note == &inItem;
    };
    
    auto iter = std::find_if(mNotes.cbegin(), mNotes.cend(), finder);
    if (iter == mNotes.cend()) {
        return nullptr;
    }
    
    return &(*iter).getNote();
}


void
Chord::observeNotes()
{
    if (mIsObservingNotes) {
        return;
    }
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.registerObserver(*this);
    }
    
    mIsObservingNotes = true;
}


void
Chord::unobserveNotes()
{
    if (!mIsObservingNotes) {
        return;
    }
    
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.unregisterObserver(*this);
    }
    
    mIsObservingNotes = false;
}


void
Chord::updateInternal()
{
    if (getIsQuiescent()) {
        return;
    }
    
    if (mNotes.empty()) {
        return;
    }
    
    if (!mIsRegistered) {
        return;
    }
    
    if (!mIsInserted) {
        return;
    }
    
    sort();
    
    updateBeamPassOne();
    
    updateDurations();
    
    auto ioHeads = getChordHeadPtrStructs(mNotes);
    updateStemPolicy(ioHeads);
        
    updateHorizonalPositions();
    
    updateBeamPassTwo();
    
    ChordHead::calculateAccidentalPositions(ioHeads);
    updateAccidentals();
    
    updateArpeggiate();
    
    sort();
    
    updateDrawables();
    
    ItemImpl::setNeedsDisplay(*this);
        
    notifyObservers({ItemChangeType::Chord});
    
    if (getIsBeamed()) {
        ItemImpl::setNeedsDisplay(*getBeam());
    }
}

void
Chord::updateDurations()
{
    for (const auto& chordNote : mNotes) {
        chordNote.getNote().setAccidentalOffsetDefault();
    }

    auto isFirst = true;
    const auto duration = getDuration();
    double largestHeadWidth = 0.0;

    // Set duration and find largest notehead width
    for (auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        chordNote.chordHead.side = ChordSide::Unknown;
        
        if (!duration.equals(note.getDuration())) {
            note.setDuration(duration);
        }
        
        if (isFirst) {
            largestHeadWidth = note.getFrames().head.getWidth();
            isFirst = false;
        } else {
            const auto currentHeadWidth = note.getFrames().head.getWidth();
            if (currentHeadWidth > largestHeadWidth) {
                largestHeadWidth = currentHeadWidth;
            }
        }
    }
}

void
Chord::updateStemPolicy(ChordHeadPtrs& ioHeads)
{
    const auto direction = ChordHead::calculateStemDirection(ioHeads);
    
    StemPolicy localStemPolicy = mStemPolicy;
    localStemPolicy.policy = Policy::SPECIFIED;
    
    auto beam = getBeam();
    if (beam) {
        localStemPolicy = beam->getStemPolicy();
    } else if (!TemporalValue::getHasStem(getDuration().getTemporalType())) {
        localStemPolicy = StemPolicy{StemDirection::NONE, Policy::SPECIFIED};
    } else if (mStemPolicy.getIsAutomatic()) {
        localStemPolicy.direction = direction;
    }
    
    // Save the stem policy back into our data member if we are automatic
    if (!beam && mStemPolicy.getIsAutomatic()) {
        mStemPolicy.direction = localStemPolicy.direction;
    }
    
    ChordHead::calculateHeadSides(ioHeads, localStemPolicy.direction);
}

void
Chord::updateAccidentals()
{
    for (const auto& chordNote : mNotes) {
        auto& note = chordNote.getNote();
        note.setAccidentalOffset(chordNote.chordHead.accidentalOffset);
    }
}

void
Chord::updateArpeggiate()
{
    if (mArpeggiateType == ArpeggiateType::NONE) {
        return;
    }
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    drawable->setArpeggiateType(mArpeggiateType);

    double accidentalOffset = 0.0;
    for (const auto& chordNote : mNotes) {
        accidentalOffset = std::min(accidentalOffset,
                                    chordNote.chordHead.accidentalOffset);
    }

    const auto offset = drawable->getArpeggiateOffset();
    drawable->setArpeggiateOffset({
        accidentalOffset - ARPEGGIATE_SPACE,
        offset.y
    });
}

void
Chord::updateHorizonalPositions()
{
    const auto bottomNote = getBottomNote();
    
    const auto bottomIter = std::find_if(std::cbegin(mNotes),
                                         std::cend(mNotes),
                                         [&](const ChordNote& inItem) {
        return inItem.getNote().getItemID() == bottomNote.getItemID();
    });
    KOMP_ASSERT(bottomIter != std::cend(mNotes));
    
    const auto& bottomChordNote = *bottomIter;
    const auto normalSide = bottomChordNote.chordHead.side;
    const auto movedSide = ChordHead::oppositeSide(normalSide);
    
    // Adjust x positions
    const auto chordX = getX();
    for (const auto& chordNote : mNotes) {
        
        auto& note = chordNote.getNote();
        
        const auto headWidth = note.getFrames().head.getWidth();
        const auto stemThickness = getStemInfo().thickness;
        const auto halfStem = stemThickness / 2.0;
        const bool moveLeft = (chordNote.chordHead.side == movedSide) && (movedSide == ChordSide::Left);
        const bool moveRight = (chordNote.chordHead.side == movedSide) && (movedSide == ChordSide::Right);
        
        if (moveLeft) {
            const auto noteX = chordX - headWidth + halfStem;
            note.setX(noteX);
        } else if (moveRight) {
            const auto noteX = chordX + headWidth - halfStem;
            note.setX(noteX);
        } else {
            const auto noteX = chordX;
            note.setX(noteX);
        }
    }
}

void
Chord::sort()
{
    std::ranges::sort(mNotes,
                      [&](const ChordNote& left, const ChordNote& right) {
        return left < right;
    });
}


bool
Chord::addFirstNote(const Note& inNote)
{
    setX(inNote.getX());
    setDuration(inNote.getDuration());
    setTime(inNote.getTime());
    TemporalImpl::initializeStemPolicy(*this, mStemPolicy);
    setIsRaw(inNote.getIsRaw());
    return true;
}


bool
Chord::addAdditionalNote(Note& inNote)
{
    inNote.setDuration(getDuration());
    inNote.setTime(getTime());
    inNote.setIsRaw(getIsRaw());
    return true;
}


void
Chord::updateDrawables()
{
    if (!mIsRegistered) {
        return;
    }    
    notifyObservers({ItemChangeType::Updated});
}


void
Chord::completeConstruction()
{
    if (mConstructorNotes.empty()) {
        return;
    }
        
    for (Note& note : mConstructorNotes) {
        addNoteImpl(note);
    }

    migrateAttachments();
    
    mConstructorNotes.clear();
}


bool
Chord::addNoteImpl(Note& noteToAdd)
{
    if (exists(noteToAdd)) {
        return false;
    }
    
    auto findPredicate = [&](const ChordNote& inChordNote) {
        return &inChordNote.getNote() == &noteToAdd;
    };
    
    auto iter = std::find_if(mNotes.cbegin(), mNotes.cend(), findPredicate);
    if (iter != mNotes.cend()) {
        return false;
    }
    
    const auto wasObserving = mIsObservingNotes;
    const auto isFirstNote = mNotes.empty();
    unobserveNotes();
    if (isFirstNote) {
        addFirstNote(noteToAdd);
    } else {
        addAdditionalNote(noteToAdd);
    }

    auto chordNote = ChordNote(noteToAdd);
    mNotes.emplace_back(chordNote);
    sort();
    
    registerNotes();
    
    updateInternal();
    if (wasObserving) {
        observeNotes();
    }
    return true;
}


ChordHeadPtrs
Chord::getChordHeadPtrStructs(ChordNotes& chordNotes) const
{
    ChordHeadPtrs heads;
    for (auto& chordNote : chordNotes) {
        heads.emplace_back(&chordNote.chordHead);
    }
    return heads;
}

// We need to update the beam before and after
// updating the chord layout because the chord layout relies
// on the beam to set the stem direction, which effects the
// chord layout, after which the beam needs to recalculate
// because the chord layout affects the stems' x positions.
void
Chord::updateBeamPassOne()
{
    updateBeam();
}

void 
Chord::updateBeamPassTwo()
{
    updateBeam();
}

void
Chord::updateBeam()
{
    if (!getIsBeamed()) {
        return;
    }

    if (getIsQuiescent()) {
        return;
    }
    
    const auto beam = getBeam();
    if (beam->getIsQuiescent()) {
        return;
    }
    
    beam->updateBeam();
}

/// Get the temporal's arpeggiate
ArpeggiateType
Chord::getArpeggiate() const
{
    return mArpeggiateType;
}

/// Set the temporal's arpeggiate
void
Chord::setArpeggiate(ArpeggiateType inArpeggiate)
{
    if (mArpeggiateType == inArpeggiate) {
        return;
    }
    
    mArpeggiateType = inArpeggiate;
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    drawable->setArpeggiateType(mArpeggiateType);

    if (!mIsRegistered) {
        return;
    }

    update();

    ItemImpl::setNeedsDisplay(*this);
}

// Move attachments from individual notes to chord so placements works.
// Marks will have to be moved back to a note when saving, as MusicXML
// does not support chord attachments.
void
Chord::migrateAttachments()
{
    std::map<MarkType, PlacementType> marksToAdd;
    
    for (Note& note : mConstructorNotes) {
        auto& attachments = note.getAttachments();
        const auto& marks = attachments.getMarks();
        for (Mark& mark : marks) {
            const auto& data = mark.getMarkData();
            marksToAdd[mark.getMarkType()] = data.getPlacementType();
            attachments.removeMark(mark);
            mLocation.getMeasure().removeItem(mark);
        }
    }
    
    if (marksToAdd.empty()) {
        return;
    }
        
    // Add the collected marks to the chord
    auto& attachments = getAttachments();
    auto& score = mLocation.getMeasure().getScore();
    auto& registry = score.getItemRegistry();
    
    for (const auto& markInfo : marksToAdd) {
        auto& mark = attachments.addMark(registry,
                                         *this,
                                         markInfo.first,
                                         markInfo.second);
        const auto& markData = mark.getMarkData();
        const auto& markPlacement = markData.getMarkPlacement();
        const auto placementType = markData.getPlacementType();
        markPlacement->placeMark(mark, placementType);
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
