// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ClipboardPasteItemsCommand.h"

// Local
#include "BeamIntf.h"
#include "Chord.h"
#include "ChordSymbol.h"
#include "Curve.h"
#include "Curve.h"
#include "ItemRegistry.h"
#include "Mark.h"
#include "Rest.h"
#include "Slur.h"
#include "Temporal.h"
#include "Throw.h"
#include "Tie.h"
#include "TupletIntf.h"
#include "Wedge.h"

namespace komp
{

ClipboardPasteItemsCommand::ClipboardPasteItemsCommand(CommandFactory& inFactory,
                                                       ItemRegistry& inRegistry,
                                                       Score& inScore,
                                                       PasteItemsContext inPasteContext)
: komp::Command("Paste", inFactory, inRegistry)
, mScore{inScore}
, mContext{std::move(inPasteContext)}
, mMarkMemos{}
{
}


CommandResult
ClipboardPasteItemsCommand::executeImpl()
{
    KOMP_LOG_COMMAND
    Data data(mFactory, mRegistry, mContext);
    
    insertTime(data);
    copyItems(data);
    copyChordSymbols(data);
    copySpanners(data);
    
    mScore.layout();
    
    return true;
}


void
ClipboardPasteItemsCommand::undo()
{
    KOMP_LOG_COMMAND
    undoSubCommands();
}


void
ClipboardPasteItemsCommand::redo()
{
    KOMP_LOG_COMMAND
    redoSubCommands();
}

void
ClipboardPasteItemsCommand::insertTime(Data& inData)
{
    // Check and see if there is enough space at the paste location
    // for all of the items
}


void
ClipboardPasteItemsCommand::copyItems(Data& data)
{
    for (Item& item : data.context.items) {

        if (!item.is<Temporal>()) {
            continue;
        }

        auto& temporal = item.as<Temporal>();        
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
        destLoc.setMeasure(*data.measure);
        destLoc.setTrack(data.staff->getTrack());
        destLoc.setHand(data.staff->getHand());
        
        if (data.current.origTemporal->getIsNote()) {
            
            data.current.destTemporal = &mRegistry.createItem<Note>(destLoc,
                                                                    duration.getTemporalType(),
                                                                    komp::NoteheadType::NORMAL,
                                                                    pitch);
        } else if (data.current.origTemporal->getIsRest()) {
            data.current.destTemporal = &mRegistry.createItem<Rest>(destLoc,
                                                                    duration.getTemporalType());
        } else {
            KOMP_THROW("Chord or other unknown temporal type encountered");
        }
        
        data.current.destTemporal->setDuration(duration);
        data.current.destTemporal->setTime(time);
        stripClone(data);
        copyTemporal(data);
    }
    KOMP_ASSERT(data.current.isEmpty());
    
    copyWedges(data);
}

void
ClipboardPasteItemsCommand::stripClone(Data& inData)
{
    Temporal& clone = *inData.current.destTemporal;
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
ClipboardPasteItemsCommand::copyTemporal(Data& inData)
{
    if (inData.current.origTemporal->getIsChord()) {
        return;
    }
    
    parseChord(inData);
    insertClone(inData);
    insertChord(inData);
    parseBeam(inData);
    insertBeam(inData);
    parseTuplet(inData);
    insertTuplet(inData);
    insertMarks();
    enqueueSpanners(inData);
}


void
ClipboardPasteItemsCommand::parseChord(Data& inData)
{
    const auto& orig = inData.current.origTemporal;
    auto& dest = inData.current.destTemporal;
    const auto chordItem = orig->getChord();
    if (!chordItem) {
        KOMP_ASSERT(inData.current.chorded.size() == 0);
        return;
    }
    
    const auto& chord = chordItem->as<Chord>();
    KOMP_ASSERT(orig->getItemTraits().getItemType() == ItemType::Note);
    const auto isFirst = chord.getIsFirst(orig->as<Note>());
    
    if (isFirst) {
        KOMP_ASSERT(inData.current.chorded.size() == 0);
    }
    
    KOMP_ASSERT(dest->getItemTraits().getItemType() == ItemType::Note);
    inData.current.chorded.push_back(&dest->as<Note>());
}


void
ClipboardPasteItemsCommand::parseBeam(Data& inData)
{
    auto orig = inData.current.origTemporal;
    auto dest = inData.current.destTemporal;
    
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
            KOMP_ASSERT(inData.current.beamed.size() == 0);
        }
        return;
    }
    
    const auto isFirst = beam->getIsFirst(*orig);
    if (isFirst) {
        KOMP_ASSERT(inData.current.beamed.size() == 0);
    }
    
    auto iter = std::find_if(inData.current.beamed.cbegin(),
                             inData.current.beamed.cend(),
                             [&](Temporal* temporal) {
        return dest == temporal;
    });
    
    if (iter == inData.current.beamed.cend()) {
        inData.current.beamed.push_back(dest);
    }
}


void
ClipboardPasteItemsCommand::parseTuplet(Data& inData)
{
    auto orig = inData.current.origTemporal;
    auto dest = inData.current.destTemporal;
    
    if (orig->getIsChordMember() &&
        !dest->getIsChordMember()) {
        return;
    }
    
    if (orig->getIsChordMember()) {
        orig = orig->getChord();
        dest = dest->getChord();
    }
    
    if (orig->getTuplets().size() == 0) {
        KOMP_ASSERT(inData.current.tupleted.size() == 0);
        KOMP_ASSERT(inData.current.tuplet == nullptr);
        return;
    }
    
    const auto& tuplets = orig->getTuplets();
    TupletIntf& tuplet = tuplets.front();
    const auto& tupletTemporals = tuplet.getTemporals();
    KOMP_ASSERT(tupletTemporals.size() > 0);
    const auto isFirst = &tupletTemporals.front().get() == orig;
    if (isFirst) {
        KOMP_ASSERT(inData.current.tupleted.size() == 0);
        KOMP_ASSERT(inData.current.tuplet == nullptr);
        inData.current.tupleted.push_back(dest);
        inData.current.tuplet = &tuplet;
    }
    
    auto iter = std::find_if(inData.current.tupleted.cbegin(),
                             inData.current.tupleted.cend(),
                             [&](Temporal* temporal) {
        return dest == temporal;
    });
    
    if (iter == inData.current.tupleted.cend()) {
        inData.current.tupleted.push_back(dest);
    }
}


void
ClipboardPasteItemsCommand::insertClone(Data& inData)
{
    auto temporal = inData.current.destTemporal;
    static constexpr const auto updateItemTimes = false;
    auto command = inData.factory.insertTemporalCommand(temporal->getItemID(),
                                                        *inData.measure,
                                                        updateItemTimes);
    command->execute();
    addSubCommand(std::move(command));
    if (temporal->getItemTraits().getItemType() == ItemType::Note) {
        auto& note = temporal->as<Note>();
        note.deduceSlot();
    }
    temporal->setX(inData.current.origTemporal->getX());
    temporal->setTime(inData.current.origTemporal->getTime());
}


void
ClipboardPasteItemsCommand::insertChord(Data& inData)
{
    const auto& orig = inData.current.origTemporal;
    const auto& chord = orig->getChord();
    
    if (!chord) {
        return;
    }
    
    KOMP_ASSERT(orig->getItemTraits().getItemType() == ItemType::Note);
    const auto isLast = chord->getIsLast(orig->as<Note>());
    if (!isLast) {
        return;
    }
    
    KOMP_ASSERT(inData.current.chorded.size() >= 2);
    auto& measure = inData.current.chorded.front()->getLocation().getMeasure();
    NoteRefVec chorded;
    for (auto chord : inData.current.chorded) {
        chorded.emplace_back(*chord);
    }
    auto insertChordCommand = inData.factory.insertChordCommand(measure,
                                                                chorded);
    insertChordCommand->execute();
    addSubCommand(std::move(insertChordCommand));
    inData.current.chorded.clear();
}


void
ClipboardPasteItemsCommand::insertBeam(Data& inData)
{
    auto orig = inData.current.origTemporal;
    auto dest = inData.current.destTemporal;
    
    if (orig->getIsChordMember() && !dest->getIsChordMember()) {
        return;
    }
    
    if (orig->getIsChordMember()) {
        orig = orig->getChord();
    }
    
    const auto& beam = orig->getBeam();
    
    if (!beam) {
        KOMP_ASSERT(inData.current.beamed.size() == 0);
        return;
    }
    
    const auto isLast = beam->getIsLast(*orig);
    if (!isLast) {
        return;
    }
    
    KOMP_ASSERT(inData.current.beamed.size() >= 2);
    std::vector<ItemID> itemIDS;
    for (const auto item : inData.current.beamed) {
        itemIDS.push_back(item->getItemID());
    }
    auto insertBeamCommand = inData.factory.insertBeamCommand(itemIDS);
    insertBeamCommand->execute();
    addSubCommand(std::move(insertBeamCommand));
    inData.current.beamed.clear();
}


void
ClipboardPasteItemsCommand::insertTuplet(Data& inData)
{
    auto orig = inData.current.origTemporal;
    auto dest = inData.current.destTemporal;
    
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
    
    KOMP_ASSERT(inData.current.tupleted.size() > 0);
    KOMP_ASSERT(inData.current.tuplet);
    TupletParams params;
    params.tupletRatio = inData.current.tuplet->getTupletRatio();
    TemporalRefVec tupleted;
    for (auto tuplet : inData.current.tupleted) {
        tupleted.emplace_back(*tuplet);
    }
    params.setTemporals(tupleted);
    KOMP_ASSERT(params.getIsDurationValid());
    auto insertTupletCommand = inData.factory.insertTuplet(*inData.measure,
                                                           params);
    insertTupletCommand->execute();
    addSubCommand(std::move(insertTupletCommand));
    inData.current.tupleted.clear();
    inData.current.tuplet = nullptr;
}


void
ClipboardPasteItemsCommand::insertMarks()
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
ClipboardPasteItemsCommand::copyChordSymbols(const Data& inData)
{
    //    const auto& chordSymbols = inData.origMeasure->items<ChordSymbol>();
    //    for (const ChordSymbol& chordSymbol : chordSymbols) {
    //
    //        const auto& loc = chordSymbol.getLocation();
    //
    //        ChordSymbolParams params(loc.getTrack());
    //        params.hand = loc.getHand();
    //        params.isSlotSpecified = true;
    //        params.slot = chordSymbol.getSlot();
    //        params.isXDifferentFromTime = !chordSymbol.getIsTimeLocked();
    //
    //        if (params.isXDifferentFromTime) {
    //            params.measureRelativeX = chordSymbol.getX();
    //        }
    //
    //        params.value = chordSymbol.getChordSymbolValue();
    //        params.time = chordSymbol.getTime();
    //        params.voice = loc.getVoice();
    //        auto cmd = mFactory.insertChordSymbolCommand(*inData.measure,
    //                                                     params);
    //        cmd->execute();
    //        addSubCommand(std::move(cmd));
    //    }
}


void
ClipboardPasteItemsCommand::enqueueSpanners(Data& inData)
{
    const auto& attachments = inData.current.origTemporal->getAttachments();
    const auto& curveStarts = attachments.getOutgoingCurves();
    const auto& curveEnds = attachments.getIncomingCurves();
    const Item* searchItem = nullptr;
    
    const auto finder = [&](const SpannerSpec& inSpannerSpec) {
        return inSpannerSpec.spanner == searchItem;
    };
    
    for (const auto& curveStart : curveStarts) {
        searchItem = &curveStart.get();
        const auto iter = std::find_if(inData.spannersFound.begin(),
                                       inData.spannersFound.end(),
                                       finder);
        
        if (iter != inData.spannersFound.end()) {
            iter->originalTemporalStart = inData.current.origTemporal;
            iter->clonedTemporalStart = inData.current.destTemporal;
        } else {
            SpannerSpec newSpec;
            newSpec.spanner = &curveStart.get();
            newSpec.originalTemporalStart = inData.current.origTemporal;
            newSpec.clonedTemporalStart = inData.current.destTemporal;
            inData.spannersFound.push_back(newSpec);
        }
    }
    
    for (const auto& curveEnd : curveEnds) {
        searchItem = &curveEnd.get();
        const auto iter = std::find_if(inData.spannersFound.begin(),
                                       inData.spannersFound.end(),
                                       finder);
        
        if (iter != inData.spannersFound.end()) {
            iter->originalTemporalEnd = inData.current.origTemporal;
            iter->clonedTemporalEnd = inData.current.destTemporal;
        } else {
            SpannerSpec newSpec;
            newSpec.spanner = &curveEnd.get();
            newSpec.originalTemporalEnd = inData.current.origTemporal;
            newSpec.clonedTemporalEnd = inData.current.destTemporal;
            inData.spannersFound.push_back(newSpec);
        }
    }
}


void
ClipboardPasteItemsCommand::copySpanners(const Data& inData)
{
    for (auto& spanner : inData.spannersFound) {
        
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
    std::copy_if(inData.spannersFound.begin(),
                 inData.spannersFound.end(),
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
ClipboardPasteItemsCommand::copyWedges(const Data& inData)
{
    //    const auto filter = [&](Item& inItem) {
    //
    //        if (&inItem.getLocation().getStaff() != inData.origStaff) {
    //            return false;
    //        }
    //
    //        if (!inItem.getItemTraits().getItemType() == ItemType::Wedge) {
    //            return false;
    //        }
    //
    //        const auto wedge = inItem.as<Wedge>();
    //        const auto wedgeEndMeasureIndex = wedge.getEndMeasure()->getIndex();
    //        const auto wedgeBeginMeasureIndex = wedge.getLocation().getMeasure().getIndex();
    //        const auto wedgeMeasureSpan = wedgeEndMeasureIndex - wedgeBeginMeasureIndex;
    //        const auto originEndMeasureIndex = mParams.origin.endMeasure.get().getIndex();
    //
    //        if (wedgeEndMeasureIndex > originEndMeasureIndex) {
    //            return false;
    //        }
    //
    //        // check to see if the ending measure exists
    //        const auto destMeasureIndex = inData.destMeasure->getIndex();
    //        const auto lastMeasureIndex = mScore.getMeasureCount() - 1;
    //        const auto wedgeDesiredEndIndex = destMeasureIndex + wedgeMeasureSpan;
    //
    //        if (wedgeDesiredEndIndex > lastMeasureIndex) {
    //            return false;
    //        }
    //
    //        return true;
    //    };
    //
    //    const auto wedges = inData.origMeasure->items<Wedge>(inData.origStaff->getTrack(),
    //                                                       filter);
    //    for (const Wedge& wedge : wedges) {
    //        const auto wedgeBeginMeasureIndex = wedge.getLocation().getMeasure().getIndex();
    //        const auto wedgeMeasureSpan = wedge.getEndMeasure()->getIndex() - wedgeBeginMeasureIndex;
    //        const auto wedgeDesiredEndIndex = inData.destMeasure->getIndex() + wedgeMeasureSpan;
    //        const auto& wedgeEndMeasure = mScore.getMeasure(wedgeDesiredEndIndex);
    //        WedgeParams wparams;
    //        wparams.spannerParams.startMeasure = inData.destMeasure;
    //        wparams.spannerParams.endMeasure = wedgeEndMeasure.get();
    //        wparams.spannerParams.startTime = wedge.getTime();
    //        wparams.spannerParams.endTime = wedge.getEndTime();
    //        wparams.spannerParams.hand = inData.destStaff->getHand();
    //        wparams.placementType = PlacementType::AUTOMATIC;
    //        wparams.wedgeType = wedge.getWedgeType();
    //        auto cmd = mFactory.insertWedgeCommand(wparams.spannerParams,
    //                                               wparams.wedgeType,
    //                                               wparams.placementType);
    //        const auto cmdResult = cmd->execute();
    //        addSubCommand(std::move(cmd));
    //        KOMP_ASSERT(cmdResult.getInsertedItems().size() == 1);
    //        auto& insertedWedge = mRegistry.getItem(cmdResult.getInsertedItems().front());
    //        insertedWedge.setSlot(wedge.getSlot());
    //    }
}

void
ClipboardPasteItemsCommand::setTime(const Data& inData,
                                    const TemporalRefVec& inTemporals)
{
    const auto temporal = inData.current.destTemporal;

    // Find the first note with an x greater than or equal to my note's x
    auto findIter = inTemporals.cbegin();
    const auto temporalX = temporal->getX();
    for (; findIter != inTemporals.cend(); ++findIter) {
        const Temporal& found = *findIter;
        if (found.getX() >= temporalX) {
            break;
        }
    }
    
    if (findIter != inTemporals.end()) {
        const Temporal& found = *findIter;
        temporal->setTime(found.getTime());
    } else {
        if (!inTemporals.empty()) {
            const Temporal& lastTemporal = inTemporals.back();
            const auto lastTemporalDuration = lastTemporal.getDuration().getClocks();
            const auto lastTemporalTime = lastTemporal.getTime();
            temporal->setTime(lastTemporalTime + lastTemporalDuration);
        } else {
            temporal->setTime(0.0);
        }
    }
    
    auto laterNotesFilter = [&](komp::Item& inItem) {
        
        const auto& itemLoc = inItem.getLocation();
        const auto& temporalLoc = temporal->getLocation();
        
        if (itemLoc.getHand() != temporalLoc.getHand()) {
            return false;
        }
        
        if (itemLoc.getVoice() != temporalLoc.getVoice()) {
            return false;
        }
        
        if (&itemLoc.getTrack() != &temporalLoc.getTrack()) {
            return false;
        }
        
        if (inItem.getX() < temporal->getX()) {
            return false;
        }
        
        if (!inItem.is<komp::Temporal>()) {
            return false;
        }
        
        const auto& inTemporal = inItem.as<komp::Temporal>();
        if (&inTemporal == temporal) {
            return false;
        }
        
        if (inTemporal.getIsChordMember()) {
            return false;
        }
        
        return true;
    };
    
    const auto& temporalLoc = temporal->getLocation();
    auto temporalsToPush = temporalLoc.getMeasure().items<komp::Temporal>(temporalLoc.getTrack(),
                                                                          laterNotesFilter);
    const auto amount = temporal->getDuration().getClocks();
    
    for (komp::Temporal& temporal : temporalsToPush) {
        const auto currentTime = temporal.getTime();
        auto timeStateCommand = mFactory.itemStateCommand(temporal.getMemento(),
                                                          temporal.getMemento());
        addSubCommand(std::move(timeStateCommand));
        const auto newTime = currentTime + amount;
        temporal.setTime(newTime);
    }
}

void
ClipboardPasteItemsCommand::fixTimes(const Data& inData)
{
    auto fixTimes = mFactory.fixTimesCommand(*inData.measure);
    fixTimes->execute();
    addSubCommand(std::move(fixTimes));
}


} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

