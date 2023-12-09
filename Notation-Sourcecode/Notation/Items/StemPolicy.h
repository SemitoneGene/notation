// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "StemDirection.h"
#include "Policy.h"

namespace komp
{
    /// \brief  Information needed to decide which way the stem goes.
    ///
    /// \discussion It is necessary to know both the stem direction (or
    /// lack thereof) and whether or not the stem direction should
    /// be automatic updated or if it has been overriden by the client.
    ///
    struct StemPolicy
    {
        StemDirection direction;
        Policy policy;

        inline bool getIsStemUp() const { return direction == StemDirection::UP; }
        inline bool getIsStemDown() const { return direction == StemDirection::DOWN; }
        inline bool getHasStem() const { return direction != StemDirection::NONE; }
        inline bool getIsAutomatic() const { return policy == Policy::AUTOMATIC; }
        inline bool getIsSpecified() const { return policy == Policy::SPECIFIED; }

        StemPolicy(StemDirection inDirection, Policy inPolicy)
        : direction{inDirection}
        , policy{inPolicy}
        {

        }

        /// allows for a short-handed setting of the direction
        /// and assumes that the client intends to override
        StemPolicy(StemDirection inDirection)
        : StemPolicy(inDirection, Policy::SPECIFIED)
        {

        }

        /// default constructor for convenience, assumes
        /// automatic stem setting policy
        StemPolicy()
        : StemPolicy(StemDirection::NONE, Policy::AUTOMATIC)
        {

        }
    };

    inline bool operator==(const StemPolicy& lhs, const StemPolicy& rhs)
    {
        if (lhs.direction != rhs.direction) {
            return false;
        }

        if (lhs.policy != rhs.policy) {
            return false;
        }

        return true;
    }

    inline bool operator!=(const StemPolicy& lhs, const StemPolicy& rhs)
    {
        return !(lhs == rhs);
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
