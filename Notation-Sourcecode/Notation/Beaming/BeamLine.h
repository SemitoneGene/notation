// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include "BeamLineIntf.h"
#include "RelativePoint.h"

namespace komp
{
    /// \brief  see BeamLineIntf for documentation
    class BeamLine : public BeamLineIntf
	{
	public:
        BeamLine(Item& inNoteA, Item& inNoteB);
        
        virtual BeamLineIntfUPtr clone() const override;
		virtual Temporal& getNoteA() const override;
        virtual Temporal& getNoteB() const override;
        virtual const RelativePoint& getPointA() const override;
        virtual const RelativePoint& getPointB() const override;
        virtual RelativePoint& getPointA() override;
        virtual RelativePoint& getPointB() override;
        virtual bool getIsVertical() const override;
		virtual double getSlope() const override;
        virtual double getScoreYIntercept(double inScoreX) const override;
		virtual double getThickness() const override;
		virtual void setThickness(double inValue) override;
        virtual Rect getRect() const override;
        virtual int getLevel() const override;
        virtual void setLevel(int inValue) override;
        virtual bool getIsAbove() const override;
        virtual void setIsAbove(bool inValue) override;
        
    private:
        RelativePoint mPointA;
        RelativePoint mPointB;
        double mThickness;
        int mLevel;
        bool mIsAbove;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
