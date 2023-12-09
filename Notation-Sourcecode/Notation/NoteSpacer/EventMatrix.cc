// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "EventMatrix.h"

// Local
#include "ChordSymbol.h"
#include "Clef.h"
#include "EventNode.h"
#include "Measure.h"
#include "Rest.h"
#include "Score.h"
#include "Sim.h"
#include "TempoMark.h"
#include "Temporal.h"
#include "Throw.h"
#include "Track.h"

namespace komp
{
EventMatrix::EventMatrix(Measure& ioMeasure)
: mMap{}
, mMeasure{ioMeasure}
{
    populateSims();
    linkSims();
    linkEvents();
    callCalculateOnSims();
    setControllingEvents();
}


bool
EventMatrix::getIsEmpty() const
{
    if (mMap.empty()) {
        return true;
    }
    
    for (const auto& simPair : mMap) {
        if (!simPair.second.getIsEmpty()) {
            return false;
        }
    }
    
    return true;
}


const Sim*
EventMatrix::getFirstSim() const
{
    if (mMap.empty()) {
        return nullptr;
    }
    
    const auto beginIter = mMap.begin();
    auto& beginSim = beginIter->second;
    auto sim = &beginSim;
    return sim;
}


Sim*
EventMatrix::getFirstSim()
{
    if (mMap.empty()) {
        return nullptr;
    }
    
    const auto beginIter = mMap.begin();
    auto& beginSim = beginIter->second;
    auto sim = &beginSim;
    return sim;
}


const Sim*
EventMatrix::getLastSim() const
{
    if (mMap.empty()) {
        return nullptr;
    }
    
    const auto last = mMap.rbegin();
    auto& lastSim = last->second;
    auto sim = &lastSim;
    return sim;
}


Sim*
EventMatrix::getLastSim()
{
    if (mMap.empty()) {
        return nullptr;
    }
    
    const auto last = mMap.rbegin();
    auto& lastSim = last->second;
    auto sim = &lastSim;
    return sim;
}


void
EventMatrix::visitAllEvents(EventVisitorIntf& ioVisitor)
{
    auto sim = getFirstSim();
    
    for ( ; sim != nullptr; sim = sim->getNextSim()) {
        auto event = sim->getFirstEvent();
        for ( ; event != nullptr; event = event->getNextSimEvent()) {
            ioVisitor.visit(*event, *sim);
        }
    }
    
    ioVisitor.notifyDoneVisiting();
}


void
EventMatrix::visitPlanewise(const PlaneID& inPlaneID, EventVisitorIntf& ioVisitor)
{
    std::set<PlaneID> planes;
    auto sim = getFirstSim();
    EventIntf* event = nullptr;
    
    // find first event matching the PlaneID
    for ( ; sim != nullptr; sim = sim->getNextSim()) {
        event = sim->getEvent(inPlaneID);
        if (event) {
            break;
        }
    }
    
    for ( ; event != nullptr; event = event->getNextPlanarEvent()) {
        
        // TODO: problem - I have no way to pass the correct Sim*
        ioVisitor.visit(*event, *sim);
    }
    
    ioVisitor.notifyDoneVisiting();
}


void
EventMatrix::visitSims(SimVisitorIntf& ioSimVisitor)
{
    auto sim = getFirstSim();
    
    for ( ; sim != nullptr; sim = sim->getNextSim()) {
        ioSimVisitor.visit(*sim);
    }
}


void
EventMatrix::populateSims()
{
    std::set<int> timeTruth;
    const auto& allItems = mMeasure.allItems();
    
    for (Item& item : allItems) {
        
        const auto isTemporal = item.is<Temporal>();
        const auto isChordSymbol = item.getItemTraits().getItemType() == ItemType::ChordSymbol;
        
        if (!isTemporal && !isChordSymbol) {
            continue;
        }
        
        const auto& loc = item.getLocation();
        
        PlaneID plane;
        plane.trackIndex = mMeasure.getScore().getStaffIndex(loc.getTrack());
        plane.hand = loc.getHand();
        plane.voice = loc.getVoice();
        const auto time = roundToInt(item.getTime());
        const auto timeExists = (mMap.find(time) != mMap.cend());
        
        if (!timeExists) {
            mMap.emplace(time, time);
        }
        
        if (isTemporal) {
            
            auto& temporal = item.as<Temporal>();
            const auto isChordMember = temporal.getIsChordMember();
            
            if (isChordMember) {
                continue;
            }
            
            plane.planeType = PlaneType::Temporals;
            auto event = std::make_unique<EventNode>(plane, item);
            mMap.at(time).addEvent(std::move(event));
            
        } else if (isChordSymbol) {
            
            plane.planeType = PlaneType::ChordSymbols;
            auto event = std::make_unique<EventNode>(plane, item);
            mMap.at(time).addEvent(std::move(event));
            
        }
    }
}


void
EventMatrix::linkSims()
{
    const auto mapEnd = mMap.end();
    
    const auto increment = [&](SimMap::iterator& ioIter) {
        if (ioIter != mapEnd) {
            ++ioIter;
        }
    };
    
    auto previous = mMap.begin();
    
    auto curr = mMap.begin();
    increment(curr);
    
    auto next = mMap.begin();
    increment(next);
    increment(next);
    
    // the iter named 'previous' currently sits on the
    // zeroeth element, so populate it's 'next' link with
    // the iter named 'current' which sits on the one-eth
    // element.
    if (previous != mapEnd) {
        
        if (curr != mapEnd) {
            previous->second.setNextSim(&(curr->second));
        }
    }
    
    // now iterate through the rest of the elements using the
    // 'previous' and 'next' iters to set these values on 'curr'
    for ( ; curr != mapEnd; increment(previous), increment(curr), increment(next)) {
        
        if (previous != mapEnd) {
            curr->second.setPreviousSim(&(previous->second));
        }
        
        if (next != mapEnd) {
            curr->second.setNextSim(&(next->second));
        }
    }
}


void
EventMatrix::linkEvents()
{
    for (auto& pair : mMap) {
        
        auto events = pair.second.getSimEvents();
        
        if (events.empty()) {
            continue;
        }
        
        const auto eventsEnd = events.end();
        
        const auto increment = [&](decltype(events)::iterator& ioIter) {
            if (ioIter != eventsEnd) {
                ++ioIter;
            }
        };
        
        auto previous = events.begin();
        
        {
            const auto prevPlanar = findPreviousPlanarEvent(previous->second, pair.second);
            const auto nextPlanar = findNextPlanarEvent(previous->second, pair.second);
            previous->second->setPreviousPlanarEvent(prevPlanar);
            previous->second->setNextPlanarEvent(nextPlanar);
        }
        
        auto curr = events.begin();
        increment(curr);
        
        auto next = events.begin();
        increment(next);
        increment(next);
        
        // the iter named 'previous' currently sits on the
        // zeroeth element, so populate it's 'next' link with
        // the iter named 'current' which sits on the one-eth
        // element.
        if (previous != eventsEnd) {
            
            if (curr != eventsEnd) {
                previous->second->setNextSimEvent(curr->second);
            }
        }
        
        // now iterate through the rest of the elements using the
        // 'previous' and 'next' iters to set these values on 'curr'
        for ( ; curr != eventsEnd; increment(previous), increment(curr), increment(next)) {
            
            if (previous != eventsEnd) {
                curr->second->setPreviousSimEvent(previous->second);
            }
            
            if (next != eventsEnd) {
                curr->second->setNextSimEvent(next->second);
            }
            
            const auto previousPlanarEvent = findPreviousPlanarEvent(curr->second, pair.second);
            const auto nextPlanarEvent = findNextPlanarEvent(curr->second, pair.second);
            curr->second->setPreviousPlanarEvent(previousPlanarEvent);
            curr->second->setNextPlanarEvent(nextPlanarEvent);
        }
    }
}


void
EventMatrix::callCalculateOnSims()
{
    for (auto& simPair : mMap) {
        simPair.second.calculate();
    }
}


void
EventMatrix::setControllingEvents()
{
    auto sim = getFirstSim();
    if (!sim) {
        return;
    }
    
    auto currentControllingEvent = sim->getShortestEvent();
    if (!currentControllingEvent) {
        KOMP_ASSERT(currentControllingEvent);
    }
    KOMP_ASSERT(currentControllingEvent);
    auto controllingEventRemainingTime = currentControllingEvent->getDuration();
    
    for ( ; sim != nullptr; sim = sim->getNextSim()) {
        auto previousSimDuration = 0.0;
        auto previousSim = sim->getPreviousSim();
        
        if (previousSim) {
            previousSimDuration = static_cast<double>(previousSim->getDuration());
        }
        
        controllingEventRemainingTime -= previousSimDuration;
        
        if (controllingEventRemainingTime < 0.1) {
            currentControllingEvent = sim->getShortestEvent();
            if (currentControllingEvent) {
                controllingEventRemainingTime = currentControllingEvent->getDuration();
            } else {
                controllingEventRemainingTime = 0.0;
            }
        }
        
        sim->setControllingEvent(currentControllingEvent);
        sim->setControllingEventRemainingTime(controllingEventRemainingTime);
    }
}


EventIntf*
EventMatrix::findPreviousPlanarEvent(const EventIntf* inEvent, const Sim& inSim) const
{
    EventIntf* result = nullptr;
    const auto planeID = inEvent->getPlaneID();
    auto sim = inSim.getPreviousSim();
    
    for ( ; sim != nullptr; sim = sim->getPreviousSim()) {
        const auto event = sim->getEvent(planeID);
        if (event) {
            result = event;
            break;
        }
    }
    
    return result;
}


EventIntf*
EventMatrix::findNextPlanarEvent(const EventIntf* inEvent,
                                 const Sim& inSim) const
{
    EventIntf* result = nullptr;
    const auto planeID = inEvent->getPlaneID();
    auto sim = inSim.getNextSim();
    
    for ( ; sim != nullptr; sim = sim->getNextSim()) {
        const auto event = sim->getEvent(planeID);
        if (event) {
            result = event;
            break;
        }
    }
    
    return result;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
