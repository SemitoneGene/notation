// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "BarlineType.h"
#include "BeamStrokeParams.h"
#include "ChordSymbolParams.h"
#include "CommandIntf.h"
#include "CurveParams.h"
#include "Duration.h"
#include "EndingParams.h"
#include "Forward.h"
#include "KeySignatureType.h"
#include "PasteParams.h"
#include "PlacementType.h"
#include "SpannerParams.h"
#include "StemType.h"
#include "TempoValue.h"
#include "TupletParams.h"
#include "WedgeType.h"

namespace komp
{
    class CommandFactory
    {
    public:
        CommandFactory(Score& inScore);
        
        ~CommandFactory() = default;

        /// CommandFactory cannot be copy-constructed
        CommandFactory(const CommandFactory& other) = delete;

        /// CommandFactory cannot be move-constructed
        CommandFactory(CommandFactory&& other) = delete;

        /// CommandFactory cannot be copy-assigned
        CommandFactory& operator=(const CommandFactory& other) = delete;

        /// CommandFactory cannot be move-assigned
        CommandFactory& operator=(CommandFactory&& other) = delete;
        
        /// Returns an empty command whose execute() function is
        /// a no-op. the the undo and redo commands will execute
        /// any sub commands, so the genericCommand object can
        /// be used as a container for sub commands
        CommandIntfUPtr genericCommand(std::string inName);

        /// Changes the measure(s) time signature (meter)
        CommandIntfUPtr changeTimeSignatureCommand(size_t inIndex,
                                                   const TimeSignatureParams& inParams);

        CommandIntfUPtr clipboardPasteItemsCommand(PasteItemsContext inPasteContext);
        CommandIntfUPtr clipboardPasteRangeCommand(PasteRangeParams inPasteParams);

        /// Inserts a Note or Rest and updates the times of subsequent
        /// temporals in the measure
        /// @param inItemID - the temporal to insert
        /// @param inMeasure - the measure into which the temporal will be inserted
        /// @param inDoUpdateItemTimes - true if you want measure->offsetItemTimes to be called
        CommandIntfUPtr insertTemporalCommand(ItemID inItemID,
                                              Measure& inMeasure,
                                              bool inDoUpdateItemTimes = true);
        
        /// Removes a Note or Rest but does NOT update the times of
        /// subsequent temporals in the measure
        ///
        CommandIntfUPtr deleteTemporalCommand(const Temporal& inTemporal);
        
        /// This command is a little different.  To use this command, first save
        /// a memento of your item like this auto memento = item->getMemento()
        /// then make the state changes that you want to make on your item. Then
        /// create the undo/redo command with
        /// factory->itemStateCommand(memento, item) and add it to the
        /// undo stack.  The execute() function will be a no-op in this case.
        ///
        CommandIntfUPtr itemStateCommand(ItemUPtr inBeforeStateChange,
                                         ItemUPtr inAfterStateChange);

        CommandIntfUPtr chordStateCommand(ItemUPtrVec&& inOriginalNoteMementos,
                                          ItemUPtr inChordMemento,
                                          ItemUPtr inChordAlteredState);


        /// Adds a mark to the given temporal
        CommandIntfUPtr insertMarkCommand(ItemID inItemID,
                                          komp::MarkType inMarkType,
                                          komp::PlacementType inPlacementType);
    
        /// Removes the given Mark from it's temporal and from the measure
        CommandIntfUPtr deleteMarkCommand(const Mark& inMark);
        
        /// This top-level command will delegate to other commands as necessary
        /// based on the items it finds in the vector. This function tries to
        /// handle any collection of items properly.
        ///
        CommandIntfUPtr deleteItemsCommand(const ItemRefVec& inItems);
        
        /// deletes a simple item that does not have any additional logic required
        CommandIntfUPtr deleteItemCommand(ItemID inItemID);

        /// Flip the stem direction of the note
        CommandIntfUPtr flipStemCommand(const Note& inNote);

        CommandIntfUPtr insertBeamCommand(const std::vector<ItemID>& inItems,
                                          StemDirection inDirection = StemDirection::UNSPECIFIED);
        CommandIntfUPtr deleteBeamCommand(const BeamIntf& inBeam);

        CommandIntfUPtr insertClefCommand(const Clef& inClef,
                                          Measure& inMeasure);

        CommandIntfUPtr deleteMeasureCommand(size_t inIndex);
        CommandIntfUPtr insertMeasureCommand(size_t inIndex);
        CommandIntfUPtr resizeMeasureCommand(size_t inIndex,
                                             double inOriginalWidth,
                                             double inNewWidth);
        
        CommandIntfUPtr insertSlurCommand(const Temporal& inStart,
                                          const Temporal& inLast);
        CommandIntfUPtr insertTieCommand(const Temporal& inStart,
                                         const Temporal& inSecond);

        CommandIntfUPtr insertWedgeCommand(const SpannerParams& inParams,
                                          WedgeType inWedgeType,
                                          PlacementType inPlacementType);
        
        CommandIntfUPtr deleteCurveCommand(const Curve& inCurve);

        /// Changes the duration of a temporal, attempts to handle beams and chords correctly
        CommandIntfUPtr durationCommand(Measure& inMeasure,
                                        const Temporal& inTemporal,
                                        Duration inDuration);
        
        /// \discussion Adds a beam to a span of notes. If the first note of the span is the
        /// starting point of an existing beam, then a second (or third, etc.) beam will
        /// be added to the span of notes.  If beams are found which start somewhere other
        /// than the first note, those beams will be deleted before proceeding. The
        /// isFractional parameter can only work if two notes are adjacent.  In all cases,
        /// the duration of the notes will be altered by the addition of a beam.
        ///
        CommandIntfUPtr beamStrokeCommand(const BeamStrokeParams& inData);

        /// takes the first note (by x-position) in each track, hand, voice and
        /// sets it to zero, then proceeds rightward by x-position and cascade
        /// each temporal's time based on the duration of the temporal(s) that
        /// came before it.
        CommandIntfUPtr fixTimesCommand(Measure& inMeasure);
        
        /// creates a chord object comprised of the inNotes.  for least surprise, the
        /// inNotes should be of the same track, hand, measure, voice, duration/type
        CommandIntfUPtr insertChordCommand(Measure& inMeasure,
                                           const NoteRefVec& inNotes);
        
        /// creates a chord object comprised of the inNotes.  for least surprise, the
        /// inNotes should be of the same track, hand, measure, voice, duration/type
        /// this one freezes the stem direction as well
        CommandIntfUPtr insertChordCommand(Measure& inMeasure,
                                           const NoteRefVec& inNotes,
                                           StemType inStemDirection);
        
        /// deletes a chord but leaves the unchorded notes in the measure
        CommandIntfUPtr deleteChordCommand(const Chord& inChord,
                                           bool inDoDeleteNotes);
        
        /// insert a disembodied 'raw' accidental
        CommandIntfUPtr insertAccidentalCommand(const MusicLocation& inMusicLocation,
                                               const Point& inScorePoint,
                                               AccidentalType inAccidentalType);

        /// insert a a tuplet, will throw if tuplet durations are not valid
        CommandIntfUPtr insertTuplet(Measure& inMeasure,
                                     const TupletParams& inParams);

        /// removes a tuplet. throws if the item you give is not a tuplet
        CommandIntfUPtr deleteTuplet(Measure& inMeasure,
                                     const TupletIntf& inTuplet);

        /// makes all Notes and Chords into an Eighth if they have longer duration
        CommandIntfUPtr makeDurationsBeamable(const TemporalRefVec& inTemporals);

        /// Changes the barlines (unless the new barlines are equal to the old)
        CommandIntfUPtr barlinesCommand(Measure& inMeasure,
                                       BarlineType inLeftBarline,
                                       BarlineType inRightBarline);

        /// Insert an ending
        CommandIntfUPtr insertEndingCommand(Measure& inMeasure,
                                           const EndingParams& inParams);

        /// Changes the key signature (unless the new key signature is equal to the old)
        /// and updates the pitch of any affected notes.  Stops processing if a future
        /// key signature is encountered
        CommandIntfUPtr keySignatureCommand(Measure& inMeasure,
                                            KeySignatureType inKeySignature);

        /// updates or inserts a tempo mark
        CommandIntfUPtr tempoCommand(Measure& inMeasure,
                                     Track& inTrack,
                                     double inTime,
                                     TempoValue inTempoValue);

        /// adds the note to the existing beam
        CommandIntfUPtr addTemporalToBeam(const BeamIntf& inBeam,
                                         const Temporal& inTemporal);

        /// inserts a chord symbol into the score
        CommandIntfUPtr insertChordSymbolCommand(Measure& inMeasure,
                                                 const ChordSymbolParams& inParams);
        
        /// Adds the arpeggiate to the chord
        CommandIntfUPtr addArpeggiateCommand(const Chord& inChord,
                                             ArpeggiateType inArpeggiate);

        /// Remove the arpeggiate from the chord
        CommandIntfUPtr removeArpeggiateCommand(const Chord& inChord);

        Score& getScore() { return mScore; }

    private:
        Score& mScore;
    };
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
