// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "MarkPlacement.h"

namespace komp
{
    class ArticulationPlacement : public MarkPlacement
    {
    public:

        /// specify the amount of spaces (in staff spaces) between
        /// the mark and the notehead, otherwise DEFAULT
        ArticulationPlacement(double inGapSpaces);

        ArticulationPlacement();
        virtual ~ArticulationPlacement();

        static constexpr const double DEFAULT_GAP_SPACES = 0.4;

        virtual MarkPlacementUPtr clone() const override;
        virtual void placeMark(Mark& inMark, PlacementType inPlacement) override;
        virtual void placeMark(Mark& outMark) override;
        
    private:
        void place(Mark& mark, PlacementType placement);
        bool getIsAbove(const Temporal& inTemporal) const;

    private:
        double mGapSpaces;
    };
    
} //namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
