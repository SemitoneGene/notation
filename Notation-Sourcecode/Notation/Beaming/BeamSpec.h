// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BeamSpecIntf.h"
#include "ItemObserver.h"

namespace komp
{
    /// \brief  See BeamSpecIntf for documentation
    ///
	class BeamSpec : public BeamSpecIntf
    {
	public:
		
		BeamSpec();
        ~BeamSpec();
		BeamSpec(const BeamSpec& other);
		BeamSpec(BeamSpec&& other);
		BeamSpec& operator=(const BeamSpec& other);
		BeamSpec& operator=(BeamSpec&& other);
        virtual BeamSpecIntfUPtr clone() const override;
        
        virtual void replaceChordedNotes() override;
        virtual bool getIsValid() const override;
        virtual Measure& getMeasure() const override;
        virtual BeamPlacement getBeamPlacement() const override;
        virtual Policy getBeamPlacementPolicy() const override;
        virtual bool calculateBeamLines(StemPolicy& ioStemPolicy) override;
		virtual int getBeamLineCount() const override;
		virtual const BeamLineIntf& getBeamLine(int inIndex) const override;
        virtual BeamLineIntf& getBeamLine(int inIndex) override;
        virtual void removeBeamLine(int inIndex) override;
        virtual void addBeamLine(const BeamLineIntf& inBeamLine) override;
        virtual void clearBeamLines() override;
        virtual const BeamLineIntfUPtrVec& getBeamLines() override;
        virtual int getMaxLevel() const override;
		virtual int getBeamedNoteCount() const override;
		virtual const BeamedNoteIntf& getBeamedNote(int inIndex) const override;
        virtual BeamedNoteIntf& getBeamedNote(int inIndex) override;
        virtual bool addBeamedNote(const BeamedNoteIntf& inBeamedNote) override;
        virtual bool removeBeamedNote(const Note& inNote) override;
        virtual const BeamedNoteIntfUPtrVec& getBeamedNotes() override;
        virtual void addStrategy(BeamStrategyIntfUPtr&& inStrategy) override;
        virtual void clearStrategies() override;
		virtual Point getDrawPoint() const override;
        
    private:
        bool calculateBeamLinesImpl(StemPolicy& ioStemPolicy);
        StemPolicy choosePlacement(StemPolicy inStemPolicy);
        void clearState();
        void clearBeamChunks();
        void sortBeamedNotes();

    private:
    	BeamedNoteIntfUPtrVec mBeamedNotes;
    	BeamLineIntfUPtrVec mBeamLines;
        BeamStrategyIntfUPtrVec mBeamStrategies;
        StemPolicy mStemPolicy;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
