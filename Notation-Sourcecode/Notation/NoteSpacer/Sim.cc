// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Sim.h"

// Local
#include "EventIntf.h"
#include "Throw.h"

namespace komp
{
Sim::Sim(int inTime)
: mTime{inTime}
{
    
}


Sim*
Sim::getPreviousSim() const
{
    return mPreviousSim;
}


Sim*
Sim::getNextSim() const
{
    return mNextSim;
}


void
Sim::setPreviousSim(Sim* inSim)
{
    mPreviousSim = inSim;
}


void
Sim::setNextSim(Sim* inSim)
{
    mNextSim = inSim;
}


void
Sim::calculate()
{
    findShortestEvent();
}


bool
Sim::getIsEmpty() const
{
    return mSimEvents.empty();
}


int
Sim::getTime() const
{
    return mTime;
}


int
Sim::getDuration() const
{
    const auto nextSim = getNextSim();
    if (nextSim) {
        const auto nextSimTime = nextSim->getTime();
        const auto thisTime = this->getTime();
        const auto result = nextSimTime - thisTime;
        return result;
    }
    
    const auto shortestEvent = getShortestEvent();
    if (shortestEvent) {
        return roundToInt(shortestEvent->getDuration());
    }
    
    return 1.0;
}


void
Sim::addEvent(EventUPtr&& inEvent)
{
    const auto& item = inEvent->getItem();
    const auto incomingTime = komp::roundToInt(item.getTime());
    KOMP_ASSERT(incomingTime == mTime);
    const auto planeID = inEvent->getPlaneID();
    const auto existing = mSimEvents.find(planeID);
    
    if (existing != mSimEvents.cend()) {
        // TODO: FIXME  This happens a lot!
        //LOG_F(ERROR, "two items occupying the same time in the same plane. skipping the duplicate.");
        inEvent = nullptr;
        return;
    }
    
    inEvent->setSim(this);
    mSimEvents.emplace(planeID, std::move(inEvent));
}


double
Sim::getLeftWidth() const
{
    const auto controllingEvent = getControllingEvent();
    if (!controllingEvent) {
        return NAN;
    }
    
    return controllingEvent->getLeftSpace();
}


double
Sim::getRightWidth() const
{
    const auto controllingEvent = getControllingEvent();
    
    if (!controllingEvent) {
        return NAN;
    }
    
    return controllingEvent->getRightSpace();
}


double
Sim::getIdealWidth() const
{
    return mIdealWidth;
}


void
Sim::setIdealWidth(double inIdealWidth)
{
    mIdealWidth = inIdealWidth;
}


double
Sim::getLargestLeftWidth() const
{
    double largest = -1.0;
    
    for (const auto& eventPair : mSimEvents) {
        if (eventPair.second->getLeftSpace() > largest) {
            largest = eventPair.second->getLeftSpace();
        }
    }
    
    if (largest < -0.999999999999999999) {
        return NAN;
    } else if (largest < 0.0) {
        return 0.0;
    }
    
    return largest;
}


double
Sim::getLargestRightWidth() const
{
    double largest = -1.0;
    
    for (const auto& eventPair : mSimEvents) {
        if (eventPair.second->getRightSpace() > largest) {
            largest = eventPair.second->getRightSpace();
        }
    }
    
    if (largest < -0.999999999999999999) {
        return NAN;
    } else if (largest < 0.0) {
        return 0.0;
    }
    
    return largest;
}


RawSimEvents
Sim::getSimEvents() const
{
    RawSimEvents result;
    
    for (const auto& pair : mSimEvents) {
        result.emplace(pair.first, pair.second.get());
    }
    
    return result;
}


EventIntf*
Sim::getEvent(const PlaneID& inPlaneID) const
{
    const auto iter = mSimEvents.find(inPlaneID);
    
    if (iter == mSimEvents.cend()) {
        return nullptr;
    }
    
    return iter->second.get();
}


EventIntf*
Sim::getFirstEvent()
{
    if (mSimEvents.empty()) {
        return nullptr;
    }
    
    auto iter = mSimEvents.begin();
    auto& pair = *iter;
    auto& event = pair.second;
    return event.get();
}


const EventIntf*
Sim::getFirstEvent() const
{
    if (mSimEvents.empty()) {
        return nullptr;
    }
    
    auto iter = mSimEvents.begin();
    auto& pair = *iter;
    auto& event = pair.second;
    return event.get();
}


EventIntf*
Sim::getShortestEvent() const
{
    return mShortestEvent;
}


EventIntf*
Sim::getControllingEvent() const
{
    return mControllingEvent;
}


void
Sim::setControllingEvent(EventIntf* inEvent)
{
    mControllingEvent = inEvent;
}


void
Sim::findShortestEvent()
{
    std::vector<EventIntf*> events;
    for (const auto& eventPair : mSimEvents) {
        events.push_back(eventPair.second.get());
    }
    
    if (events.empty()) {
        mShortestEvent = nullptr;
        return;
    }
    
    const auto sorter = [&](EventIntf* inLeft, EventIntf* inRight) {
        return inLeft->getDuration() < inRight->getDuration();
    };
    
    std::ranges::sort(events, sorter);
    mShortestEvent = events.front();
}


double
Sim::getControllingEventRemainingTime() const
{
    return mControllingEventRemainingTime;
}


void
Sim::setControllingEventRemainingTime(double inTime)
{
    mControllingEventRemainingTime = inTime;
}


double
Sim::getX() const
{
    return mX;
}


void
Sim::setX(double inX)
{
    mX = inX;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
