// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "EventIntf.h"
#include "Sim.h"
#include "EventVisitorIntf.h"
#include "SimVisitorIntf.h"

namespace komp
{
    /// \brief  Collection of Events that need to be spaced.
    ///
    /// \discussion Holds a collection of Sim objects and EventIntf
    /// objects which are organized virtually in a two dimensional matrix
    /// where one dimension is timewise (each Sim represents a timewise
    /// simultaneous collection of events) and the other dimension is
    /// 'planar', where a plane is analogous to track, hand and voice.
    ///
	class EventMatrix
	{
	public:
		EventMatrix(Measure& ioMeasure);

    public:
        bool getIsEmpty() const;
        const Sim* getFirstSim() const;
        Sim* getFirstSim();
        const Sim* getLastSim() const;
        Sim* getLastSim();
        void visitAllEvents(EventVisitorIntf& ioVisitor);
        void visitPlanewise(const PlaneID& inPlaneID, EventVisitorIntf& ioVisitor);
        void visitSims(SimVisitorIntf& ioSimVisitor);

    private:
        using SimMap = std::map<int, Sim>;
        using SimMapPair = std::pair<int, Sim>;
        SimMap mMap;
        Measure& mMeasure;

    private:
        void populateSims();
        void linkSims();
        void linkEvents();
        void callCalculateOnSims();
        void setControllingEvents();
        EventIntf* findPreviousPlanarEvent(const EventIntf* inEvent, const Sim& inSim) const;
        EventIntf* findNextPlanarEvent(const EventIntf* inEvent, const Sim& inSim) const;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
