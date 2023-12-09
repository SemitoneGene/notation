// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"
#include "StemDirection.h"

namespace komp
{
    /// \brief  data about the anaysis of a potential Beam Stroke
    ///
    /// \discussion data populated by the BeamStrokeAnalyzer to aid
    /// in the classification and proper handling of beam strokes
    ///
    struct BeamStrokeParams
    {
        BeamStrokeParams() = delete;
        
        BeamStrokeParams(Measure& inMeasure,
                         Track& inTrack,
                         komp::Temporal& inStart,
                         komp::Temporal& inEnd,
                         const TemporalRefVec& inTemporals);

        /// the temporal near the start of the stroke
        Temporal& getStart() const;

        /// the temporal near the end of the stroke
        Temporal& getEnd() const;

        /// get all of the affected temporals, but filtering so that
        /// chord members are not present
        /// all of the temporals that are involved in the beam stroke.
        /// if it is a flag stroke, size will be 1. if beam, size >= 2
        const TemporalRefVec& getTemporals() const;

        /// the measure
        Measure& getMeasure() const;

        /// the track
        Track& getTrack() const;

        /// start and end are the same temporal
        bool isSameTemporal;

        /// start and end are in the same measure
        bool isSameMeasure;

        /// start and end are in the same track
        bool isSameTrack;

        /// start and end are in the same hand
        bool isSameHand;

        /// start and end are in the same voice
        bool isSameVoice;

        /// are the start and end temporals distinct, yet in the same
        /// measure, track, hand, and voice. in otherwords, do they
        /// represent two temporals that can be connected by a beam
        bool isLocationBeamable;

        /// are the start and end the same, and without an existing
        /// beam. in other words, is it a single temporal which is
        /// eligible to have a flag added to it
        bool isLocationFlaggable;

        /// number of unique beam objects found in the temporals
        int numBeams;

        /// number of rests found in the temporals
        int numRests;

        /// number of notes or chords found with a duration of quarter
        /// or longer. i.e. these are the temporals that need to be changed
        /// to eighth note duration
        int numUnbeamableDurations;

        /// number of notes and chords that are not part of a beam, rests
        /// are not counted
        int numUnbeamedTemporals;

        /// is there a single, continuous beam which traverses all the temporals
        bool isContinuousBeam;

        /// is there more than one temporal type affected by the beam stroke
        bool isMultiTemporalType;

        /// Are all of the notes currently unbeamed and the beam starts near the stem of one note
        /// and ends near the stem of another note.
        bool isInitialBeam;

        /// Are the beam stroke's location and charactierstics consistent with a fractional beam.
        bool isFractionalBeam;

        /// Is the beam being added to notes that are already part of the same beam, but more than a
        /// single temporal type is involved. i.e. is a beam being added to a beam construct that is
        /// complex in some way.
        bool isAdditionalBeam;

        /// Is the beam being added to notes that are part of more than one beam construct. In other
        /// words the user is trying to add a main beam that will group-together two or more existing
        /// beam objects.
        bool isConnectionBeam;

        /// Is the beam being added to notes that are already part of the same beam, and all of them
        /// are of the same duration. In other words the user is trying to go from Eighth notes to
        /// Sixteenths, or Sixteenths to Thirty-Seconds, etc.
        bool isHalvingBeam;

        /// are the notes that are being beamed all of the same stem direction.
        bool isExistingStemDirectionDeterministic;

        /// if the stem direction can be determined from the existing notes, this
        /// will say UP or DOWN
        StemDirection stemDirection;

        Measure& measure;
        Track& track;
        Temporal& start;
        Temporal& end;
        TemporalRefVec temporals;
    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
