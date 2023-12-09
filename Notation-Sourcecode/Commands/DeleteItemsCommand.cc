// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DeleteItemsCommand.h"

// Local
#include "BeamIntf.h"
#include "Chord.h"
#include "Curve.h"
#include "FixTimesCommand.h"
#include "ItemRegistry.h"
#include "Mark.h"
#include "MeasureLocation.h"
#include "Note.h"
#include "Score.h"
#include "Temporal.h"
#include "Throw.h"
#include "TupletIntf.h"

namespace komp
{
DeleteItemsCommand::DeleteItemsCommand(CommandFactory& inFactory,
                                       ItemRegistry& inRegistry,
                                       Score& inScore,
                                       const ItemRefVec& inItems)
: komp::Command("Delete Items", inFactory, inRegistry)
, mScore(inScore)
, mItems()
{
    for (const Item& i : inItems) {
        mItems.emplace_back(i.getItemID(), false);
    }
}


CommandResult
DeleteItemsCommand::executeImpl()
{
    auto items = getItems();
    using MeasureVoice = std::pair<MeasureLocation, int>;
    auto compare = [&](const MeasureVoice& lhs, const MeasureVoice& rhs) {
        return &lhs.first.getMeasure() < &rhs.first.getMeasure()
        && &lhs.first.getTrack() < &rhs.first.getTrack()
        && lhs.first.getHand() < rhs.first.getHand()
        && lhs.second < rhs.second;
    };
    std::set<MeasureVoice, decltype(compare)> measuresToFix(compare);
    
    // Delete any tuplets that are affected by the delete
    for (const auto& tupletItem : items) {
        
        if (!tupletItem.first.get().is<TupletIntf>()) {
            continue;
        }
        
        const auto& tuplet = tupletItem.first.get().as<TupletIntf>();
        const auto loc = tuplet.getLocation();
        const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                loc.getTrack(),
                                                loc.getHand());
        const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
        measuresToFix.emplace(mv);
        deleteTuplet(tuplet, items);
    }
    
    for (const auto& temporalItem : items) {
        if (!temporalItem.first.get().is<Temporal>()) {
            continue;
        }
        const auto& temporal = temporalItem.first.get().as<Temporal>();
        const auto& tuplets = temporal.getTuplets();
        for (const TupletIntf& tuplet : tuplets) {
            const auto& loc = tuplet.getLocation();
            const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                    loc.getTrack(),
                                                    loc.getHand());
            const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
            measuresToFix.emplace(mv);
            deleteTuplet(tuplet, items);
        }
    }
    
    // Delete any beams affected by the delete
    for (const auto& beamItem : items) {
        
        if (!beamItem.first.get().is<BeamIntf>()) {
            continue;
        }
        
        auto& beam = beamItem.first.get().as<BeamIntf>();
        const auto& loc = beam.getLocation();
        const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                loc.getTrack(),
                                                loc.getHand());
        const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
        measuresToFix.emplace(mv);
        deleteBeam(beam, items);
    }
    
    // Delete curves that are in the selection
    for (const auto& curveItem : items) {
        
        if (!curveItem.first.get().is<Curve>()) {
            continue;
        }
        
        auto& curve = curveItem.first.get().as<Curve>();
        const auto& loc = curve.getLocation();
        const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                loc.getTrack(),
                                                loc.getHand());
        const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
        measuresToFix.emplace(mv);
        deleteCurve(curve, items);
    }
    
    // Delete curves that are attached to temporals that are in the selection
    for (const auto& temporalItem : items) {
        
        if (!temporalItem.first.get().is<Temporal>()) {
            continue;
        }
        
        const auto& temporal = temporalItem.first.get().as<Temporal>();
        const auto& attachments = temporal.getAttachments();
        for (auto& curve : attachments.getIncomingCurves()) {
            
            const auto& loc = curve.get().getLocation();
            const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                    loc.getTrack(),
                                                    loc.getHand());
            const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
            measuresToFix.emplace(mv);
            deleteCurve(curve.get(), items);
        }
        
        for (const auto& curve : attachments.getOutgoingCurves()) {
            
            const auto& loc = curve.get().getLocation();
            const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                    loc.getTrack(),
                                                    loc.getHand());
            const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
            measuresToFix.emplace(mv);
            deleteCurve(curve.get(), items);
        }
        
        const auto chordParent = temporal.getChord();
        if (chordParent) {
            
            const auto& chord = chordParent->as<Chord>();
            for (const auto& curveNote : chord.getNotes()) {
                
                const auto& attachments = curveNote.get().getAttachments();
                for (auto& curve : attachments.getIncomingCurves()) {
                    
                    const auto& loc = curve.get().getLocation();
                    const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                            loc.getTrack(),
                                                            loc.getHand());
                    const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
                    measuresToFix.emplace(mv);
                    deleteCurve(curve.get(), items);
                }
                
                for (const auto& curve : attachments.getOutgoingCurves()) {
                    
                    const auto& loc = curve.get().getLocation();
                    const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                            loc.getTrack(),
                                                            loc.getHand());
                    const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
                    measuresToFix.emplace(mv);
                    deleteCurve(curve.get(), items);
                }
            }
        }
    }
    
    for (auto& temporalItem : items) {
        if (!temporalItem.first.get().is<Temporal>()) {
            continue;
        }
        
        const auto& temporal = temporalItem.first.get().as<Temporal>();
        const auto& marks = temporal.getAttachments().getMarks();
        
        for (const Mark& mark : marks) {
            
            const auto& loc = temporal.getLocation();
            const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                    loc.getTrack(),
                                                    loc.getHand());
            const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
            measuresToFix.emplace(mv);
            deleteMark(mark, items, temporalItem.second);
        }
        
        const auto& attachments = temporal.getAttachments();
        
        const auto& incomingCurves = attachments.getIncomingCurves();
        for (const auto& curve : incomingCurves) {
            deleteItem(curve.get(), items, temporalItem.second);
        }
        
        const auto& outgoingCurves = attachments.getOutgoingCurves();
        for (const auto& curve : outgoingCurves) {
            deleteItem(curve.get(), items, temporalItem.second);
        }
    }
    
    // Delete any chords that are in the selection
    std::vector<NoteRef> removedNotes;
    
    for (auto& chordItem : items) {
        
        if (chordItem.first.get().getItemTraits().getItemType() != ItemType::Chord) {
            continue;
        }
        
        auto& chord = chordItem.first.get().as<Chord>();
        const auto& loc = chord.getLocation();
        const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                loc.getTrack(),
                                                loc.getHand());
        const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
        measuresToFix.emplace(mv);
        
        const auto chordNotes = chord.getNotes();
        removedNotes.insert(removedNotes.end(),
                            chordNotes.cbegin(),
                            chordNotes.cend());
        
        deleteChord(chord, items, chordItem.second);
    }
    
    // Remove the notes the chord deleted
    for (auto iter = removedNotes.begin();
         iter != removedNotes.end();
         ++iter) {
        
        auto predicate = [&](const ItemBool& inItemBool) {
            const auto note = *iter;
            return inItemBool.first.get().getItemID() == note.get().getItemID();
        };
        
        const auto findIter = std::find_if(std::cbegin(items),
                                           std::cend(items),
                                           predicate);
        if (findIter != std::cend(items)) {
            items.erase(findIter);
        }
    }
    
    // Delete notes
    for (auto& noteItem : items) {
        
        if (noteItem.first.get().getItemTraits().getItemType() != ItemType::Note) {
            continue;
        }
        
        const auto& note = noteItem.first.get().as<Note>();
        
        // If the note is part of a chord, it requires special handling
        if (note.getIsChordMember()) {
            const auto parent = note.getChord();
            KOMP_ASSERT(parent);
            const auto& loc = note.getLocation();
            const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                    loc.getTrack(),
                                                    loc.getHand());
            const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
            measuresToFix.emplace(mv);
            deleteChordedNote(note, items, noteItem.second);
        } else {
            const auto& loc = note.getLocation();
            const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                    loc.getTrack(),
                                                    loc.getHand());
            const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
            measuresToFix.emplace(mv);
            deleteTemporal(note, items, noteItem.second);
        }
    }
    
    // Delete any remaining temporals
    for (auto& temporalItem : items) {
        
        if (!temporalItem.first.get().is<Temporal>()) {
            continue;
        }
        
        auto& temporal = temporalItem.first.get().as<Temporal>();
        const auto& loc = temporal.getLocation();
        const auto measureLoc = MeasureLocation(loc.getMeasure(),
                                                loc.getTrack(),
                                                loc.getHand());
        const auto mv = MeasureVoice{measureLoc, loc.getVoice()};
        measuresToFix.emplace(mv);
        deleteTemporal(temporal, items, temporalItem.second);
    }
    
    for (auto& markItem : items) {

        if (markItem.second) {
            // Item has already been deleted.
            continue;
        }
        
        if (markItem.first.get().getItemTraits().getItemType() == ItemType::Mark) {
            auto& mark = markItem.first.get().as<Mark>();
            deleteMark(mark, items, markItem.second);
        } else {
            auto& itemToDelete = markItem.first.get();
            deleteItem(itemToDelete, items, markItem.second);
        }
    }
    
    for (const auto& measureVoice : measuresToFix) {
        auto fixTimesCommand = std::make_unique<FixTimesCommand>(mFactory,
                                                                 mRegistry,
                                                                 measureVoice.first.getMeasure());
        fixTimesCommand->execute();
        addSubCommand(std::move(fixTimesCommand));
    }
    
    mScore.updateItems();
    return true;
}


void
DeleteItemsCommand::undo()
{
    mScore.setIsQuiescent(true);
    undoSubCommands();
    mScore.setIsQuiescent(false);
    mScore.updateItems();
    //KOMP_ASSERT(mScore.getAreTemporalTimesValid());
}


void
DeleteItemsCommand::redo()
{
    mScore.setIsQuiescent(true);
    redoSubCommands();
    mScore.setIsQuiescent(false);
    mScore.updateItems();
    //KOMP_ASSERT(mScore.getAreTemporalTimesValid());
}


void
DeleteItemsCommand::addSubCommand(CommandIntfUPtr inCommand)
{
    Command::addSubCommand(std::move(inCommand));
}

DeleteItemsCommand::ItemBools
DeleteItemsCommand::getItems() const
{
    std::vector<ItemBool> output;
    const auto& itemRegistry = mRegistry;
    
    for (const auto& i : mItems) {
        
        if (i.second) {
            continue;
        }
        
        auto& ptr = itemRegistry.getItem(i.first);
        output.emplace_back(ptr, i.second);
    }
    
    return output;
}

void
DeleteItemsCommand::deleteTuplet(const TupletIntf& inItem, ItemBools& outItems)
{
    auto& measure = inItem.getLocation().getMeasure();
    //        if (!measure) {
    //            // tuplet already deleted
    //            setItemDeleted(outTuplet, outItems);
    //            return false;
    //        }
    
    
    auto command = mFactory.deleteTuplet(measure, inItem);
    command->execute();
    addSubCommand(std::move(command));
    setItemDeleted(inItem, outItems);
}


void
DeleteItemsCommand::deleteBeam(const BeamIntf& inItem, ItemBools& outItems)
{
    auto command = mFactory.deleteBeamCommand(inItem);
    command->execute();
    addSubCommand(std::move(command));
    setItemDeleted(inItem, outItems);
}


void
DeleteItemsCommand::deleteCurve(const Curve& inItem, ItemBools& outItems)
{
    auto command = mFactory.deleteCurveCommand(inItem);
    command->execute();
    addSubCommand(std::move(command));
    setItemDeleted(inItem, outItems);
}


void
DeleteItemsCommand::deleteChord(const Chord& inChord,
                                ItemBools& outItems,
                                bool& ioDeleted)
{
    if (ioDeleted) {
        // Chord was already deleted.
        setItemDeleted(inChord, outItems);
        return;
    }
        
    const auto notes = inChord.getNotes();
    for (const auto& note : notes) {
        setItemDeleted(note, outItems);
    }

    auto command = mFactory.deleteChordCommand(inChord, true);
    command->execute();
    addSubCommand(std::move(command));
    ioDeleted = true;
    
    setItemDeleted(inChord, outItems);
}


void
DeleteItemsCommand::deleteChordedNote(const Note& inNote,
                                      ItemBools& outItems,
                                      bool& ioDeleted)
{
    // Find out which notes will remain in the chord
    KOMP_ASSERT(inNote.getIsChordMember());
    
    const auto chord = inNote.getChord();
    const auto chordNotes = chord->getNotes();
    std::set<Note*> notesToDelete;
    std::set<Note*> notesThatRemain;
    
    for (const auto& note : chordNotes) {
        const auto& finder = [&](const ItemBool& inItemBoolPair) {
            return &note.get() == &inItemBoolPair.first.get();
        };
        const auto findIter = std::find_if(std::cbegin(outItems),
                                           std::cend(outItems),
                                           finder);
        const auto isNoteBeingDeleted = findIter != std::cend(outItems);
        if (isNoteBeingDeleted) {
            notesToDelete.insert(&note.get());
        } else {
            notesThatRemain.insert(&note.get());
        }
    }
    
    // If zero items remain, delete the chord.
    if (notesThatRemain.empty()) {
        deleteChord(*chord, outItems, ioDeleted);
        return;
    }
    
    // If two items remain, just delete the others.
    if (notesThatRemain.size() >= 2) {
        for (const auto& note : notesToDelete) {
            auto chord = note->getChord();
            KOMP_ASSERT(chord);
            auto cm = chord->getChordMemento();
            chord->removeNote(*note);
            auto mementoCmd = mFactory.chordStateCommand(std::move(cm.noteMementos),
                                                         std::move(cm.chordMemento),
                                                         chord->getMemento());
            addSubCommand(std::move(mementoCmd));
            deleteTemporal(*note, outItems, ioDeleted);
        }
        //KOMP_ASSERT(getScore()->getAreTemporalTimesValid());
        return;
    }
    
    // If one item remains, delete the others and convert the
    // item remaining to a single note.
    if (notesThatRemain.size() == 1) {
        const auto remainingNote = *notesThatRemain.begin();
        const auto& loc = remainingNote->getLocation();
        
        const auto pitch = remainingNote->getPitch();
        const auto noteheadType = remainingNote->getNoteheadType();
        const auto duration = remainingNote->getDuration();
        const auto x = remainingNote->getX();
        KOMP_ASSERT(!remainingNote->getBeam());
        KOMP_ASSERT(remainingNote->getTuplets().empty());
        deleteChord(*chord, outItems, ioDeleted);
        
        auto& replacementNote = mRegistry.createItem<Note>(loc,
                                                           duration.getTemporalType(),
                                                           noteheadType,
                                                           pitch);
        auto insertNoteCommand = mFactory.insertTemporalCommand(replacementNote.getItemID(),
                                                                loc.getMeasure());
        insertNoteCommand->execute();
        addSubCommand(std::move(insertNoteCommand));
        replacementNote.setDuration(duration);
        replacementNote.setTime(loc.getTime());
        replacementNote.setX(x);
        auto fixTimes = mFactory.fixTimesCommand(loc.getMeasure());
        fixTimes->execute();
        addSubCommand(std::move(fixTimes));
        return;
    }
}


void
DeleteItemsCommand::deleteTemporal(const Temporal& inItem,
                                   ItemBools& outItems,
                                   bool& ioDeleted)
{
    KOMP_ASSERT(!(inItem.getIsChordMember()));
    KOMP_ASSERT(inItem.getTuplets().empty());
    
    if (inItem.getIsBeamed()) {
        if (!ioDeleted) {
            deleteBeamedNotes(inItem, outItems, ioDeleted);
        }
    } else {
        if (!ioDeleted) {
            auto deleteTemporalCommand = mFactory.deleteTemporalCommand(inItem);
            deleteTemporalCommand->execute();
            addSubCommand(std::move(deleteTemporalCommand));
            setItemDeleted(inItem, outItems);
        }
    }
}


void
DeleteItemsCommand::deleteItem(const Item& inItem,
                               ItemBools& outItems,
                               bool& ioDeleted)
{
    if (ioDeleted) {
        return;
    }
    
//    if (!ioDeleted->first->getLocation().getMeasure()) {
//        ioDeleted = true;
//        return false;
//    }
    
    auto command = mFactory.deleteItemCommand(inItem.getItemID());
    command->execute();
    addSubCommand(std::move(command));
    setItemDeleted(inItem, outItems);
}


void
DeleteItemsCommand::deleteBeamedNotes(const Temporal& inNote,
                                         ItemBools& outItems,
                                         bool& ioDeleted)
{
    auto beam = inNote.getBeam();
    const auto& beamedNotes = beam->getBeamedNotes();
    
    std::vector<std::pair<Temporal&, bool>> isBeamedNoteBeingDeleted;
    
    for (const auto& beamedNote : beamedNotes) {
        auto& thisNote = beamedNote->getNote();
        
        auto predicate = [&](ItemBool& inItemBool) {
            return &inItemBool.first.get() == &thisNote;
        };
        
        auto itemIter = std::find_if(outItems.begin(), outItems.end(), predicate);
        const auto isDeleted = itemIter != outItems.end();
        isBeamedNoteBeingDeleted.emplace_back(thisNote, isDeleted);
    }
    std::pair<Temporal*, Temporal*> beamedSpan;
    
    auto deleteBeamCommand = mFactory.deleteBeamCommand(*beam);
    deleteBeamCommand->execute();
    addSubCommand(std::move(deleteBeamCommand));
    
    for (const auto& noteInfo : isBeamedNoteBeingDeleted) {
        if (noteInfo.second) {
            auto deleteTemporalCommand = mFactory.deleteTemporalCommand(noteInfo.first);
            deleteTemporalCommand->execute();
            addSubCommand(std::move(deleteTemporalCommand));
            setItemDeleted(noteInfo.first, outItems);
            
            if (beamedSpan.first && beamedSpan.second) {
                auto insertBeamCommand = mFactory.insertBeamCommand({
                    beamedSpan.first->getItemID(),
                    beamedSpan.second->getItemID()
                });
                insertBeamCommand->execute();
                addSubCommand(std::move(insertBeamCommand));
            }
            beamedSpan.first = nullptr;
            beamedSpan.second = nullptr;
        } else {
            if (beamedSpan.first == nullptr) {
                beamedSpan.first = &noteInfo.first;
            } else {
                beamedSpan.second = &noteInfo.first;
                
            }
        }
    }
    
    if (beamedSpan.first && beamedSpan.second) {
        auto insertBeamCommand = mFactory.insertBeamCommand({
            beamedSpan.first->getItemID(),
            beamedSpan.second->getItemID()
        });
        insertBeamCommand->execute();
        addSubCommand(std::move(insertBeamCommand));
    }
}


void
DeleteItemsCommand::deleteMark(const Mark& inItem,
                               ItemBools& outItems,
                               bool& ioDeleted)
{
    if (ioDeleted) {
        return;
    }
    
    auto command = mFactory.deleteMarkCommand(inItem);
    command->execute();
    addSubCommand(std::move(command));
    setItemDeleted(inItem, outItems);
}

void
DeleteItemsCommand::setItemDeleted(const Item& inItem,
                                   ItemBools& outItems) const
{
    auto predicate = [&](ItemBool& inItemBool) {
        return &inItemBool.first.get() == &inItem;
    };
    
    const auto iter = std::find_if(outItems.begin(),
                                   outItems.end(),
                                   predicate);
    if (iter == outItems.cend()) {
        return;
    }
    
    iter->second = true;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
