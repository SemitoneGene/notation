// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Pitch.h"

// System
#include <sstream>

// Komp
#include "Clef.h"
#include "Throw.h"

namespace komp
{
Pitch::Pitch()
{
}

Pitch::Pitch(PitchStep inStep, int inOctave, int inAlter)
: step{inStep}
, octave{inOctave}
, alter{inAlter}
{
    
}


Pitch::Pitch(ClefType clef, int slot, int midiPitch)
: step{PitchStep::C}
, octave{4}
, alter{0}
{
    setUsingKompNoteInfo(clef, slot, midiPitch);
}

Pitch::Pitch(ClefType clef, int slot)
: step{PitchStep::C}
, octave{4}
, alter{0}
{
    const auto midiPitch = midiNoteForSlot(slot, clef);
    setUsingKompNoteInfo(clef, slot, midiPitch);
}

const std::map<PitchStep, int> Pitch::sStepSemitoneMap =
{
    std::pair<PitchStep, int>{ PitchStep::C,  0 },
    std::pair<PitchStep, int>{ PitchStep::D,  2 },
    std::pair<PitchStep, int>{ PitchStep::E,  4 },
    std::pair<PitchStep, int>{ PitchStep::F,  5 },
    std::pair<PitchStep, int>{ PitchStep::G,  7 },
    std::pair<PitchStep, int>{ PitchStep::A,  9 },
    std::pair<PitchStep, int>{ PitchStep::B, 11 }
};

const std::map<int, PitchStep> Pitch::sSemitoneStepMap =
{
    std::pair<int, PitchStep>{  0, PitchStep::C },
    std::pair<int, PitchStep>{  2, PitchStep::D },
    std::pair<int, PitchStep>{  4, PitchStep::E },
    std::pair<int, PitchStep>{  5, PitchStep::F },
    std::pair<int, PitchStep>{  7, PitchStep::G },
    std::pair<int, PitchStep>{  9, PitchStep::A },
    std::pair<int, PitchStep>{ 11, PitchStep::B },
};

int
Pitch::chromaticValue() const
{
    return sStepSemitoneMap.at(step) + alter;
};

int
Pitch::getMidiNote() const
{
    // TODO: What was this supposed to do?
//    const auto isAlterNegative = alter < 0;
//
//    int octaveOffset = 0;
//    Pitch tempPitch{ step, alter, 0 };
//    for (int i = 0; i < abs(alter); ++i) {
//        if (isAlterNegative) {
//            --tempPitch.alter;
//            if (tempPitch.chromaticValue() == 11) {
//                ++octaveOffset;
//            }
//        } else {
//            ++tempPitch.alter;
//            if (tempPitch.chromaticValue() == 0 ) {
//                ++octaveOffset;
//            }
//        }
//    }
    
    auto answer = chromaticValue();
    answer += 12 * octave;
    answer += MIDDLE_C_MIDI_VALUE - (12 * 4);
    return answer;
}


int
Pitch::slot(ClefType clef) const
{
    const int middleCSlot = Clef::clefInfo(clef).middleCSlot();
    const int distanceFromMiddleC = slotDistanceFromMiddleC();
    return middleCSlot + distanceFromMiddleC;
}


int
Pitch::slotDistanceFromMiddleC() const
{
    if (step == PitchStep::C && octave == 4) {
        return 0;
    }
    bool isAboveMiddleC = false;
    
    if (step == PitchStep::C) {
        isAboveMiddleC = octave > 4;
    } else if (octave == 4) {
        isAboveMiddleC = true;
    } else {
        isAboveMiddleC = octave > 4;
    }
    
    Pitch iterator{ PitchStep::C, 4, 0 };
    int absStepDistance = 0;
    while( iterator.step != step || iterator.octave != octave ) {
        if (isAboveMiddleC ) {
            iterator.incrementStepwise();
        } else {
            iterator.decrementStepwise();
        }
        ++absStepDistance;
    }
    return isAboveMiddleC ? absStepDistance : -1 * absStepDistance;
}


void
Pitch::setUsingKompNoteInfo(ClefType clef, int slot, int midiPitch)
{
    const auto slotMidiNote = midiNoteForSlot(slot, clef);
    alter = midiPitch - slotMidiNote;
    octave = ( ( midiPitch - alter ) / 12) - 1;
    step = midiNoteToStep(midiPitch, alter);
}


int
Pitch::midiNoteForSlot(int slot, ClefType clef)
{
    static const int semitonesPerSlotFromC[7] = { 0, 2, 4, 5, 7, 9, 11 };
    const auto& clefInfo = Clef::clefInfo(clef);
    int slotDistanceFromMiddleC = slot - clefInfo.middleCSlot();
    bool isBelowMiddleC = slotDistanceFromMiddleC < 0;
    
    if (isBelowMiddleC) {
        slotDistanceFromMiddleC *= -1;
    }
    
    const auto octavesAway = slotDistanceFromMiddleC / 7;
    const auto remainder = slotDistanceFromMiddleC % 7;
    int additionalSemitones = 0;
    
    if (remainder != 0) {
        if (isBelowMiddleC) {
            additionalSemitones = 12 - semitonesPerSlotFromC[7 - remainder];
        } else {
            additionalSemitones = semitonesPerSlotFromC[remainder];
        }
    }
    
    const auto totalSemitonesAway = ( octavesAway * 12 ) + additionalSemitones;
    const auto answer = isBelowMiddleC ? 60 - totalSemitonesAway : 60 + totalSemitonesAway;
    return answer;
}

std::string
Pitch::getString() const
{
    std::stringstream ss;
    switch (step)
    {
        case PitchStep::A: ss << 'A'; break;
        case PitchStep::B: ss << 'B'; break;
        case PitchStep::C: ss << 'C'; break;
        case PitchStep::D: ss << 'D'; break;
        case PitchStep::E: ss << 'E'; break;
        case PitchStep::F: ss << 'F'; break;
        case PitchStep::G: ss << 'G'; break;
        default: ss << "C"; break;
    }
    
    switch (alter)
    {
        case -2: ss << "bb"; break;
        case -1: ss << 'b'; break;
        case 1: ss << '#'; break;
        case 2: ss << 'x'; break;
        default: break;
    }
    
    ss << octave;
    
    return ss.str();
}

static bool isIn(const char c, const char* const collection)
{
    std::string col{collection};
    return std::find(col.cbegin(), col.cend(), c) != col.cend();
}

bool
Pitch::setString(const std::string& inString)
{
    if (inString.size() < 2) {
        return false;
    }
    
    auto c = inString.cbegin();
    auto e = inString.cend();
    
    if (!isIn(*c, "ABCDEFG")) {
        return false;
    }
    
    PitchStep theStep = PitchStep::C;
    
    switch (*c) {
        case 'A': theStep = PitchStep::A; break;
        case 'B': theStep = PitchStep::B; break;
        case 'C': theStep = PitchStep::C; break;
        case 'D': theStep = PitchStep::D; break;
        case 'E': theStep = PitchStep::E; break;
        case 'F': theStep = PitchStep::F; break;
        case 'G': theStep = PitchStep::G; break;
        default: theStep = PitchStep::A; break;
    }
    
    ++c;
    if (c == e) {
        return false;
    }
    
    if (!isIn(*c, "b#x-0123456789")) {
        return false;
    }
    
    int theAlter = 0;
    
    if (isIn(*c, "b#x")) {
        if (*c == 'b') {
            --theAlter;
            auto nextChar = c + 1;
            if (nextChar == e) {
                return false;
            }
            if (*nextChar == 'b') {
                --theAlter;
                ++c;
            }
        } else if (*c == '#') {
            theAlter = 1;
        } else if (*c == 'x') {
            theAlter = 2;
        }
        ++c;
        if (c == e) {
            return false;
        }
    }
    
    if (!isIn(*c, "-0123456789")) {
        return false;
    }
    
    bool isOctaveNegative = false;
    
    if (*c == '-') {
        isOctaveNegative = true;
        ++c;
    }
    
    if (c == e) {
        return false;
    }
    
    if (!isIn(*c, "0123456789")) {
        return false;
    }
    
    int theOctave = 0;
    std::string octaveString{c, e};
    std::stringstream ss{octaveString};
    ss >> theOctave;
    
    if (isOctaveNegative) {
        theOctave *= -1;
    }
    
    step = theStep;
    octave = theOctave;
    alter = theAlter;
    
    return true;
}

void
Pitch::minimizeAlter()
{
    while (alter > 2) {
        incrementStepwise();
        alter -= 2;
    }
    
    while (alter < 2) {
        incrementStepwise();
        alter += 2;
    }
    
    if (alter == 0) {
        return;
    }
    
    if ((alter == 1) && (step == PitchStep::B)) {
        incrementStepwise();
        alter = 0;
    }
    
    if ((alter == 1) && (step == PitchStep::E)) {
        incrementStepwise();
        alter = 0;
    }
    
    if ((alter == -1) && (step == PitchStep::C)) {
        decrementStepwise();
        alter = 0;
    }
    
    if ((alter == -1) && (step == PitchStep::F)) {
        decrementStepwise();
        alter = 0;
    }
}


void
Pitch::incrementStepwise()
{
    if (step == PitchStep::B) {
        ++octave;
        step = PitchStep::C;
    } else {
        step = static_cast<PitchStep>(static_cast<int>(step) + 1);
    }
}


void
Pitch::decrementStepwise()
{
    if (step == PitchStep::C) {
        --octave;
        step = PitchStep::B;
    } else {
        step = static_cast<PitchStep>(static_cast<int>(step) - 1 );
    }
}


void
Pitch::incrementSemitone()
{
    alter += 1;
    minimizeAlter();
}


void
Pitch::decrementSemitone()
{
    alter -= 1;
    minimizeAlter();
}


PitchStep
Pitch::midiNoteToStep(int midiNote, int alter)
{
    const int stepMidiNote = midiNote - alter;
    const int semitoneValue = stepMidiNote % 12;
    const auto iter = sSemitoneStepMap.find(semitoneValue);
    if (iter == sSemitoneStepMap.cend()) {
        // this shouldn't happen under normal circumstances
        ++midiNote;
        return midiNoteToStep(midiNote, alter);
    }
    return iter->second;
}

bool
operator<(const Pitch& lhs, const Pitch& rhs)
{
    if (static_cast<int>(lhs.alter) < static_cast<int>(rhs.alter)) {
        return true;
    } else if (static_cast<int>(lhs.alter) > static_cast<int>(rhs.alter)) {
        return false;
    }
    
    if (static_cast<int>(lhs.step) < static_cast<int>(rhs.step)) {
        return true;
    } else if (static_cast<int>(lhs.step) > static_cast<int>(rhs.step)) {
        return false;
    }
    
    if (static_cast<int>(lhs.octave) < static_cast<int>(rhs.octave)) {
        return true;
    } else if (static_cast<int>(lhs.octave) > static_cast<int>(rhs.octave)) {
        return false;
    }
    
    return false;
}

bool
operator>(const Pitch& lhs, const Pitch& rhs)
{
    return rhs < lhs;
}


bool
operator==(const Pitch& lhs, const Pitch& rhs)
{
    return (!(lhs < rhs)) && (!(lhs > rhs));
}


bool
operator!=(const Pitch& lhs, const Pitch& rhs)
{
    return !(lhs == rhs);
}


bool
operator<=(const Pitch& lhs, const Pitch& rhs)
{
    return !(lhs == rhs);
}


std::ostream&
operator<<(std::ostream& os, const Pitch& pitch)
{
    os << pitch.getString();
    return os;
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
