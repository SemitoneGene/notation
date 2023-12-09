// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Note.h"

// Local
#include "BeamIntf.h"
#include "Chord.h"
#include "CurrentState.h"
#include "DrawableContext.h"
#include "DrawableNoteParts.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "KeySignature.h"
#include "NoteheadType.h"
#include "Mark.h"
#include "MarkImpl.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"
#include "TemporalConstants.h"
#include "TemporalImpl.h"
#include "TemporalValue.h"
#include "Track.h"
#include "TupletIntf.h"

namespace komp
{

static const std::unordered_map<AccidentalType, double> sAccidentalOffsets =
{
    { AccidentalType::FLAT, ACCIDENTAL_FLAT_SPACES },
    { AccidentalType::SHARP, ACCIDENTAL_SHARP_SPACES },
    { AccidentalType::DOUBLE_SHARP, ACCIDENTAL_DOUBLE_SHARP_SPACES },
    { AccidentalType::NATURAL, ACCIDENTAL_NATURAL_SPACES }
};

Note::Note(const MusicLocation& inMusicLocation,
           TemporalType inTemporalType,
           NoteheadType inNoteheadType,
           Pitch inPitch,
           bool isGrace)
: mAttachments{*this}
, mDuration{Duration{inTemporalType}}
, mNoteheadType(inNoteheadType)
, mPitch{inPitch}
, mSpaceTimePoint{*this}
, mIsGrace{isGrace}
, mLocation{inMusicLocation}
{
    setSlot(inMusicLocation.getSlot());
    setTime(inMusicLocation.getTime());
    TemporalImpl::initializeStemPolicy(*this, mStemPolicy);
    TemporalImpl::initializeStemPolicy(*this, mStemPolicy);
    updateStemDirection();
}


const ItemTraits&
Note::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Note);
}


ItemUPtr
Note::getMemento() const
{
    return std::make_unique<Note>(*this);
}

void
Note::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Note* downcastedP = dynamic_cast<const Note*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    // Get items that will require observers to be notified
    const auto oldX = getX();
    const auto oldSlot = getSlot();
    auto changes = mDeferredChanges;
    
    this->operator=(*downcastedP);
    update();
    setIsQuiescent(wasQuiescent);
    
    if (oldX != getX()) {
        changes.insert(ItemChangeType::X);
    }
    
    if (oldSlot != getSlot()) {
        changes.insert(ItemChangeType::Slot);
    }
    
    if (!changes.empty()) {
        notifyObservers(changes);
    }
}


void
Note::update()
{
    if (!mIsRegistered) {
        return;
    }

    notifyObservers({ItemChangeType::Updated});
    
    mAttachments.setNeedsDisplay();
    ItemImpl::setNeedsDisplay(*this);
}


void
Note::itemIDChanged()
{
    mSpaceTimePoint.setItem(*this);
    mIsRegistered = true;
}

void
Note::itemIDInvalidated()
{
    mIsRegistered = false;
}

void
Note::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
Note::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
Note::notifyObservers(const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        mDeferredChanges.insert(inChanges.begin(), inChanges.end());
        return;
    }
    
    ItemImpl::notifyObservers(*this,
                              mObservers,
                              std::move(inChanges),
                              mObserverSpinCount);
    
    placeMarks();
}


void
Note::observedItemChanged(const Item& observedItem,
                          const ItemChanges& inChanges)
{
}


bool
Note::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
Note::setIsQuiescent(bool inValue)
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
Note::getFrame() const
{
    const auto frames = getFrames();
    const auto result = frames.getCompleteFrame();
    return result;
}


Region
Note::getRegion() const
{
#ifdef KOMP_BUILD_APPLE
    CGMutablePathRef path = CGPathCreateMutable();
    const auto drawPoint = getDrawPoint();
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    const auto noteheadFrame = drawable->getNoteheadFrame(drawPoint);
    const auto noteDotsFrame = drawable->getNoteDotsFrame(drawPoint);
    const auto accidentalFrame = drawable->getAccidentalFrame(drawPoint);
    const auto stemFrame = drawable->getStemFrame(drawPoint);
    const auto flagsFrame = drawable->getNoteFlagsFrame(drawPoint);
    
    if (!noteheadFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, noteheadFrame);
    }
    
    if (!noteDotsFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, noteDotsFrame);
    }
    
    if (!accidentalFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, accidentalFrame);
    }
    
    if (!stemFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, stemFrame);
    }
    
    if (!flagsFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, flagsFrame);
    }
    
    if (getIsBeamed()) {
        
        const auto beam = getBeam();
        const auto& beamedNotes = beam->getBeamedNotes();
        
        for (const auto& beamedNote : beamedNotes) {
            if (&beamedNote->getNote() == this) {
                const auto x1 = beamedNote->getStemBeamsidePoint().getScoreX();
                const auto y1 = beamedNote->getStemBeamsidePoint().getScoreY();
                const auto x2 = beamedNote->getStemNoteheadPoint().getScoreX();
                const auto y2 = beamedNote->getStemNoteheadPoint().getScoreY();
                const Rect stemRect(y1, x1, y2, x2);
                
                if (!stemRect.getIsEmpty()) {
                    CGPathAddRect(path, NULL, stemRect);
                }
            }
        }
    }
    
    // TODO: add leger lines?
    
    // Make sure to close the path to hit testing works
    CGPathCloseSubpath(path);
    
    return path;
#elif KOMP_BUILD_LINUX
    QPainterPath path;

    const auto drawPoint = getDrawPoint();
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    const auto noteheadFrame = drawable->getNoteheadFrame(drawPoint);
    const auto noteDotsFrame = drawable->getNoteDotsFrame(drawPoint);
    const auto accidentalFrame = drawable->getAccidentalFrame(drawPoint);
    const auto stemFrame = drawable->getStemFrame(drawPoint);
    const auto flagsFrame = drawable->getNoteFlagsFrame(drawPoint);
    
    if (!noteheadFrame.getIsEmpty()) {
        path.addRect(noteheadFrame);
    }
    
    if (!noteDotsFrame.getIsEmpty()) {
        path.addRect(noteDotsFrame);
    }
    
    if (!accidentalFrame.getIsEmpty()) {
        path.addRect(accidentalFrame);
    }
    
    if (!stemFrame.getIsEmpty()) {
        path.addRect(stemFrame);
    }
    
    if (!flagsFrame.getIsEmpty()) {
        path.addRect(flagsFrame);
    }
    
    if (getIsBeamed()) {
        
        const auto beam = getBeam();
        const auto& beamedNotes = beam->getBeamedNotes();
        
        for (const auto& beamedNote : beamedNotes) {
            if (&beamedNote->getNote() == this) {
                const auto x1 = beamedNote->getStemBeamsidePoint().getScoreX();
                const auto y1 = beamedNote->getStemBeamsidePoint().getScoreY();
                const auto x2 = beamedNote->getStemNoteheadPoint().getScoreX();
                const auto y2 = beamedNote->getStemNoteheadPoint().getScoreY();
                const Rect stemRect(y1, x1, y2, x2);
                
                if (!stemRect.getIsEmpty()) {
                    path.addRect(stemRect);
                }
            }
        }
    }
    
    // TODO: add leger lines?
    
    return Region{path};
#endif
}


Point
Note::getAnchor() const
{
    const auto headFrame = getFrames().head;
    const auto center = headFrame.getCenter();
    return center;
}


bool
Note::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}

void
Note::createDrawable()
{
    ItemImpl::createDrawable<DrawableNoteParts>(*this);
}

void
Note::inserted()
{
    // This is here because the code setting the accidental offset relies on
    // SpaceSize which can only be known after measure insertion.
    setAccidental(getAccidental());
    
    update();
    notifyObservers({ItemChangeType::Inserted});
}


void
Note::removed()
{
    const auto& beam = getBeam();
    if (beam) {
        KOMP_THROW("You must break my beam before I can be taken out of the measure.");
    }
    notifyObservers({ItemChangeType::Removed});
}


void
Note::removeSelf()
{
    ItemImpl::removeSelf(*this);
    notifyObservers({ItemChangeType::Removed});
}


const MusicLocation&
Note::getLocation() const
{
    return mLocation;
}


double
Note::getX() const
{
    return mSpaceTimePoint.getMeasureRelativeX();
}


void
Note::setX(double inValue)
{
    if (inValue == getX()) {
        return;
    }
    
    ItemImpl::setX(*this, inValue, mSpaceTimePoint);
    updateBeam();
    update();
}


double
Note::getScoreX() const
{
    return mSpaceTimePoint.getScoreX();
}


int
Note::getSlot() const
{
    return ItemImpl::getSlot(mSpaceTimePoint);
}


void
Note::setSlot(int inValue)
{
    if (inValue == getSlot()) {
        return;
    }
    
    if (getStemInfo().isStemOwner
        && getStemPolicy().getIsAutomatic()
        && TemporalValue::getNumFlags(getDuration().getTemporalType())
        && !getIsRaw()) {
        if (getSlot() >= MIDDLE_SLOT) {
            setStemPolicy(StemPolicy{StemDirection::DOWN, Policy::AUTOMATIC});
        } else {
            setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        }
    }
    
    ItemImpl::setSlot(*this, inValue, mSpaceTimePoint);
    
    const auto beam = getBeam();
    if (beam && !getIsQuiescent() && !beam->getIsQuiescent()) {
        beam->observedItemChanged(*this, ItemChanges{ItemChangeType::Slot});
        const auto temporals = beam->getNotes();
        for (Temporal& t : temporals) {
            t.observedItemChanged(*this, ItemChanges{ItemChangeType::Slot});
            t.update();
        }
    }
    
    const auto chord = getChord();
    if (chord) {
        chord->observedItemChanged(*this, ItemChanges{ItemChangeType::Slot});
        const auto chordBeam = chord->getBeam();
        if (chordBeam) {
            const auto temporals = chordBeam->getNotes();
            for (Temporal& t : temporals) {
                t.observedItemChanged(*this, ItemChanges{ItemChangeType::Slot});
                t.update();
            }
        }
    }
    
    update();
    
    if (!mIsRegistered) {
        return;
    }
    
    deducePitch();
}


double
Note::getScoreY() const
{
    return mSpaceTimePoint.getScoreY();
}


Point
Note::getDrawPoint() const
{
    return ItemImpl::getDrawPoint(*this, mSpaceTimePoint);
}


double
Note::getTime() const
{
    return mSpaceTimePoint.getTime();
}


int
Note::getRoundedTime() const
{
    return ItemImpl::getRoundedTime(*this);
}


void
Note::setTime(double inValue)
{
    if (inValue == getTime()) {
        return;
    }
    
    ItemImpl::setTime(*this, mSpaceTimePoint, inValue);
    mLocation.setTime(inValue);
    update();
}


double
Note::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
Note::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}


bool
Note::getIsNote() const
{
    return true;
}


bool
Note::getIsRest() const
{
    return false;
}


bool
Note::getIsChord() const
{
    return false;
}


bool
Note::getIsChordMember() const
{
    return mChord;
}


Chord*
Note::getChord() const
{
    return mChord;
}


void
Note::setChord(Chord* inChord)
{
    if (inChord == mChord) {
        return;
    }
    
    mChord = inChord;
    update();
    notifyObservers({ItemChangeType::Chord});
}

void
Note::clearChord()
{
    if (mChord) {
        mChord = nullptr;
        update();
        notifyObservers({ItemChangeType::Chord});
    }
}

bool
Note::getIsLayoutItem() const
{
    return !getIsChordMember();
}


Pitch
Note::getPitch() const
{
    return mPitch;
}


unsigned short
Note::getGlyph() const
{
    const auto& font = FontTheme::instance();
    const auto temporalType = getDuration().getTemporalType();
    
    unsigned short result;
    
    switch (mNoteheadType)
    {
    case NoteheadType::NORMAL:
        result = font.NoteheadTypeMap.at(temporalType);
        break;
        
    case NoteheadType::CLUSTER:
        result = font.ClusterHeadMap.at(temporalType);
        break;
        
    case NoteheadType::DIAMOND:
        result = font.DiamondHeadMap.at(temporalType);
        break;
        
    case NoteheadType::TRIANGLE:
        result = font.TriangleHeadMap.at(temporalType);
        break;
        
    case NoteheadType::TRIANGLE_INVERTED:
        result = font.TriangleInvertedHeadMap.at(temporalType);
        break;
        
    case NoteheadType::SQUARE:
        result = font.SquareHeadMap.at(temporalType);
        break;
        
    case NoteheadType::X:
        result = font.XHeadMap.at(temporalType);
        break;
        
    case NoteheadType::CIRCLED_X:
        result = font.CircledXHeadMap.at(temporalType);
        break;
        
    case NoteheadType::PLUS:
        result = font.PlusHeadMap.at(temporalType);
        break;
        
    case NoteheadType::SLASH_H_END:
        result = font.SlashHorzEndHeadMap.at(temporalType);
        break;
        
    case NoteheadType::SLASH_V_END:
        result = font.SlashVertEndHeadMap.at(temporalType);
        break;
        
    default:
        result = font.NoteheadTypeMap.at(temporalType);
        break;
    }
    
    return result;
}


Duration
Note::getDuration() const
{
    return mDuration;
}


void
Note::setDuration(komp::Duration inDuration)
{
    if (inDuration.equals(getDuration())) {
        return;
    }
    
    TemporalImpl::setDuration(*this, inDuration, mDuration);
    updateStemDirection();
    update();
}


bool
Note::getIsRaw() const
{
    return mIsRaw;
}


void
Note::setIsRaw(bool inValue)
{
    if (inValue == mIsRaw) {
        return;
    }
    
    mIsRaw = inValue;
    
    if (!inValue) {
        setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
        updateStemDirection();
    } else {
        setStemPolicy(StemPolicy{StemDirection::NONE, Policy::AUTOMATIC});
    }
    
    if (getIsChordMember()) {
        const auto& chordTemporal = getChord();
        chordTemporal->setIsRaw(inValue);
    }
}


StemPolicy
Note::getStemPolicy() const
{
    const auto beam = getBeam();
    if (beam) {
        return beam->getStemPolicy();
    }
    
    const auto& chord = getChord();
    if (chord) {
        return chord->getStemPolicy();
    }
    
    // TODO: having an issue with stem direction being set to UP
    // instead of NONE when isRaw.  Don't have time to fix it right
    // now so will force the return to be correct here.
    auto result = mStemPolicy;
    
    if (getIsRaw()) {
        result.direction = StemDirection::NONE;
    }
    
    return result;
}


void
Note::setStemPolicy(StemPolicy inPolicy)
{
    if (inPolicy == mStemPolicy) {
        return;
    }
        
    TemporalImpl::setStemPolicy(*this, inPolicy, mStemPolicy);
    update();
    notifyObservers({ItemChangeType::StemDirection});
}


StemInfo
Note::getStemInfo() const
{
    if (!mIsRegistered) {
        return StemInfo{};
    }
    
    StemInfo info;
    info.staff = &getLocation().getStaff();
    KOMP_ASSERT(info.staff);
    info.thickness = STEM_THICKNESS;
    info.temporalY = getScoreY();
    
    // Get stem placement info
    const auto& font = FontTheme::instance();
    if (font.StemPlacementMap.find(mNoteheadType) != font.StemPlacementMap.end()) {
        const auto stemPlacement = font.StemPlacementMap.at(mNoteheadType);
        const auto upAttachPoint = stemPlacement.upSE;
        const auto upAttachX = upAttachPoint.x;
        const auto upAttachY = -1.0 * upAttachPoint.y;
        const auto downAttachPoint = stemPlacement.downNW;
        const auto downAttachX = downAttachPoint.x;
        const auto downAttachY = -1.0 * downAttachPoint.y;
        
        info.yUp = upAttachY;
        info.xUp = upAttachX;
        info.yDown = downAttachY;
        info.xDown = downAttachX;
    } else {
        LOG_F(WARNING, "Missing stem placement info: %d", mNoteheadType);
    }
    
    const auto slot = getSlot();
    info.attachSlot = slot;
    info.escapeSlot = slot;
    const auto slotSize = Staff::getSlotSize();
    info.attachSlotPreciseUp = static_cast<double>(slot) + (info.yUp / slotSize);
    info.attachSlotPreciseDown = static_cast<double>(slot) + (info.yDown / slotSize);
    info.escapeSlotPreciseUp = info.attachSlotPreciseUp;
    info.escapeSlotPreciseDown = info.attachSlotPreciseDown;
    
    // Additional info
    info.temporalType = getDuration().getTemporalType();
    info.hasFlag = TemporalValue::getNumFlags(info.temporalType);
    info.flagsFontSize = Staff::getFontSize();
    
    if (getIsChordMember() || getIsBeamed()) {
        info.isStemOwner = false;
    } else {
        info.isStemOwner = true;
    }
    
    return info;
}


PlaybackNote
Note::getPlaybackNote() const
{
    return TemporalImpl::getPlaybackNote(*this);
}


Attachments&
Note::getAttachments()
{
    return mAttachments;
}


const Attachments&
Note::getAttachments() const
{
    return mAttachments;
}


bool
Note::getIsBeamed() const
{
    return mBeam;
}


BeamIntf*
Note::getBeam() const
{
    return mBeam;
}


void
Note::clearBeam()
{
    mBeam = nullptr;
    update();
    notifyObservers({ItemChangeType::Beam});
}


void
Note::setBeam(BeamIntf& inBeam)
{
    if (&inBeam != mBeam) {
        mBeam = &inBeam;
        
        mAttachments.setNeedsDisplay();
        ItemImpl::setNeedsDisplay(*this);

        notifyObservers({ItemChangeType::Beam, ItemChangeType::Updated});
    }
}


bool
Note::getIsTupleted() const
{
    return TemporalImpl::getIsTupleted(mTuplets);
}


TupletIntfRefVec
Note::getTuplets() const
{
    return TemporalImpl::getTuplets(mTuplets);
}


bool
Note::addTuplet(TupletIntf& inTuplet)
{
    return TemporalImpl::addTuplet(*this, mTuplets, inTuplet);
}


bool
Note::removeTuplet(TupletIntf& inTuplet)
{
    return TemporalImpl::removeTuplet(*this, mTuplets, inTuplet);
}


NoteFrames
Note::getFrames() const
{
    NoteFrames frames{};
    const auto drawPoint = getDrawPoint();
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    frames.head = drawable->getNoteheadFrame(drawPoint);
    frames.accidental = drawable->getAccidentalFrame(drawPoint);
    frames.dots = drawable->getNoteDotsFrame(drawPoint);
    frames.techniques = mAttachments.getTechniquesFrame(drawPoint);
    if (getStemInfo().isStemOwner) {
        frames.stem = drawable->getStemFrame(drawPoint);
    } else if (!getIsChordMember()) {
        const auto beam = getBeam();
        if (beam) {
            frames.stem = beam->getStemFrame(*this);
        }
    }
    frames.flags = drawable->getNoteFlagsFrame(drawPoint);
    frames.drawableLegerLines = drawable->getLegerLinesFrame(drawPoint);
    return frames;
}


bool
Note::getIsTupletStart() const
{
    const auto& tuplets = getTuplets();
    if (tuplets.size() == 0) {
        return false;
    }
    
    const TupletIntf& tuplet = tuplets.at(0);
    const auto chord = getChord();
    
    // if I'm the bottom note of a chord, and if the chord is the first
    // temporal in a tuplet, then I am a tuplet start
    if (chord) {
        const auto& bottomNote = chord->getBottomNote();
        if (getItemID() == bottomNote.getItemID()) {
            const Temporal& firstTupletTemporal = tuplet.getTemporals().front();
            if (chord == &firstTupletTemporal) {
                return true;
            }
        }
    }
    
    // if I'm not in a chord and I'm the first temporal in a tuplet, then true
    if (!chord) {
        Temporal& firstTupletTemporal = tuplet.getTemporals().front();
        if (&firstTupletTemporal == this) {
            return true;
        }
    }
    
    return false;
}


bool
Note::getIsTupletStop() const
{
    const auto& tuplets = getTuplets();
    if (tuplets.size() == 0) {
        return false;
    }
    
    const TupletIntf& tuplet = tuplets.at(0);
    const auto& chord = getChord();
    
    // if I'm the top note of a chord, and if the chord is the last
    // temporal in a tuplet, then I am a tuplet stop
    if (chord) {
        const auto& topNote = chord->getTopNote();
        if (getItemID() == topNote.getItemID()) {
            const Temporal& lastTupletTemporal = tuplet.getTemporals().back();
            if (chord == &lastTupletTemporal) {
                return true;
            }
        }
    }
    
    // if I'm not in a chord and I'm the last temporal in a tuplet, then true
    if (!chord) {
        Temporal& lastTupletTemporal = tuplet.getTemporals().back();
        if (&lastTupletTemporal == this) {
            return true;
        }
    }
    
    return false;
}


bool
Note::getIsGrace() const
{
    return mIsGrace;
}

// MARK: - Accidental

AccidentalType
Note::getAccidental() const
{
    return mAccidentalType;
}


void
Note::setAccidental(AccidentalType inAccidental)
{
    if (mAccidentalType == inAccidental) {
        return;
    }
    
    mAccidentalType = inAccidental;
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    drawable->setAccidentalType(inAccidental);
    
    if (!mIsRegistered) {
        return;
    }
    
    if (drawable->getAccidentalType() != AccidentalType::NONE) {
        setAccidentalOffsetDefault();
    }
    deducePitch();
    
    const auto chord = getChord();
    if (chord && !chord->getIsQuiescent() && !getIsQuiescent()) {
        chord->update();
    }
    ItemImpl::setNeedsDisplay(*this);
}


double
Note::getAccidentalOffset() const
{
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    return drawable->getAccidentalOffset().x;
}


void
Note::setAccidentalOffset(double inValue)
{
    if (getAccidentalOffset() == inValue) {
        return;
    }
    
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    drawable->setAccidentalOffset(Point{inValue, 0.0});
    update();
}


void
Note::setAccidentalOffsetDefault()
{
    if (!mIsRegistered) {
        return;
    }
        
    double distance = 0.0;
    
    const auto found = sAccidentalOffsets.find(mAccidentalType);
    if (found != sAccidentalOffsets.cend()) {
        const auto spaces = found->second;
        distance = spaces * getSpaceSize();
    }
        
    const auto drawable = ItemImpl::getDrawable<DrawableNoteParts>(*this);
    const auto frame = drawable->getAccidentalFrame(Point{});
    const auto accidentalWidth = frame.getWidth();
    const auto xOffset = distance + accidentalWidth;
    setAccidentalOffset(-1 * xOffset);
    update();
}


void
Note::deduceSlot()
{
    auto clef = mLocation.getClef();
    auto slot = getPitch().slot(clef);
    setSlot(slot);
    
    if (!mIsRegistered) {
        return;
    }
    
    deducePitch();
}


void
Note::deduceAccidental()
{
    deduceSlot();
    const auto operativeAccidental = mLocation.getAccidental();
    const auto requiredAccidental = AccidentalInfo::getAccidentalType(getPitch().alter);
    
    if (operativeAccidental == requiredAccidental) {
        return;
    } else if (requiredAccidental == AccidentalType::NONE) {
        setAccidental(AccidentalType::NATURAL);
    } else {
        setAccidental(requiredAccidental);
    }
}

void
Note::deducePitch()
{
    const auto slot = getSlot();
    const auto& clef = mLocation.getClef();
    const auto& key = mLocation.getKeySignature();
    const auto keyAccidental = KeySignature::getAccidental(key,
                                                           clef,
                                                           slot);
    
    const auto midiNoteForSlot = Pitch::midiNoteForSlot(slot, clef);

    auto accidental = getAccidental();
    
    // If our current accidental does not specify pitch alteration
    // then we must look back in the measure to figure out what
    // our pitch should be
    if (accidental == AccidentalType::NONE
    || !AccidentalInfo::isPitchAltering(accidental)) {
        
        // get the notes for this track hand measure and slot
        auto predicate = [&](Item& inItem) {
            
            if (inItem.getLocation().getHand() != getLocation().getHand()) {
                return false;
            }
            
            const auto& itemTraits = inItem.getItemTraits();
            const auto itemType = itemTraits.getItemType();
            if (itemType != ItemType::Note) {
                return false;
            }
            
            auto& note = inItem.as<Note>();
            if (note.getSlot() != slot) {
                return false;
            }
            
            return true;
        };
        
        auto notes = mLocation.getMeasure().items<Note>(mLocation.getTrack(),
                                                        predicate);
        
        // iterate backwards through those notes and check
        // the accidental
        auto isPrecedingAccidentalFound = false;
        for (auto riter = notes.crbegin(); riter != notes.crend(); ++riter) {
            const auto currentAccidental = (*riter).get().getAccidental();
            if (currentAccidental == AccidentalType::TRIPLE_FLAT ||
                currentAccidental == AccidentalType::DOUBLE_FLAT ||
                currentAccidental == AccidentalType::FLAT ||
                currentAccidental == AccidentalType::NATURAL ||
                currentAccidental == AccidentalType::SHARP ||
                currentAccidental == AccidentalType::DOUBLE_SHARP ||
                currentAccidental == AccidentalType::TRIPLE_SHARP) {
                
                // if an accidental is found break out of the loop
                accidental = currentAccidental;
                isPrecedingAccidentalFound = true;
                break;
            }
        }
        
        // if the loop finishes without finding an accidental
        // then take the accidental of the key signature
        if (!isPrecedingAccidentalFound) {
            accidental = keyAccidental;
        }
    }
    
    Pitch pitch{clef, slot, midiNoteForSlot};
    if (AccidentalInfo::isPitchAltering(accidental)) {
        pitch.alter = AccidentalInfo::getPitchAlter(accidental);
    }
    
    if (mPitch != pitch) {
        mPitch = pitch;
        if (mIsRegistered) {
            notifyObservers({ItemChangeType::Pitch});
            update();
        }
    }
}


void
Note::beginDrag(const CurrentState& inState)
{
    const auto x = getScoreX();
    const auto y = getScoreY();
    const auto& point = inState.getPoint();
    const auto deltaX = point.x - x;
    const auto deltaY = point.y - y;
    mDragStartOffset = Point{deltaX, deltaY};
}


void
Note::drag(const CurrentState& inState)
{
    const auto& point = inState.getPoint();
    const Point adjustedPoint {
        point.x - mDragStartOffset.x,
        point.y - mDragStartOffset.y
    };
    
    const auto slot = getLocation().getStaff().pixelToSlot(adjustedPoint.y);
    if (slot == getSlot()) {
        return;
    }
    
    setSlot(slot);
    deducePitch();
    
    updateBeam();
    updateStemDirection();
    update();
    
    if (getIsBeamed()) {
        mBeam->updateDrag(inState);
    }

    updateTuplet();
    updateChord();
}


void
Note::endDrag(const CurrentState& inState)
{
    update();
    mDragStartOffset = Point{};
    
    if (getIsBeamed()) {
        mBeam->completeDrag(inState);
    }
    
    updateTuplet();
    updateChord();
}

void
Note::updateStemDirection()
{
    TemporalImpl::updateStemDirection(*this);
}

void
Note::updateBeam()
{
    const auto beam = getBeam();
    if (beam && !getIsQuiescent() && !beam->getIsQuiescent()) {
        mBeam->updateBeam();
    }
}

void
Note::updateChord()
{
    const auto chord = getChord();
    if (chord) {
        chord->update();
        ItemImpl::setNeedsDisplay(*chord);
    }
}

void
Note::updateTuplet()
{
    if (getIsTupleted()) {
        for (const auto& tuplet : mTuplets) {
            ItemImpl::setNeedsDisplay(tuplet);
        }
    }
}

void
Note::placeMarks()
{
    MarkImpl::placeMarks(*this);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
