// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "EventIntf.h"

namespace komp
{
    class EventNode : public EventIntf
	{
	public:
		EventNode(PlaneID inPlaneID,
                  Item& inItem);
        
    public:
        virtual EventUPtr clone() const override;
        virtual PlaneID getPlaneID() const override;
        virtual Item& getItem() const override;
        virtual double getDuration() const override;

        virtual double getLeftSpace() const override;
        virtual double getRightSpace() const override;
        virtual double getWidth() const override;
        virtual double getBlockingWidth() const override;

        virtual EventIntf* getNextPlanarEvent() const override;
        virtual EventIntf* getPreviousPlanarEvent() const override;
        virtual EventIntf* getNextSimEvent() const override;
        virtual EventIntf* getPreviousSimEvent() const override;

        virtual void setNextPlanarEvent(EventIntf* inEvent) override;
        virtual void setPreviousPlanarEvent(EventIntf* inEvent) override;
        virtual void setNextSimEvent(EventIntf* inEvent) override;
        virtual void setPreviousSimEvent(EventIntf* inEvent) override;

        virtual Sim* getSim() const override;
        virtual void setSim(Sim* inSim) override;

    private:
        PlaneID mPlaneID;
        Item& mItem;
        Rect mStartingFrame;
        double mStartingX{0.0};
        EventIntf* mNextPlanarEvent{nullptr};
        EventIntf* mPreviousPlanarEvent{nullptr};
        EventIntf* mNextSimEvent{nullptr};
        EventIntf* mPreviousSimEvent{nullptr};
        Sim* mSim;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
