// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableChordString.h"

// Local
#include "ChordSymbol.h"
#include "DrawableChordAtom.h"
#include "DrawableChordAtomFactory.h"
#include "Throw.h"

namespace komp
{
DrawableChordString::DrawableChordString()
{
}

// MARK: - Drawable Interface

void
DrawableChordString::removeFromParent() const
{
    mDrawables.removeFromParent();
}

void
DrawableChordString::restoreParent() const
{
    mDrawables.restoreParent();
}

Point
DrawableChordString::getOffset() const
{
    return mDrawables.getOffset();
}

void
DrawableChordString::setOffset(const Point& inPoint)
{
    mDrawables.setOffset(inPoint);
}

void
DrawableChordString::setZOrder(int inOrder)
{
    mDrawables.setZOrder(inOrder);
}

void
DrawableChordString::setScale(double inScale)
{
    mDrawables.setScale(inScale);
}

void
DrawableChordString::setRotation(double inDegrees)
{
}

Rect
DrawableChordString::getFrame(const Point& inPoint) const
{
    return mDrawables.getFrame(inPoint);
}

double
DrawableChordString::getWidth() const
{
    return mDrawables.getWidth();
}


double
DrawableChordString::getHeight() const
{
    return mDrawables.getHeight();
}


Region
DrawableChordString::getRegion(const Point& inPoint) const
{
    return mDrawables.getRegion(inPoint);
}


Point
DrawableChordString::getAnchor(const Point& inPoint) const
{
    return mDrawables.getAnchor(inPoint);
}


DrawResult
DrawableChordString::getMetrics(const Point& inPoint) const
{
    return mDrawables.getMetrics(inPoint);
}


void
DrawableChordString::draw(const DrawParams& inDrawParams) const
{
    getMetrics(inDrawParams.drawPoint);
    mDrawables.draw(inDrawParams);
}


ChordSymbolString
DrawableChordString::getString() const
{
    ChordSymbolString result;
    
    const auto drawablesCount = mDrawables.getDrawableCount();
    
    for (int i = 0; i < drawablesCount; ++i) {
        const auto& drawableP = &mDrawables.getDrawable(i);
        const auto& chordAtomP = dynamic_cast<const DrawableChordAtom*>(drawableP);
        KOMP_ASSERT(chordAtomP);
        result.push_back(chordAtomP->getChordSymbolChar());
    }
    
    return result;
}


void
DrawableChordString::setString(const ChordSymbolString& inString)
{
    mDrawables.clearDrawables();
    double xPosition = 0.0;
    
    for (const auto& chordChar : inString) {
        const auto& atom = DrawableChordAtomFactory::getDrawableChordAtom(chordChar);

        xPosition += atom.getOffset().x;
        const auto yPosition = atom.getOffset().y;
        
        const auto& drawableText = atom.getDrawableText();
        const auto& chars = drawableText.getText();
        
        if (chars.empty()) {
            return;
        }
        
        auto chordAtom = std::make_unique<DrawableChordAtom>(atom.getParams());
        chordAtom->setOffset(Point{xPosition, yPosition});
        mDrawables.addDrawable(std::move(chordAtom));
        xPosition += atom.getWidth();
        xPosition += atom.getRightMargin();
        xPosition += atom.getLeftMargin();
    }
}

// MARK: - ItemObserver

void
DrawableChordString::observedItemChanged(const Item& observedItem,
                                         const ItemChanges& inChanges)
{
    if (ItemObserver::contains(inChanges, ItemChangeType::Inserted)) {
        restoreParent();
    }

    if (ItemObserver::contains(inChanges, ItemChangeType::Removed)) {
        removeFromParent();
    }
    
    if (ItemObserver::contains(inChanges, ItemChangeType::Updated)) {
        const auto chord = dynamic_cast<const ChordSymbol*>(&observedItem);
        const auto& chordString = chord->getChordSymbolValue().getString();
        setString(chordString);
    }
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
