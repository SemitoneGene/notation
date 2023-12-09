// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "IdealWidthsVisitor.h"

// Local
#include "EventIntf.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Constants.h"
#include "Throw.h"
#include "Sim.h"
#include "Staff.h"

#include <cmath>

namespace komp
{
    IdealWidthsVisitor::IdealWidthsVisitor(OverviewVisitor inOverviewVisitor)
    : mIsVisitCalled{false}
    , mOverviewVisitor{std::move(inOverviewVisitor)}
    {

    }


    void
    IdealWidthsVisitor::visit(Sim& ioSim)
    {
        mIsVisitCalled = true;
        
        const auto controllingEvent = ioSim.getControllingEvent();
        if (!controllingEvent) {
            return;
        }

        const auto controllingEventDuration = controllingEvent->getDuration();
        KOMP_ASSERT(controllingEventDuration != 0);
        const auto controllingEventDurationInFractionsOfWholeNote = controllingEventDuration / (static_cast<double>(TIME_BASE) * 4);
        const auto idealWidthForControllingEvent = mOverviewVisitor.getIdealSpaceForDuration(controllingEventDurationInFractionsOfWholeNote);
        const auto remainingTime = ioSim.getControllingEventRemainingTime();
        const auto nextSim = ioSim.getNextSim();
        const auto timeToNextSim = nextSim ? nextSim->getTime() - ioSim.getTime() : remainingTime;

        const auto fraction = timeToNextSim / controllingEventDuration;
        const auto width = fraction * idealWidthForControllingEvent;
        ioSim.setIdealWidth(width);

        if (!ioSim.getPreviousSim()) {
            // this is the first sim, set x to accomodate left space
            const auto x = ioSim.getLargestLeftWidth() + 1.0;
            ioSim.setX(x);
        } else {
            const auto previousSim = ioSim.getPreviousSim();
            const auto previousX = previousSim->getX();
            const auto currentX = previousX + (previousSim->getIdealWidth() * Staff::getSpacing());
            ioSim.setX(currentX);
        }
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
