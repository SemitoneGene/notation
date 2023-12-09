// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "kern/IDType.h"

namespace komp
{
    enum class PlaneType
    {
        Temporals,      ///< Notes, Chords, Rests, Mid-Measure-Clefs
        ChordSymbols    ///< ChordSymbols
    };

    struct PlaneID
    {
    public:
        int trackIndex;
        int hand;
        int voice;
        PlaneType planeType;

        PlaneID()
        : trackIndex{-1}
        , hand{-1}
        , voice{-1}
        , planeType{PlaneType::Temporals}
        {

        }
    };


    inline bool operator<(const PlaneID& inLeft, const PlaneID& inRight)
    {
        if (inLeft.trackIndex < inRight.trackIndex) {
            return true;
        } else if (inLeft.trackIndex < inRight.trackIndex) {
            return false;
        }

        if (inLeft.hand < inRight.hand) {
            return true;
        } else if (inLeft.hand < inRight.hand) {
            return false;
        }

        if (inLeft.voice < inRight.voice) {
            return true;
        } else if (inLeft.voice < inRight.voice) {
            return false;
        }

        if (inLeft.planeType < inRight.planeType) {
            return true;
        } else if (inLeft.planeType < inRight.planeType) {
            return false;
        }

        return false;
    }


    inline bool operator>(const PlaneID& inLeft, const PlaneID& inRight)
    {
        return inRight < inLeft;
    }


    inline bool operator==(const PlaneID& inLeft, const PlaneID& inRight)
    {
        return (!(inLeft < inRight)) && (!(inLeft > inRight));
    }


    inline bool operator!=(const PlaneID& inLeft, const PlaneID& inRight)
    {
        return !(inLeft == inRight);
    }


    inline bool operator<=(const PlaneID& inLeft, const PlaneID& inRight)
    {
        return (inLeft < inRight) || (inLeft == inRight);
    }


    inline bool operator>=(const PlaneID& inLeft, const PlaneID& inRight)
    {
        return (inLeft > inRight) || (inLeft == inRight);
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
