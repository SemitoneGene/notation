// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ClipboardPasteRangeCommand.h"

// Local
#include "ItemRegistry.h"
#include "BeamIntf.h"
#include "Mark.h"
#include "Curve.h"
#include "Temporal.h"
#include "Chord.h"
#include "Rest.h"
#include "Throw.h"
#include "TupletIntf.h"
#include "ChordSymbol.h"
#include "Curve.h"
#include "Tie.h"
#include "Slur.h"
#include "Wedge.h"

namespace komp
{

ClipboardPasteRangeCommand::ClipboardPasteRangeCommand(CommandFactory& inFactory,
                                                       ItemRegistry& inRegistry,
                                                       Score& inScore,
                                                       PasteRangeParams inPasteParams)
: komp::Command("Paste", inFactory, inRegistry)
, mScore{inScore}
, mParams{std::move(inPasteParams)}
, mMarkMemos{}
{
    //        KOMP_ASSERT(mParams.origin.startMeasure.get().getIndex()
    //                 <= mParams.origin.endMeasure.get().getIndex());
    //        KOMP_ASSERT(mParams.origin.startStaff.get().getIndex()
    //                 <= mParams.origin.endStaff.get().getIndex());
}


CommandResult
ClipboardPasteRangeCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    Data data{mFactory, mRegistry, mParams};
    data.isTopStaffSelected = mParams.origin.startStaff.get().getIndex() == 0;
    data.isTopStaffInDestination = mParams.destination.startStaff.get().getIndex() == 0;
    
    // Iterate over each measure in the origin, so long as a measure exists
    // in the destination to receive the contents
    auto orig = data.params.origin.startMeasure.get().iter();
    const auto lastOrig = data.params.origin.endMeasure.get().iter();
    auto dest = data.params.destination.startMeasure.get().iter();
    const auto measuresEnd = mScore.getMeasuresEnd();
    for ( ;
         orig <= lastOrig &&
         orig != measuresEnd &&
         dest != measuresEnd;
         ++orig, ++dest) {
        
        resetData((*orig).get(), (*dest).get(), data);
        copyStaves(data);
    }
    
    copyChordSymbols(data);
    copySpanners(data);
    
    return true;
}


void
ClipboardPasteRangeCommand::undo()
{
    KOMP_LOG_COMMAND
    undoSubCommands();
}


void
ClipboardPasteRangeCommand::redo()
{
    KOMP_LOG_COMMAND
    redoSubCommands();
}


void
ClipboardPasteRangeCommand::resetData(Measure* inOrigMeasure,
                                      Measure* inDestMeasure,
                                      Data& data)
{
    data.origMeasure = inOrigMeasure;
    data.destMeasure = inDestMeasure;
    data.origStaff = nullptr;
    data.destStaff = nullptr;
    data.voice = 0;
}


void
ClipboardPasteRangeCommand::copyStaves(Data& data)
{
    // Iterate over each staff in the origin, so long as a staff exists
    // in the destination to receive the contents
    auto sourceStaffIndex = data.params.origin.startStaff.get().getIndex();
    const auto sourceEndStaffIndex = data.params.origin.endStaff.get().getIndex();
    auto destStaffIndex = data.params.destination.startStaff.get().getIndex();
    const auto destEndStaffIndex = data.params.destination.endStaff.get().getIndex();
    for ( ;
         sourceStaffIndex <= sourceEndStaffIndex && destStaffIndex <= destEndStaffIndex;
         ++sourceStaffIndex, ++destStaffIndex) {
        data.origStaff = data.params.origin.startStaff.get().getStaffSystem()->getStaves().at(sourceStaffIndex).get();
        data.destStaff = data.params.destination.startStaff.get().getStaffSystem()->getStaves().at(destStaffIndex).get();
        deleteItems(data);
        
        for (data.voice = 0; data.voice < MAX_VOICES; ++data.voice) {
            copyItems(data);
        }
    }
}


void
ClipboardPasteRangeCommand::deleteItems(Data& data)
{
    const auto& measure = data.destMeasure;
    auto& track = data.destStaff->getTrack();
    const auto& staff = data.destStaff;
    
    auto filter = [&](Item& inItem) {
        
        const auto& loc = inItem.getLocation();
        if (&loc.getTrack() != &track) {
            return false;
        }
        
        if (&loc.getStaff() != staff) {
            return false;
        }
        
        if (loc.getHand() != staff->getHand()) {
            return false;
        }
        
        // Only delete chord symbols if both the origin
        // and destination involve the top staff of the score
        
        if (!data.isTopStaffInDestination || !data.isTopStaffSelected) {
            if (inItem.getItemTraits().getItemType() == ItemType::ChordSymbol) {
                return false;
            }
        }
        
        return true;
    };
    
    auto items = measure->items<Item>(track, filter);
    ItemRefVec toDelete;
    for (const auto& item : items) {
        toDelete.emplace_back(item);
    }
    
    if (!toDelete.empty()) {
        auto deleteItemsCommand = data.factory.deleteItemsCommand(toDelete);
        deleteItemsCommand->execute();
        addSubCommand(std::move(deleteItemsCommand));
    }
}


void
ClipboardPasteRangeCommand::copyItems(Data& data)
{
    const auto temporals = getTemporals(data);
    for (Temporal& temporal : temporals) {
        if (temporal.getIsChord()) {
            continue;
        }
        
        const auto& loc = temporal.getLocation();
        data.current.origTemporal = &temporal;
        
        const auto time = data.current.origTemporal->getTime();
        const auto pitch = data.current.origTemporal->getPitch();
        auto duration = data.current.origTemporal->getDuration();
        
        while(!duration.getTupletRatios().empty()) {
            duration.removeTupletRatio( duration.getTupletRatios().back());
        }
        
        // TODO: MusicLocation has to be set to the destination measure
        // before the item is inserted. This is a design flaw.
        MusicLocation destLoc = loc;
        destLoc.setMeasure(*data.destMeasure);
        destLoc.setTrack(data.destStaff->getTrack());
        destLoc.setHand(data.destStaff->getHand());
        
        if (data.current.origTemporal->getIsNote()) {
            
            data.current.destTemporal = &mRegistry.createItem<Note>(destLoc,
                                                                    duration.getTemporalType(),
                                                                    komp::NoteheadType::NORMAL,
                                                                    pitch);
        } else if (data.current.origTemporal->getIsRest()) {
            data.current.destTemporal = &mRegistry.createItem<Rest>(destLoc,
                                                                    duration.getTemporalType());
        } else {
            KOMP_THROW("chord or other unknown temporal type encountered");
        }
        
        data.current.destTemporal->setDuration(duration);
        data.current.destTemporal->setTime(time);
        stripClone(data);
        copyTemporal(data);
    }
    KOMP_ASSERT(data.current.isEmpty());
    
    copyWedges(data);
}


TemporalRefVec
ClipboardPasteRangeCommand::getTemporals(Data& data)
{
    const auto& measure = data.origMeasure;
    auto& track = data.origStaff->getTrack();
    const auto& staff = data.origStaff;
    
    const auto filter = [&](Item& inItem) {
        
        const auto& loc = inItem.getLocation();
        
        if (loc.getVoice() != data.voice) {
            return false;
        }
        
        if (&loc.getStaff() != staff) {
            return false;
        }
        
        if (&loc.getTrack() != &track) {
            return false;
        }
        
        if (loc.getHand() != staff->getHand()) {
            return false;
        }
        
        return true;
    };
    
    auto temporals = measure->items<Temporal>(track, filter);
    komp::sortItems(temporals);
    return temporals;
}


void
ClipboardPasteRangeCommand::stripClone(Data& data)
{
    Temporal& clone = *data.current.destTemporal;
    const auto& beam = clone.getBeam();
    
    if (beam) {
        clone.clearBeam();
        clone.unregisterObserver(*beam);
    }
    
    const auto tuplets = clone.getTuplets();
    for (TupletIntf& tuplet : tuplets) {
        clone.removeTuplet(tuplet);
        clone.unregisterObserver(tuplet);
    }
    
    const auto chord = clone.getChord();
    if (chord) {
        clone.clearChord();
        clone.unregisterObserver(*chord);
    }
    
    const auto marks = clone.getAttachments().getMarks();
    for (Mark& mark : marks) {
        
        const auto& data = mark.getMarkData();
        const auto& point = mark.getRelativePoint();
        
        const MarkMemo markMemo(clone,
                                data.getMarkType(),
                                point.getX(),
                                point.getYSlots(),
                                data.getPlacementType());
        mMarkMemos.emplace_back(std::move(markMemo));
        clone.getAttachments().removeMark(mark);
    }
    
    const auto icurves = clone.getAttachments().getIncomingCurves();
    for (const auto& curve : icurves) {
        clone.getAttachments().removeIncomingCurve(curve);
    }
    
    const auto ocurves = clone.getAttachments().getOutgoingCurves();
    for (const auto& curve : ocurves) {
        clone.getAttachments().removeOutgoingCurve(curve);
    }
}


void
ClipboardPasteRangeCommand::copyTemporal(Data& data)
{
    if (data.current.origTemporal->getIsChord()) {
        return;
    }
    
    parseChord(data);
    insertClone(data);
    insertChord(data);
    parseBeam(data);
    insertBeam(data);
    parseTuplet(data);
    insertTuplet(data);
    insertMarks();
    enqueueSpanners(data);
}


void
ClipboardPasteRangeCommand::parseChord(Data& data)
{
    const auto& orig = data.current.origTemporal;
    auto& dest = data.current.destTemporal;
    const auto chordItem = orig->getChord();
    if (!chordItem) {
        KOMP_ASSERT(data.current.chorded.size() == 0);
        return;
    }
    
    const auto& chord = chordItem->as<Chord>();
    KOMP_ASSERT(orig->getItemTraits().getItemType() == ItemType::Note);
    const auto isFirst = chord.getIsFirst(orig->as<Note>());
    
    if (isFirst) {
        KOMP_ASSERT(data.current.chorded.size() == 0);
    }
    
    KOMP_ASSERT(dest->getItemTraits().getItemType() == ItemType::Note);
    data.current.chorded.push_back(&dest->as<Note>());
}


void
ClipboardPasteRangeCommand::parseBeam(Data& data)
{
    auto orig = data.current.origTemporal;
    auto dest = data.current.destTemporal;
    
    if (orig->getIsChordMember() &&
        !dest->getIsChordMember()) {
        return;
    }
    
    if (orig->getIsChordMember()) {
        orig = orig->getChord();
        dest = dest->getChord();
    }
    
    const auto& beam = orig->getBeam();
    
    if (!beam) {
        // TODO: This is hackish since rests are not properly participating
        // in beams but some imported files might have rests in the middle of
        // a beam.  In the future remove 'if (!orig->getIsRest())'
        if (!orig->getIsRest()) {
            KOMP_ASSERT(data.current.beamed.size() == 0);
        }
        return;
    }
    
    const auto isFirst = beam->getIsFirst(*orig);
    if (isFirst) {
        KOMP_ASSERT(data.current.beamed.size() == 0);
    }
    
    auto iter = std::find_if(data.current.beamed.cbegin(),
                             data.current.beamed.cend(),
                             [&](Temporal* temporal) {
        return dest == temporal;
    });
    
    if (iter == data.current.beamed.cend()) {
        data.current.beamed.push_back(dest);
    }
}


void
ClipboardPasteRangeCommand::parseTuplet(Data& data)
{
    auto orig = data.current.origTemporal;
    auto dest = data.current.destTemporal;
    
    if (orig->getIsChordMember() &&
        !dest->getIsChordMember()) {
        return;
    }
    
    if (orig->getIsChordMember()) {
        orig = orig->getChord();
        dest = dest->getChord();
    }
    
    if (orig->getTuplets().size() == 0) {
        KOMP_ASSERT(data.current.tupleted.size() == 0);
        KOMP_ASSERT(data.current.tuplet == nullptr);
        return;
    }
    
    const auto& tuplets = orig->getTuplets();
    TupletIntf& tuplet = tuplets.front();
    const auto& tupletTemporals = tuplet.getTemporals();
    KOMP_ASSERT(tupletTemporals.size() > 0);
    const auto isFirst = &tupletTemporals.front().get() == orig;
    if (isFirst) {
        KOMP_ASSERT(data.current.tupleted.size() == 0);
        KOMP_ASSERT(data.current.tuplet == nullptr);
        data.current.tupleted.push_back(dest);
        data.current.tuplet = &tuplet;
    }
    
    auto iter = std::find_if(data.current.tupleted.cbegin(),
                             data.current.tupleted.cend(),
                             [&](Temporal* temporal) {
        return dest == temporal;
    });
    
    if (iter == data.current.tupleted.cend()) {
        data.current.tupleted.push_back(dest);
    }
}


void
ClipboardPasteRangeCommand::insertClone(Data& data)
{
    auto temporal = data.current.destTemporal;
    const auto updateItemTimes = false;
    auto command = data.factory.insertTemporalCommand(temporal->getItemID(),
                                                      *data.destMeasure,
                                                      updateItemTimes);
    command->execute();
    addSubCommand(std::move(command));
    if (temporal->getItemTraits().getItemType() == ItemType::Note) {
        auto& note = temporal->as<Note>();
        note.deduceSlot();
    }
    temporal->setX(data.current.origTemporal->getX());
    temporal->setTime(data.current.origTemporal->getTime());
}


void
ClipboardPasteRangeCommand::insertChord(Data& data)
{
    const auto& orig = data.current.origTemporal;
    const auto& chordItem = orig->getChord();
    
    if (!chordItem) {
        return;
    }
    
    const auto& chord = chordItem->as<Chord>();
    KOMP_ASSERT(orig->getItemTraits().getItemType() == ItemType::Note);
    const auto isLast = chord.getIsLast(orig->as<Note>());
    if (!isLast) {
        return;
    }
    
    KOMP_ASSERT(data.current.chorded.size() >= 2);
    auto& measure = data.current.chorded.front()->getLocation().getMeasure();
    NoteRefVec chorded;
    for (auto chord : data.current.chorded) {
        chorded.emplace_back(*chord);
    }
    auto insertChordCommand = data.factory.insertChordCommand(measure,
                                                              chorded);
    insertChordCommand->execute();
    addSubCommand(std::move(insertChordCommand));
    data.current.chorded.clear();
}


void
ClipboardPasteRangeCommand::insertBeam(Data& data)
{
    auto orig = data.current.origTemporal;
    auto dest = data.current.destTemporal;
    
    if (orig->getIsChordMember() && !dest->getIsChordMember()) {
        return;
    }
    
    if (orig->getIsChordMember()) {
        orig = orig->getChord();
    }
    
    const auto& beam = orig->getBeam();
    
    if (!beam) {
        KOMP_ASSERT(data.current.beamed.size() == 0);
        return;
    }
    
    const auto isLast = beam->getIsLast(*orig);
    if (!isLast) {
        return;
    }
    
    KOMP_ASSERT(data.current.beamed.size() >= 2);
    std::vector<ItemID> itemIDS;
    for (const auto item : data.current.beamed) {
        itemIDS.push_back(item->getItemID());
    }
    auto insertBeamCommand = data.factory.insertBeamCommand(itemIDS);
    insertBeamCommand->execute();
    addSubCommand(std::move(insertBeamCommand));
    data.current.beamed.clear();
}


void
ClipboardPasteRangeCommand::insertTuplet(Data& data)
{
    auto orig = data.current.origTemporal;
    auto dest = data.current.destTemporal;
    
    if (orig->getIsChordMember() && !dest->getIsChordMember()) {
        return;
    }
    
    if (orig->getIsChordMember()) {
        orig = orig->getChord();
    }
    
    if (orig->getTuplets().size() == 0) {
        return;
    }
    
    const auto& tuplets = orig->getTuplets();
    const TupletIntf& tuplet = tuplets.front();
    const auto& tupletTemporals = tuplet.getTemporals();
    KOMP_ASSERT(tupletTemporals.size() > 0);
    const bool isLast = &tupletTemporals.back().get() == orig;
    
    if (!isLast) {
        return;
    }
    
    KOMP_ASSERT(data.current.tupleted.size() > 0);
    KOMP_ASSERT(data.current.tuplet);
    TupletParams params;
    params.tupletRatio = data.current.tuplet->getTupletRatio();
    TemporalRefVec tupleted;
    for (auto tuplet : data.current.tupleted) {
        tupleted.emplace_back(*tuplet);
    }
    params.setTemporals(tupleted);
    KOMP_ASSERT(params.getIsDurationValid());
    auto insertTupletCommand = data.factory.insertTuplet(*data.destMeasure,
                                                         params);
    insertTupletCommand->execute();
    addSubCommand(std::move(insertTupletCommand));
    data.current.tupleted.clear();
    data.current.tuplet = nullptr;
}


void
ClipboardPasteRangeCommand::insertMarks()
{
    for (const auto& markMemo : mMarkMemos) {
        auto& attachments = markMemo.clone.getAttachments();
        auto& mark = attachments.addMark(mRegistry,
                                         markMemo.clone,
                                         markMemo.markType,
                                         markMemo.placementType);
        auto& point = mark.getRelativePoint();
        point.setX(markMemo.temporalRelativeX);
        point.setYSlots(markMemo.temporalRelativeSlots);
    }
    
    mMarkMemos.clear();
}


void
ClipboardPasteRangeCommand::copyChordSymbols(const Data& data)
{
    if (!data.isTopStaffInDestination || !data.isTopStaffSelected) {
        return;
    }
    
    const auto& chordSymbols = data.origMeasure->items<ChordSymbol>();
    for (const ChordSymbol& chordSymbol : chordSymbols) {
        
        const auto& loc = chordSymbol.getLocation();
        
        ChordSymbolParams params(loc.getTrack());
        params.hand = loc.getHand();
        params.isSlotSpecified = true;
        params.slot = chordSymbol.getSlot();
        params.isXDifferentFromTime = !chordSymbol.getIsTimeLocked();
        
        if (params.isXDifferentFromTime) {
            params.measureRelativeX = chordSymbol.getX();
        }
        
        params.value = chordSymbol.getChordSymbolValue();
        params.time = chordSymbol.getTime();
        params.voice = loc.getVoice();
        auto cmd = mFactory.insertChordSymbolCommand(*data.destMeasure,
                                                     params);
        cmd->execute();
        addSubCommand(std::move(cmd));
    }
}


void
ClipboardPasteRangeCommand::enqueueSpanners(Data& data)
{
    const auto& attachments = data.current.origTemporal->getAttachments();
    const auto& curveStarts = attachments.getOutgoingCurves();
    const auto& curveEnds = attachments.getIncomingCurves();
    const Item* searchItem = nullptr;
    
    const auto finder = [&](const SpannerSpec& inSpannerSpec) {
        return inSpannerSpec.spanner == searchItem;
    };
    
    for (const auto& curveStart : curveStarts) {
        searchItem = &curveStart.get();
        const auto iter = std::find_if(data.spannersFound.begin(), data.spannersFound.end(), finder);
        
        if (iter != data.spannersFound.end()) {
            iter->originalTemporalStart = data.current.origTemporal;
            iter->clonedTemporalStart = data.current.destTemporal;
        } else {
            SpannerSpec newSpec;
            newSpec.spanner = &curveStart.get();
            newSpec.originalTemporalStart = data.current.origTemporal;
            newSpec.clonedTemporalStart = data.current.destTemporal;
            data.spannersFound.push_back(newSpec);
        }
    }
    
    for (const auto& curveEnd : curveEnds) {
        searchItem = &curveEnd.get();
        const auto iter = std::find_if(data.spannersFound.begin(), data.spannersFound.end(), finder);
        
        if (iter != data.spannersFound.end()) {
            iter->originalTemporalEnd = data.current.origTemporal;
            iter->clonedTemporalEnd = data.current.destTemporal;
        } else {
            SpannerSpec newSpec;
            newSpec.spanner = &curveEnd.get();
            newSpec.originalTemporalEnd = data.current.origTemporal;
            newSpec.clonedTemporalEnd = data.current.destTemporal;
            data.spannersFound.push_back(newSpec);
        }
    }
}


void
ClipboardPasteRangeCommand::copySpanners(const Data& data)
{
    for (auto& spanner : data.spannersFound) {
        
        if (!spanner.clonedTemporalStart || !spanner.clonedTemporalEnd) {
            continue;
        }
        
        if (spanner.spanner->is<Curve>()) {
            if (spanner.spanner->getItemTraits().getItemType() == ItemType::Tie) {
                auto tieCmd = mFactory.insertTieCommand(*spanner.clonedTemporalStart,
                                                        *spanner.clonedTemporalEnd);
                tieCmd->execute();
                this->addSubCommand(std::move(tieCmd));
            } else if (spanner.spanner->getItemTraits().getItemType() == ItemType::Slur) {
                auto slurCmd = mFactory.insertSlurCommand(*spanner.clonedTemporalStart,
                                                          *spanner.clonedTemporalEnd);
                slurCmd->execute();
                this->addSubCommand(std::move(slurCmd));
            }
        }
    }
    
    const auto findTiesThatStartOnTheLastNoteOfAMeasure = [&](const SpannerSpec& inSpec) {
        if (inSpec.spanner->getItemTraits().getItemType() != ItemType::Tie) {
            return false;
        }
        
        if (inSpec.clonedTemporalEnd) {
            return false;
        }
        
        const auto& orig = inSpec.originalTemporalStart->getLocation();
        const auto& cloned = inSpec.clonedTemporalStart->getLocation();
        
        const auto startTime = cloned.getTime();
        const auto voice = orig.getVoice();
        auto& track = orig.getTrack();
        const auto hand = orig.getHand();
        
        const auto additionalNotesFinder = [&](Item& inItem) {
            
            const auto& loc = inItem.getLocation();
            
            if (&loc.getTrack() != &track) {
                return false;
            }
            
            if (loc.getVoice() != voice) {
                return false;
            }
            
            if (loc.getHand() != hand) {
                return false;
            }
            
            if (loc.getTime() <= startTime) {
                return false;
            }
            
            return true;
        };
        
        const auto additionalTemporals = cloned.getMeasure().items<Temporal>(track,
                                                                             additionalNotesFinder);
        if (additionalTemporals.size() > 0) {
            return false;
        }
        
        return true;
    };
    
    std::vector<SpannerSpec> lastNotesWithTies;
    std::copy_if(data.spannersFound.begin(),
                 data.spannersFound.end(),
                 std::back_inserter(lastNotesWithTies),
                 findTiesThatStartOnTheLastNoteOfAMeasure);
    
    for (const auto& tieSpanner : lastNotesWithTies) {
        
        const auto& tieLoc = tieSpanner.clonedTemporalStart->getLocation();
        
        const auto slot = tieLoc.getSlot();
        const auto& measure = tieLoc.getMeasure();
        const auto endMeasureIndex = measure.getIndex() + 1;
        const auto& score = mScore;
        
        if (endMeasureIndex > score.getMeasureCount() - 1) {
            continue;
        }
        
        const auto& endMeasure = score.getMeasure(endMeasureIndex);
        
        const auto findEligibleEndNotes = [&] (Item& inItem) {
            
            const auto& loc = inItem.getLocation();
            
            if (loc.getTime() != 0) {
                return false;
            }
            
            if (loc.getSlot() != slot) {
                return false;
            }
            
            if (&loc.getTrack() != &tieLoc.getTrack()) {
                return false;
            }
            
            if (loc.getHand() != tieLoc.getHand()) {
                return false;
            }
            
            if (loc.getVoice() != tieLoc.getVoice()) {
                return false;
            }
            
            return true;
        };
        
        const auto eligibleNotes = endMeasure->items<Note>(tieLoc.getTrack(),
                                                           findEligibleEndNotes);
        
        if (eligibleNotes.empty()) {
            continue;
        }
        
        const auto& endNote = eligibleNotes.front();
        auto insertTieCmd = mFactory.insertTieCommand(*tieSpanner.clonedTemporalStart,
                                                      endNote);
        insertTieCmd->execute();
        addSubCommand(std::move(insertTieCmd));
    }
}


void
ClipboardPasteRangeCommand::copyWedges(const Data& data)
{
    const auto filter = [&](Item& inItem) {
        
        if (&inItem.getLocation().getStaff() != data.origStaff) {
            return false;
        }
        
        if (!inItem.is<Wedge>()) {
            return false;
        }
        
        const auto wedge = inItem.as<Wedge>();
        const auto wedgeEndMeasureIndex = wedge.getEndMeasure()->getIndex();
        const auto wedgeBeginMeasureIndex = wedge.getLocation().getMeasure().getIndex();
        const auto wedgeMeasureSpan = wedgeEndMeasureIndex - wedgeBeginMeasureIndex;
        const auto originEndMeasureIndex = mParams.origin.endMeasure.get().getIndex();
        
        if (wedgeEndMeasureIndex > originEndMeasureIndex) {
            return false;
        }
        
        // check to see if the ending measure exists
        const auto destMeasureIndex = data.destMeasure->getIndex();
        const auto lastMeasureIndex = mScore.getMeasureCount() - 1;
        const auto wedgeDesiredEndIndex = destMeasureIndex + wedgeMeasureSpan;
        
        if (wedgeDesiredEndIndex > lastMeasureIndex) {
            return false;
        }
        
        return true;
    };
    
    const auto wedges = data.origMeasure->items<Wedge>(data.origStaff->getTrack(),
                                                       filter);
    for (const Wedge& wedge : wedges) {
        const auto wedgeBeginMeasureIndex = wedge.getLocation().getMeasure().getIndex();
        const auto wedgeMeasureSpan = wedge.getEndMeasure()->getIndex() - wedgeBeginMeasureIndex;
        const auto wedgeDesiredEndIndex = data.destMeasure->getIndex() + wedgeMeasureSpan;
        const auto& wedgeEndMeasure = mScore.getMeasure(wedgeDesiredEndIndex);
        WedgeParams wparams;
        wparams.spannerParams.startMeasure = data.destMeasure;
        wparams.spannerParams.endMeasure = wedgeEndMeasure.get();
        wparams.spannerParams.startTime = wedge.getTime();
        wparams.spannerParams.endTime = wedge.getEndTime();
        wparams.spannerParams.hand = data.destStaff->getHand();
        wparams.placementType = PlacementType::AUTOMATIC;
        wparams.wedgeType = wedge.getWedgeType();
        auto cmd = mFactory.insertWedgeCommand(wparams.spannerParams,
                                               wparams.wedgeType,
                                               wparams.placementType);
        const auto cmdResult = cmd->execute();
        addSubCommand(std::move(cmd));
        KOMP_ASSERT(cmdResult.getInsertedItems().size() == 1);
        auto& insertedWedge = mRegistry.getItem(cmdResult.getInsertedItems().front());
        insertedWedge.setSlot(wedge.getSlot());
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

