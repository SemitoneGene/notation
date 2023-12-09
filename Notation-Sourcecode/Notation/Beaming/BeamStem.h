// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BeamIntf.h"
#include "BeamStrategyIntf.h"

namespace komp
{
    /// \brief Sets the points and thickness of the beamedNotes' stems
    ///
    /// \discussion This strategy object should be called after the main beam
    /// is placed in its final position.  BeamStem will iterate through the
    /// BeamSpec's notes and fill in the details for each BeamedNote to
    /// draw it's stem to the beam line.
    ///
    class BeamStem : public BeamStrategyIntf
    {
    public:
        virtual BeamStrategyIntfUPtr clone() const override;
        virtual bool applyStrategy(BeamSpecIntf& outBeamSpec) const override;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
