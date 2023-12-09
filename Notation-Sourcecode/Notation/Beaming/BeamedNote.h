// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Interface
#include "BeamedNoteIntf.h"

// System
#include <vector>
namespace komp
{
    /// \brief  see documentation in BeamedNoteIntf
	class BeamedNote : public BeamedNoteIntf
	{
	public:
		BeamedNote(Temporal& inNote);
        virtual BeamedNoteIntfUPtr clone() const override;
        virtual BeamedNoteIntfUPtr clone(Temporal& inTemporal) const override;
        virtual int getCount() const override;
		virtual bool getIsBroken(int inBeamLevel) const override;
		virtual void setIsBroken(int inBeamLevel, bool inValue) override;
        virtual BeamChunk getBeamChunk(int inBeamLevel) const override;
        virtual const std::vector<BeamChunk>& getBeamChunks() const override;
        virtual void setBeamChunk(int inBeamLevel, BeamChunk inBeamChunk) override;
        virtual Temporal& getNote() const override;
        virtual bool getDoDrawStem() const override;
        virtual void setDoDrawStem(bool inValue) override;
        virtual double getStemThickness() const override;
        virtual void setStemThickness(double inValue) override;
        virtual RelativePoint& getStemNoteheadPoint() override;
        virtual const RelativePoint& getStemNoteheadPoint() const override;
        virtual RelativePoint& getStemBeamsidePoint() override;
        virtual const RelativePoint& getStemBeamsidePoint() const override;
        
	private:
        Temporal& mTemporal;
        std::vector<bool> mIsBroken;
        std::vector<BeamChunk> mBeamChunks;
        bool mDoDrawStem{false};
        double mStemThickness{1.0};
        RelativePoint mStemNoteheadPoint;
        RelativePoint mStemBeamsidePoint;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
