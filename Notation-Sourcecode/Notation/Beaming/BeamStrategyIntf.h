// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "BeamIntf.h"

namespace komp
{
    /// \brief  apply an algorithm to change the state of BeamSpecIntf
    ///
    /// \discussion The BeamSpec object holds as many BeamStrategy objects
    /// as needed to calculate the beam complex's structure.  When needed,
    /// the BeamSpec can apply a BeamStrategy to itself by calling
    /// someStrategy->applyStrategy(*this)
    ///
	class BeamStrategyIntf
	{
	public:
		virtual ~BeamStrategyIntf() {}
        virtual BeamStrategyIntfUPtr clone() const = 0;

        /// @parain outBeamSpec - the BeamSpec object who's state will be changed
        /// @result - the algorithm will change the state of the passed BeamSpec
        /// @return - true if the strategy was applied, false if it was not
        virtual bool applyStrategy(BeamSpecIntf& outBeamSpec) const = 0;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
