// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Beam.h"

// Local
#include "Chord.h"
#include "DrawableContext.h"
#include "DrawableLine.h"
#include "Engraver.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Note.h"
#include "BeamLineIntf.h"
#include "Score.h"
#include "ScoreColors.h"

namespace komp
{
Beam::Beam(const MusicLocation& inLocation,
                 const BeamSpecIntf& inBeamSpec)
: BeamIntf()
, mBeamSpec(inBeamSpec.clone())
, mLocation{inLocation}
{
    KOMP_ASSERT(mBeamSpec->getIsValid());
    KOMP_ASSERT(mBeamSpec->getBeamedNoteCount() > 1);

    for (const auto& note : getBeamedNotes()) {
        KOMP_ASSERT(!note->getNote().getIsChordMember());
    }
}


Beam::Beam(const Beam& other)
: BeamIntf(other)
, mBeamSpec(other.mBeamSpec->clone())
, mAreNotesNotified(other.mAreNotesNotified)
, mCache(other.mCache)
, mHasStemFlipEventOccurred(other.mHasStemFlipEventOccurred)
, mLocation(other.mLocation)
{
    
}


void
Beam::copyState(const Beam& other)
{
    mBeamSpec = other.mBeamSpec->clone();
    mAreNotesNotified = other.mAreNotesNotified;
    mCache = other.mCache;
    mHasStemFlipEventOccurred = other.mHasStemFlipEventOccurred;
}


Beam&
Beam::operator=(const Beam& other)
{
    copyState(other);
    return *this;
}


const ItemTraits&
Beam::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Beam);
}


ItemUPtr
Beam::getMemento() const
{
    return std::make_unique<Beam>(*this);
}


void
Beam::restoreMemento(const Item& inItem)
{
    bool wasQuiescent = getIsQuiescent();
    const Item* itemP = &inItem;
    const Beam* downcastedP = dynamic_cast<const Beam*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}


void
Beam::update()
{
    if (getIsQuiescent()) {
        return;
    }

    if (mCache.isDirty) {
        updateBeam();
    }
    
    ItemImpl::setNeedsDisplay(*this);
    notifyObservers({ItemChangeType::Updated});
}


void
Beam::itemIDChanged()
{
    
}


void
Beam::itemIDInvalidated()
{
    
}


void
Beam::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
Beam::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
Beam::notifyObservers(const ItemChanges& inChanges)
{
    ItemImpl::notifyObservers(*this, mObservers, std::move(inChanges));
}


void
Beam::observedItemChanged(const Item& observedItem, const ItemChanges& inChanges)
{
    // Stem Flippers
    const auto isSlotChange = ItemObserver::contains(inChanges, ItemChangeType::Slot);
    const auto isChordChange = ItemObserver::contains(inChanges, ItemChangeType::Chord);
    const auto isStemChange = ItemObserver::contains(inChanges, ItemChangeType::StemDirection);
    const auto isHandChange = ItemObserver::contains(inChanges, ItemChangeType::Hand);
    const auto isVoiceChange = ItemObserver::contains(inChanges, ItemChangeType::Voice);
    const auto isUpdated = ItemObserver::contains(inChanges, ItemChangeType::Updated);
    
    const auto isXChange = ItemObserver::contains(inChanges, ItemChangeType::X);
    const auto isDurationChange = ItemObserver::contains(inChanges, ItemChangeType::Duration);
    const auto isMeasureChange = ItemObserver::contains(inChanges, ItemChangeType::Measure);
        
    if (isSlotChange || isChordChange || isStemChange || isHandChange || isVoiceChange) {
        mHasStemFlipEventOccurred = true;
        mCache.isDirty = true;
        updateDrawables();
    } else if (isXChange || isDurationChange || isMeasureChange || isUpdated) {
        //mCache.isDirty = true;
    }
        
    if (!getIsQuiescent()) {
        ItemImpl::notifyObservers(*this, mObservers, std::move(inChanges));
    }
}


bool
Beam::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
Beam::setIsQuiescent(bool inValue)
{
    mCache.isDirty = true;
    mIsQuiescent = inValue;
}


Rect
Beam::getFrame() const
{
    if (mCache.isDirty) {
        updateBeamSpec();
    }
    
    if (mCache.isFrameDirty) {
        cacheFrame();
    }
    
    return mCache.frame;
}


Region
Beam::getRegion() const
{
    if (mCache.isDirty) {
        updateBeamSpec();
    }
    
    if (mCache.isFrameDirty) {
        cacheFrame();
    }
    
    return mCache.region;
}


Point
Beam::getAnchor() const
{
    return komp::Point{getScoreX(), getScoreY()};
}


bool
Beam::contains(const Point& inPoint, bool inPrecise) const
{
    const auto result = ItemImpl::contains(*this, inPoint, inPrecise);
    return result;
}


// MARK: - Drawing

void
Beam::createDrawable()
{
    ItemImpl::createDrawable<DrawableComposite>(*this);
}

void
Beam::updateDrawables() const
{
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    drawable->clearDrawables();
    
    const auto& loc = getLocation();
    
    const auto& measure = loc.getMeasure();
    const auto& staff = loc.getStaff();
    
    // The old-fashioned beaming code does everything in absolute score
    // position, however drawables drawing code uses a coordinate system that
    // is relative to 'something local'. so we must choose something from our
    // beam to be the { 0, 0 } for the drawables.
    const auto& scoreDrawPoint = mBeamSpec->getDrawPoint();
    const auto measureX = measure.getDataX();
    
    const auto& beamedNote = getBeamedNotes().front()->getNote();
    const auto scale = beamedNote.getIsGrace() ? .75 : 1.0;
    
    for (const auto& stem : mCache.stems) {
        // Figure out exactly where the line is supposed to be on the score
        const auto scoreX1 = stem.stemLine.measureRelativeX1 + measureX;
        const auto scoreX2 = stem.stemLine.measureRelativeX2 + measureX;
        const auto scoreY1 = staff.slotToPixel(stem.stemLine.slot1);
        const auto scoreY2 = staff.slotToPixel(stem.stemLine.slot2);
        const Point scorePointA{ scoreX1, scoreY1 };
        const Point scorePointB{ scoreX2, scoreY2 };
        
        // Subtract the draw point from these to get 'local' points relative
        // to this beam's draw point
        const Point localPointA = scorePointA - scoreDrawPoint;
        const Point localPointB = scorePointB - scoreDrawPoint;
        
        // TODO: Make a drawable line interface that takes point info instead
        // of length and degrees to calculate the length and degrees of the line
        /// and stroke as that is how drawable line's interface is.
        const auto length = localPointA.distance(localPointB);
        const auto degrees = localPointA.degrees(localPointB);
        
        // Construct the drawable line for the stem
        auto drawableLine = std::make_unique<DrawableLine>();
        drawableLine->setLength(length);
        drawableLine->setDegrees(degrees);
        drawableLine->setThickness(stem.stemLine.thickness);
        drawableLine->setScale(scale);
        
        // Use the localPointA as the 'offset'.
        // This beam's draw point will be added to it at draw time.
        drawableLine->setOffset(localPointA);
        drawable->addDrawable(std::move(drawableLine));
    }
    
    for (const auto& line : mCache.beamLines) {
        // figure out exactly where the line is supposed to be on the score
        const auto scoreX1 = line.measureRelativeX1 + measureX;
        const auto scoreX2 = line.measureRelativeX2 + measureX;
        const auto scoreY1 = staff.slotToPixel(line.slot1);
        const auto scoreY2 = staff.slotToPixel(line.slot2);
        const Point scorePointA{ scoreX1, scoreY1 };
        const Point scorePointB{ scoreX2, scoreY2 };
        
        // subtract the draw point from these to get 'local' points relative to this beam's draw point
        const auto localPointA = scorePointA - scoreDrawPoint;
        const auto localPointB = scorePointB - scoreDrawPoint;
        
        // TODO: Make a drawable line interface that takes point info instead
        // of length and degrees to calculate the length and degrees of the line
        /// and stroke as that is how drawable line's interface is.
        const auto length = localPointA.distance(localPointB);
        const auto degrees = localPointA.degrees(localPointB);
        
        // Construct the drawable line
        auto drawableLine = std::make_unique<DrawableLine>();
        drawableLine->setLength(length);
        drawableLine->setDegrees(degrees);
        drawableLine->setThickness(line.thickness);
        drawableLine->setScale(scale);
        
        // use the localPointA as the 'offset', this beam's draw point will be added to it at draw time.
        drawableLine->setOffset(localPointA);
        drawable->addDrawable(std::move(drawableLine));
    }
}

void
Beam::inserted()
{
    mBeamSpec->replaceChordedNotes();
    notifyNotes();
    updateBeam();
    observe();
    notifyObservers({ItemChangeType::Inserted});
}


void
Beam::removed()
{
    unobserve();
    notifyNotesRemoved();
    notifyObservers({ItemChangeType::Removed});
}


void
Beam::removeSelf()
{
    auto& measure = getLocation().getMeasure();
    measure.removeItem(*this);
}


const MusicLocation&
Beam::getLocation() const
{
    return mLocation;
}


double
Beam::getX() const
{
    if (mCache.isDirty) {
        updateBeamSpec();
    }
    
    if (mCache.getMainBeam()) {
        const auto mainBeamOffsetX1 = mCache.getMainBeam()->measureRelativeX1;
        return mainBeamOffsetX1;
    } else {
        return 0.0;
    }
}

/// \brief Ignored because X is determined by the BeamSpec and first note.
void
Beam::setX(double inValue)
{    
}


double
Beam::getScoreX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto measureX = measure.getDataX();
    const auto thisX = getX();
    const auto resultX = measureX + thisX;
    return resultX;
}


int
Beam::getSlot() const
{
    if (mBeamSpec->getBeamLineCount() == 0) {
        return 0;
    }
    
    if (mBeamSpec->getBeamedNoteCount() == 0) {
        return 0;
    }
    
    const auto& mainBeam = mBeamSpec->getBeamLine(0);
    const auto slot = mainBeam.getPointA().getSlot();
    return komp::roundToInt(slot);
}


/// \brief Ignored because the sllot is determined by the BeamSpec and first note
void
Beam::setSlot(int inValue)
{
}


double
Beam::getScoreY() const
{
    const auto& loc = getLocation();
    const auto& staff = loc.getStaff();
    const auto& mainBeam = mBeamSpec->getBeamLine(0);
    const auto slot = mainBeam.getPointA().getSlot();
    
    return staff.slotToPixel(slot);
}


Point
Beam::getDrawPoint() const
{
    // this was changed during the large draw performance re-write in april 2020. side-effects of this change unknown!
    return mBeamSpec->getDrawPoint();
}

double
Beam::getTime() const
{
    if (mBeamSpec->getBeamedNoteCount() > 0) {
        return mBeamSpec->getBeamedNote(0).getNote().getTime();
    } else {
        return 0.0;
    }
}


int
Beam::getRoundedTime() const
{
    return komp::roundToInt(getTime());
}


/// \brief Ignored because the beam's time is bound to the first beamed temporal.
void
Beam::setTime(double inValue)
{
}


double
Beam::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
Beam::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}

void
Beam::notifyMeasureXChanged()
{
    mCache.isFrameDirty = true;
}


void
Beam::notifyStaffYChanged()
{
    mCache.isFrameDirty = true;
}


void
Beam::beginDrag(const CurrentState& inState)
{
    mCache.isDirty = true;
    updateBeamSpec();
    update();
}


void
Beam::drag(const CurrentState& inState)
{
    mCache.isDirty = true;
    updateBeamSpec();
    update();
}


void
Beam::endDrag(const CurrentState& inState)
{
    mCache.isDirty = true;
    updateBeamSpec();
    update();
}


// MARK: - Beam Code

Rect
Beam::getBeamLinesFrame() const
{
    if (mCache.isDirty) {
        updateBeamSpec();
    }
    const auto& loc = getLocation();
    const auto measureDataX = loc.getMeasure().getDataX();
    const auto& staff = loc.getStaff();
    
#ifdef KOMP_BUILD_APPLE
    Region path{::CGPathCreateMutable()};
#elif KOMP_BUILD_LINUX
    Region path{QPainterPath()};
#endif
    
    for (const auto& lineCache : mCache.beamLines) {
        const auto scoreX1 = measureDataX + lineCache.measureRelativeX1;
        const auto scoreX2 = measureDataX + lineCache.measureRelativeX2;
        const auto scoreY1 = staff.slotToPixel(lineCache.slot1);
        const auto scoreY2 = staff.slotToPixel(lineCache.slot2);
        
        Rect frame{
            std::min(scoreY1, scoreY2),
            std::min(scoreX1, scoreX2),
            std::max(scoreY1, scoreY2),
            std::max(scoreX1, scoreX2)
        };
        
        mCache.frame.initializeOrUnion(frame);
        path.addRect(frame);        
    }
    return path.getBounds();
}


void
Beam::updateBeam()
{
    if (getIsQuiescent()) {
        return;
    }
    
    if (!mCache.isDirty) {
        return;
    }
    
    notifyNotes();
    updateBeamSpec();
    notifyObservers({ItemChangeType::Updated});
}


const BeamedNoteIntfUPtrVec&
Beam::getBeamedNotes() const
{
    return mBeamSpec->getBeamedNotes();
}


TemporalRefVec
Beam::getNotes() const
{
    TemporalRefVec items;
    for (const auto& beamedNote : getBeamedNotes()) {
        auto& note = beamedNote->getNote();
        items.emplace_back(note);
    }
    return items;
}


StemPolicy
Beam::getStemPolicy() const
{
    return mCache.stemPolicy;
}


void
Beam::setStemPolicy(StemPolicy inPolicy)
{
    if (inPolicy == getStemPolicy()) {
        return;
    }
    
    mCache.stemPolicy = inPolicy;
    mCache.isDirty = true;
    mCache.isStemDirty = false;
    mHasStemFlipEventOccurred = false;
    updateBeamSpec();
}


bool
Beam::getIsFlat() const
{
    if (mCache.isDirty) {
        updateBeamSpec();
    }
    
    if (!mCache.getMainBeam()) {
        return true;
    }
    
    return mCache.getMainBeam()->slope == 0.0;
}


double
Beam::getYIntercept(double inScoreX) const
{
    if (mCache.isDirty) {
        updateBeamSpec();
    }
    
    if (mCache.isFrameDirty) {
        cacheFrame();
    }
    
    if (!mCache.getMainBeam()) {
        return 0.0;
    }
    
    const auto& mainBeam = *mCache.getMainBeam();
    const auto measureRelativeX = std::min(mainBeam.measureRelativeX1,
                                           mainBeam.measureRelativeX2);
    const auto beamScoreX = mCache.measureDataX + measureRelativeX;
    
    if (!std::isfinite(mainBeam.slope)) {
        return 0.0;
    }
    
    const auto result = BeamLineIntf::getYIntercept(beamScoreX,
                                                       mCache.mainBeamStartY,
                                                       mainBeam.slope,
                                                       inScoreX);
    
    return result;
}


double
Beam::getBeamLineThickness(int inBeamLineIndex) const
{
    if (!mBeamSpec->getIsValid() ||
        mBeamSpec->getBeamLineCount() <= 0) {
        return 0.0;
    }
    
    if (inBeamLineIndex < 0
        || inBeamLineIndex > mBeamSpec->getBeamLineCount()) {
        
    }
    
    return mBeamSpec->getBeamLine(inBeamLineIndex).getThickness();
}


Rect
Beam::getStemFrame(const Temporal& inTemporal) const
{
    for (const auto& beamedNote : getBeamedNotes()) {
        
        if (&beamedNote->getNote() == &inTemporal) {
            
            const auto& headPoint = beamedNote->getStemNoteheadPoint();
            const auto& beamPoint = beamedNote->getStemBeamsidePoint();
            
            const auto noteheadY = headPoint.getScoreY();
            const auto beamsideY = beamPoint.getScoreY();
            
            const double halfThickness = beamedNote->getStemThickness() / 2.0;
            double x1 = beamPoint.getScoreX() - halfThickness;
            double x2 = x1 + beamedNote->getStemThickness();
            
            Rect result{0, 0, 0, 0, };
            result.top = noteheadY;
            result.bottom = beamsideY;
            result.left = x1;
            result.right = x2;
            
            if (result.bottom < result.top) {
                std::swap(result.top, result.bottom);
            }
            
            if (result.right < result.left) {
                std::swap(result.right, result.left);
            }
            
            return result;
        }
    }
    return Rect{};
}


bool
Beam::getIsMember(const Temporal& inTemporal) const
{
    const auto& beamedNotes = getBeamedNotes();
    for (const auto& bn : beamedNotes) {
        if (&bn->getNote() == &inTemporal) {
            return true;
        }
    }
    return false;
}


bool
Beam::getIsFirst(const Temporal& inTemporal) const
{
    const auto& beamedNotes = getBeamedNotes();
    if (beamedNotes.empty()) {
        return false;
    }
    return &beamedNotes.front()->getNote() == &inTemporal;
}


bool
Beam::getIsLast(const Temporal& inTemporal) const
{
    const auto& beamedNotes = getBeamedNotes();
    if (beamedNotes.empty()) {
        return false;
    }
    return &beamedNotes.back()->getNote() == &inTemporal;
}


const BeamSpecIntf&
Beam::getBeamSpec() const
{
    return *mBeamSpec;
}


void
Beam::notifyNotes()
{
    if (mAreNotesNotified) {
        return;
    }
    
    mAreNotesNotified = true;
    
    for (int i = 0; i < mBeamSpec->getBeamedNoteCount(); ++i) {
        auto& note = mBeamSpec->getBeamedNote(i).getNote();
        note.setBeam(*this);
        note.registerObserver(*this);
    }
}


void
Beam::notifyNotesRemoved()
{
    for (int i = 0; i < mBeamSpec->getBeamedNoteCount(); ++i) {
        auto& note = mBeamSpec->getBeamedNote(i).getNote();
        note.clearBeam();
        note.unregisterObserver(*this);
    }
}


void
Beam::updateBeamSpec() const
{
    if (!mCache.isDirty) {
        return;
    }
    
    if (!mBeamSpec->getIsValid()) {
        return;
    }
    
    const auto originalStemPolicy = mCache.stemPolicy;
    const bool isAutomatic = originalStemPolicy.getIsAutomatic();
    const bool isDirectionBad = (!originalStemPolicy.getIsStemUp()) && (!originalStemPolicy.getIsStemDown());
    const bool doSpecify = !isDirectionBad && (!isAutomatic || !mHasStemFlipEventOccurred);
    const bool doUpdatePolicy = isDirectionBad || (isAutomatic && mHasStemFlipEventOccurred);
    
    auto stemPolicy = mCache.stemPolicy;
    
    // If someone has purposefully set the stem direction, but the policy is automatic
    // we should preserve the user's stem flippage until a drag or slot change occurs
    if (doSpecify) {
        stemPolicy.policy = Policy::SPECIFIED;
    }
    
    mBeamSpec->calculateBeamLines(stemPolicy);
    
    if (doUpdatePolicy) {
        mCache.stemPolicy.direction = stemPolicy.direction;
    }
    
    clearCache();
    cacheBeamLineValues();
    cacheStemValues();
    mCache.isFrameDirty = true;
    updateDrawables();
}


void
Beam::observe()
{
    const BeamSpecIntf& beamSpec = *mBeamSpec;
    
    for (int i = 0; i < beamSpec.getBeamedNoteCount(); ++i) {
        const auto& beamedNote = beamSpec.getBeamedNote(i);
        auto& temporal = beamedNote.getNote();
        temporal.registerObserver(*this);
        const auto& chordTemporal = temporal.getChord();
        
        if (chordTemporal) {
            const auto& chord = chordTemporal->as<Chord>();
            for (auto& chordNote : chord.getNotes()) {
                chordNote.get().registerObserver(*this);
            }
        }
    }
}


void
Beam::unobserve()
{
    const BeamSpecIntf& beamSpec = *mBeamSpec;
    
    for (int i = 0; i < beamSpec.getBeamedNoteCount(); ++i) {
        const auto& beamedNote = beamSpec.getBeamedNote(i);
        auto& temporal = beamedNote.getNote();
        temporal.unregisterObserver(*this);
        const auto& chordTemporal = temporal.getChord();
        
        if (chordTemporal) {
            const auto& chord = chordTemporal->as<Chord>();
            for (auto& chordNote : chord.getNotes()) {
                chordNote.get().unregisterObserver(*this);
            }
        }
    }
}


void
Beam::clearCache() const
{
    mCache.beamLines.clear();
    mCache.stems.clear();
    mCache.frame = Rect{0.0, 0.0, 0.0, 0.0};
    mCache.region = Region{};
    mCache.measureDataX = 0.0;
    mCache.mainBeamStartY = 0.0;
}


void
Beam::cacheBeamLineValues() const
{
    const auto& beamLines = mBeamSpec->getBeamLines();
    bool isStillDirty = false;
    
    for (const auto& beamLine : beamLines) {
        
        const auto& pointA = beamLine->getPointA();
        const auto& pointB = beamLine->getPointB();
        
        LineCache lcache;
        lcache.measureRelativeX1 = pointA.getMeasureRelativeX();
        lcache.measureRelativeX2 = pointB.getMeasureRelativeX();
        lcache.slot1 = pointA.getSlot();
        lcache.slot2 = pointB.getSlot();
        lcache.thickness = beamLine->getThickness();
        
        if (!beamLine->getIsVertical()) {
            lcache.slope = beamLine->getSlope();
        } else {
            isStillDirty = true;
        }
        
        // Cache the frame relative to the measure
        Rect frame{std::max(lcache.slot1, lcache.slot2),
            std::min(lcache.measureRelativeX1, lcache.measureRelativeX2),
            std::min(lcache.slot1, lcache.slot2),
            std::max(lcache.measureRelativeX1, lcache.measureRelativeX2)};
        
        mCache.beamLines.emplace_back(std::move(lcache));
    }
    
    if (isStillDirty) {
        mCache.isDirty = true;
    } else {
        mCache.isDirty = false;
    }
}


void
Beam::cacheStemValues() const
{
    const auto& beamedNotes = getBeamedNotes();
    for (const auto& beamedNote : beamedNotes) {
        
        const auto& headPoint = beamedNote->getStemNoteheadPoint();
        const auto& beamPoint = beamedNote->getStemBeamsidePoint();
        
        LineCache lcache;
        lcache.measureRelativeX1 = headPoint.getMeasureRelativeX();
        lcache.measureRelativeX2 = beamPoint.getMeasureRelativeX();
        lcache.slot1 = headPoint.getSlot();
        lcache.slot2 = beamPoint.getSlot();
        lcache.thickness = beamedNote->getStemThickness();
        mCache.stems.emplace_back(StemCache {
            beamedNote->getNote(),
            std::move(lcache)
        });
    }
}


void
Beam::cacheFrame() const
{
#ifdef KOMP_BUILD_APPLE
    mCache.frame = Rect{0.0, 0.0, 0.0, 0.0};
    mCache.measureDataX = 0.0;
    mCache.mainBeamStartY = 0.0;
    
    const auto& loc = getLocation();
    const auto& measure = loc.getMeasure();
    const auto& staff = loc.getStaff();
    
    auto path = ::CGPathCreateMutable();
    const auto measureDataX = measure.getDataX();
    for (const auto& lineCache : mCache.beamLines) {
        
        const auto scoreX1 = measureDataX + lineCache.measureRelativeX1;
        const auto scoreX2 = measureDataX + lineCache.measureRelativeX2;
        
        const auto scoreY1 = staff.slotToPixel(lineCache.slot1);
        const auto scoreY2 = staff.slotToPixel(lineCache.slot2);
        
        const Rect frame{std::min(scoreY1, scoreY2),
            std::min(scoreX1, scoreX2),
            std::max(scoreY1, scoreY2),
            std::max(scoreX1, scoreX2)};
        
        mCache.frame.initializeOrUnion(frame);
        ::CGPathAddRect(path, NULL, frame);
    }
    
    for (const auto& stemCache : mCache.stems) {
        
        const auto& lineCache = stemCache.stemLine;
        
        const auto scoreX1 = measureDataX + lineCache.measureRelativeX1;
        const auto scoreX2 = measureDataX + lineCache.measureRelativeX2;
        
        const auto scoreY1 = staff.slotToPixel(lineCache.slot1);
        const auto scoreY2 = staff.slotToPixel(lineCache.slot2);
        
        const Rect frame{std::min(scoreY1, scoreY2),
            std::min(scoreX1, scoreX2),
            std::max(scoreY1, scoreY2),
            std::max(scoreX1, scoreX2)};
        
        mCache.frame.initializeOrUnion(frame);
        ::CGPathAddRect(path, NULL, frame);
    }
    
    mCache.measureDataX = measureDataX;
    
    if (mCache.getMainBeam()) {
        const auto& beam = *mCache.getMainBeam();
        if (beam.measureRelativeX1 < beam.measureRelativeX2) {
            mCache.mainBeamStartY = staff.slotToPixel(beam.slot1);
        } else {
            mCache.mainBeamStartY = staff.slotToPixel(beam.slot2);
        }
    }
    
    mCache.region = path;
#else
    
    mCache.frame = Rect{0.0, 0.0, 0.0, 0.0};
    mCache.measureDataX = 0.0;
    mCache.mainBeamStartY = 0.0;
    
    const auto& loc = getLocation();
    const auto& measure = loc.getMeasure();
    const auto& staff = loc.getStaff();

    QPainterPath path;
    
    const auto measureDataX = measure.getDataX();
    for (const auto& lineCache : mCache.beamLines) {
        
        const auto scoreX1 = measureDataX + lineCache.measureRelativeX1;
        const auto scoreX2 = measureDataX + lineCache.measureRelativeX2;
        
        const auto scoreY1 = staff.slotToPixel(lineCache.slot1);
        const auto scoreY2 = staff.slotToPixel(lineCache.slot2);
        
        const Rect frame{std::min(scoreY1, scoreY2),
            std::min(scoreX1, scoreX2),
            std::max(scoreY1, scoreY2),
            std::max(scoreX1, scoreX2)};
        
        mCache.frame.initializeOrUnion(frame);
        path.addRect(frame);
    }
    
    for (const auto& stemCache : mCache.stems) {
        
        const auto& lineCache = stemCache.stemLine;
        
        const auto scoreX1 = measureDataX + lineCache.measureRelativeX1;
        const auto scoreX2 = measureDataX + lineCache.measureRelativeX2;
        
        const auto scoreY1 = staff.slotToPixel(lineCache.slot1);
        const auto scoreY2 = staff.slotToPixel(lineCache.slot2);
        
        const Rect frame{std::min(scoreY1, scoreY2),
            std::min(scoreX1, scoreX2),
            std::max(scoreY1, scoreY2),
            std::max(scoreX1, scoreX2)};
        
        mCache.frame.initializeOrUnion(frame);
        path.addRect(frame);
    }
    
    mCache.measureDataX = measureDataX;
    
    if (mCache.getMainBeam()) {
        const auto& beam = *mCache.getMainBeam();
        if (beam.measureRelativeX1 < beam.measureRelativeX2) {
            mCache.mainBeamStartY = staff.slotToPixel(beam.slot1);
        } else {
            mCache.mainBeamStartY = staff.slotToPixel(beam.slot2);
        }
    }
    mCache.region = path;
#endif
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
