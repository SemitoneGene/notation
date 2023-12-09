// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

#include "Forward.h"
#include "Geometry.h"

namespace komp
{
    /// \brief  represents a single line that is part of a beam.
    ///
    /// \discussion Holds two notes, note A and note B when represent the start note
    /// and end note of a beam line.  Note A and Note B can be the same note, for
    /// example in the case of a fractional beam.  The start and end points of the
    /// beam line are represented, relative to the notes, by RelativePoint objects.
    ///
    class BeamLineIntf
	{
	public:
		virtual ~BeamLineIntf() {}
        virtual BeamLineIntfUPtr clone() const = 0;
		virtual Temporal& getNoteA() const = 0;
        virtual Temporal& getNoteB() const = 0;
        
        /// @name Spatial point accessors
        //@{
        virtual const RelativePoint& getPointA() const = 0;
        virtual const RelativePoint& getPointB() const = 0;
        virtual RelativePoint& getPointA() = 0;
        virtual RelativePoint& getPointB() = 0;
        //@}
        
        /// Get a rectangle outline for the beam line, in score pixels
        virtual Rect getRect() const = 0;

        /// True if the the start and end X are the same
        virtual bool getIsVertical() const = 0;

        /// The slope of the line in pixels.
        virtual double getSlope() const = 0;

        /// The slope of the line in pixels.
        static inline double getSlope(double x1,
                                      double x2,
                                      double y1,
                                      double y2)
        {
            const double deltaY = y2 - y1;
            const double deltaX = x2 - x1;
            double slope = deltaY / deltaX;
            return slope;
        }

        /// get the thickness of the line in score pixels.
        virtual double getThickness() const = 0;

        /// set the thickness of the beam in score pixels
        virtual void setThickness(double inValue) = 0;

        /// get the y-value, in score pixels, that corresponds
        /// to the given x-value, in score pixels.
        virtual double getScoreYIntercept(double inScoreX) const = 0;

        static inline double getYIntercept(double inLineStartX,
                                           double inLineStartY,
                                           double inLineSlope,
                                           double inInterceptX)
        {
            const double xDistanceFromLineStart = inInterceptX - inLineStartX;
            const double yDistance = xDistanceFromLineStart * inLineSlope;
            const double value = inLineStartY + yDistance;
            return value;
        }
       
        ///@{ @name Level
        ///
        /// Level refers to how 'deep' the beam line is which tells
        /// us what the semantic meaning of the beam line.  For example
        /// level 0 == the eighth note beam line
        /// level 1 == the sixteenth note beam line
        /// level 2 == the thrity-second note beam line... etc.
        ///
        virtual int getLevel() const = 0;
        virtual void setLevel(int inValue) = 0;
        ///@}
        
        virtual bool getIsAbove() const = 0;
        virtual void setIsAbove(bool inValue) = 0;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
