// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableKeySignature.h"

// Local
#include "DrawableText.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "KeySignature.h"
#include "Score.h"
#include "Staff.h"

namespace komp
{
DrawableKeySignature::DrawableKeySignature()
: mDrawables{}
{
}

void
DrawableKeySignature::removeFromParent() const
{
    mDrawables.removeFromParent();
}

void
DrawableKeySignature::restoreParent() const
{
    mDrawables.restoreParent();
}

Point
DrawableKeySignature::getOffset() const
{
    return mDrawables.getOffset();
}

void
DrawableKeySignature::setOffset(const Point& inPoint)
{
    mDrawables.setOffset(inPoint);
}

void
DrawableKeySignature::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
}

void
DrawableKeySignature::setScale(double inScale)
{
    mDrawables.setScale(inScale);
}

void
DrawableKeySignature::setRotation(double inDegrees)
{
}

Rect
DrawableKeySignature::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}

double
DrawableKeySignature::getWidth() const
{
    return mDrawables.getWidth();
}

double
DrawableKeySignature::getHeight() const
{
    return mDrawables.getHeight();
}

Region
DrawableKeySignature::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}

Point
DrawableKeySignature::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}

DrawResult
DrawableKeySignature::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}

void
DrawableKeySignature::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
}

void
DrawableKeySignature::configure(const KeySignatureValue& inKey,
                                const MusicLocation& inLocation)
{
    mDrawables.clearDrawables();
    
    const auto& measure = inLocation.getMeasure();
    const auto& score = measure.getScore();
    
    bool useNaturals = false;
    
    const auto measIndex = measure.getIndex();
    if (measIndex > 0) {
        if (inKey.getValue() == KeySignatureType::C) {
            
            useNaturals = true;
            
            if (inLocation.getTime() >= 1.0) {
                auto loc = inLocation;
                loc.setTime(inLocation.getTime() - 1.0);
                const auto prevKey = loc.getKeySignature();
                if (prevKey == KeySignatureType::C) {
                    return;
                }
            } else {
                const auto& prev = score.getMeasure(measure.getIndex() - 1);
                const MusicLocation loc(*prev.get(),
                                        inLocation.getTrack(),
                                        inLocation.getHand(),
                                        inLocation.getVoice(),
                                        0,
                                        prev->getClocks());
                const auto keyToDraw = loc.getKeySignature();
                if (keyToDraw == KeySignatureType::C) {
                    return;
                }
            }
        }
    }
    
    const auto spacing = Staff::getSpacing();
    const auto clef = inLocation.getClef();
    
    auto slots = inKey.calculateSlots(clef);
    if (slots.empty()) {
        return;
    }
    
    const auto& fontTheme = FontTheme::instance();
    auto accidental = fontTheme.MarkInfo.at(MarkType::SHARP).getCharacter();
    
    if (useNaturals) {
        accidental = fontTheme.MarkInfo.at(MarkType::NATURAL).getCharacter();
    } else if (inKey.getAccidentals() < 0) {
        accidental = fontTheme.MarkInfo.at(MarkType::FLAT).getCharacter();
    }
    
    const auto& staff = inLocation.getStaff();
    const auto fontSize = staff.getFontSize();
    
    double offsetX = 0.0;
    
    // The vertical anchor position in the middle slot
    const auto centerY = staff.slotToPixel(MIDDLE_SLOT);
    
    for (const auto slot : slots) {
        auto drawable = std::make_unique<DrawableText>();
        drawable->setText(DrawableChars{accidental});
        drawable->setFontSize(fontSize);
        const auto slotY = staff.slotToPixel(slot);
        const auto offsetY = slotY - centerY;
        drawable->setOffset({
            offsetX,
            offsetY
        });
        mDrawables.addDrawable(std::move(drawable));
        offsetX += spacing;
    }
}

// MARK: - ItemObserver

void
DrawableKeySignature::observedItemChanged(const Item& observedItem,
                                          const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto keySignature = dynamic_cast<const KeySignature*>(&observedItem);
        configure(keySignature->getKey(), observedItem.getLocation());
    }
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
