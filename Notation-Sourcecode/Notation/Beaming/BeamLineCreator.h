// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BeamStrategyIntf.h"

namespace komp
{
    /// \brief  clears and recreates the BeamLine collection for BeamSpec
    ///
    /// \discussion This strategy object should be called after the BeamChunks
    /// have been determined. This strategy will read the beam chunk info
    /// and create the beam line objects that are required by the beam.
    ///
    /// @pre Placement and BeamChunks should be already calculated
    ///
    class BeamLineCreator : public BeamStrategyIntf
    {
    public:
        virtual BeamStrategyIntfUPtr clone() const override;
        
        /// clears and recreates the BeamLine collection for BeamSpec
        /// @pre Placement and BeamChunks should be already calculated
        ///
        virtual bool applyStrategy(BeamSpecIntf& outBeamSpec) const override;

    private:
        struct LocalData;
        void calculateBeamLinesAtLevel(LocalData& data) const;
        void createUnbrokenLine(LocalData& data) const;
        void createBrokenRight(LocalData& data) const;
        void createBrokenLeft(LocalData& data) const;
        void adjustEndpointBeamXValues(LocalData& data) const;
        double findStemX(Temporal& inTemporal, BeamPlacement inBeamPlacement) const;
        std::pair<Temporal*, Temporal*> findBeamsideNotes(LocalData& data) const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
