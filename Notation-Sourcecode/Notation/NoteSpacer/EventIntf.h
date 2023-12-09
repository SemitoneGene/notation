// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Item.h"
#include "PlaneID.h"

#include <map>

namespace komp
{
    class EventIntf;
    using EventUPtr = std::unique_ptr<EventIntf>;

    using SimEvents = std::map<PlaneID, EventUPtr>;
    using SimEventPair = std::pair<PlaneID, EventUPtr>;
    using RawSimEvents = std::map<PlaneID, EventIntf*>;
    using RawSimEventPair = std::pair<PlaneID, EventIntf*>;

    class Sim;

    /// \brief  A Note to be considered by the spacing algorithms.
    ///
    /// \discussion Can be a note, chord, rest, or other type of Score Item
    /// which needs to be considered during note spacing.
    ///
	class EventIntf
	{
	public:
        virtual ~EventIntf() = default;

        virtual EventUPtr clone() const = 0;

        virtual PlaneID getPlaneID() const = 0;
    	virtual Item& getItem() const = 0;

        /// the duration (in clocks) of the item
        virtual double getDuration() const = 0;

        /// the amout of space this item requires to the left
        /// of the beatline
        virtual double getLeftSpace() const = 0;

        /// the amout of space this item requires to the right
        /// of the beatline
        virtual double getRightSpace() const = 0;

        /// the width of the item's frame
        virtual double getWidth() const = 0;

        /// the minimum amount of space required to the right
        /// in order to avoid colliding with the next planar
        /// event. i.e. this->getRightSpace() + next->getLeftSpace()
        virtual double getBlockingWidth() const = 0;

        virtual EventIntf* getNextPlanarEvent() const = 0;
        virtual EventIntf* getPreviousPlanarEvent() const = 0;
        virtual EventIntf* getNextSimEvent() const = 0;
        virtual EventIntf* getPreviousSimEvent() const = 0;

        virtual void setNextPlanarEvent(EventIntf* inEvent) = 0;
        virtual void setPreviousPlanarEvent(EventIntf* inEvent) = 0;
        virtual void setNextSimEvent(EventIntf* inEvent) = 0;
        virtual void setPreviousSimEvent(EventIntf* inEvent) = 0;

        virtual Sim* getSim() const = 0;
        virtual void setSim(Sim* inSim) = 0;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
