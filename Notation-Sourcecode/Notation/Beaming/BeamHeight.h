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
    /// \brief  Moves the beamlines into position.
    ///
    /// \discussion BeamHeight changes the y-values of the beam lines
    /// without changing the slope of the lines.  It expects
    /// Above vs. Below to be decided and for the slopes of
    /// the lines to have already been applied.
    ///
    class BeamHeight : public BeamStrategyIntf
    {
    public:
        virtual BeamStrategyIntfUPtr clone() const override;
        virtual bool applyStrategy(BeamSpecIntf& outBeamSpec) const override;

    private:
        double calculateStemLength(int inMaxLevel) const;
        Temporal&getClearanceTemporal(const BeamedNoteIntf& inBeamedNote,
                                      const BeamSpecIntf& inBeamSpec) const;
        Temporal& getClearanceTemporal(Temporal& inTemporal,
                                       const BeamSpecIntf& inBeamSpec) const;
        Temporal& getClearanceTemporal(Item& inItem,
                                       const BeamSpecIntf& inBeamSpec) const;
        
    private:
        // Elaine Gould
        static constexpr const double BEAM_SPACING = 0.75;
        static constexpr const double MINIMUM_DISTANCE_SPACES = 2.25;
        static constexpr const double IDEAL_STEM_LENGTH_SPACES = 3.5;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
