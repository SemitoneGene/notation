// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableBarline.h"

// Local
#include "DrawableLine.h"
#include "Engraver.h"
#include "Staff.h"

namespace komp
{
// Barline size constants are in pixels
// TODO: These should be configurable
static constexpr const double ThinBarWidth = 0.75;
static constexpr const double ThickBarWidth = 2.0;
static constexpr const double DotSize = 2.0;
static constexpr const double DotOffsetY = DotSize / 2.0;


DrawableBarline::DrawableBarline(BarlineType inType, double inHeight)
: mDrawables{}
, mBarline(inType)
{
    init(inHeight);
}

void
DrawableBarline::removeFromParent() const
{
    mDrawables.removeFromParent();
}

void
DrawableBarline::restoreParent() const
{
    mDrawables.restoreParent();
}

Point
DrawableBarline::getOffset() const
{
    return mDrawables.getOffset();
}

void
DrawableBarline::setOffset(const Point& inPoint)
{
    mDrawables.setOffset(inPoint);
}

void
DrawableBarline::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
}

void
DrawableBarline::setScale(double inScale)
{
    mDrawables.setScale(inScale);
}

void
DrawableBarline::setRotation(double inDegrees)
{
}

Rect
DrawableBarline::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}

double
DrawableBarline::getWidth() const
{
    return mDrawables.getWidth();
}

double
DrawableBarline::getHeight() const
{
    return mDrawables.getHeight();
}

Region
DrawableBarline::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}

Point
DrawableBarline::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}

DrawResult
DrawableBarline::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}

void
DrawableBarline::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
}

std::unique_ptr<DrawableEllipse>
DrawableBarline::createDot() const
{
    return std::make_unique<DrawableEllipse>(DotSize, DotSize);
}

static std::unique_ptr<DrawableLine>
createLine(double length, double thickness) {
    auto thinLine = std::make_unique<DrawableLine>();
    thinLine->setLength(length);
    thinLine->setDegrees(90.0);
    thinLine->setThickness(thickness);
    return thinLine;
}

static std::unique_ptr<DrawableLine>
createThinLine(double length) {
    return createLine(length, ThinBarWidth);
}

static std::unique_ptr<DrawableLine>
createThickLine(double length) {
    return createLine(length, ThickBarWidth);
}

void
DrawableBarline::init(double inHeight)
{
    const auto& engraver = Engraver::instance();
    const auto lineHeight = inHeight + engraver.staffLineWidth();
    const auto lineY = -(engraver.staffLineWidth() / 2.0);
    
    auto offsetX = 0.0;
    
    switch (mBarline)
    {
        case BarlineType::LEFT_DOUBLE_LIGHT:
        case BarlineType::RIGHT_DOUBLE_LIGHT:
        {
            auto line = createThinLine(lineHeight);
            line->setOffset(Point{0, lineY});
            mDrawables.addDrawable(std::move(line));
            offsetX = ThickBarWidth;
            // Fall through to add the second line
        }
            
        case BarlineType::LEFT_NORMAL:
        case BarlineType::RIGHT_NORMAL:
        {
            auto line = createThinLine(lineHeight);
            line->setOffset(Point{offsetX, lineY});
            mDrawables.addDrawable(std::move(line));
            break;
        }
            
        case BarlineType::LEFT_HEAVY_LIGHT:
        case BarlineType::LEFT_REPEAT:
        {
            // TODO: Center repeat dots in a tab staff
            
            // Draw the thick line
            auto thickLine = createThickLine(lineHeight);
            thickLine->setOffset(Point{0, lineY});
            mDrawables.addDrawable(std::move(thickLine));
            offsetX += ThickBarWidth;
            
            // Draw the thin line
            auto thinLine = createThinLine(lineHeight);
            offsetX += ThinBarWidth;
            thinLine->setOffset(Point{offsetX, lineY});
            mDrawables.addDrawable(std::move(thinLine));
            
            if (mBarline == BarlineType::LEFT_REPEAT) {
                
                const auto spacing = Staff::getSpacing();
                offsetX += DotSize;
                
                auto offsetY = spacing + DotOffsetY;
                auto topDot = createDot();
                topDot->setOffset(Point{offsetX, offsetY});
                mDrawables.addDrawable(std::move(topDot));
                
                offsetY += spacing;
                auto bottomDot = createDot();
                bottomDot->setOffset(Point{offsetX, offsetY});
                mDrawables.addDrawable(std::move(bottomDot));
            }
            break;
        }
            
        case BarlineType::RIGHT_LIGHT_HEAVY:
        case BarlineType::RIGHT_REPEAT:
        {
            // Draw the thick line
            auto thickLine = createThickLine(lineHeight);
            thickLine->setOffset(Point{0, lineY});
            mDrawables.addDrawable(std::move(thickLine));
            
            auto offsetX = ThickBarWidth;
            
            // Draw the thin line
            auto thinLine = createThinLine(lineHeight);
            offsetX += ThinBarWidth;
            thinLine->setOffset(Point{-offsetX, lineY});
            mDrawables.addDrawable(std::move(thinLine));
            
            if (mBarline == BarlineType::RIGHT_REPEAT) {
                const auto spacing = Staff::getSpacing();
                offsetX += DotSize * 2;
                
                auto offsetY = spacing + DotOffsetY;
                auto topDot = createDot();
                topDot->setOffset(Point{-offsetX, offsetY});
                mDrawables.addDrawable(std::move(topDot));
                
                offsetY += spacing;
                auto bottomDot = createDot();
                bottomDot->setOffset(Point{-offsetX, offsetY});
                mDrawables.addDrawable(std::move(bottomDot));
            }
            break;
        }
            
        case BarlineType::LEFT_NONE:
        case BarlineType::RIGHT_NONE:
        case BarlineType::UNKNOWN:
            break;
    }
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
