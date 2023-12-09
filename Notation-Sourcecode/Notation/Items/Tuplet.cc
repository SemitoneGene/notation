// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Tuplet.h"

// Local
#include "BeamIntf.h"
#include "Drawable.h"
#include "DrawableContext.h"
#include "DrawableLine.h"
#include "DrawableText.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Rest.h"
#include "Temporal.h"

namespace komp
{
static constexpr const double ABOVE_BEAMED_CLEARANCE = 2.0;
static constexpr const double BELOW_CLEARANCE = 8.0;
static constexpr const double BROKEN_CLEARANCE = 2.5;

Tuplet::Tuplet(const MusicLocation& inLocation,
               const TupletParams& inTupletParams)
: mTemporals{inTupletParams.getTemporals()}
, mTupletRatio{std::move(inTupletParams.tupletRatio)}
, mLocation(inLocation)
{
    sort(mTemporals);
    validate();
}


Tuplet::~Tuplet()
{
    //unsetTemporals();
}

const ItemTraits&
Tuplet::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Tuplet);
}


ItemUPtr
Tuplet::getMemento() const
{
    return std::make_unique<Tuplet>(*this);
}


void
Tuplet::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Tuplet* downcastedP = dynamic_cast<const Tuplet*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    
    update();
    sort(mTemporals);
    setIsQuiescent(wasQuiescent);
}


void
Tuplet::update()
{
    if (getIsQuiescent()) {
        return;
    }
    updateTuplet();
    notifyObservers({ItemChangeType::Updated});
}


void
Tuplet::itemIDChanged()
{
}


void
Tuplet::itemIDInvalidated()
{
}

void
Tuplet::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
Tuplet::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
Tuplet::observedItemChanged(const Item& observedItem,
                            const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        return;
    }
    
    sort(mTemporals);
    validate();
    
    auto doUpdate = false;
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Duration);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Pitch);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Slot);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::StemDirection);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::TemporalType);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::Updated);
    doUpdate |= ItemObserver::contains(inChanges, ItemChangeType::X);

    if (doUpdate) {
        updateTuplet();
    }
}


bool
Tuplet::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
Tuplet::setIsQuiescent(bool inValue)
{
    mIsQuiescent = inValue;
}


Rect
Tuplet::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    const auto frame = drawable->getFrame(getDrawPoint());
    return frame;
}


Region
Tuplet::getRegion() const
{
    return ItemImpl::getRegion<DrawableComposite>(*this);
}


Point
Tuplet::getAnchor() const
{
    // TODO: implement
    return Point{};
}


bool
Tuplet::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}


void
Tuplet::inserted()
{
    sort(mTemporals);
    setTemporals();
    updateTuplet();
    notifyObservers({ItemChangeType::Inserted});
}


void
Tuplet::removed()
{
    unsetTemporals();
    notifyObservers({ItemChangeType::Removed});
}


void
Tuplet::removeSelf()
{
    // TODO: implement
}


const MusicLocation&
Tuplet::getLocation() const
{
    return mLocation;
}


double
Tuplet::getX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto measureDataX = measure.getDataX();
    const auto scoreX = getScoreX();
    const auto result = scoreX - measureDataX;
    return result;
}


void
Tuplet::setX(double inValue)
{
    sort(mTemporals);
    // TODO: currently this cannot be changed, it is provided by the first temporal
}


double
Tuplet::getScoreX() const
{
    if (mTemporals.size() == 0) {
        return 0;
    }
    
    const auto& first = getFirst();
    
    if (!first) {
        return 0;
    }
    
    return first->getDrawPoint().x;
}


int
Tuplet::getSlot() const
{
    if (mTemporals.size() == 0) {
        return 6;
    }
    
    const auto& first = getFirst();
    
    if (!first) {
        return 6;
    }
    
    return first->getSlot();
}


void
Tuplet::setSlot(int inValue)
{
    sort(mTemporals);
    // TODO: currently this cannot be changed, it is provided by the first temporal
}


double
Tuplet::getScoreY() const
{
    if (mTemporals.size() == 0) {
        return 0;
    }
    
    const auto& first = getFirst();
    
    if (!first) {
        return 0;
    }
    
    return first->getDrawPoint().y;
}


Point
Tuplet::getDrawPoint() const
{
    if (mTemporals.size() == 0) {
        return Point{};
    }
    
    const auto& first = getFirst();
    
    if (!first) {
        return Point{};
    }
    
    return first->getDrawPoint();
}


double
Tuplet::getTime() const
{
    KOMP_ASSERT(mTemporals.size() > 0);
    return mTemporals.front().get().getTime();
}


int
Tuplet::getRoundedTime() const
{
    KOMP_ASSERT(mTemporals.size() > 0);
    return mTemporals.front().get().getRoundedTime();
}


void
Tuplet::setTime(double inValue)
{
    sort(mTemporals);
    const auto currentStartTime = getTime();
    
    if (inValue == currentStartTime) {
        return;
    }
    
    const auto timeDelta = inValue - currentStartTime;
    
    for (Temporal& temporal : mTemporals) {
        const auto temporalTime = temporal.getTime();
        const auto newTime = temporalTime + timeDelta;
        temporal.setTime(newTime);
    }
    notifyObservers({ItemChangeType::Time});
}


double
Tuplet::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
Tuplet::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}


const TemporalRefVec&
Tuplet::getTemporals() const
{
    return mTemporals;
}


bool
Tuplet::isMember(const Temporal& inTemporal) const
{
    const auto iter = std::find_if(mTemporals.cbegin(),
                                   mTemporals.cend(),
                                   [&](const Temporal& filterTemporal) {
        return &inTemporal == &filterTemporal;
    });
    return iter != mTemporals.cend();
}


Temporal*
Tuplet::getFirst() const
{
    if (mTemporals.size() == 0) {
        return nullptr;
    } else if (mTemporals.size() == 1) {
        return &mTemporals.front().get();
    }
    
    auto copy = mTemporals;
    sort(copy);
    Temporal& first = copy.front();
    const Temporal& last = copy.back();
    KOMP_ASSERT(first.getTime() < last.getTime());
    return &first;
}


Temporal*
Tuplet::getLast() const
{
    if (mTemporals.size() == 0) {
        return nullptr;
    } else if (mTemporals.size() == 1) {
        return &mTemporals.back().get();
    }
    
    auto copy = mTemporals;
    sort(copy);
    Temporal& first = copy.front();
    Temporal& last = copy.back();
    KOMP_ASSERT(first.getTime() < last.getTime());
    return &last;
}


const TupletRatio&
Tuplet::getTupletRatio() const
{
    return mTupletRatio;
}


void
Tuplet::updateTuplet()
{
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    drawable->clearDrawables();
    
    if (mTemporals.size() == 0) {
        return;
    }
    
    sort(mTemporals);
    const auto avoidanceFrame = analyzeAndSetAttributes();
    createDrawableObjects(avoidanceFrame);
}


void
Tuplet::unsetTemporals()
{
    if (!mAreTemporalsSet) {
        return;
    }
    
    for (Temporal& temporal: mTemporals) {
        temporal.unregisterObserver(*this);
        
        const auto& beam = temporal.getBeam();
        if (beam) {
            beam->unregisterObserver(*this);
        }
        
        temporal.removeTuplet(*this);
    }
    
    mAreTemporalsSet = false;
}


void
Tuplet::notifyObservers(const ItemChanges& inChanges)
{
    ItemImpl::notifyObservers(*this, mObservers, std::move(inChanges));
}


void
Tuplet::validate() const
{
    KOMP_ASSERT(mTemporals.size() > 0);
    double durationCounter = 0.0;
    
    for (Temporal& temporal : mTemporals) {
        
        const auto& itemTraits = temporal.getItemTraits();
        const auto itemType = itemTraits.getItemType();
        
        if (itemType == ItemType::Note) {
            const auto& note = temporal.as<Note>();
            if (note.getIsChordMember()) {
                KOMP_THROW("A chorded note cannot not be added to a tuplet. The owning chord object should be added");
            }
        }
        
        auto unalteredDuration = temporal.getDuration();
        unalteredDuration.clearTupletRatios();
        durationCounter += unalteredDuration.getClocks();
    }
    
    const auto totalDuration = static_cast<int>(std::ceil(durationCounter - 0.5));
    const auto clocks = mTupletRatio.getActual().getRoundedClocks();
    KOMP_ASSERT(totalDuration == clocks);
}


void
Tuplet::sort(TemporalRefVec& outTemporals) const
{
    std::ranges::sort(outTemporals,
                      [&](const Temporal& lhs,
                          const Temporal& rhs) {
                
        const auto lmeasure = lhs.getLocation().getMeasure().getIndex();
        const auto rmeasure = rhs.getLocation().getMeasure().getIndex();
        
        if (lmeasure < rmeasure) {
            return true;
        }
        
        if (lmeasure > rmeasure) {
            return false;
        }
        
        if (lhs.getTime() < rhs.getTime()) {
            return true;
        }
        
        if (lhs.getTime() > rhs.getTime()) {
            return false;
        }
        
        if (lhs.getX() < rhs.getX()) {
            return true;
        }
        
        if (lhs.getX() > rhs.getX()) {
            return false;
        }
        
        if (lhs.getSlot() < rhs.getSlot()) {
            return true;
        }
        
        if (lhs.getSlot() > rhs.getSlot()) {
            return false;
        }
        
        return &lhs < &rhs;
    });
}


void
Tuplet::setTemporals()
{
    sort(mTemporals);
    if (mAreTemporalsSet) {
        return;
    }
    
    for (Temporal& temporal: mTemporals) {
        temporal.addTuplet(as<TupletIntf>());
    }
    
    for (Temporal& temporal : mTemporals) {
        temporal.registerObserver(*this);
        const auto& beam = temporal.getBeam();
        if (beam) {
            beam->registerObserver(*this);
        }
    }
    
    mAreTemporalsSet = true;
}


Rect
Tuplet::analyzeAndSetAttributes()
{
    // find out if all the notes in the tuplet are in the same beam
    const auto firstBeam = mTemporals.front().get().getBeam();
    const auto lastBeam = mTemporals.back().get().getBeam();
    mIsBeamedTuplet = firstBeam != nullptr && firstBeam == lastBeam;
    
    double stemAvg = 0.0;
    Rect combinedNotesFrame;
    
    // analyze the temporal frames and stem directions
    for (const Temporal& temporal : mTemporals) {
        
        const auto& itemTraits = temporal.getItemTraits();
        const auto itemType = itemTraits.getItemType();

        if (itemType != ItemType::Rest) {
            stemAvg += temporal.getStemPolicy().getIsStemUp() ? 1.0 : -1.0;
        }
        
        const auto temporalFrame = temporal.getFrame();
        combinedNotesFrame.initializeOrUnion(temporalFrame);
        
        const auto& beam = temporal.getBeam();
        if (beam) {
            const auto beamFrame = beam->getFrame();
            combinedNotesFrame.initializeOrUnion(beamFrame);
        }
    }
    
    bool isStemUp = stemAvg >= 0.0;
    const Rect avoidanceFrame = combinedNotesFrame;
    
    if (mHasBracketPolicy == Policy::AUTOMATIC) {
        mHasBracket = !mIsBeamedTuplet;
    }
    
    if (mAboveBelowPolicy == Policy::AUTOMATIC) {
        
        if (mHasBracket) {
            mAboveBelow = AboveBelow::ABOVE;
        } else {
            mAboveBelow = isStemUp ? AboveBelow::ABOVE : AboveBelow::BELOW;
        }
    }
    
    if (mHookDirectionPolicy == Policy::AUTOMATIC) {
        mHookDirection = mAboveBelow == AboveBelow::ABOVE ?
        UpDown::DOWN : UpDown::UP;
    }
    
    if (mHookLengthPolicy == Policy::AUTOMATIC) {
        mHookLength = DEFAULT_HOOK_LENGTH_SPACES;
    }
    
    if (mThicknessPolicy == Policy::AUTOMATIC) {
        mThickness = DEFAULT_THICKNESS_SPACES;
    }
    
    mYBoundary = 0.0;
    
    if (mIsBeamedTuplet) {
        auto beam = mTemporals.front().get().getBeam();
        KOMP_ASSERT(beam);
        auto beamFrame = beam->getFrame();
        const auto middleX = beamFrame.getCenter().x;
        const auto yIntercept = beam->getYIntercept(middleX);
        if (mAboveBelow == AboveBelow::ABOVE) {
            mYBoundary = yIntercept;
        } else {
            mYBoundary = yIntercept + beam->getBeamLineThickness(0);
        }
    } else {
        if (mAboveBelow == AboveBelow::ABOVE) {
            mYBoundary = avoidanceFrame.top;
        } else {
            mYBoundary = avoidanceFrame.bottom;
        }
    }
    
    return avoidanceFrame;
}

void
Tuplet::createDrawable()
{
    ItemImpl::createDrawable<DrawableComposite>(*this);
}

void
Tuplet::createDrawableObjects(const Rect& inAvoidanceFrame)
{
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    drawable->clearDrawables();
    createTupletNumber(inAvoidanceFrame);
    createTupletBracket(inAvoidanceFrame);
    moveOutsideStaff();
}


void
Tuplet::createTupletNumber(const Rect& inAvoidanceFrame)
{
    if (mNumberType == TupletNumberType::NONE) {
        return;
    }
    
    const auto& fontInstance = FontTheme::instance();
    auto chars = fontInstance.getTupletNumber(getTupletRatio().getActual().getCount());
    auto text = std::make_unique<DrawableText>();
    
    if (!getTupletRatio().getIsNormalRatio()) {
        chars.emplace_back(fontInstance.getTupletColon());
        auto normalChars = fontInstance.getTupletNumber(getTupletRatio().getNormal().getCount());
        for (const auto& nc : normalChars) {
            chars.push_back(nc);
        }
    }
    
    text->setText(chars);
    text->setFontSize(Staff::getFontSize());
    const auto textWidth = text->getWidth();
    const auto textHalfWidth = textWidth / 2.0;
    
    double middleX = 0.0;
    const auto& first = getFirst();
    if (mIsBeamedTuplet) {
        const auto& beam = first->getBeam();
        KOMP_ASSERT(beam);
        const auto& beamFrame = beam->getFrame();
        middleX = beamFrame.getCenter().x;
    } else {
        middleX = inAvoidanceFrame.getCenter().x;
    }
    double slop = 0.0;
    
    if (!mIsBeamedTuplet) {
        slop = BROKEN_CLEARANCE;
    } else if (mAboveBelow == AboveBelow::ABOVE) {
        slop = ABOVE_BEAMED_CLEARANCE;
    }
    
    const auto drawX = middleX - textHalfWidth;
    
    auto drawY = 0.0;
    if (mAboveBelow == AboveBelow::ABOVE) {
        drawY = inAvoidanceFrame.top - slop;
    } else {
        drawY = inAvoidanceFrame.bottom + slop + BELOW_CLEARANCE;
    }
    
    Point offset{};
    const auto drawPoint = getDrawPoint();
    offset.x = drawX - drawPoint.x;
    offset.y = drawY - drawPoint.y;
    text->setOffset(offset);
    
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);
    drawable->addDrawable(std::move(text));
}


void
Tuplet::createTupletBracket(const Rect& inAvoidanceFrame)
{
    if (!mHasBracket) {
        return;
    }
    
    if (mTemporals.empty()) {
        return;
    }
    const auto drawable = ItemImpl::getDrawable<DrawableComposite>(*this);

    // TODO: support tuplets with a bracket but no number?
    KOMP_ASSERT(mNumberType != TupletNumberType::NONE);
    KOMP_ASSERT(drawable->getDrawableCount() == 1);
    auto& number = drawable->getDrawable(0);
    
    Point drawPoint;
    auto numberFrame = number.getFrame(drawPoint);
    
    const auto numberYCenter = numberFrame.getCenter().y;
    const auto spaceSize = Staff::getSpacing();
    const auto thickness = spaceSize * mThickness;
    const auto halfThickness = thickness / 2.0;
    const auto lineY = numberYCenter - halfThickness;
    const auto leftLength = numberFrame.left;
    const auto relativeRightX = inAvoidanceFrame.getWidth();
    const auto rightLength = relativeRightX - numberFrame.right;
    
    auto leftLine = std::make_unique<DrawableLine>();
    leftLine->setLength(leftLength);
    leftLine->setDegrees(0.0);
    leftLine->setThickness(thickness);
    Point leftLineOffset{0.0, lineY};
    leftLine->setOffset(leftLineOffset);
    drawable->addDrawable(std::move(leftLine));
    
    auto rightLine = std::make_unique<DrawableLine>();
    rightLine->setLength(rightLength);
    rightLine->setDegrees(0.0);
    rightLine->setThickness(thickness);
    Point rightLineOffset{numberFrame.right, lineY};
    rightLine->setOffset(rightLineOffset);
    drawable->addDrawable(std::move(rightLine));
    
    const double hookLength = spaceSize * DEFAULT_HOOK_LENGTH_SPACES;
    const double hookDegrees = mHookDirection == UpDown::UP ? 270.0 : 90.0;
    const double hookStartY = lineY + (mHookDirection == UpDown::UP ? halfThickness : -1.0 * halfThickness);
    
    auto leftHook = std::make_unique<DrawableLine>();
    leftHook->setLength(hookLength);
    leftHook->setDegrees(hookDegrees);
    leftHook->setThickness(thickness);
    Point leftHookOffset{0.0, hookStartY};
    leftHook->setOffset(leftHookOffset);
    drawable->addDrawable(std::move(leftHook));
    
    auto rightHook = std::make_unique<DrawableLine>();
    rightHook->setLength(hookLength);
    rightHook->setDegrees(hookDegrees);
    rightHook->setThickness(thickness);
    Point rightHookOffset{relativeRightX, hookStartY};
    rightHook->setOffset(rightHookOffset);
    drawable->addDrawable(std::move(rightHook));
}


void
Tuplet::moveOutsideStaff()
{
    const auto tupletFrame = getFrame();
    const auto& staff = getLocation().getStaff();
    const auto spaceSize = Staff::getSpacing();
    const auto halfSpace = spaceSize / 2.0;
    const auto staffTop = staff.getScoreY();
    const auto staffBottom = staff.getScoreBottom();
    const bool isAbove = tupletFrame.top < staffTop;
    
    double moveAmount = 0.0;
    bool needsToMove = false;
    
    if (isAbove) {
        needsToMove = tupletFrame.bottom >= staffTop;
        moveAmount = std::abs(staffTop - tupletFrame.bottom) + halfSpace;
    } else {
        needsToMove = tupletFrame.top <= staffBottom;
        moveAmount = std::abs(staffBottom - tupletFrame.top) + halfSpace;
    }
    
    if (!needsToMove) {
        return;
    }
    
    if (isAbove) {
        moveAmount *= -1.0;
    }
    
    const auto drawables = ItemImpl::getDrawable<DrawableComposite>(*this);
    for (int i = 0; i < drawables->getDrawableCount(); ++i) {
        auto& drawable = drawables->getDrawable(i);
        auto offset = drawable.getOffset();
        offset.y += moveAmount;
        drawable.setOffset(offset);
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
