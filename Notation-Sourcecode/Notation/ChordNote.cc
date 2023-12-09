// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ChordNote.h"

// Local
#include "Chord.h"
#include "Note.h"

namespace komp
{

ChordNote::ChordNote(Note& inNote)
: chordHead(inNote.getSlot()
            , inNote.getItemID())
, mNote(inNote)
{
    update();
}


Note&
ChordNote::getNote() const
{
    return mNote.get();
}


void
ChordNote::update()
{
    auto note = getNote();
    const auto frames = note.getFrames();
    chordHead.accidentalFrame = frames.accidental;
    chordHead.hasAccidental = !frames.accidental.getIsEmpty()
                              && note.getAccidental() != AccidentalType::NONE;
    chordHead.noteheadFrame = frames.head;
    chordHead.accidentalOffset = note.getAccidentalOffset();
    chordHead.noteDrawPoint = note.getDrawPoint();
    
    const auto chordTemporal = note.getChord();
    if (!chordTemporal) {
        return;
    }
    
    if (chordTemporal->getItemTraits().getItemType() != ItemType::Chord) {
        return;
    }
    
    const auto& chord = chordTemporal->as<Chord>();
    chordHead.chordDrawPoint = note.getChord()->getDrawPoint();
    const auto& notes = chord.getNotes();
    chordHead.noteheadFrames.clear();
    
    for (const auto& note : notes) {
        chordHead.noteheadFrames.push_back(note.get().getFrames().head);
    }
}

bool
operator<(const ChordNote& lhs, const ChordNote& rhs)
{
    const auto& l = lhs.getNote();
    const auto& r = rhs.getNote();
    
    const auto leftSlot = l.getSlot();
    const auto rightSlot = r.getSlot();
    
    if (leftSlot < rightSlot) {
        return true;
    }
    
    if (leftSlot > rightSlot) {
        return false;
    }
    
    const auto leftPitch = l.getPitch().getMidiNote();
    const auto rightPitch = r.getPitch().getMidiNote();
    
    if (leftPitch < rightPitch) {
        return true;
    }
    
    if (leftPitch > rightPitch) {
        return false;
    }
    
    const auto leftX = l.getX();
    const auto rightX = r.getX();
    
    if (leftX < rightX) {
        return true;
    }
    
    if (leftX > rightX) {
        return false;
    }
    
    return &l < &r;
}


bool
operator>(const ChordNote& lhs, const ChordNote& rhs)
{
    return rhs < lhs;
}


bool
operator==(const ChordNote& lhs, const ChordNote& rhs)
{
    const bool isLessThan = lhs < rhs;
    const bool isGreaterThan = lhs > lhs;
    return !isLessThan && !isGreaterThan;
}


bool
operator!=(const ChordNote& lhs, const ChordNote& rhs)
{
    return !(lhs == rhs);
}


bool
operator<=(const ChordNote& lhs, const ChordNote& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}


bool
operator>=(const ChordNote& lhs, const ChordNote& rhs)
{
    return (lhs > rhs) || (lhs == rhs);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

