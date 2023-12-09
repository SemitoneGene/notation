// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "EventVisitorIntf.h"
#include "PlaneID.h"

#include <set>

namespace komp
{
    /// \brief  The class is named OverviewVisitor.
    ///
    /// \discussion The class named OverviewVisitor does stuff.
    ///
    class OverviewVisitor : public EventVisitorIntf
	{
	public:
        virtual ~OverviewVisitor() = default;
		OverviewVisitor(Measure& inMeasure);
        
    public:
    	virtual void visit(EventIntf& ioEvent, Sim& ioSim) override;
        virtual void notifyBeginVisiting() override;
        virtual void notifyDoneVisiting() override;

        /// the minimum duration, in fractions of a whole note,
        /// of any item in the matrix
        double getMinDurationWholeNotes() const;

        /// the maximum duration, in fractions of a whole note,
        /// of any item in the matrix
        double getMaxDurationWholeNotes() const;

        const double getClocksPerWholeNote() const;
        const double getGourlayK() const;

        /// uses Gourlay's formulae to return the ideal width, in 'noteheads' (which
        /// we interpret to be the same as staff-spaces) for a given duration.  The
        /// input duration is denominated in whole notes.  So for example 1.0 is a
        /// whole note, 0.5 a half-note, 0.25 a quarter note, etc.
        const double getIdealSpaceForDuration(double inDurationWholeNotes) const;

        const Measure& getMeasure() const;
        
    private:
        bool mIsVisitCalled;
        double mMinDurationWholeNotes;
        double mMaxDurationWholeNotes;
        double mGourlayK;
        int mNumEvents;
        int mNumSims;
        Sim* mPreviousSim;
        std::set<PlaneID> mPlaneIDs;
        Measure& mMeasure;

    private:
    	void calculateGourlayK();
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
