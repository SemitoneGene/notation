// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableText.h"

// System
#include <cmath>

// Local
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Preferences.h"
#include "Throw.h"

namespace komp
{
DrawableText::DrawableText()
: mFontSize{FontTheme::instance().getMusicFontSize()}
{
}

void
DrawableText::removeFromParent() const
{
    if (mLayer) {
        mLayer->removeFromParent();
    }
    mOutline.removeFromParent();
}

void
DrawableText::restoreParent() const
{
    if (mLayer) {
        mLayer->restoreParent();
    }
    mOutline.restoreParent();
}

Point
DrawableText::getOffset() const
{
    return mOffset;
}

void
DrawableText::setOffset(const Point& inOffset)
{
    if (mOffset == inOffset) {
        return;
    }
    
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableText::setZOrder(int inOrder)
{
    if (mZOrder == inOrder) {
        return;
    }
    
    KOMP_SET_DIRTY
    mZOrder = inOrder;
    mOutline.setZOrder(inOrder);
}

void
DrawableText::setScale(double inScale)
{
    if (mScale == inScale) {
        return;
    }
    
    KOMP_SET_DIRTY
    mScale = inScale;
    mOutline.setScale(inScale);
}


void
DrawableText::setRotation(double inDegrees)
{
    if (mRotation == inDegrees) {
        return;
    }
    
    KOMP_SET_DIRTY
    mRotation = inDegrees;
    mOutline.setRotation(mRotation);
}


Rect
DrawableText::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableText::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableText::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableText::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableText::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableText::getMetrics(const Point& inPoint) const
{
    if (mText.empty()) {
        return DrawResult{};
    }

    if (mCache.getIsDirty()) {
        
        auto drawPoint = inPoint;
        drawPoint.moveBy(mOffset);

#ifdef KOMP_BUILD_APPLE
        const komp::Region region = CGPathCreateMutable();
        auto path = region.getMutable();
#elif KOMP_BUILD_LINUX
        komp::Region region = QPainterPath();
#endif
        DrawResult result;
        
        // Get the cached font
        auto fontName = mFontName;
        const auto& fontManager = komp::FontManager::instance();
        if (fontName.empty()) {
            fontName = fontManager.getNotationFontName();
        }
        const auto fontSize = getFontSize();
        
        bool isFirst = true;
        double accumulatedXOffset = 0.0;
        
        for (const auto& character : mText) {
            
            accumulatedXOffset += character.getLeftMargin();
            
            // Copy the draw params and offset
            auto offsetDrawPoint = drawPoint;
            offsetDrawPoint.moveBy(character.getXOffset(),
                                   character.getYOffset());
            offsetDrawPoint.moveBy(accumulatedXOffset, 0.0);
            
            // Get the frame with origins
            const auto drawableFrame = fontManager.getCharacterFrame(fontName,
                                                                     fontSize,
                                                                     character.getChar());
            
            auto position = applyRawFrameOffsets(offsetDrawPoint, drawableFrame);
            const auto glyphFrame = calculateGlyphFrame(character,
                                                        position,
                                                        Size(drawableFrame.getWidth(),
                                                             drawableFrame.getHeight()));
            accumulateResultFrame(glyphFrame, result.frame, isFirst);
            
            auto glyphPath = fontManager.getCharacterPath(fontName,
                                                          fontSize,
                                                          character.getChar());
            
            // Offset the horizontal position for the next glyph in the sequence
            auto adjustedPath = glyphPath.moveBy({accumulatedXOffset, 0});
#ifdef KOMP_BUILD_APPLE
            CGPathAddPath(path, NULL, adjustedPath);
#elif KOMP_BUILD_LINUX
            region.addPath(adjustedPath);
#endif
            accumulatedXOffset += glyphFrame.getWidth();
            accumulatedXOffset += character.getRightMargin();
        }
        
        result.region = region;
        mCache.setCache(result, inPoint);
    }
    
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableText::draw(const DrawParams& inDrawParams) const
{
    if (mText.empty()) {
        return;
    }
        
    auto offset = getOffset();
    if (mScale != 1.0) {
        offset.scaleBy(mScale);
    }
    auto drawPoint = inDrawParams.drawPoint;
    drawPoint.moveBy(offset);
    
    const auto metrics = getMetrics(drawPoint);

    if (!mLayer) {
        mLayer = std::make_unique<DrawableLayer>(inDrawParams.context,
                                                 metrics.region);
        mLayer->restoreParent();
        mLayer->disableStrokeColor();
    }
    
    mLayer->setZOrder(mZOrder);
    mLayer->setScale(mScale);
    mLayer->setRotation(mRotation);
    mLayer->moveTo(drawPoint);
    mLayer->setFillColor(inDrawParams.colors.fillColor);
    
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();
        auto bounds = metrics.frame;
        bounds.moveTo(mOffset.x,
                      mOffset.y - (bounds.getHeight() / 2.0));
        mOutline.setBounds(bounds);
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}


const DrawableChars&
DrawableText::getText() const
{
    return mText;
}


void
DrawableText::setText(DrawableChars inText)
{
    if (mText == inText) {
        return;
    }
    
    KOMP_SET_DIRTY
    mText = std::move(inText);
    removeFromParent();
    mLayer = nullptr;
}


double
DrawableText::getFontSize() const
{
    return mFontSize;
}


void
DrawableText::setFontSize(double inValue)
{
    KOMP_ASSERT(inValue > 0.0);
    if (areFloatsEqual(inValue, mFontSize)) {
        return;
    }
    
    KOMP_SET_DIRTY
    mFontSize = inValue;
}


std::string
DrawableText::getFontName() const
{
    return mFontName;
}


void
DrawableText::setFontName(std::string_view inName)
{
    if (inName == mFontName) {
        return;
    }
    
    KOMP_SET_DIRTY
    mFontName = std::move(inName);
}

// MARK: - Private

Rect
DrawableText::calculateGlyphFrame(const DrawableTextChar& inChar,
                                  const Point& inPosition,
                                  const Size& inSize) const
{
    // Calculate the frame using the raw frame. The raw frame has
    // an origin of 0,0 at the bottom-left. This differs from our
    // drawing canvas, which assumes 0,0 at the top-left.
    Rect glyphFrame;
    glyphFrame.left = inPosition.x;
    glyphFrame.right = glyphFrame.left + inSize.width;
    glyphFrame.bottom = inPosition.y;
    glyphFrame.top = glyphFrame.bottom - inSize.height;
    return glyphFrame;
}


Point
DrawableText::applyRawFrameOffsets(const Point& inPoint,
                                   const Rect& inDrawableFrame) const
{
    // Apply the raw frame offsets to draw point. The offsets may contain non-zero
    // values which must be applied so that our frame rect will be properly centered.
    auto position = inPoint;
    position.x += inDrawableFrame.left;
    position.y -= inDrawableFrame.top;
    return position;
}

void
DrawableText::accumulateResultFrame(const Rect& inGlyphFrame,
                                    Rect& ioResultFrame,
                                    bool& inIsFirst) const
{
    if (inIsFirst) {
        inIsFirst = false;
        ioResultFrame = inGlyphFrame;
    } else {
        ioResultFrame.unionRect(inGlyphFrame);
    }
}

// MARK: - ItemObserver

void
DrawableText::observedItemChanged(const Item& observedItem,
                                  const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto& font = Preferences::instance().TextFont;
        setFontName(font.Font);
        setFontSize(font.Size);
        KOMP_SET_DIRTY
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

