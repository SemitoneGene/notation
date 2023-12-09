// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "BeamIntf.h"
#include "BeamSpecIntf.h"
#include "BeamedNoteIntf.h"

namespace komp
{
    /// \brief  create and insert beams into the score
    ///
    /// \discussion Provides functions for requesting beams to be automatically and
    /// inserted into the score. Also provides functions for manually requesting
    /// creation of some of the continuant parts that make up a beam (for more
    /// advanced beaming requirements).
    ///
    class BeamFactory
    {
    public:
        BeamFactory();

        /// BeamFactory cannot be copied
        BeamFactory(const BeamFactory& other) = delete;

        /// BeamFactory cannot bea copy-assigned
        BeamFactory& operator=(const BeamFactory& other) = delete;

        /// BeamFactory can be moved
        BeamFactory(BeamFactory&& other) = default;
        
        /// BeamFactory can be move-assigned
        BeamFactory& operator=(BeamFactory&& other) = default;
        
        /// Beam all of the notes beginning with TemporalA and anding
        /// with TemporalB. If all the notes cannot be beamed, i.e.
        /// a quarter note is encountered or a rest is foune, then beam
        /// as many as possible up until the unbeamable temporal
        /// the beam will be inserted into the measure, but is also returned
        BeamIntf& beamSpan(ItemRegistry& inRegistry,
                           const Temporal& inTemporalA,
                           const Temporal& inTemporalB);
        
        /// Beam the notes between TemporalA and TemporalB, but break the beams
        /// according to the beat grouping of the time signature.
        void autoBeam(ItemRegistry& inRegistry,
                      const Temporal& inTemporalA,
                      const Temporal& inTemporalB);

        /// Beam the notes in the input vector, but break the beams
        /// according to the beat grouping of the time signature.
        void autoBeam(ItemRegistry& inRegistry,
                      const TemporalRefVec& inTemporals);

        /// Looks into the time signature of the measure(s) that the temporals
        /// belong to, will group the notes into the beats as appropriate and
        /// return a set of beam specs that represent proper groupings of notes
        /// ignores track, hand and voice, so be careful
        BeamSpecIntfUPtrVec groupBeats(const TemporalRefVec& inTemporals) const;

        /// Returns a single beam spec that spans all of the temporals. if it
        /// encounters an un-beamable note (e.g. quarter note) is stops and
        /// returns whatever it has created so far.  Assumes correctly sorted.
        BeamSpecIntfUPtr createBeamSpec(const TemporalRefVec& inTemporals) const;

        /// Creates an empty BeamSpec
        BeamSpecIntfUPtr createBeamSpec() const;
        
        /// creates and returns a beam pointer, but does not insert it into score
        BeamIntf& createBeam(ItemRegistry& inRegistry,
                             const BeamSpecIntf& inBeamSpec) const;

        /// create a beamed note
        BeamedNoteIntfUPtr createBeamedNote(Temporal& inTemporal);

        /// inserts the beam into the score
        void insertBeam(BeamIntf& inBeam) const;

    private:
        TemporalRefVec getFilteredTemporals(const Temporal& inTemporalA,
                                            const Temporal& inTemporalB) const;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
