// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BeamStrategyIntf.h"
#include "BeamIntf.h"

namespace komp
{
    /// \brief  calculates the slope of the beam lines
    ///
    /// \discussion see the applyStrategy function comments
    ///
    class BeamSlope : public BeamStrategyIntf
    {
    public:
        virtual BeamStrategyIntfUPtr clone() const override;

        /// \discussion Takes the given BeamSpec and alters the Y-positions of its
        /// BeamLines to get the proper slope.
        ///
        /// @pre the BeamSpec should have at least one BeamLine.
        //
        /// @result the beam lines will have their PointA YSlots set to 0, and
        /// their PointB Y-Slots set to whatever value is required to create the
        /// proper slope.
        ///
        virtual bool applyStrategy(BeamSpecIntf& outBeamSpec) const override;

    private:
        struct Data;
        
        double getMaxDeltaYSpaces(int numNotes) const;
        void analyzeNotes(Data& ioData) const;
        void calculateYPositions(Data& ioData) const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
