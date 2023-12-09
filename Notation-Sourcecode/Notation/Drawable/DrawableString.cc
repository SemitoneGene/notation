// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableString.h"

// System
#include <cmath>

// Local
#include "DrawableContext.h"
#include "DrawableLayerFactory.h"
#include "Engraver.h"
#include "FontManager.h"
#include "Preferences.h"

namespace komp
{
DrawableString::DrawableString()
: mOffset{0.0, 0.0}
, mZOrder{0}
, mString{}
, mFontSize{0.0}
, mFontName{}
, mCache{}
, mOutline{false, true}
{
}

void
DrawableString::removeFromParent() const
{
    if (mLayer) {
        mLayer->removeFromParent();
        mOutline.removeFromParent();
    }
}

void
DrawableString::restoreParent() const
{
    if (mLayer) {
        mLayer->restoreParent();
        mOutline.restoreParent();
    }
}

Point
DrawableString::getOffset() const
{
    return mOffset;
}


void
DrawableString::setOffset(const Point& inOffset)
{
    KOMP_SET_DIRTY
    mOffset = inOffset;
}

void
DrawableString::setZOrder(int inOrder)
{
    KOMP_SET_DIRTY
    mZOrder = inOrder;
}

void
DrawableString::setScale(double inScale)
{
    KOMP_SET_DIRTY
    mScale = inScale;
}

void
DrawableString::setRotation(double inDegrees)
{
}

Rect
DrawableString::getFrame(const Point& inPoint) const
{
    return mCache.getFrame(*this, inPoint);
}


double
DrawableString::getWidth() const
{
    return getFrame(Point{}).getWidth();
}


double
DrawableString::getHeight() const
{
    return getFrame(Point{}).getHeight();
}


Region
DrawableString::getRegion(const Point& inPoint) const
{
    return mCache.getRegion(*this, inPoint);
}


Point
DrawableString::getAnchor(const Point& inPoint) const
{
    return mCache.getAnchor(*this, inPoint);
}

DrawResult
DrawableString::getMetrics(const Point& inPoint) const
{
    if (mString.empty()) {
        return DrawResult{};
    }
    
    if (mCache.getIsDirty()) {
        
        DrawResult result;
        
        auto fontName = mFontName;
        const auto& fontManager = komp::FontManager::instance();
        if (fontName.size() == 0) {
            fontName = fontManager.getNotationFontName();
        }
        
        const auto fontSize = getFontSize();
        const auto text = mString.c_str();
        
        auto drawPoint = inPoint;
        drawPoint.moveBy(mOffset);
        
        // TODO: createText() has already done text measurement
        const auto textSize = MeasureText(fontName,
                                          fontSize,
                                          text);
        
        // Adjust text origin from center point for correct placement
        const auto top = drawPoint.y - (textSize.height / 2.0);
        const auto left = drawPoint.x - (textSize.width / 2.0);
        const auto bottom = top + textSize.height;
        const auto right = left + textSize.width;
        result.frame = Rect(top, left, bottom, right);
        mCache.setCache(result, inPoint);
    }
    return mCache.getDrawResult(*this, inPoint);
}

void
DrawableString::draw(const DrawParams& inDrawParams) const
{
    if (mString.empty()) {
        return;
    }
    
    // We have to recreate the layer each time as the color is encoded in
    // the attributed string.
    if (mLayer) {
        removeFromParent();
        mLayer = nullptr;
    }
    
    auto drawParams = inDrawParams;
    drawParams.drawPoint.moveBy(mOffset);
    
    const auto result = getMetrics(drawParams.drawPoint);
    
    auto fontName = mFontName;
    const auto& fontManager = komp::FontManager::instance();
    if (fontName.size() == 0) {
        fontName = fontManager.getNotationFontName();
    }
    
    const auto fontSize = getFontSize();
    const auto text = mString.c_str();
    const auto point = drawParams.drawPoint;
    
    DrawableLayerFactory factory;
    mLayer = factory.createText(inDrawParams.context,
                                fontName,
                                fontSize,
                                text,
                                {point},
                                inDrawParams.colors.fillColor);
    
    mLayer->setZOrder(mZOrder);
    mLayer->setScale(mScale);
    mLayer->moveTo(point);
    
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();        
        auto bounds = result.frame;
        bounds.moveTo(mOffset.x - (bounds.getWidth() / 2.0),
                      mOffset.y - (bounds.getHeight() / 2.0));
        mOutline.setBounds(bounds);
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}

std::string_view
DrawableString::getString() const
{
    return mString;
}


void
DrawableString::setString(std::string_view inString)
{
    KOMP_SET_DIRTY
    mString = inString;
    removeFromParent();
    mLayer.reset();
    mLayer = nullptr;
}


double
DrawableString::getFontSize() const
{
    return mFontSize;
}


void
DrawableString::setFontSize(double inValue)
{
    KOMP_SET_DIRTY
    mFontSize = inValue;
}


std::string
DrawableString::getFontName() const
{
    return mFontName;
}


void
DrawableString::setFontName(std::string inName)
{
    KOMP_SET_DIRTY
    mFontName = std::move(inName);
}

// MARK: - ItemObserver

void
DrawableString::observedItemChanged(const Item& observedItem,
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

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


