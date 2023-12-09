// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Rest.h"

// System
#include <iomanip>
#include <sstream>
#include <string>

// Local
#include "BeamIntf.h"
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "ItemImpl.h"
#include "Measure.h"
#include "Note.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"
#include "TemporalImpl.h"
#include "TupletIntf.h"

namespace komp
{
Rest::Rest(const MusicLocation& inMusicLocation,
           TemporalType inTemporalType)
: mAttachments{*this}
, mBeam{nullptr}
, mIsRaw{false}
, mIsRegistered{false}
, mChord{nullptr}
, mDuration{inTemporalType}
, mPitch{}
, mSpaceTimePoint{*this}
, mObservers{}
, mStemPolicy{}
, mTuplets{}
, mObserverSpinCount{0}
, mIsQuiescent{true}
, mLocation(inMusicLocation)
{
    setSlot(slotForType());
    setTime(inMusicLocation.getTime());
    TemporalImpl::initializeStemPolicy(*this, mStemPolicy);
}


const ItemTraits&
Rest::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Rest);
}


ItemUPtr
Rest::getMemento() const
{
    auto result = std::make_unique<Rest>(*this);
    
    const auto& measure = getLocation().getMeasure();
    auto& score = measure.getScore();
    auto& selection = score.getSelection();
    if (selection->contains(*this)) {
        selection->removeItem(*const_cast<Rest*>(this));
    }
    return result;
}


void
Rest::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Rest* downcastedP = dynamic_cast<const Rest*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    update();
    setIsQuiescent(wasQuiescent);
}


void
Rest::update()
{
    if (!mIsRegistered) {
        return;
    }
    
    notifyObservers({ItemChangeType::Updated});
    
    mAttachments.setNeedsDisplay();
    ItemImpl::setNeedsDisplay(*this);
}

void
Rest::itemIDChanged()
{
    mSpaceTimePoint.setItem(*this);
    mIsRegistered = true;
}


void
Rest::itemIDInvalidated()
{
    mIsRegistered = false;
}


void
Rest::registerObserver(ItemObserver& inObserver)
{
    ItemImpl::registerObserver(mObservers, inObserver);
}


void
Rest::unregisterObserver(ItemObserver& inObserver)
{
    ItemImpl::unregisterObserver(mObservers, inObserver);
}


void
Rest::notifyObservers(const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        mDeferredChanges.insert(inChanges.begin(), inChanges.end());
        return;
    }

    ItemImpl::notifyObservers(*this,
                              mObservers,
                              std::move(inChanges),
                              mObserverSpinCount);
}


void
Rest::observedItemChanged(const Item& observedItem,
                          const ItemChanges& inChanges)
{
    if (getIsQuiescent()) {
        return;
    }
}


bool
Rest::getIsQuiescent() const
{
    return mIsQuiescent;
}


void
Rest::setIsQuiescent(bool inValue)
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

Region
Rest::getRegion() const
{
#ifdef KOMP_BUILD_APPLE
    CGMutablePathRef path = CGPathCreateMutable();
    const auto drawPoint = getDrawPoint();
    
    const auto drawable = ItemImpl::getDrawable<DrawableRestParts>(*this);
    const auto noteheadFrame = drawable->getNoteheadFrame(drawPoint);
    const auto drawableNoteDotsFrame = drawable->getNoteDotsFrame(drawPoint);
    
    if (!noteheadFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, noteheadFrame);
    }
    
    if (!drawableNoteDotsFrame.getIsEmpty()) {
        CGPathAddRect(path, NULL, drawableNoteDotsFrame);
    }
    
    // Make sure to close the path to hit testing works
    CGPathCloseSubpath(path);
    
    return path;
#elif KOMP_BUILD_LINUX
    const auto drawPoint = getDrawPoint();
    
    const auto drawable = ItemImpl::getDrawable<DrawableRestParts>(*this);
    const auto noteheadFrame = drawable->getNoteheadFrame(drawPoint);
    const auto drawableNoteDotsFrame = drawable->getNoteDotsFrame(drawPoint);

    QPainterPath path;
    if (!noteheadFrame.getIsEmpty()) {
        path.addRect(noteheadFrame);
    }
    
    if (!drawableNoteDotsFrame.getIsEmpty()) {
        path.addRect(drawableNoteDotsFrame);
    }
        
    return Region{path};
#endif
}

Point
Rest::getAnchor() const
{
    const auto headFrame = getFrames().head;
    const auto center = headFrame.getCenter();
    return center;
}

bool
Rest::contains(const Point& inPoint, bool inPrecise) const
{
    return ItemImpl::contains(*this, inPoint, inPrecise);
}


Rect
Rest::getFrame() const
{
    const auto frames = getFrames();
    const auto result = frames.getCompleteFrame();
    return result;
}

void
Rest::createDrawable()
{
    ItemImpl::createDrawable<DrawableRestParts>(*this);
}

void
Rest::inserted()
{
    update();
    notifyObservers({ItemChangeType::Inserted});
}


void
Rest::removed()
{
    notifyObservers({ItemChangeType::Removed});
}


void
Rest::removeSelf()
{
    ItemImpl::removeSelf(*this);
}


const MusicLocation&
Rest::getLocation() const
{
    return mLocation;
}


double
Rest::getX() const
{
    return mSpaceTimePoint.getMeasureRelativeX();
}


void
Rest::setX(double inValue)
{
    if (inValue == getX()) {
        return;
    }
    
    ItemImpl::setX(*this, inValue, mSpaceTimePoint);
    update();
}


double
Rest::getScoreX() const
{
    return mSpaceTimePoint.getScoreX();
}


int
Rest::getSlot() const
{
    return ItemImpl::getSlot(mSpaceTimePoint);
}


void
Rest::setSlot(int inValue)
{
    if (inValue == getSlot()) {
        return;
    }
    
    ItemImpl::setSlot(*this, inValue, mSpaceTimePoint);
    update();
}


double
Rest::getScoreY() const
{
    return mSpaceTimePoint.getScoreY();
}


Point
Rest::getDrawPoint() const
{
    return ItemImpl::getDrawPoint(*this, mSpaceTimePoint);
}


double
Rest::getTime() const
{
    return mSpaceTimePoint.getTime();
}


int
Rest::getRoundedTime() const
{
    return ItemImpl::getRoundedTime(*this);
}


void
Rest::setTime(double inValue)
{
    if (inValue == getTime()) {
        return;
    }
    
    ItemImpl::setTime(*this, mSpaceTimePoint, inValue);
    mLocation.setTime(inValue);
    update();
}


double
Rest::getSpaceSize() const
{
    return ItemImpl::getSpaceSize(*this);
}


double
Rest::getFontSize() const
{
    return ItemImpl::getFontSize(*this);
}


bool
Rest::getIsNote() const
{
    return false;
}


bool
Rest::getIsRest() const
{
    return true;
}


bool
Rest::getIsChord() const
{
    return false;
}


bool
Rest::getIsChordMember() const
{
    return mChord;
}


Chord*
Rest::getChord() const
{
    return mChord;
}


void
Rest::setChord(Chord* inChord)
{
    if (inChord == mChord) {
        return;
    }
    mChord = inChord;
    update();
    notifyObservers({ItemChangeType::Chord});
}

void
Rest::clearChord()
{
    if (mChord) {
        mChord = nullptr;
        update();
        notifyObservers({ItemChangeType::Chord});
    }
}

bool
Rest::getIsLayoutItem() const
{
    return true;
}


Pitch
Rest::getPitch() const
{
    return mPitch;
}


unsigned short
Rest::getGlyph() const
{
    const auto& font = FontTheme::instance();
    const auto temporalType = getDuration().getTemporalType();
    const auto uchar = font.RestMap.at(temporalType);
    return uchar;
}


Duration
Rest::getDuration() const
{
    return mDuration;
}


void
Rest::setDuration(Duration inDuration)
{
    if (inDuration.equals(getDuration())) {
        return;
    }

    TemporalImpl::setDuration(*this, inDuration, mDuration);
    update();
}


bool
Rest::getIsRaw() const
{
    return mIsRaw;
}


void
Rest::setIsRaw(bool inValue)
{
    if (inValue == mIsRaw) {
        return;
    }
    
    mIsRaw = inValue;
    
    if (!inValue) {
        setStemPolicy(StemPolicy{StemDirection::UP, Policy::AUTOMATIC});
    } else {
        setStemPolicy(StemPolicy{StemDirection::NONE, Policy::AUTOMATIC});
    }
    
    if (getIsChordMember()) {
        const auto& chordTemporal = getChord();
        chordTemporal->setIsRaw(inValue);
    }
}


StemPolicy
Rest::getStemPolicy() const
{
    const auto& beam = getBeam();
    
    if (beam) {
        return beam->getStemPolicy();
    }
    
    const auto& chord = getChord();
    
    if (chord) {
        return chord->getStemPolicy();
    }
    
    return mStemPolicy;
}


void
Rest::setStemPolicy(StemPolicy inPolicy)
{
    if (inPolicy == getStemPolicy()) {
        return;
    }
    
    const bool wasOldPolicyAutomatic = getStemPolicy().policy == Policy::AUTOMATIC;
    const bool isNewPolicyAutomatic = inPolicy.policy == Policy::AUTOMATIC;
    
    TemporalImpl::setStemPolicy(*this, inPolicy, mStemPolicy);
    if (!wasOldPolicyAutomatic && isNewPolicyAutomatic) {
        //            updateStemDirection();
    }
    update();
    
    notifyObservers({ItemChangeType::StemDirection});
}

StemInfo
Rest::getStemInfo() const
{
    return StemInfo{};
}


PlaybackNote
Rest::getPlaybackNote() const
{
    return TemporalImpl::getPlaybackNote(*this);
}


Attachments&
Rest::getAttachments()
{
    return mAttachments;
}


const Attachments&
Rest::getAttachments() const
{
    return mAttachments;
}


bool
Rest::getIsBeamed() const
{
    return getBeam() != nullptr;
}


BeamIntf*
Rest::getBeam() const
{
    return mBeam;
}

void
Rest::clearBeam()
{
    mBeam = nullptr;
    update();
    notifyObservers({ItemChangeType::Beam});
}

void
Rest::setBeam(BeamIntf& inBeam)
{
    if (&inBeam != mBeam) {
        mBeam = &inBeam;
        update();
        notifyObservers({ItemChangeType::Beam});
    }
}


bool
Rest::getIsTupleted() const
{
    return TemporalImpl::getIsTupleted(mTuplets);
}


TupletIntfRefVec
Rest::getTuplets() const
{
    return TemporalImpl::getTuplets(mTuplets);
}


bool
Rest::addTuplet(TupletIntf& inTuplet)
{
    return TemporalImpl::addTuplet(*this, mTuplets, inTuplet);
}


bool
Rest::removeTuplet(TupletIntf& inTuplet)
{
    return TemporalImpl::removeTuplet(*this, mTuplets, inTuplet);
}


NoteFrames
Rest::getFrames() const
{
    NoteFrames frames{};
    const auto drawPoint = getDrawPoint();
    const auto drawable = ItemImpl::getDrawable<DrawableRestParts>(*this);
    frames.head = drawable->getNoteheadFrame(drawPoint);
    frames.dots = drawable->getNoteDotsFrame(drawPoint);
    return frames;
}


bool
Rest::getIsTupletStart() const
{
    const auto& tuplets = getTuplets();
    if (tuplets.size() == 0) {
        return false;
    }
    
    const TupletIntf& tuplet = tuplets.at(0);
    
    // if I'm not in a chord and I'm the first temporal in a tuplet, then true
    const auto firstTupletTemporal = tuplet.getTemporals().front();
    if (&firstTupletTemporal.get() == this) {
        return true;
    }
    
    return false;
}


bool
Rest::getIsTupletStop() const
{
    const auto& tuplets = getTuplets();
    if (tuplets.size() == 0) {
        return false;
    }
    
    const TupletIntf& tuplet = tuplets.at(0);
    
    // if I'm not in a chord and I'm the last temporal in a tuplet, then true
    const auto lastTupletTemporal = tuplet.getTemporals().back();
    if (&lastTupletTemporal.get() == this) {
        return true;
    }
    
    return false;
}

bool
Rest::getIsGrace() const
{
    return false;
}

//    void
//    Rest::drawSlash(const DrawableContext& inContext, Staff* inStaff, double inX)
//    {
//        const auto startY = inStaff->slotToPixel(4) - 1;
//        const auto endY = startY - Staff::getSpacing() * 2;
//        const auto endX = inX + Staff::getSpacing() * 2;
//
//        inContext->drawThickLine( inX, startY, endX, endY, 3);
//    }

//    void
//    Rest::drawCompressed(const DrawableContext& inContext, Staff* inStaff)
//    {
//        // Figure how far in the rest begins, using the Engraver::instance().spacing().RestWidth
//        // percentage as a guide. Thick line straddles the middle staff line.
//        const auto restWidth = (Measure::DEFAULT_MEASURE_WIDTH / 2) - Engraver::instance().spacing().RestWidth;
//        const auto offset = getMeasure()->getNoteSpace() * (restWidth / Measure::DEFAULT_MEASURE_WIDTH);
//        Rect frame;
//        frame.left = getMeasure()->getDataX() + offset;
//        frame.right = getMeasure()->getDataX() + getMeasure()->getNoteSpace() - offset;
//
//        // NOTE: The +3 rounds the offset value (+2) with an extra
//        // padding (+1) to help sizes 2 and 4 on the screen.
//        frame.top = inStaff->slotToPixel(6) - (Staff::getSpacing() + 3) / 4;
//        frame.bottom = frame.top + (Staff::getSpacing() + 1) / 2;
//
//        inContext->setFillColor(0.0, 0.0, 0.0, 1.0);
//        inContext->fillRect(frame);
//
//        // Two shorter lines at end extend from second line
//        // to the fourth line.
//        frame.top = inStaff->slotToPixel(8);
//        frame.bottom = inStaff->slotToPixel(4);
//        inContext->drawLine(frame.left, frame.top, frame.left, frame.bottom - 1);
//        inContext->drawLine(frame.right, frame.top, frame.right, frame.bottom - 1);
//
//        // Center the number 1.5 spaces above the staff
//        frame.left = getMeasure()->getDataX() + getMeasure()->getNoteSpace() / 2;
//        frame.top -= Staff::getSpacing() * 3 / 2;
//        const auto& font = Preferences::instance().CompressedRestFont;
//        const std::string count = std::to_string(mCompressedCount);
//        const auto textSize = MeasureText(font.Font, font.Size, count.c_str());
//
//        // Set the text color
//        const auto& color = ScoreColors::instance().textColor();
//
//        inContext->drawText(font.Font,
//                            font.Size,
//                            frame.left - textSize.width / 2,
//                            frame.top - textSize.height,
//                            count.c_str(),
//                            color);
//    }

void
Rest::drag(const CurrentState& inState)
{
    // Overriden to disable dragging
}

// MARK: - Positioning

int
Rest::slotForType() const
{
    switch (getDuration().getTemporalType())
    {
        case TemporalType::WHOLE:
            return 8;
            
        case TemporalType::BREVE:
        case TemporalType::HALF:
        case TemporalType::QUARTER:
        case TemporalType::EIGHTH:
        case TemporalType::SIXTEENTH:
        case TemporalType::THIRTY_SECOND:
        case TemporalType::SIXTY_FOURTH:
        case TemporalType::ONE_TWENTY_EIGHTH:
        case TemporalType::TWO_FIFTY_SIXTH:
        case TemporalType::FIVE_TWELFTH:
        case TemporalType::TEN_TWENTY_FOURTH:
            return 6;
            
        default:
        {
            LOG_F(ERROR, "Unhandled temporal type");
            return 6;
        }
    }
}

int
Rest::slotForVoice(int inTotalVoices) const
{
    int slot;
    
    switch (getDuration().getTemporalType())
    {
        case TemporalType::WHOLE:
            slot = 8;
            break;
            
        case TemporalType::BREVE:
        case TemporalType::HALF:
        case TemporalType::QUARTER:
        case TemporalType::EIGHTH:
        case TemporalType::SIXTEENTH:
        case TemporalType::THIRTY_SECOND:
        case TemporalType::SIXTY_FOURTH:
        case TemporalType::ONE_TWENTY_EIGHTH:
        case TemporalType::TWO_FIFTY_SIXTH:
        case TemporalType::FIVE_TWELFTH:
        case TemporalType::TEN_TWENTY_FOURTH:
            slot = 6;
            break;
            
        default:
        {
            LOG_F(ERROR, "Unhandled temporal type");
            slot = 6;
            break;
        }
    }
    
    // Apply voice offsets based on voice and total voices in score
    if (inTotalVoices > 1) {
        const auto voice = getLocation().getVoice();
        if (voice == 1 || voice == 3) {
            slot -= 4;
        } else {
            slot += 4;
        }
    }
    
    return slot;
}

} // namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
