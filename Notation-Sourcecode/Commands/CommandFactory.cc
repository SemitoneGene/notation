// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "CommandFactory.h"

// Local
#include "AddArpeggiateCommand.h"
#include "AddMarkCommand.h"
#include "AddTemporalToBeamCommand.h"
#include "BarlinesCommand.h"
#include "BeamIntf.h"
#include "BeamStrokeCommand.h"
#include "ChangeTimeSignatureCommand.h"
#include "ChordStateCommand.h"
#include "Clef.h"
#include "ClipboardPasteItemsCommand.h"
#include "ClipboardPasteRangeCommand.h"
#include "Command.h"
#include "Curve.h"
#include "DeleteBeamCommand.h"
#include "DeleteChordCommand.h"
#include "DeleteCurveCommand.h"
#include "DeleteItemCommand.h"
#include "DeleteItemsCommand.h"
#include "DeleteItemsCommand.h"
#include "DeleteMarkCommand.h"
#include "DeleteMarkCommand.h"
#include "DeleteMeasureCommand.h"
#include "DeleteTemporalCommand.h"
#include "DeleteTupletCommand.h"
#include "DurationCommand.h"
#include "FixTimesCommand.h"
#include "FlipStemCommand.h"
#include "InsertAccidentalCommand.h"
#include "InsertBeamCommand.h"
#include "InsertChordCommand.h"
#include "InsertChordSymbolCommand.h"
#include "InsertClefCommand.h"
#include "InsertEndingCommand.h"
#include "InsertMeasureCommand.h"
#include "InsertSlurCommand.h"
#include "InsertTemporalCommand.h"
#include "InsertTieCommand.h"
#include "InsertTupletCommand.h"
#include "InsertWedgeCommand.h"
#include "ItemStateCommand.h"
#include "KeySignatureCommand.h"
#include "MakeDurationsBeamable.h"
#include "ResizeMeasureCommand.h"
#include "TempoCommand.h"
#include "Throw.h"

namespace komp
{
CommandFactory::CommandFactory(Score& inScore)
: mScore{inScore}
{
    
}

CommandIntfUPtr
CommandFactory::genericCommand(std::string inName)
{
    return std::make_unique<Command>(inName, *this, mScore.getItemRegistry());
}

CommandIntfUPtr
CommandFactory::changeTimeSignatureCommand(size_t inIndex,
                                           const TimeSignatureParams& inParams)
{
    return std::make_unique<ChangeTimeSignatureCommand>(*this,
                                                        mScore.getItemRegistry(),
                                                        mScore,
                                                        inIndex,
                                                        inParams);
}

CommandIntfUPtr
CommandFactory::clipboardPasteItemsCommand(PasteItemsContext inPasteContext)
{
    return std::make_unique<ClipboardPasteItemsCommand>(*this,
                                                        mScore.getItemRegistry(),
                                                        mScore,
                                                        std::move(inPasteContext));
}

CommandIntfUPtr
CommandFactory::clipboardPasteRangeCommand(PasteRangeParams inPasteParams)
{
    return std::make_unique<ClipboardPasteRangeCommand>(*this,
                                                        mScore.getItemRegistry(),
                                                        mScore,
                                                        std::move(inPasteParams));
}

CommandIntfUPtr
CommandFactory::insertTemporalCommand(ItemID inItemID,
                                      Measure& inMeasure,
                                      bool inDoUpdateItemTimes)
{
    return std::make_unique<InsertTemporalCommand>(*this,
                                                   mScore.getItemRegistry(),
                                                   inItemID,
                                                   inMeasure,
                                                   inDoUpdateItemTimes);
}

CommandIntfUPtr
CommandFactory::deleteTemporalCommand(const Temporal& inTemporal)
{
    return std::make_unique<DeleteTemporalCommand>(*this,
                                                   mScore.getItemRegistry(),
                                                   inTemporal);
}

CommandIntfUPtr
CommandFactory::itemStateCommand(ItemUPtr inBeforeStateChange,
                                 ItemUPtr inAfterStateChange)
{
    return std::make_unique<ItemStateCommand>(*this,
                                              mScore.getItemRegistry(),
                                              std::move(inBeforeStateChange),
                                              std::move(inAfterStateChange));
}


CommandIntfUPtr
CommandFactory::chordStateCommand(ItemUPtrVec&& inOriginalNoteMementos,
                                  ItemUPtr inChordMemento,
                                  ItemUPtr inChordAlteredState)
{
    return std::make_unique<ChordStateCommand>(*this,
                                               mScore.getItemRegistry(),
                                               std::move(inOriginalNoteMementos),
                                               std::move(inChordMemento),
                                               std::move(inChordAlteredState));
}

CommandIntfUPtr
CommandFactory::insertMarkCommand(ItemID inItemID,
                                  komp::MarkType inMarkType,
                                  komp::PlacementType inPlacementType)
{
    return std::make_unique<AddMarkCommand>(*this,
                                            mScore.getItemRegistry(),
                                            inItemID,
                                            inMarkType,
                                            inPlacementType);
}


CommandIntfUPtr
CommandFactory::deleteMarkCommand(const Mark& inMark)
{
    return std::make_unique<DeleteMarkCommand>(*this,
                                               mScore.getItemRegistry(),
                                               inMark);
}


CommandIntfUPtr
CommandFactory::deleteItemsCommand(const ItemRefVec& inItems)
{
    return std::make_unique<DeleteItemsCommand>(*this,
                                                mScore.getItemRegistry(),
                                                mScore,
                                                inItems);
}


CommandIntfUPtr
CommandFactory::deleteItemCommand(ItemID inItemID)
{
    const auto& item = mScore.getItemRegistry().getItem(inItemID);
    auto& measure = item.getLocation().getMeasure();
    return std::make_unique<DeleteItemCommand>(*this,
                                               mScore.getItemRegistry(),
                                               measure,
                                               inItemID);
}


CommandIntfUPtr
CommandFactory::flipStemCommand(const Note& inNote)
{
    return std::make_unique<FlipStemCommand>(*this,
                                             mScore.getItemRegistry(),
                                             inNote);
}


CommandIntfUPtr
CommandFactory::insertBeamCommand(const std::vector<ItemID>& inItems,
                                  StemDirection inDirection)
{
    return std::make_unique<InsertBeamCommand>(*this,
                                               mScore.getItemRegistry(),
                                               inItems,
                                               inDirection);
}


CommandIntfUPtr
CommandFactory::deleteBeamCommand(const BeamIntf& inBeam)
{
    auto& registry = mScore.getItemRegistry();
    return std::make_unique<DeleteBeamCommand>(*this,
                                               registry,
                                               inBeam);
}


CommandIntfUPtr
CommandFactory::insertClefCommand(const Clef& inClef,
                                  Measure& inMeasure)
{
    return std::make_unique<InsertClefCommand>(*this,
                                               mScore.getItemRegistry(),
                                               inMeasure,
                                               inClef.getItemID());
}

CommandIntfUPtr
CommandFactory::deleteMeasureCommand(size_t inIndex)
{
    return std::make_unique<DeleteMeasureCommand>(*this,
                                                  mScore.getItemRegistry(),
                                                  mScore,
                                                  inIndex);
}

CommandIntfUPtr
CommandFactory::insertMeasureCommand(size_t inIndex)
{
    return std::make_unique<InsertMeasureCommand>(*this,
                                                  mScore.getItemRegistry(),
                                                  mScore,
                                                  inIndex);
}

CommandIntfUPtr
CommandFactory::resizeMeasureCommand(size_t inIndex,
                                     double inOriginalWidth,
                                     double inNewWidth)
{
    return std::make_unique<ResizeMeasureCommand>(*this,
                                                  mScore.getItemRegistry(),
                                                  mScore,
                                                  inIndex,
                                                  inOriginalWidth,
                                                  inNewWidth);
}


CommandIntfUPtr
CommandFactory::insertWedgeCommand(const SpannerParams& inParams,
                                   WedgeType inWedgeType,
                                   PlacementType inPlacementType)
{
    return std::make_unique<InsertWedgeCommand>(*this,
                                                mScore.getItemRegistry(),
                                                inParams,
                                                inWedgeType,
                                                inPlacementType);
}


CommandIntfUPtr
CommandFactory::insertSlurCommand(const Temporal& inStart,
                                  const Temporal& inLast)
{
    return std::make_unique<InsertSlurCommand>(*this,
                                               mScore.getItemRegistry(),
                                               inStart.getItemID(),
                                               inLast.getItemID());
}


CommandIntfUPtr
CommandFactory::insertTieCommand(const Temporal& inStart,
                                 const Temporal& inSecond)
{
    return std::make_unique<InsertTieCommand>(*this,
                                              mScore.getItemRegistry(),
                                              inStart.getItemID(),
                                              inSecond.getItemID());
}


CommandIntfUPtr
CommandFactory::deleteCurveCommand(const Curve& inCurve)
{
    return std::make_unique<DeleteCurveCommand>(*this,
                                                mScore.getItemRegistry(),
                                                inCurve.getItemID());
}


CommandIntfUPtr
CommandFactory::durationCommand(Measure& inMeasure,
                                const Temporal& inTemporal,
                                Duration inDuration)
{
    return std::make_unique<DurationCommand>(*this,
                                             mScore.getItemRegistry(),
                                             inMeasure,
                                             inTemporal.getItemID(),
                                             inDuration);
}


CommandIntfUPtr
CommandFactory::beamStrokeCommand(const BeamStrokeParams& inData)
{
    return std::make_unique<BeamStrokeCommand>(*this,
                                               mScore.getItemRegistry(),
                                               inData);
}


CommandIntfUPtr
CommandFactory::fixTimesCommand(Measure& inMeasure)
{
    return std::make_unique<FixTimesCommand>(*this,
                                             mScore.getItemRegistry(),
                                             inMeasure);
}


CommandIntfUPtr
CommandFactory::insertChordCommand(Measure& inMeasure,
                                   const NoteRefVec& inNotes)
{
    return std::make_unique<InsertChordCommand>(*this,
                                                mScore.getItemRegistry(),
                                                inMeasure,
                                                inNotes);
}


CommandIntfUPtr
CommandFactory::insertChordCommand(Measure& inMeasure,
                                   const NoteRefVec& inNotes,
                                   StemType inStemDirection)
{
    return std::make_unique<InsertChordCommand>(*this,
                                                mScore.getItemRegistry(),
                                                inMeasure,
                                                inNotes,
                                                inStemDirection);
}


CommandIntfUPtr
CommandFactory::deleteChordCommand(const Chord& inChord,
                                   bool inDoDeleteNotes)
{
    return std::make_unique<DeleteChordCommand>(*this,
                                                mScore.getItemRegistry(),
                                                inChord,
                                                inDoDeleteNotes);
}


CommandIntfUPtr
CommandFactory::insertAccidentalCommand(const MusicLocation& inMusicLocation,
                                        const Point& inScorePoint,
                                        AccidentalType inAccidentalType)
{
    return std::make_unique<InsertAccidentalCommand>(*this,
                                                     mScore.getItemRegistry(),
                                                     inMusicLocation,
                                                     inScorePoint,
                                                     inAccidentalType);
}


CommandIntfUPtr
CommandFactory::insertTuplet(Measure& inMeasure,
                             const TupletParams& inParams)
{
    return std::make_unique<InsertTupletCommand>(*this,
                                                 mScore.getItemRegistry(),
                                                 inMeasure,
                                                 inParams);
}


CommandIntfUPtr
CommandFactory::deleteTuplet(Measure& inMeasure, const TupletIntf& inTuplet)
{
    return std::make_unique<DeleteTupletCommand>(*this,
                                                 mScore.getItemRegistry(),
                                                 inMeasure,
                                                 inTuplet);
}


CommandIntfUPtr
CommandFactory::makeDurationsBeamable(const TemporalRefVec& inTemporals)
{
    return std::make_unique<MakeDurationsBeamable>(*this,
                                                   mScore.getItemRegistry(),
                                                   inTemporals);
}


CommandIntfUPtr
CommandFactory::keySignatureCommand(Measure& inMeasure,
                                    KeySignatureType inKeySignature)
{
    return std::make_unique<KeySignatureCommand>(*this,
                                                 mScore.getItemRegistry(),
                                                 mScore,
                                                 inMeasure,
                                                 inKeySignature);
}


CommandIntfUPtr
CommandFactory::barlinesCommand(Measure& inMeasure,
                                BarlineType inLeftBarline,
                                BarlineType inRightBarline)
{
    return std::make_unique<BarlinesCommand>(*this,
                                             mScore.getItemRegistry(),
                                             inMeasure,
                                             inLeftBarline,
                                             inRightBarline);
}


CommandIntfUPtr
CommandFactory::insertEndingCommand(Measure& inMeasure,
                                    const EndingParams& inParams)
{
    return std::make_unique<InsertEndingCommand>(*this,
                                                 mScore.getItemRegistry(),
                                                 inMeasure,
                                                 inParams);
}


CommandIntfUPtr
CommandFactory::tempoCommand(Measure& inMeasure,
                             Track& inTrack,
                             double inTime,
                             TempoValue inTempoValue)
{
    return std::make_unique<TempoCommand>(*this,
                                          mScore.getItemRegistry(),
                                          inMeasure,
                                          inTrack,
                                          inTime,
                                          inTempoValue);
}


CommandIntfUPtr
CommandFactory::addTemporalToBeam(const BeamIntf& inBeam,
                                  const Temporal& inTemporal)
{
    return std::make_unique<AddTemporalToBeamCommand>(*this,
                                                      mScore.getItemRegistry(),
                                                      inBeam.getLocation().getMeasure(),
                                                      inBeam.getItemID(),
                                                      inTemporal.getItemID());
}


CommandIntfUPtr
CommandFactory::insertChordSymbolCommand(Measure& inMeasure,
                                         const ChordSymbolParams& inParams)
{
    return std::make_unique<InsertChordSymbolCommand>(*this,
                                                      mScore.getItemRegistry(),
                                                      inMeasure,
                                                      inParams);
}


CommandIntfUPtr
CommandFactory::addArpeggiateCommand(const Chord& inChord,
                                     ArpeggiateType inArpeggiate)
{
    return std::make_unique<AddArpeggiateCommand>(*this,
                                                  mScore.getItemRegistry(),
                                                  inChord.getItemID(),
                                                  inArpeggiate);
}


CommandIntfUPtr
CommandFactory::removeArpeggiateCommand(const Chord& inChord)
{
    return std::make_unique<RemoveArpeggiateCommand>(*this,
                                                     mScore.getItemRegistry(),
                                                     inChord.getItemID());
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
