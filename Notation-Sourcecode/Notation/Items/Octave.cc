// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Octave.h"

// System
#include <iostream>

// Local
#include "DrawableContext.h"
#include "DrawableText.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"

namespace komp
{
// TODO: Make this configurable
static const double OCTAVE_SCALE = 0.8;
//static const double OFFSET_H = 3.0;

const std::map<OctaveType, int> Octave::sShiftMap =
{
    { OctaveType::O,           12 },
    { OctaveType::O_ALTA,      12 },
    { OctaveType::O_BASSA,    -12 },
    { OctaveType::O_BASSA_BA, -12 },
    { OctaveType::O_BASSA_VB, -12 },
    { OctaveType::Q,           24 },
    { OctaveType::Q_ALTA,      24 },
    { OctaveType::Q_BASSA,    -24 },
    { OctaveType::Q_BASSA_MB, -24 },
    { OctaveType::V,           36 },
    { OctaveType::V_ALTA,      36 },
    { OctaveType::V_BASSA,    -36 },
    { OctaveType::V_BASSA_MB, -36 },
    { OctaveType::UNKNOWN,      0 },
};

Octave::Octave(const MusicLocation& inLocation,
               const SpannerParams& inParams,
               OctaveType inType)
: Spanner(inLocation, inParams)
, mType(inType)
, mResizeType(ResizeType::NONE)
{
    // Set the octave shift based on the type
    const auto iter = sShiftMap.find(inType);
    KOMP_ASSERT(iter != sShiftMap.cend());
    mOctave = iter->second;
    mPlacement = mOctave < 0 ? PlacementType::BELOW_ITEM : PlacementType::ABOVE_ITEM;
    
    switch (mPlacement)
    {
    case PlacementType::ABOVE_ITEM:
        setSlot(14);
        break;
        
    case PlacementType::BELOW_ITEM:
        setSlot(-2);
        break;
        
    default:
        setSlot(0);
    }
    
    mEndSlot = getSlot();
    
    init();
}

Octave::Octave(const Octave& inOctave)
: Spanner(inOctave)
, mType(inOctave.mType)
, mPlacement(inOctave.mPlacement)
, mOctave(inOctave.mOctave)
{
    
}

Octave&
Octave::operator=(const Octave& other)
{
    copyState(other);
    return *this;
}


const ItemTraits&
Octave::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::Octave);
}


void
Octave::copyState(const Octave& other)
{
    if (this != &other) {
        Spanner::operator=(other);
        mType = other.mType;
        mPlacement = other.mPlacement;
        mOctave = other.mOctave;
    }
}

ItemUPtr
Octave::getMemento() const
{
    return std::make_unique<Octave>(*this);
}

void
Octave::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const Octave* downcastedP = dynamic_cast<const Octave*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    copyState(*downcastedP);
    setIsQuiescent(wasQuiescent);
}

Point
Octave::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}

Rect
Octave::getFrame() const
{
    const auto frame = mDrawables.getFrame(getDrawPoint());
    return frame;
}

void
Octave::createDrawable()
{
}

// MARK: - Draggable

void
Octave::beginDrag(const CurrentState& inState)
{
    const auto left = mControlLeft;
    const auto right = mControlRight;
    const auto& point = inState.getPoint();
    
    if (left.contains(point)) {
        mResizeType = ResizeType::LEFT;
    } else if (right.contains(point)) {
        mResizeType = ResizeType::RIGHT;
    } else {
        mResizeType = ResizeType::NONE;
    }
}

void
Octave::init()
{
    LOG_F(WARNING, "Octave: FIXME");
    
    //        // Get the starting point
    //        const auto drawX = getMeasure()->getDataX() + getX();
    //
    //        // Convert the staff position to a pixel position
    //        const auto drawY = getStaff()->slotToPixel(getSlot(), false);
    //
    //        // Get the font size for the staff
    //        //const auto fontSize = getStaff()->getTextSize() * OCTAVE_SCALE;
    //
    //        // Get the size of the octave character
    //        const CGSize size = measureCharacter(FontTheme::instance().OctaveMap[mType]);
    //        const auto halfHeight = size.height / 2.0;
    //
    //        // Construct the frame
    //        Rect frameRect;
    //        frameRect.left = drawX;
    //        frameRect.top = drawY - halfHeight;
    //        frameRect.setHeight(size.height);
    //        const auto& endMeasure = mEndMeasure.lock();
    //        const auto endPosition = endMeasure->getDataX() + endMeasure->positionForTime(mEndTime);
    //        if (endPosition < frameRect.left) {
    //            KOMP_THROW( "right value is calculated to be less than left value" );
    //        }
    //        frameRect.setRight(endPosition);
    //
    //        // Calculate the edit handles
    //        mControlLeft = frameRect;
    //        mControlLeft.setWidth(frameRect.getHeight());
    //
    //        mControlRight = frameRect;
    //        mControlRight.left = mControlRight.right - mControlRight.getHeight();
    //        mControlRight.setWidth(frameRect.getHeight());
    
    const auto fontSize = Staff::getFontSize() * OCTAVE_SCALE;
    const auto octave = FontTheme::instance().OctaveMap[mType];
    auto symbol = std::make_unique<DrawableText>();
    symbol->setText(DrawableChars{octave});
    symbol->setFontSize(fontSize);
    mDrawables.addDrawable(std::move(symbol));
    
    //        const auto offsetV = frameRect.getHeight() / 2;
    //        inContext->drawNotation(fontSize,
    //                                frameRect.left,
    //                                frameRect.top + offsetV,
    //                                FontTheme::instance().OctaveMap[mType]);
    
    // Draw the lines
    //        const auto size = measureCharacter(FontTheme::instance().OctaveMap[mType]);
    //        const auto startX = frameRect.left + size.width + OFFSET_H;
    //        const auto startY = frameRect.top + offsetV;
    //
    //        // Draw the horizontal line
    //        // TODO: Allow dash style to be configured?
    //        //const auto lineWidth = Engraver::instance().legerLineWidth();
    //        //inContext->setLineWidth(lineWidth);
    //        const auto endX = frameRect.right;
    //        inContext->drawDashedLine(startX,
    //                                  startY,
    //                                  endX,
    //                                  startY,
    //                                  4,
    //                                  8,
    //                                  0);
    //
    //        // Draw the vertical end
    //        const auto halfHeight = size.height / 2.0;
    //        auto endY = startY;
    //        if (mPlacement == PlacementType::ABOVE_ITEM) {
    //            endY += halfHeight;
    //        } else {
    //            endY -= halfHeight;
    //        }
    //
    //        // Draw the horizontal line
    //        inContext->drawDashedLine(endX,
    //                                  startY,
    //                                  endX,
    //                                  endY,
    //                                  4,
    //                                  8,
    //                                  0);
    //        const auto& measure = getLocation().getMeasure();
    //        auto& score = measure.getScore();
    //        const auto selected = score.getSelection()->contains(*this);
    //
    //        if (selected) {
    //            // FIXME
    //            // Draw the edit handles
    //            //const auto& colors = ScoreColors::instance();
    //            //inContext->setStrokeColor(colors.selectedItemColor());
    //            //inContext->strokeRect(mControlLeft);
    //            //inContext->strokeRect(mControlRight);
    //        }
}

void
Octave::drag(const CurrentState& inState)
{
#if 0
    if (inPoint.moved(dragLast())) {
        
        // Get the score and current state
        const auto& score = getScore();
        const auto state = score->getCurrentState(inPoint, getStaff());
        
        // Get the measures
        const auto& measure = getMeasure();
        const auto& dragMeasure = state.getMeasure();
        
        // Use the time of a note near this symbol so octavas, pedals
        // and dynamics will affect the expected note. Octave ends come after the
        // corresponding octave start.
        const auto time = measure->timeForPosition(state.trackIndex(),
                                                   getX(),
                                                   getVoice(),
                                                   true);
        switch (mResizeType)
        {
        case LEFT:
            {
                break;
            }
            
        case RIGHT:
            {
                setEndMeasure(state.measure);
                mEndTime = measure->timeForPosition(state.trackIndex(),
                                                    getX(),
                                                    getVoice(),
                                                    true);
                mEndX = state.getX();
                break;
            }
            
        case NONE:
            {
                // Get the span of the octave
                const auto lastDuration = duration();
                
                // Check if we need to move the item to a new measure
                const auto& currentMeasure = score->getMeasure(state.getMeasure());
                if (currentMeasure != measure) {
                    // Remove the octave and place it in the new measure
                    measure->removeItem(*this);
                    currentMeasure->insertItem(*this, time);
                }
                
                // Adjust the positions
                setSlot(state.getSlot());
                setX(state.getX() - dragOffset().x);
                
                // Update the end measure
                const auto startTime = measure->startTime();
                const auto endMeasure = score->measureForTime(startTime
                                                              + mTime
                                                              + lastDuration);
                setEndMeasure(endMeasure);
                
                // Set the new time and position
                const auto timeDelta = time - mTime;
                mEndTime -= timeDelta;
                setTime(time);
                updatePosition();
                break;
            }
        }
        
        score->invalidate();
    }
#endif
}

void
Octave::endDrag(const CurrentState& inState)
{
}

Point
Octave::calculateDragOffset() const
{
    Point offset;
    
    switch (mResizeType)
    {
    case ResizeType::LEFT:
    case ResizeType::RIGHT:
        {
            offset = Point(0, 0);
            break;
        }
        
    case ResizeType::NONE:
        {
            offset = Spanner::calculateDragOffset();
            break;
        }
    }
    
    return offset;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
