// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableTempoMark.h"

// Local
#include "DrawableString.h"
#include "DrawableText.h"
#include "DrawableTextChar.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Preferences.h"
#include "Staff.h"
#include "TempoMark.h"
#include "TemporalConstants.h"
#include "Throw.h"

namespace
{
static const std::string EQUALS = " = ";
static const double TempoNoteScale = 0.5;
}

namespace komp
{

DrawableTempoMark::DrawableTempoMark()
: mDrawables{}
, mOutline{false, true}
{
}

void
DrawableTempoMark::removeFromParent() const
{
    mDrawables.removeFromParent();
    mOutline.removeFromParent();
}

void
DrawableTempoMark::restoreParent() const
{
    mDrawables.restoreParent();
    mOutline.restoreParent();
}

Point
DrawableTempoMark::getOffset() const
{
    return mDrawables.getOffset();
}


void
DrawableTempoMark::setOffset(const Point& inPoint)
{
    mDrawables.setOffset(inPoint);
}

void
DrawableTempoMark::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
    mOutline.setZOrder(inOrder);
}

void
DrawableTempoMark::setScale(double inScale)
{
    mDrawables.setScale(inScale);
    mOutline.setScale(inScale);
}

void
DrawableTempoMark::setRotation(double inDegrees)
{
}

Rect
DrawableTempoMark::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}


double
DrawableTempoMark::getWidth() const
{
    return mDrawables.getWidth();
}


double
DrawableTempoMark::getHeight() const
{
    return mDrawables.getHeight();
}


Region
DrawableTempoMark::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}


Point
DrawableTempoMark::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}

DrawResult
DrawableTempoMark::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}

void
DrawableTempoMark::draw(const DrawParams& inDrawParams) const
{
    mDrawables.draw(inDrawParams);
    if (Preferences::instance().getShowItemFrames()) {
        mOutline.restoreParent();
        mOutline.draw(inDrawParams);
    } else {
        mOutline.removeFromParent();
    }
}

const TempoValue&
DrawableTempoMark::getTempo() const
{
    return mTempo;
}

void
DrawableTempoMark::setTempo(const TempoValue& inTempo)
{
    mTempo = inTempo;
    mDrawables.clearDrawables();
    
    static constexpr const double DotSize = 8.0;
    static constexpr const double StringOffset = 4.0;
    static constexpr const double WholeOffset = -1.5;
    
    double posX = 0.0;
    
    const auto& theme = FontTheme::instance();
    const auto fontSize = Staff::getFontSize() * TempoNoteScale;
    
    // Tempo note symbol
    const auto beatType = mTempo.getBeatType();
    const auto symbol = theme.NoteMap.at(beatType);
    auto note = std::make_unique<DrawableText>();
    note->setText(DrawableChars{symbol});
    note->setFontSize(fontSize);
    note->setOffset(Point{
        posX,
        beatType == TemporalType::WHOLE ? WholeOffset : 0.0
    });
    posX += note->getWidth();
    mDrawables.addDrawable(std::move(note));
    
    if (mTempo.getBeatTypeDots()) {
        auto dot = std::make_unique<DrawableText>();
        DrawableChars dots;
        const auto dotSymbol = theme.MarkInfo.at(MarkType::DOT).getCharacter();
        dots.push_back(DrawableTextChar{dotSymbol});
        dot->setText(dots);
        dot->setFontSize(DotSize);
        posX += dot->getWidth();
        dot->setOffset(Point{
            posX,
            beatType == TemporalType::WHOLE ? WholeOffset : 0.0
        });
        posX += dot->getWidth();
        mDrawables.addDrawable(std::move(dot));
    }
    
    // Tempo string (ie: = 120)
    const auto equalsString = EQUALS;
    auto tempoString = std::make_unique<DrawableString>();
    const auto tempo = std::to_string(mTempo.getRoundedBpm());
    tempoString->setString(equalsString + tempo);
    const auto& font = Preferences::instance().TempoMarkFont;
    tempoString->setFontName(font.Font);
    tempoString->setFontSize(font.Size);
    const auto posY = font.Size / 4.0;
    const auto width = tempoString->getWidth() / 2.0;
    posX += StringOffset + width;
    tempoString->setOffset(Point{posX, -posY});
    mDrawables.addDrawable(std::move(tempoString));
        
    mOutline.setBounds(getFrame(Point{}));
}

// MARK: - ItemObserver

void
DrawableTempoMark::observedItemChanged(const Item& observedItem,
                                       const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto tempoMark = dynamic_cast<const TempoMark*>(&observedItem);
        setTempo(tempoMark->getTempoValue());
    }
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
