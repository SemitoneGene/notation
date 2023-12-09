// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableArpeggiate.h"

// System
#include <cmath>

// Local
#include "Chord.h"
#include "ChordNote.h"
#include "DrawableLine.h"
#include "DrawableText.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Note.h"
#include "Staff.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Throw.h"
#include "WiggleType.h"

namespace komp
{
DrawableArpeggiate::DrawableArpeggiate()
{
}

DrawableArpeggiate::DrawableArpeggiate(const Item& inItem)
{
}

DrawableArpeggiate::~DrawableArpeggiate() = default;

void
DrawableArpeggiate::removeFromParent() const
{
    mDrawable.removeFromParent();
}

void
DrawableArpeggiate::restoreParent() const
{
    mDrawable.restoreParent();
}

Point
DrawableArpeggiate::getOffset() const
{
    return mDrawable.getOffset();
}


void
DrawableArpeggiate::setOffset(const Point& inOffset)
{
    if (inOffset == mDrawable.getOffset()) {
        return;
    }
    
    KOMP_SET_DIRTY
    mDrawable.setOffset(inOffset);
}

void
DrawableArpeggiate::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mDrawable.setZOrder(inOrder);
}

void
DrawableArpeggiate::setScale(double inScale)
{
    mDrawable.setScale(inScale);
}

void
DrawableArpeggiate::setRotation(double inDegrees)
{
}

Rect
DrawableArpeggiate::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableArpeggiate::getWidth() const
{
    return mCache.getWidth(*this);
}


double
DrawableArpeggiate::getHeight() const
{
    return mCache.getHeight(*this);
}


Region
DrawableArpeggiate::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableArpeggiate::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}


DrawResult
DrawableArpeggiate::getMetrics(const Point& inPoint) const
{
    if (mCache.getIsDirty()) {
        auto drawPoint = inPoint;
        drawPoint.moveBy(mDrawable.getOffset());
        const auto result = mDrawable.getMetrics(drawPoint);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}


void
DrawableArpeggiate::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawable.draw(inDrawParams);
}

void
DrawableArpeggiate::clearState()
{
    KOMP_SET_DIRTY
    mDrawable.clearDrawables();
    mDrawable.setOffset(Point{0.0, 0.0});
}


void
DrawableArpeggiate::observedItemChanged(const Item& observedItem,
                                        const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        KOMP_SET_DIRTY
    }
}

const ArpeggiateType&
DrawableArpeggiate::getType() const
{
    return mType;
}

void
DrawableArpeggiate::setType(ArpeggiateType inType)
{
    mType = inType;
}

static double
getWiggleHeight(WiggleType inWiggle)
{
    auto& fontManager = FontManager::instance();
    const auto& fontName = fontManager.getNotationFontName();
    const auto fontSize = FontTheme::instance().getMusicFontSize();
    const auto& wiggle = FontTheme::instance().WiggleMap.at(inWiggle);
    const auto frame = fontManager.getCharacterFrame(fontName,
                                                     fontSize,
                                                     wiggle);
    // Wiggle will be rotated when drawn
    return frame.getWidth();
}

static DrawableUPtr
makeWiggle(WiggleType inWiggle, double inRotation)
{
    const auto fontSize = FontTheme::instance().getMusicFontSize();

    auto drawable = std::make_unique<DrawableText>();
    const auto& wiggle = FontTheme::instance().WiggleMap.at(inWiggle);
    drawable->setText(DrawableChars{wiggle});
    drawable->setFontSize(fontSize);
    drawable->setRotation(inRotation);
    return drawable;
}

static constexpr const auto OverlapReduction = 1.0;

void
DrawableArpeggiate::update(const Chord& inChord)
{
    KOMP_SET_DIRTY
    mDrawable.clearDrawables();
    
    if (mType == ArpeggiateType::NONE) {
        return;
    }
        
    // Get chord height. Don't call getFrame() or any other method that will
    // cache metrics as the arpeggiate metrics have not been calculated.
    const auto& topNote = inChord.getTopNote();
    const auto top = topNote.getFrame().top;
    const auto& bottomNote = inChord.getBottomNote();
    const auto bottom = bottomNote.getFrame().bottom;
    const auto height = bottom - top;
                
    const auto arpeggiate = inChord.getArpeggiate();
    switch (arpeggiate)
    {
    case ArpeggiateType::STANDARD:
        {
            static constexpr const auto StartY = 2.5;
            static constexpr const auto Rotation = -90.0;

            auto offsetY = StartY;

            auto drawable = makeWiggle(WiggleType::UP, Rotation);
            drawable->setOffset({0, offsetY});
            mDrawable.addDrawable(std::move(drawable));

            const auto wiggleHeight = getWiggleHeight(WiggleType::UP) - OverlapReduction;
            const auto count = static_cast<int>(ceil(height / wiggleHeight));

            // Draw the wiggles from the top note to the bottom note
            for (auto index = 0; index < count; ++index) {
                auto drawable = makeWiggle(WiggleType::UP, Rotation);
                drawable->setOffset({0, offsetY});
                mDrawable.addDrawable(std::move(drawable));
                
                offsetY += wiggleHeight;
            }

            break;
        }
        
    case ArpeggiateType::UP:
        {
            static constexpr const auto OffsetX = 0.5;
            static constexpr const auto OffsetY = 4.8;
            static constexpr const auto Rotation = -90.0;
            static constexpr const auto StartY = 6.5;

            auto offsetY = StartY;

            auto arrowDrawable = makeWiggle(WiggleType::UP_ARROW, Rotation);
            arrowDrawable->setOffset({0, offsetY});
            mDrawable.addDrawable(std::move(arrowDrawable));

            const auto arrowHeight = getWiggleHeight(WiggleType::UP_ARROW) - OverlapReduction;
            const auto availableHeight = height - arrowHeight;
            const auto wiggleHeight = getWiggleHeight(WiggleType::UP)  - OverlapReduction;
            const auto count = static_cast<int>(ceil(availableHeight / wiggleHeight));
            
            offsetY += OffsetY;
            
            // Draw the wiggles from the bottom of the arrow to the bottom note
            for (auto index = 0; index < count; ++index) {
                
                auto drawable = makeWiggle(WiggleType::UP, Rotation);
                drawable->setOffset({OffsetX, offsetY});
                mDrawable.addDrawable(std::move(drawable));

                offsetY += wiggleHeight;
            }

            break;
        }
        
    case ArpeggiateType::DOWN:
        {
            static constexpr const auto ArrowX = 2.0;
            static constexpr const auto OffsetY = 4.8;
            static constexpr const auto Rotation = 90.0;
            static constexpr const auto WiggleX = -ArrowX;

            const auto arrowHeight = getWiggleHeight(WiggleType::DOWN_ARROW);

            auto offsetY = height - arrowHeight;

            // Draw the bottom arrow
            auto arrowDrawable = makeWiggle(WiggleType::DOWN_ARROW, Rotation);
            arrowDrawable->setOffset({-ArrowX, offsetY});
            mDrawable.addDrawable(std::move(arrowDrawable));

            offsetY -= OffsetY;

            // Draw the wiggles from the top of the bottom arrow to the top note
            const auto availableHeight = height - arrowHeight;
            const auto wiggleHeight = getWiggleHeight(WiggleType::DOWN) - OverlapReduction;
            const auto count = static_cast<int>(ceil(availableHeight / wiggleHeight));
                                    
            for (auto index = 0; index < count; ++index) {

                auto drawable = makeWiggle(WiggleType::DOWN, Rotation);
                drawable->setOffset({WiggleX, offsetY});
                mDrawable.addDrawable(std::move(drawable));

                offsetY -= wiggleHeight;
            }
            break;
        }
        
    case ArpeggiateType::NON:
        {
            static constexpr const auto HeightPad = 1.0;
            static constexpr const auto LinePad = HeightPad + 1.0;
            static constexpr const auto Length = 4.0;
            static constexpr const auto OffsetX = -4.0;
            static constexpr const auto Rotation = 90.0;
            static constexpr const auto Thickness = 0.85;
            static constexpr const auto HalfThickness = (Thickness / 2.0);

            const auto lineHeight = height + LinePad;
            
            const auto halfSpace = Staff::getSpacing() / 2.0;
            const auto lineOffsetY = -(halfSpace + HeightPad);
            const auto lineTop = lineOffsetY + HalfThickness;
            const auto lineBottom = lineOffsetY + lineHeight - HalfThickness;

            auto verticalLine = std::make_unique<DrawableLine>();
            verticalLine->setLength(lineHeight);
            verticalLine->setDegrees(Rotation);
            verticalLine->setThickness(Thickness);
            verticalLine->setOffset({OffsetX, lineOffsetY});
            mDrawable.addDrawable(std::move(verticalLine));
            
            auto topLine = std::make_unique<DrawableLine>();
            topLine->setLength(Length);
            topLine->setThickness(Thickness);
            topLine->setOffset({OffsetX, lineTop});
            mDrawable.addDrawable(std::move(topLine));
            
            auto bottomLine = std::make_unique<DrawableLine>();
            bottomLine->setLength(Length);
            bottomLine->setThickness(Thickness);
            bottomLine->setOffset({OffsetX, lineBottom});
            mDrawable.addDrawable(std::move(bottomLine));
            
            break;
        }
                
    default:
        KOMP_THROW( "Shouldn't be here" );
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

