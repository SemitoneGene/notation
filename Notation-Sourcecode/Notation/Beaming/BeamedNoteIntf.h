// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Temporal.h"
#include "RelativePoint.h"
#include "BeamChunk.h"

namespace komp
{
    /// \brief  BeamedNoteIntf purpose is to encapsulate a note that will
    /// participate in a beam.
    ///
    /// \discussion BeamedNoteIntf represents a note that is part of a beam, tells
    /// us how the beam should behave with respect to the note, and tells us the
    /// dimensions of the notes’s stem.
    ///
	class BeamedNoteIntf
	{
	public:
		virtual ~BeamedNoteIntf() {}
		
        virtual BeamedNoteIntfUPtr clone() const = 0;

        /// like a normal clone, but using inTemporal instead of the
        /// existing temporal.  All other values are cloned normally.
        virtual BeamedNoteIntfUPtr clone(Temporal& inTemporal) const = 0;
        
        /// get the count of beam segments that this note has. typically this is
        /// required by the note's duration, eg quarter=0, eighth=1, sixteenth=2
		virtual int getCount() const = 0;

        /// isBroken describes whether or not a beam line extending to the right from
        /// this note is broken before the next note. there is an isBroken bool for
        /// each of the beam segments, eg the total number of isBroken bools is equal
        /// to getCount()
		virtual bool getIsBroken(int inBeamLevel) const = 0;

		/// set the isBroken flag for one of the beam segments
        /// @param inBeamLevel - must be between 0 and getCount()
        /// @param inValue - the value to set
		virtual void setIsBroken(int inBeamLevel, bool inValue) = 0;

		/// the 'BeamChunks' represent the step-by-step description of this note's
        /// participation in the beam.
        /// @param inBeamLevel - must be between 0 and getCount()
        /// @return the beam chunk value at the given beam level
        virtual BeamChunk getBeamChunk(int inBeamLevel) const = 0;
        
        /// @return an immutable reference to all of the beam chunks for this note
        virtual const std::vector<BeamChunk>& getBeamChunks() const = 0;

		/// sets the beam chunk - note that this value and 'isBroken' could conflict
        /// with one another. it is the client's responsibility to use and interpret
        /// isBroken and BeamChunk to suit the needs of the client
        /// @param inBeamLevel - must be between 0 and getCount()
        /// @param inBeamChunk - the value to set
        virtual void setBeamChunk(int inBeamLevel, BeamChunk inBeamChunk) = 0;

		/// @return the note encapsulated by this object
		virtual Temporal& getNote() const = 0;
        
        /// @name stem functions
        /// \brief  stem drawing information for this note
        //@{
        virtual bool getDoDrawStem() const = 0;
        virtual void setDoDrawStem(bool inValue) = 0;
        virtual double getStemThickness() const = 0;
        virtual void setStemThickness(double inValue) = 0;
        virtual RelativePoint& getStemNoteheadPoint() = 0;
        virtual const RelativePoint& getStemNoteheadPoint() const = 0;
        virtual RelativePoint& getStemBeamsidePoint() = 0;
        virtual const RelativePoint& getStemBeamsidePoint() const = 0;
        //@}
	};
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
