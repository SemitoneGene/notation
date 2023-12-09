// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "EventNode.h"

// Local
#include "Temporal.h"
#include "Measure.h"

namespace komp
{
    EventNode::EventNode(PlaneID inPlaneID,
                         Item& inItem)
    : mPlaneID{inPlaneID}
    , mItem{inItem}
	{
        const auto measureDataX = mItem.getLocation().getMeasure().getDataX();
        const auto moveByX = measureDataX * -1.0;
        mStartingFrame = mItem.getFrame();
        mStartingFrame.moveBy(moveByX, 0.0);
        mStartingX = mItem.getX();
    }


    EventUPtr
    EventNode::clone() const
    {
        return std::make_unique<EventNode>(*this);
    }


    PlaneID
    EventNode::getPlaneID() const
    {
        return mPlaneID;
    }


    Item&
    EventNode::getItem() const
    {
        return mItem;
    }


    double
    EventNode::getDuration() const
    {
        if (mItem.is<Temporal>()) {
            const auto& temporal = mItem.as<Temporal>();
            return temporal.getDuration().getClocks();
        } else {
            const auto next = getNextPlanarEvent();
            const double myTime = mItem.getTime();

            double nextTime = NAN;

            if (next) {
                nextTime = next->getItem().getTime();
            } else {
                nextTime = myTime + (static_cast<double>(TIME_BASE) * 4);
            }

            KOMP_ASSERT(std::isfinite(nextTime));
            const auto durClocks = nextTime - myTime;
            const auto result = std::max(0.0, durClocks);
            return result;
        }
    }


    double
    EventNode::getLeftSpace() const
    {
        const auto result = mStartingX - mStartingFrame.left;
        if (result < 0) {
            return 0.0;
        }
        
        return result;
    }


    double
    EventNode::getRightSpace() const
    {
        const auto result = mStartingFrame.right - mStartingX;
        if (result < 0) {
            return 0.0;
        }

        return result;
    }


    double
    EventNode::getWidth() const
    {
        return mStartingFrame.getWidth();
    }


    double
    EventNode::getBlockingWidth() const
    {
        const auto next = getNextPlanarEvent();
        double nextLeftSpace = 0.0;

        if (next) {
            nextLeftSpace = next->getLeftSpace();
        }

        const double myRightSpace = getRightSpace();
        const double result = myRightSpace + nextLeftSpace;
        return result;
    }


    EventIntf*
    EventNode::getNextPlanarEvent() const
    {
        return mNextPlanarEvent;
    }


    EventIntf*
    EventNode::getPreviousPlanarEvent() const
    {
        return mPreviousPlanarEvent;
    }


    EventIntf*
    EventNode::getNextSimEvent() const
    {
        return mNextSimEvent;
    }


    EventIntf*
    EventNode::getPreviousSimEvent() const
    {
        return mPreviousSimEvent;
    }


    void
    EventNode::setNextPlanarEvent(EventIntf* inEvent)
    {
        mNextPlanarEvent = inEvent;
    }


    void
    EventNode::setPreviousPlanarEvent(EventIntf* inEvent)
    {
        mPreviousPlanarEvent = inEvent;
    }


    void
    EventNode::setNextSimEvent(EventIntf* inEvent)
    {
        mNextSimEvent = inEvent;
    }


    void
    EventNode::setPreviousSimEvent(EventIntf* inEvent)
    {
        mPreviousSimEvent = inEvent;
    }


    Sim*
    EventNode::getSim() const
    {
        return mSim;
    }


    void
    EventNode::setSim(Sim* inSim)
    {
        mSim = inSim;
    }

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
