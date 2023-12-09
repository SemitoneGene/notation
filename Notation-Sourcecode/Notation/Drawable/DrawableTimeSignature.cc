// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableTimeSignature.h"

// Local
#include "DrawableText.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "Preferences.h"
#include "Staff.h"
#include "TimeSignatureParams.h"

namespace komp
{
DrawableTimeSignature::DrawableTimeSignature()
: mDrawables{}
, mStaff(nullptr)
, mType(TimeSignatureType::NUMERIC)
, mBeatsPerBar(4)
, mBeatUnit(4)
, mOutline{false, true}
{
}

void
DrawableTimeSignature::update(Staff* inStaff,
                              TimeSignatureType inType,
                              int inBeatsPerBar,
                              int inBeatUnit)
{
    mStaff = inStaff;
    mType = inType;
    mBeatsPerBar = inBeatsPerBar;
    mBeatUnit = inBeatUnit;
    init();
}

void
DrawableTimeSignature::removeFromParent() const
{
    mDrawables.removeFromParent();
    mOutline.removeFromParent();
}

void
DrawableTimeSignature::restoreParent() const
{
    mDrawables.restoreParent();
    mOutline.restoreParent();
}

Point
DrawableTimeSignature::getOffset() const
{
    return mDrawables.getOffset();
}

void
DrawableTimeSignature::setOffset(const Point& inPoint)
{
    mDrawables.setOffset(inPoint);
    mOutline.setOffset(inPoint);
}

void
DrawableTimeSignature::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
    mOutline.setZOrder(inOrder);
}

void
DrawableTimeSignature::setScale(double inScale)
{
    mDrawables.setScale(inScale);
    mOutline.setScale(inScale);
}

void
DrawableTimeSignature::setRotation(double inDegrees)
{
}

Rect
DrawableTimeSignature::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}

double
DrawableTimeSignature::getWidth() const
{
    return mDrawables.getWidth();
}

double
DrawableTimeSignature::getHeight() const
{
    return mDrawables.getHeight();
}

Region
DrawableTimeSignature::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}

Point
DrawableTimeSignature::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}

DrawResult
DrawableTimeSignature::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}

void
DrawableTimeSignature::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
    
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}

Size
DrawableTimeSignature::measureCharacter(unsigned short inChar) const
{
    const auto fontSize = Staff::getFontSize();
    return MeasureCharacter(fontSize, inChar);
}

void
DrawableTimeSignature::init()
{
    if (mStaff->type() != StaffType::NOTATION) {
        // If tab or other non-note staff, exit
        return;
    }
    
    mDrawables.clearDrawables();
    
    // Get the font information
    const auto fontSize = Staff::getFontSize();
    const auto& theme = FontTheme::instance();
    const auto beatUnitOffset = Staff::getSpacing() * 2;
        
    // Calculate the offset
    const auto& spacing = Engraver::instance().spacing();
    const auto meterSpace = spacing.DataSpace + spacing.MeterSpace;
    switch (mType)
    {
        case TimeSignatureType::NUMERIC:
        {
            auto beatsPerBarWidth = 0.0;
            auto beatUnitWidth = 0.0;

            // Handle the standard case
            if (mBeatsPerBar < 10 && mBeatUnit < 10) {
                
                const auto beatPerBarChar = theme.TimeSignatureTable[mBeatsPerBar];
                const auto beatUnitChar = theme.TimeSignatureTable[mBeatUnit];
                
                beatsPerBarWidth = measureCharacter(beatPerBarChar).width;
                beatUnitWidth = measureCharacter(beatUnitChar).width;
                
                auto timeBaseOffsetX = 0.0;
                if (beatsPerBarWidth > beatUnitWidth) {
                    timeBaseOffsetX = (beatsPerBarWidth - beatUnitWidth) / 2.0;
                }
                
                auto beatOffsetX = 0.0;
                if (beatUnitWidth > beatsPerBarWidth) {
                    beatOffsetX = (beatUnitWidth - beatsPerBarWidth) / 2.0;
                }
                
                auto beatPerBarText = std::make_unique<DrawableText>();
                beatPerBarText->setText(DrawableChars{beatPerBarChar});
                beatPerBarText->setFontSize(fontSize);
                beatPerBarText->setOffset({beatOffsetX, 0});
                mDrawables.addDrawable(std::move(beatPerBarText));
                
                auto beatUnitText = std::make_unique<DrawableText>();
                beatUnitText->setOffset({timeBaseOffsetX, beatUnitOffset});
                beatUnitText->setText(DrawableChars{beatUnitChar});
                beatUnitText->setFontSize(fontSize);
                mDrawables.addDrawable(std::move(beatUnitText));
                
                return;
            }
            
            if (mBeatsPerBar < 10) {
                
                auto beatPerBarText = std::make_unique<DrawableText>();
                const auto beatPerBarChar = theme.TimeSignatureTable[mBeatsPerBar];
                beatsPerBarWidth = measureCharacter(beatPerBarChar).width;
                beatPerBarText->setText(DrawableChars{beatPerBarChar});
                mDrawables.addDrawable(std::move(beatPerBarText));
                
            } else {
                
                const int tens = mBeatsPerBar / 10;
                const int remainder = mBeatsPerBar % 10;
                
                const auto tensChar = theme.TimeSignatureTable[tens];
                const auto remainderChar = theme.TimeSignatureTable[remainder];
                
                const auto tensWidth = measureCharacter(tensChar).width;
                const auto remainderWidth = measureCharacter(remainderChar).width;
                
                beatsPerBarWidth = tensWidth + remainderWidth;
                
                auto beatPerBarText = std::make_unique<DrawableText>();
                beatPerBarText->setText(DrawableChars{tensChar});
                beatPerBarText->setFontSize(fontSize);
                beatPerBarText->setOffset(Point(meterSpace - (tensWidth / 2),
                                                0));
                mDrawables.addDrawable(std::move(beatPerBarText));
                
                auto beatUnitText = std::make_unique<DrawableText>();
                beatUnitText->setText(DrawableChars{remainderChar});
                beatUnitText->setFontSize(fontSize);
                beatUnitText->setOffset(Point(meterSpace + (remainderWidth / 2),
                                              0));
                mDrawables.addDrawable(std::move(beatUnitText));
            }
            
            if (mBeatUnit < 10) {
                
                const auto beatUnitChar = theme.TimeSignatureTable[mBeatUnit];
                const auto beatUnitSize = measureCharacter(beatUnitChar);
                beatUnitWidth = beatUnitSize.width;
                
                auto timeBaseOffsetX = 0.0;
                if (beatsPerBarWidth > beatUnitWidth) {
                    timeBaseOffsetX = (beatsPerBarWidth - beatUnitWidth) / 2.0;
                }
                
                auto beatUnitText = std::make_unique<DrawableText>();
                beatUnitText->setText(DrawableChars{beatUnitChar});
                beatUnitText->setFontSize(fontSize);
                beatUnitText->setOffset({timeBaseOffsetX, beatUnitOffset});
                mDrawables.addDrawable(std::move(beatUnitText));
                
            } else {
                
                const int tens = mBeatUnit / 10;
                const int remainder = mBeatUnit % 10;
                
                const auto tensChar = theme.TimeSignatureTable[tens];
                const auto remainderChar = theme.TimeSignatureTable[remainder];
                
                const auto remainderWidth = measureCharacter(remainderChar).width;
                
                auto beatPerBarText = std::make_unique<DrawableText>();
                beatPerBarText->setText(DrawableChars{tensChar});
                const auto beatPerBarTextX = meterSpace + (remainderWidth / 2);
                beatPerBarText->setOffset(Point(beatPerBarTextX, beatUnitOffset));
                mDrawables.addDrawable(std::move(beatPerBarText));
                
                auto beatUnitText = std::make_unique<DrawableText>();
                beatUnitText->setText(DrawableChars{remainderChar});
                const auto beatUnitTextX = meterSpace + (remainderWidth / 2);
                beatUnitText->setOffset(Point(beatUnitTextX, beatUnitOffset));
                mDrawables.addDrawable(std::move(beatUnitText));
            }
            break;
        }
            
        case TimeSignatureType::COMMON:
        case TimeSignatureType::CUT:
        {
            auto signature = std::make_unique<DrawableText>();
            const auto glyph = theme.TimeSignatureMap.at(mType);
            signature->setText(DrawableChars{glyph});
            mDrawables.addDrawable(std::move(signature));
            break;
        }
            
        default:
            break;
    }
    
    mOutline.setBounds(getFrame(Point{}));
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
