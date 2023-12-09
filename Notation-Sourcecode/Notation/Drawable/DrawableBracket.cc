// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableBracket.h"

// Local
#include "DrawableLine.h"
#include "DrawableText.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "Staff.h"
#include "StaffSystem.h"

namespace komp
{
DrawableBracket::DrawableBracket(SystemBracketType inType,
                                 int inLevel,
                                 Staff& inStart,
                                 Staff& inStop)
: mDrawables{}
, mType(inType)
, mLevel(inLevel)
{
    init(inStart, inStop);
}

void
DrawableBracket::removeFromParent() const
{
    mDrawables.removeFromParent();
}

void
DrawableBracket::restoreParent() const
{
    mDrawables.restoreParent();
}

Point
DrawableBracket::getOffset() const
{
    return mDrawables.getOffset();
}

void
DrawableBracket::setOffset(const Point& inPoint)
{
    mDrawables.setOffset(inPoint);
}

void
DrawableBracket::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
}

void
DrawableBracket::setScale(double inScale)
{
    mDrawables.setScale(inScale);
}

void
DrawableBracket::setRotation(double inDegrees)
{
}

Rect
DrawableBracket::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}

double
DrawableBracket::getWidth() const
{
    return mDrawables.getWidth();
}

double
DrawableBracket::getHeight() const
{
    return mDrawables.getHeight();
}

Region
DrawableBracket::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}

Point
DrawableBracket::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}

DrawResult
DrawableBracket::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}

void
DrawableBracket::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
}

void
DrawableBracket::init(Staff& inStart, Staff& inStop)
{
    const auto systemHeight = inStop.bottom() - inStart.getY();
    const auto addLevelBracket = mLevel == 0 && mType == SystemBracketType::BRACKET;
    if (addLevelBracket) {
        
        addBracket(systemHeight);
        
    } else {
        
        switch (mType)
        {
            case SystemBracketType::BRACKET:
            {
                addBracket(systemHeight);
                break;
            }
                
            case SystemBracketType::BRACE:
            {
                addBrace(systemHeight);
                break;
            }
                
            case SystemBracketType::LINE:
            {
                addLine(systemHeight);
                break;
            }
                
            case SystemBracketType::SQUARE:
            {
                addSquare(systemHeight);
                break;
            }
                
            default:
                break;
        }
    }
}

void
DrawableBracket::addBracket(double inHeight)
{
    auto& engraver = Engraver::instance();
    const auto thickness = engraver.heavyBarlineWidth();
    
    const auto height = inHeight + thickness;
    const auto width = thickness;
    
    const auto bracketOffsetX = -(width / 2.0);
    const auto bracketOffsetY = -(thickness / 2.0);
    
    auto thickLine = std::make_unique<DrawableLine>();
    thickLine->setLength(height);
    thickLine->setDegrees(90.0);
    thickLine->setThickness(width);
    thickLine->setOffset({0, bracketOffsetY});
    mDrawables.addDrawable(std::move(thickLine));
    
    const auto& theme = FontTheme::instance();
    
    auto topBracket = std::make_unique<DrawableText>();
    const auto topSymbol = theme.BracketTop;
    topBracket->setText(DrawableChars{topSymbol});
    topBracket->setOffset({bracketOffsetX, bracketOffsetY});
    mDrawables.addDrawable(std::move(topBracket));
    
    auto bottomBracket = std::make_unique<DrawableText>();
    const auto bottomSymbol = theme.BracketBottom;
    bottomBracket->setText(DrawableChars{bottomSymbol});
    bottomBracket->setOffset({bracketOffsetX, height + bracketOffsetY});
    mDrawables.addDrawable(std::move(bottomBracket));
    
    const auto levelX = (mLevel + 1) * thickness;
    mDrawables.setOffset({-levelX, 0});
}

void
DrawableBracket::addBrace(double inHeight)
{
    auto& engraver = Engraver::instance();
    const auto& theme = FontTheme::instance();
    
    const auto fontSize = inHeight;
    const auto thickness = engraver.heavyBarlineWidth();
    const auto offsetX = -(CharacterWidth(fontSize, theme.Brace) + thickness);
    
    auto brace = std::make_unique<DrawableText>();
    brace->setText(DrawableChars{theme.Brace});
    brace->setFontSize(fontSize);
    brace->setOffset({offsetX,  inHeight});
    
    mDrawables.addDrawable(std::move(brace));
}

void
DrawableBracket::addLine(double inHeight)
{
    auto& engraver = Engraver::instance();
    const auto thickness = engraver.wedgeLineWidth();
    const auto offsetX = engraver.heavyBarlineWidth();
    
    auto line = std::make_unique<DrawableLine>();
    line->setLength(inHeight);
    line->setDegrees(90.0);
    line->setThickness(thickness);
    line->setOffset({-offsetX, 0});
    mDrawables.addDrawable(std::move(line));
}

void
DrawableBracket::addSquare(double inHeight)
{
    auto& engraver = Engraver::instance();
    const auto width = engraver.staffLineWidth();
    const auto thickness = engraver.staffLineWidth();
    
    const auto offsetX = engraver.heavyBarlineWidth();
    auto vertical = std::make_unique<DrawableLine>();
    vertical->setLength(inHeight);
    vertical->setDegrees(90.0);
    vertical->setThickness(thickness);
    vertical->setOffset({-(offsetX + width), 0});
    mDrawables.addDrawable(std::move(vertical));
    
    const auto hWidth = offsetX + width;
    const auto hOffset = hWidth + (width / 2.0);
    
    auto top = std::make_unique<DrawableLine>();
    top->setLength(hWidth);
    top->setThickness(thickness);
    top->setOffset({-hOffset, 0});
    mDrawables.addDrawable(std::move(top));
    
    auto bottom = std::make_unique<DrawableLine>();
    bottom->setLength(hWidth);
    bottom->setThickness(thickness);
    bottom->setOffset({-hOffset, inHeight});
    mDrawables.addDrawable(std::move(bottom));
    
    // TODO: Level
    //const auto levelX = (mLevel + 1) * thickness;
    //mDrawables.setOffset({-levelX, 0});
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
