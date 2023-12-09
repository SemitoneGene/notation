// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableEnding.h"

// Local
#include "DrawableContext.h"
#include "DrawableLine.h"
#include "Ending.h"
#include "Measure.h"
#include "Preferences.h"
#include "Staff.h"
#include "StaffSystem.h"

namespace
{
static constexpr const double LineHeight = 10.0;
static constexpr const double LineOffset = 8.0;
static constexpr const double LineThickness = 0.55;
static constexpr const double HalfLineThickness = LineThickness / 2.0;
}

namespace komp
{
DrawableEnding::DrawableEnding()
: mDrawables{}
{
}

void
DrawableEnding::removeFromParent() const
{
    mDrawables.removeFromParent();
}

void
DrawableEnding::restoreParent() const
{
    mDrawables.restoreParent();
}

Point
DrawableEnding::getOffset() const
{
    return mDrawables.getOffset();
}


void
DrawableEnding::setOffset(const Point& inOffset)
{
    mDrawables.setOffset(inOffset);
}

void
DrawableEnding::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
}

void
DrawableEnding::setScale(double inScale)
{
    mDrawables.setScale(inScale);
}

void
DrawableEnding::setRotation(double inDegrees)
{
}

Rect
DrawableEnding::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}


double
DrawableEnding::getWidth() const
{
    return mDrawables.getWidth();
}


double
DrawableEnding::getHeight() const
{
    return mDrawables.getHeight();
}


Region
DrawableEnding::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}


Point
DrawableEnding::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}


DrawResult
DrawableEnding::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}


void
DrawableEnding::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
}

static constexpr const double NumberOffsetX = 5.0;
static constexpr const double NumberOffsetY = 6.0;

void
DrawableEnding::update(const std::vector<EndingElement>& inElements)
{
    mDrawables.clearDrawables();

    auto startX = 0.0;
    auto endX = 0.0;
    
    for (const auto& element : inElements) {
        
        const auto measure = element.measure;
        const auto& system = measure->getStaffSystem();
        const auto& staves = system->getStaves();
        const auto staffHeight = staves.front()->getHeight();
        const auto offsetY = -(staffHeight + LineHeight + LineOffset);

        double offsetX = 0.0;
        
        switch (element.type)
        {
        case EndingType::BEGIN:
            {
                startX = measure->getX();
                offsetX = 0.0;
                
                auto endingString = std::make_unique<DrawableString>();
                const auto& font = Preferences::instance().EndingFont;
                endingString->setFontName(font.Font);
                endingString->setFontSize(font.Size);
                endingString->setOffset({
                    NumberOffsetX,
                    offsetY + NumberOffsetY
                });
                const auto endingNumber = std::to_string(element.ending);
                endingString->setString(endingNumber);
                mDrawables.addDrawable(std::move(endingString));
                break;
            }
            
        case EndingType::END:
            {
                // We assume there is a repeat symbol here, so
                // attempt to place line in the middle of the gap.
                // TODO: Get width of right repeat
                const auto rightGap = measure->getRightGap() / 4.0;
                const auto rightMost = measure->getWidth() - rightGap;
                endX = measure->getX() + rightMost;
                offsetX = endX - startX;
                break;
            }
            
        default:
            LOG_F(WARNING, "Unhandled continutation.");
            break;
        }
        
        if (element.status != EndingStatus::HIDDEN) {
            // Create the vertical line
            auto line = std::make_unique<DrawableLine>();
            line->setThickness(LineThickness);
            line->setLength(LineHeight);
            line->setDegrees(90.0);
            line->setOffset({offsetX, offsetY});
            mDrawables.addDrawable(std::move(line));
        }
        
        if (element.type == EndingType::END) {
            // Create horizontal line spanning the measures
            auto connector = std::make_unique<DrawableLine>();
            connector->setThickness(LineThickness);

            connector->setOffset({
                -HalfLineThickness,
                offsetY + HalfLineThickness
            });
            
            const auto connectorWidth = endX - startX;
            connector->setLength(connectorWidth);
            
            mDrawables.addDrawable(std::move(connector));
            
            startX = 0.0;
            endX = 0.0;
        }
    }
}

// MARK: - ItemObserver

void
DrawableEnding::observedItemChanged(const Item& observedItem,
                                    const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto ending = dynamic_cast<const Ending*>(&observedItem);
        update(ending->getElements());
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

