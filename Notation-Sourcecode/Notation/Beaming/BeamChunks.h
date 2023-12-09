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
    /// \brief  desides how to break up the beams
    ///
    /// \discussion the BeamChunks strategy reads from the BeamSpec's
    /// isBroken collection and determines, based on the rules of
    /// music notation, how the beam lines should start and stop in
    /// order to respect the isBroken flags.  It is possible for the
    /// isBroken flags to describe musically nonsensical beaming, in
    /// which case the BeamChunks class will do its best to create
    /// correct music notation.  The BeamChunks algorithm should be
    /// run before the BeamLines are created, and the BeamLine
    /// constructing algorithm should follow the instructions
    /// provided by the BeamChunk collections.
    ///
    class BeamChunks : public BeamStrategyIntf
    {
    public:
        virtual BeamStrategyIntfUPtr clone() const override;
        virtual bool applyStrategy(BeamSpecIntf& outBeamSpec) const override;
        
    private:
        struct LocalData;
        bool isStrategyRequired(BeamSpecIntf& outBeamSpec) const;
        bool strategyImpl(LocalData& data) const;
        void setDoesPreviousHaveMyLevel(LocalData& data) const;
        void setDoesNextHaveMyLevel(LocalData& data) const;
        void setIsPreviousNoteBroken(LocalData& data) const;
        void setBeamChunk(LocalData& data) const;
        void setBeamChunkIfPreviousHasNextDoesNotHave(LocalData& data) const;
        void setBeamChunkIfPreviousDoesNotHaveNextDoesNotHave(LocalData& data) const;
        void setBeamChunkIfPreviousHasNextHas(LocalData& data) const;
        void setBeamChunkIfPreviousDoesNotHaveNextHas(LocalData& data) const;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
