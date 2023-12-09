// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ItemSort.h"

// Local
#include "Measure.h"
#include "Score.h"
#include "Temporal.h"
#include "Track.h"

namespace komp
{
    Temporal*
    convertItemToTemporal(Item& inItem)
    {
        if (inItem.is<Temporal>()) {
            return &inItem.as<Temporal>();
        }
        return nullptr;
    }


    int
    compareTemporals(const Temporal& inLeft, const Temporal& inRight)
    {
        if (inLeft.getSlot() < inRight.getSlot()) {
            return -1;
        } else if (inLeft.getSlot() > inRight.getSlot()) {
            return 1;
        }

        const auto leftPitch = inLeft.getPitch().getMidiNote();
        const auto rightPitch = inRight.getPitch().getMidiNote();

        if (leftPitch < rightPitch) {
            return -1;
        } else if (leftPitch > rightPitch) {
            return 1;
        }

        return 0;
    }


    int
    compareMeasureIndex(const Item& inLeft, const Item& inRight)
    {
        const int leftMeasureIndex = inLeft.getLocation().getMeasure().getIndex();
        const int rightMeasureIndex = inRight.getLocation().getMeasure().getIndex();

        if (leftMeasureIndex < rightMeasureIndex) {
            return -1;
        } else if (leftMeasureIndex > rightMeasureIndex) {
            return 1;
        }

        return 0;
    }


    int
    compareTrackIndex(const Item& inLeft, const Item& inRight)
    {
        const auto& leftLoc = inLeft.getLocation();
        const auto& leftTrack = leftLoc.getTrack();
        const auto leftTrackIndex = leftTrack.getIndex();

        const auto& rightLoc = inRight.getLocation();
        const auto& rightTrack = rightLoc.getTrack();
        const auto rightTrackIndex = rightTrack.getIndex();

        if (leftTrackIndex < rightTrackIndex) {
            return -1;
        } else if (leftTrackIndex > rightTrackIndex) {
            return 1;
        }
        
        return 0;
    }


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

