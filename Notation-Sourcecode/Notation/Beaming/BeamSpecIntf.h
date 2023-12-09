// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "BeamPlacement.h"
#include "Policy.h"
#include "StemPolicy.h"
#include "Geometry.h"

namespace komp
{
    /// \brief  The control object which calculates the beam.
    ///
    /// \discussion BeamSpec is a container for BeamedNotes, BeamStrategies, and
    /// BeamLines. BeamStrategies are used to inspect (and possibly alter) the
    /// BeamedNotes, create the BeamLine objects and calculate their positions.
    /// A BeamSpec will be held by a BeamIntf object.
    ///
	class BeamSpecIntf
	{
	public:
		virtual ~BeamSpecIntf() {}
		virtual BeamSpecIntfUPtr clone() const = 0;
        
        /// is the BeamSpec in a valid, usable state
        virtual bool getIsValid() const = 0;

        virtual Measure& getMeasure() const = 0;
        
        /// replace beamed notes that are members of a chord
        /// with their parent chord instead
        virtual void replaceChordedNotes() = 0;
        
        //@{ @name BeamPlacement accessors
        virtual BeamPlacement getBeamPlacement() const = 0;
        virtual Policy getBeamPlacementPolicy() const = 0;
        //@}
        
        /// \brief Constructs the beamLines internally and calculates the line points for drawing stems.
        /// \result Returns true if the operation was successful
        virtual bool calculateBeamLines(StemPolicy& ioStemPolicy) = 0;
		
        //@{ @name BeamLine Accessors
        virtual int getBeamLineCount() const = 0;
		virtual const BeamLineIntf& getBeamLine(int inIndex) const = 0;
        virtual BeamLineIntf& getBeamLine(int inIndex) = 0;
        virtual void removeBeamLine(int inIndex) = 0;
        virtual void addBeamLine(const BeamLineIntf& inBeamLine) = 0;
        virtual void clearBeamLines() = 0;
		virtual const BeamLineIntfUPtrVec& getBeamLines() = 0;
        //@}
        
        /// \discussion returns the level (index) of the deepest beam line.
        /// another way of saying this is that it returns the one less than
        /// number of beams required for the shortest duration note in the
        /// collection. for example if there are only eighth notes then 0,
        /// if a sixteenth note exists then 1, 32nd note then 2, etc.
        virtual int getMaxLevel() const = 0;

        //@{ @name BeamedNote accessors
		virtual int getBeamedNoteCount() const = 0;
		virtual const BeamedNoteIntf& getBeamedNote(int inIndex) const = 0;
        virtual BeamedNoteIntf& getBeamedNote(int inIndex) = 0;
        virtual bool addBeamedNote(const BeamedNoteIntf& inBeamedNote) = 0;
        virtual bool removeBeamedNote(const Note& inNote) = 0;
        virtual const BeamedNoteIntfUPtrVec& getBeamedNotes() = 0;
        //@}
        
        //@{ @name Strategy Injection Accessors
        virtual void addStrategy(BeamStrategyIntfUPtr&& inStrategy) = 0;
        virtual void clearStrategies() = 0;
        //@}

        /// \discussion beams span multiple notes, so this function returns the drawPoint of the *first* note that is
        /// part of the beam. If no notes are available then { 0, 0 } is returned.
		virtual Point getDrawPoint() const = 0;
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
