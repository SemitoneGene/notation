// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "CollisionVisitor.h"

// Local
#include "EventIntf.h"
#include "Temporal.h"
#include "TemporalValue.h"
#include "Constants.h"
#include "Throw.h"
#include "Sim.h"
#include "Staff.h"

#include <cmath>

namespace
{
static constexpr double MINIMUM_PIXELS_BETWEEN_COLLISIONS = 2.5;
}

namespace komp
{
    CollisionVisitor::CollisionVisitor()
    : mIsVisitCalled{false}
    , mMeasureNoteSpace{NAN}
    , mPreviousSim{nullptr}
    {

    }


    void
    CollisionVisitor::visit(EventIntf& ioEvent, Sim& ioSim)
    {
        const auto nextEvent = ioEvent.getNextPlanarEvent();

        if (!nextEvent) {
            return;
        }

        const auto nextLeft = nextEvent->getLeftSpace();
        const auto currRight = ioEvent.getRightSpace();
        const auto requiredSpace = currRight + nextLeft + MINIMUM_PIXELS_BETWEEN_COLLISIONS;
        const auto nextX = nextEvent->getSim()->getX();
        const auto currX = ioEvent.getSim()->getX();
        const auto currSpace = nextX - currX;

        if (currSpace < requiredSpace) {

            if (currSpace <= 0.0) {
                return;
            }

            const auto amount = requiredSpace - currSpace;

            auto simPtr = &ioSim;
            auto endSimPtr = nextEvent->getSim();
            int affectedSimCount = 0;

            for ( ; simPtr != nullptr; simPtr = simPtr->getNextSim()) {
                ++affectedSimCount;

                if (simPtr == endSimPtr) {
                    break;
                }
            }

            KOMP_ASSERT(affectedSimCount >= 2);
            const auto amountEachSim = amount / static_cast<double>(affectedSimCount - 1);

            simPtr = &ioSim;
            affectedSimCount = 0;

            for ( ; simPtr != nullptr; simPtr = simPtr->getNextSim()) {
                const double amountThisSim = (affectedSimCount * amountEachSim);
                const auto oldX = simPtr->getX();
                const auto newX = oldX + amountThisSim;
                simPtr->setX(newX);
                ++affectedSimCount;
                if (simPtr == endSimPtr) {
                    break;
                }
            }
            
            if (simPtr) {
                simPtr = simPtr->getNextSim();
            }

            for ( ; simPtr != nullptr; simPtr = simPtr->getNextSim()) {
                const auto oldX = simPtr->getX();
                const auto newX = oldX + amount;
                simPtr->setX(newX);
            }
        }
    }


    void
    CollisionVisitor::notifyBeginVisiting()
    {

    }


    void
    CollisionVisitor::notifyDoneVisiting()
    {

    }


    double
    CollisionVisitor::getMeasureNoteSpace() const
    {
        return mMeasureNoteSpace;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
