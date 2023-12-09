// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <set>

// Local
#include "Forward.h"
#include "TupletRatio.h"

namespace komp
{
    /// \brief  TupletParams encapsulates all of the information required for a Tuplet
    /// object to construct itself
    ///
    /// \discussion TupletParams encapsulates all of the information required for a
    /// Tuplet object to construct itself. Caution: the temporals are held as strong
    /// pointers (SharedPtr's), so make sure you allow TupletParams to destroy itself.
    ///
    class TupletParams
    {
    public:
        TupletParams();
        TupletRatio tupletRatio;

        /// returns true if the tuplet ratio's normal clocks matches the total
        /// clocks for all of the temporals.
        bool getIsDurationValid() const;

        /// add a temporal to the collection
        void addTemporal(Temporal& inTemporal);

        /// replaces any existing temporals with inTemporals
        void setTemporals(const TemporalRefVec& inTemporals);

        /// get's the temporals (sorted by memory address value)
        TemporalRefVec getTemporals() const;

        /// takes the incoming data and tries to gues the best DurationCount value
        static DurationCount calculateDurationCount(const TemporalRefVec& inTemporals,
                                                    TemporalType inPreferedTemporalType);

    private:
        std::set<Temporal*> mTemporals;

    private:
        void correctForChords();
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
