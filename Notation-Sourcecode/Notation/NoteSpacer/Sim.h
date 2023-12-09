// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "PlaneID.h"
#include "EventIntf.h"

#include <vector>
#include <map>

namespace komp
{
    /// \brief  A Musical Simultaneity
    /// \discussion A musical simultaneity as described by Gourlay 1987 p. 2, "a
    /// sequence of simultaneities, or sims for short."  These are used by note
    /// spacing algorithms.  A Sim holds a collection of vertically-aligned,
    /// simultaneous musical events.
    ///
    class Sim
	{
	public:
        Sim(int inTime);

    public:
        Sim* getPreviousSim() const;
        Sim* getNextSim() const;

        void setPreviousSim(Sim* inSim);
        void setNextSim(Sim* inSim);

        /// once you have added all the events to the sim, you must
        /// call calculate which will analyze and complete construction
        void calculate();

        /// Gourlay: "a flag indicating whether or not there is a note
        /// [or chord or rest] beginning at this time in this voice."
        bool getIsEmpty() const;

        /// The start time of this Sim.  This value is set once a single
        /// Event has been added.  Once the time is set, all additional
        /// events must have the same Measure and Time.
        int getTime() const;

        /// The duration time of this Sim
        int getDuration() const;

        void addEvent(EventUPtr&& inEvent);

        double getLeftWidth() const;
        double getRightWidth() const;
        double getIdealWidth() const;
        void setIdealWidth(double inIdealWidth);
        double getLargestLeftWidth() const;
        double getLargestRightWidth() const;

        RawSimEvents getSimEvents() const;

        /// returns nullptr if not found
        EventIntf* getEvent(const PlaneID& inPlaneID) const;

        EventIntf* getFirstEvent();
        const EventIntf* getFirstEvent() const;

        /// get the event from this sim which has the shortest duration
        EventIntf* getShortestEvent() const;

        /// get controlling event - the controlling event is the event which
        /// currently dictates the spacing. Per Gourlay's algorithm the
        /// controlling event remains in control until it's time has expired.
        /// thus the controlling Event for this sim may not actually be a
        /// member of this sim.
        EventIntf* getControllingEvent() const;

        /// set controlling event
        /// currently dictates the spacing. Per Gourlay's algorithm the
        /// controlling event remains in control until it's time has expired.
        /// thus the controlling Event for this sim may not actually be a
        /// member of this sim.
        void setControllingEvent(EventIntf* inEvent);

        double getControllingEventRemainingTime() const;
        void setControllingEventRemainingTime(double inTime);

        double getX() const;
        void setX(double inX);
        
    private:
        Sim* mPreviousSim{nullptr};
        Sim* mNextSim{nullptr};
        SimEvents mSimEvents{};
        int mTime;
        EventIntf* mControllingEvent{nullptr};
        EventIntf* mShortestEvent{nullptr};
        double mControllingEventRemainingTime{NAN};
        double mX{NAN};
        double mIdealWidth{NAN};

    private:
    	void findShortestEvent();
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
