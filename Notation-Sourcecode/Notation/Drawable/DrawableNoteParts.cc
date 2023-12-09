// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableNoteParts.h"

// System
#include <cmath>

// Local
#include "Chord.h"
#include "Engraver.h"
#include "Preferences.h"
#include "Throw.h"
#include "Temporal.h"
#include "TemporalValue.h"

namespace komp
{
DrawableNoteParts::DrawableNoteParts()
{
}

void
DrawableNoteParts::removeFromParent() const
{
    mNotehead.removeFromParent();
    mNoteDots.removeFromParent();
    mAccidental.removeFromParent();
    mStem.removeFromParent();
    mNoteFlags.removeFromParent();
    mLegerLines.removeFromParent();
    mArpeggiate.removeFromParent();
    mOutline.removeFromParent();
}

void
DrawableNoteParts::restoreParent() const
{
    mNotehead.restoreParent();
    mNoteDots.restoreParent();
    mAccidental.restoreParent();
    mStem.restoreParent();
    mNoteFlags.restoreParent();
    mLegerLines.restoreParent();
    mArpeggiate.restoreParent();
    mOutline.restoreParent();
}

Point
DrawableNoteParts::getOffset() const
{
    return mOffset;
}

void
DrawableNoteParts::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableNoteParts::setZOrder(int inOrder)
{
    mNotehead.setZOrder(inOrder);
    mNoteDots.setZOrder(inOrder);
    mAccidental.setZOrder(inOrder);
    mStem.setZOrder(inOrder);
    mNoteFlags.setZOrder(inOrder);
    mLegerLines.setZOrder(inOrder);
    mArpeggiate.setZOrder(inOrder);
    mOutline.setZOrder(inOrder);
}

void
DrawableNoteParts::setScale(double inScale)
{
    mNotehead.setScale(inScale);
    mNoteDots.setScale(inScale);
    mAccidental.setScale(inScale);
    mStem.setScale(inScale);
    mNoteFlags.setScale(inScale);
    mLegerLines.setScale(inScale);
    mArpeggiate.setScale(inScale);
    mOutline.setScale(inScale);
}

void
DrawableNoteParts::setRotation(double inDegrees)
{
}

Rect
DrawableNoteParts::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableNoteParts::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableNoteParts::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableNoteParts::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableNoteParts::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableNoteParts::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        
        DrawResult result;
        
        const auto headResult = getNoteheadMetrics(inPoint);
        const auto dotsResult = getNoteDotsMetrics(inPoint);
        const auto accidentalResult = getAccidentalMetrics(inPoint);
        const auto stemResult = getStemMetrics(inPoint);
        const auto flagsResult = getNoteFlagsMetrics(inPoint);
        const auto legerLinesResult = getLegerLinesMetrics(inPoint);
        const auto arpeggiateResult = getArpeggiateMetrics(inPoint);
        
        result.frame.initializeOrUnion(headResult.frame);
        result.frame.initializeOrUnion(dotsResult.frame);
        result.frame.initializeOrUnion(accidentalResult.frame);
        result.frame.initializeOrUnion(stemResult.frame);
        result.frame.initializeOrUnion(flagsResult.frame);
        result.frame.initializeOrUnion(legerLinesResult.frame);
        result.frame.initializeOrUnion(arpeggiateResult.frame);
        
#ifdef KOMP_BUILD_APPLE
        const auto region = result.region.getMutable();
        if (region) {
            ::CGPathAddRect(region, NULL, result.frame);
        }
#elif KOMP_BUILD_LINUX
        result.region.addRect(result.frame);
#endif
        result.anchor = inPoint;
        mCache.setCache(result, inPoint);
    }
    
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableNoteParts::draw(const DrawParams& inDrawParams) const
{
    const auto metrics = getMetrics(inDrawParams.drawPoint);
    
    drawNotehead(inDrawParams);
    drawNoteDots(inDrawParams);
    drawAccidental(inDrawParams);
    drawStem(inDrawParams);
    drawNoteFlags(inDrawParams);
    drawLegerLines(inDrawParams);
    drawArpeggiate(inDrawParams);
    drawOutline(metrics, inDrawParams);
}

// TODO: Refactor this into smaller 'updateThis' 'updateThat' functions
// so that it does't hurt my eyes.

void
DrawableNoteParts::update(const Item& inItem)
{
    const auto temporal = dynamic_cast<const Temporal*>(&inItem);
    if (temporal->getIsChord()) {
        mDrawNotehead = false;
        mNotehead.clearState();
    } else {
        mDrawNotehead = true;
        mNotehead.update(*temporal);
    }
    
    const auto duration = temporal->getDuration();
    const auto type = duration.getTemporalType();
    const bool doIHaveDotsNow = duration.getDots() > 0 || mDrawNoteDots;
    
    if (duration.getDots() > 0
        && !temporal->getIsRaw()
        && !temporal->getIsChord()) {
        mDrawNoteDots = true;
        mNoteDots.update(*temporal);
        
    } else if (doIHaveDotsNow) {
        mDrawNoteDots = false;
        mNoteDots.clearDots();
    }
    
    if (getIsAccidentalDrawable(temporal)) {
        if (areFloatsEqual(mAccidental.getFontSize(),
                           temporal->getFontSize())) {
            mAccidental.setFontSize(temporal->getFontSize());
        }
    } else {
        mAccidental.clearState();
    }
    
    const auto stemPolicy = temporal->getStemPolicy();
    const auto stemInfo = temporal->getStemInfo();
    const auto hasStem = stemPolicy.getHasStem();
    const auto hasFlags = TemporalValue::getHasFlags(type);
    const auto isRest = temporal->getIsRest();
    const auto isRaw = temporal->getIsRaw();
    const auto isNote = temporal->getIsNote();
    const auto isChord = temporal->getIsChord();
    
    if (hasStem && !isRest && !isRaw && stemInfo.isStemOwner) {
        mDrawStem = true;
        mStem.update(stemInfo, stemPolicy);
    } else {
        mDrawStem = false;
        mStem.clearState();
    }
    
    if (hasFlags && !isRest && !isRaw && stemInfo.isStemOwner) {
        mDrawNoteFlags = true;
        mNoteFlags.update(stemInfo, stemPolicy, mStem);
        
    } else {
        mDrawNoteFlags = false;
        mNoteFlags.clearState();
    }
    
    if (isNote) {
        mLegerLines.update(*temporal);
        mDrawLegerLines = mLegerLines.getLegerLineCount() > 0;
                                
        if (temporal->getIsGrace()) {
            auto& engraver = Engraver::instance();
            const auto scale = engraver.graceNoteScale();
            mNotehead.setScale(scale);
            mNoteDots.setScale(scale);
            mAccidental.setScale(scale);
            mStem.setScale(scale);
            mNoteFlags.setScale(scale);
            mLegerLines.setScale(scale);
            mOutline.setScale(scale);
        }
    }
    
    if (isChord) {
        const auto chord = const_cast<Temporal*>(temporal)->as<Chord>();
        mArpeggiate.update(chord);
    }
    
    if (!mDrawLegerLines) {
        mLegerLines.clearState();
    }
        
    mOutline.setBounds(getFrame(Point{}));
    
    KOMP_SET_DIRTY
}


void
DrawableNoteParts::clearState()
{
    KOMP_SET_DIRTY
    setOffset(Point{0.0, 0.0});
    mDrawNotehead = false;
    mNotehead.clearState();
    mDrawNoteDots = false;
    mNoteDots.clearDots();
    mAccidental.clearState();
    mDrawStem = false;
    mStem.clearState();
    mDrawNoteFlags = false;
    mNoteFlags.clearState();
    mArpeggiate.clearState();
}


komp::Point
DrawableNoteParts::getAccidentalOffset() const
{
    return mAccidental.getOffset();
}


void
DrawableNoteParts::setAccidentalOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mAccidental.setOffset(inOffset);
}

komp::Point
DrawableNoteParts::getArpeggiateOffset() const
{
    return mArpeggiate.getOffset();
}

void
DrawableNoteParts::setArpeggiateOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mArpeggiate.setOffset(inOffset);
}


void
DrawableNoteParts::setSpacesBeforeFirstDot(double inValue)
{
    KOMP_SET_DIRTY
    mNoteDots.setSpacesBeforeFirstDot(inValue);
}


void
DrawableNoteParts::setSpacesBetweenDots(double inValue)
{
    KOMP_SET_DIRTY
    mNoteDots.setSpacesBetweenDots(inValue);
}


const AccidentalType&
DrawableNoteParts::getAccidentalType() const
{
    return mAccidental.getAccidentalType();
}


void
DrawableNoteParts::setAccidentalType(AccidentalType inType)
{
    if (inType == mAccidental.getAccidentalType()) {
        return;
    }
    KOMP_SET_DIRTY
    mAccidental.setAccidentalType(inType);
}

const ArpeggiateType&
DrawableNoteParts::getArpeggiateType() const
{
    return mArpeggiate.getType();
}


void
DrawableNoteParts::setArpeggiateType(ArpeggiateType inType)
{
    if (inType == mArpeggiate.getType()) {
        return;
    }
    KOMP_SET_DIRTY
    mArpeggiate.setType(inType);
}


DrawResult
DrawableNoteParts::getNoteheadMetrics(const Point& inPoint) const
{
    if (!mDrawNotehead) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mNotehead.getMetrics(inPoint);
}


DrawResult
DrawableNoteParts::getNoteDotsMetrics(const Point& inPoint) const
{
    if (!mDrawNoteDots) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mNoteDots.getMetrics(inPoint);
}


DrawResult
DrawableNoteParts::getAccidentalMetrics(const Point& inPoint) const
{
    if (mAccidental.getAccidentalType() == AccidentalType::NONE) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mAccidental.getMetrics(inPoint);
}


DrawResult
DrawableNoteParts::getStemMetrics(const Point& inPoint) const
{
    if (!mDrawStem) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mStem.getMetrics(inPoint);
}


DrawResult
DrawableNoteParts::getNoteFlagsMetrics(const Point& inPoint) const
{
    if (!mDrawNoteFlags) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mNoteFlags.getMetrics(inPoint);
}


DrawResult
DrawableNoteParts::getLegerLinesMetrics(const Point& inPoint) const
{
    if (!mDrawLegerLines) {
        return DrawResult{};
    }
    
    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mLegerLines.getMetrics(inPoint);
}

DrawResult
DrawableNoteParts::getArpeggiateMetrics(const Point& inPoint) const
{
    if (mArpeggiate.getType() == ArpeggiateType::NONE) {
        return DrawResult{};
    }

    auto drawPoint = inPoint;
    drawPoint.moveBy(getOffset());
    return mArpeggiate.getMetrics(inPoint);
}

void
DrawableNoteParts::drawNotehead(const DrawParams& inDrawParams) const
{
    if (!mDrawNotehead) {
        return;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    mNotehead.draw(drawParams);
}


void
DrawableNoteParts::drawNoteDots(const DrawParams& inDrawParams) const
{
    if (!mDrawNoteDots) {
        return;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    mNoteDots.draw(inDrawParams);
}


void
DrawableNoteParts::drawAccidental(const DrawParams& inDrawParams) const
{
    if (mAccidental.getAccidentalType() == AccidentalType::NONE) {
        return;
    }
    
    auto drawParams = inDrawParams;
    const auto offset = getOffset();
    drawParams.drawPoint.moveBy(offset);
    return mAccidental.draw(drawParams);
}


void
DrawableNoteParts::drawStem(const DrawParams& inDrawParams) const
{
    if (!mDrawStem) {
        return;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    return mStem.draw(drawParams);
}


void
DrawableNoteParts::drawNoteFlags(const DrawParams& inDrawParams) const
{
    if (!mDrawNoteFlags) {
        return;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    return mNoteFlags.draw(drawParams);
}


void
DrawableNoteParts::drawLegerLines(const DrawParams& inDrawParams) const
{
    if (!mDrawLegerLines) {
        return;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    return mLegerLines.draw(drawParams);
}

void
DrawableNoteParts::drawArpeggiate(const DrawParams& inDrawParams) const
{
    if (mArpeggiate.getType() == ArpeggiateType::NONE) {
        return;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(getOffset());
    return mArpeggiate.draw(drawParams);
}

void
DrawableNoteParts::drawOutline(const DrawResult& inDrawResult,
                               const DrawParams& inDrawParams) const
{
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();
        auto bounds = inDrawResult.frame;
        bounds.moveTo(0, 0);
        mOutline.setBounds(bounds);
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}

Rect
DrawableNoteParts::getNoteheadFrame(const Point& inPoint) const
{
    return mNotehead.getFrame(inPoint);
}


Rect
DrawableNoteParts::getNoteDotsFrame(const Point& inPoint) const
{
    return mNoteDots.getFrame(inPoint);
}


Rect
DrawableNoteParts::getAccidentalFrame(const Point& inPoint) const
{
    return mAccidental.getFrame(inPoint);
}


Rect
DrawableNoteParts::getStemFrame(const Point& inPoint) const
{
    return mStem.getFrame(inPoint);
}


Rect
DrawableNoteParts::getNoteFlagsFrame(const Point& inPoint) const
{
    return mNoteFlags.getFrame(inPoint);
}


Rect
DrawableNoteParts::getLegerLinesFrame(const Point& inPoint) const
{
    return mLegerLines.getFrame(inPoint);
}

Rect
DrawableNoteParts::getArpeggiateFrame(const Point& inPoint) const
{
    return mArpeggiate.getFrame(inPoint);

}

Region
DrawableNoteParts::getNoteheadRegion(const Point& inPoint) const
{
    return mNotehead.getRegion(inPoint);
}


Region
DrawableNoteParts::getNoteDotsRegion(const Point& inPoint) const
{
    return mNoteDots.getRegion(inPoint);
}


Region
DrawableNoteParts::getAccidentalRegion(const Point& inPoint) const
{
    return mAccidental.getRegion(inPoint);
}


Region
DrawableNoteParts::getStemRegion(const Point& inPoint) const
{
    return mStem.getRegion(inPoint);
}


Region
DrawableNoteParts::getNoteFlagsRegion(const Point& inPoint) const
{
    return mNoteFlags.getRegion(inPoint);
}


Region
DrawableNoteParts::getLegerLinesRegion(const Point& inPoint) const
{
    return mLegerLines.getRegion(inPoint);
}


Region
DrawableNoteParts::getArpeggiateRegion(const Point& inPoint) const
{
    return mArpeggiate.getRegion(inPoint);
}


Point
DrawableNoteParts::getNoteheadAnchor(const Point& inPoint) const
{
    return mNotehead.getAnchor(inPoint);
}


Point
DrawableNoteParts::getNoteDotsAnchor(const Point& inPoint) const
{
    return mNoteDots.getAnchor(inPoint);
}


Point
DrawableNoteParts::getAccidentalAnchor(const Point& inPoint) const
{
    return mAccidental.getAnchor(inPoint);
}


Point
DrawableNoteParts::getStemAnchor(const Point& inPoint) const
{
    return mStem.getAnchor(inPoint);
}


Point
DrawableNoteParts::getNoteFlagsAnchor(const Point& inPoint) const
{
    return mNoteFlags.getAnchor(inPoint);
}


Point
DrawableNoteParts::getLegerLinesAnchor(const Point& inPoint) const
{
    return mLegerLines.getAnchor(inPoint);
}


Point
DrawableNoteParts::getArpeggiateAnchor(const Point& inPoint) const
{
    return mArpeggiate.getAnchor(inPoint);
}


bool
DrawableNoteParts::getIsAccidentalDrawable(const Temporal* inTemporal) const
{
    if (mAccidental.getAccidentalType() == AccidentalType::NONE) {
        return false;
    }
    
    return inTemporal->getIsNote();
}

// MARK: - ItemObserver

void
DrawableNoteParts::observedItemChanged(const Item& observedItem,
                                       const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        update(observedItem);
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

