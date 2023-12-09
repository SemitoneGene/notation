// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "BeamLine.h"

// Local
#include "Note.h"
#include "Staff.h"
#include "Measure.h"
#include "Score.h"
#include "Chord.h"

namespace komp
{
    BeamLine::BeamLine(Item& inNoteA, Item& inNoteB)
    : mPointA(inNoteA)
    , mPointB(inNoteB)
    , mThickness(1.0)
    , mIsAbove(true)
    {
        setIsAbove(true);
    }
    
    
    BeamLineIntfUPtr
    BeamLine::clone() const
    {
        return std::make_unique<BeamLine>(*this);
    }
    
    
    Temporal&
    BeamLine::getNoteA() const
    {
        return getPointA().getItem().as<Temporal>();
    }
    
    
    Temporal&
    BeamLine::getNoteB() const
    {
        return getPointB().getItem().as<Temporal>();
    }
    
    
    const RelativePoint&
    BeamLine::getPointA() const
    {
        return mPointA;
    }
    
    
    RelativePoint&
    BeamLine::getPointA()
    {
        return mPointA;
    }
    
    
    const RelativePoint&
    BeamLine::getPointB() const
    {
        return mPointB;
    }
    
    
    RelativePoint&
    BeamLine::getPointB()
    {
        return mPointB;
    }


    bool
    BeamLine::getIsVertical() const
    {
        const auto xA = mPointA.getScoreX();
        const auto xB = mPointB.getScoreX();
        return xA == xB;
    }
    
    
    double
    BeamLine::getSlope() const
    {
        if (getIsVertical()) {
            KOMP_THROW("cannot calculate the slope of a vertical line");
        }
        const auto xA = mPointA.getScoreX();
        const auto yA = mPointA.getScoreY();

        const auto xB = mPointB.getScoreX();
        const auto yB = mPointB.getScoreY();
        
        const auto slope = BeamLineIntf::getSlope(xA, xB, yA, yB);
        return slope;
    }
    
    
    double
    BeamLine::getScoreYIntercept(double inScoreX) const
    {
        const auto& pointA = getPointA();
        if (inScoreX == pointA.getScoreX()) {
            return pointA.getScoreY();
        }

        const auto& pointB = getPointB();
        if (inScoreX == pointB.getScoreX()) {
            return pointB.getScoreY();
        }
        
        if (pointA.getScoreX() == pointB.getScoreX()) {
            // problematic because calculating the slope is part of the logic and
            // a slope with no delta x is undefined.  do the best possible thing
            // in this scenario and return an intercept as if we were at PointA
            return pointA.getScoreY();
        }
        
        const auto lineStartX = pointA.getScoreX();
        const auto lineStartY = pointA.getScoreY();
        const auto lineSlope = getSlope();
        const auto value = BeamLineIntf::getYIntercept(lineStartX,
                                                       lineStartY,
                                                       lineSlope,
                                                       inScoreX);
        return value;
    }
    
    
    double
    BeamLine::getThickness() const
    {
        return mThickness;
    }
    
    
    void
    BeamLine::setThickness(double inValue)
    {
        KOMP_ASSERT(inValue > 0.0);
        mThickness = inValue;
        
    }
    
    
    Rect
    BeamLine::getRect() const
    {
        double x1 = mPointA.getScoreX();
        double y1 = mPointA.getScoreY();
        double x2 = mPointB.getScoreX();
        double y2 = mPointB.getScoreY() + getThickness();

        if (x1 > x2) {
            std::swap(x1, x2);
        }
        
        if (y1 > y2) {
            std::swap(y1, y2);
        }
        
        return Rect{y1, x1, y2, x2};
    }

    int
    BeamLine::getLevel() const
    {
        return mLevel;
    }
    
    
    void
    BeamLine::setLevel(int inValue)
    {
        mLevel = inValue;
    }
    
    
    bool
    BeamLine::getIsAbove() const
    {
        return mIsAbove;
    }
    
    
    void
    BeamLine::setIsAbove(bool inValue)
    {
        mIsAbove = inValue;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
