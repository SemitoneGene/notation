// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "KeySignature.h"

// Local
#include "Clef.h"
#include "DrawableContext.h"
#include "DrawableKeySignature.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Item.h"
#include "ItemImpl.h"
#include "MarkType.h"
#include "Measure.h"
#include "Preferences.h"
#include "Pitch.h"
#include "Score.h"
#include "ScoreColors.h"
#include "StaffSystem.h"
#include "Staff.h"


namespace komp
{

// https://en.wikipedia.org/wiki/Circle_of_fifths_text_table
// 
// KeySignature value:
// 
// 0 - 7  is flats
// 8 - 14 is sharps
// 
//        Flats	      Sharps
//    Major   minor  Major	  minor
//----------------------------------
// 0    C	    a	    C	    a
// 1	F	    d	    G	    e
// 2	B♭	    g	    D	    b
// 3	E♭	    c	    A	    f♯
// 4	A♭	    f	    E	    c♯
// 5	D♭	    b♭	    B	    g♯
// 6	G♭	    e♭	    F♯	    d♯
// 7	C♭	    a♭	    C♯	    a♯
// 8	F♭	    d♭	    G♯	    e♯
//
//
// Stated differently, the legacy key signature ints
// are as follows:
//
// Legacy Int | Key     | Accidentals
// 0          | C Major | None
// 1          | F       | 1 Flat
// 2          | Bb      | 2 Flats
// 3          | Eb      | 3 Flats
// 4          | Ab      | 4 Flats
// 5          | Db      | 5 Flats
// 6          | Gb      | 6 Flats
// 7          | Cb      | 7 Flats
// 8          | G       | 1 Sharp
// 9          | D       | 2 Sharps
// 10         | A       | 3 Sharps
// 11         | E       | 4 Sharps
// 12         | B       | 5 Sharps
// 13         | F#      | 6 Sharps
// 14         | C#      | 7 Sharps

KeySignature::KeySignature(const MusicLocation& inLocation,
                           const KeySignatureValue& inKey)
: Spatial(inLocation)
, mKey(inKey)
, mPrevious(KeySignatureType::C)
{
}


const ItemTraits&
KeySignature::getItemTraits() const
{
    return ItemTraits::getItemTraits(ItemType::KeySignature);
}

ItemUPtr
KeySignature::getMemento() const
{
    return std::make_unique<KeySignature>(*this);
}

void
KeySignature::restoreMemento(const Item& inItem)
{
    const auto wasQuiescent = getIsQuiescent();
    const auto itemP = &inItem;
    const auto downcastedP = dynamic_cast<const KeySignature*>(itemP);
    if (!downcastedP) {
        KOMP_THROW("Incorrect type was passed");
    }
    
    this->operator=(*downcastedP);
    update();
    setIsQuiescent(wasQuiescent);
}

double
KeySignature::getScoreX() const
{
    const auto& measure = getLocation().getMeasure();
    const auto measureX = measure.getX();
    const auto posX = getX();
    const auto scoreX = measureX + posX;
    return scoreX;
}

double
KeySignature::getScoreY() const
{
    // A key signature is a composite of several items.
    // We set the center point in the middle of the staff.
    const auto& staff = getLocation().getStaff();
    const auto y = staff.slotToPixel(MIDDLE_SLOT);
    return y;
}

Point
KeySignature::getDrawPoint() const
{
    Point p{};
    p.x = getScoreX();
    p.y = getScoreY();
    return p;
}

void
KeySignature::inserted()
{
    update();
    notifyObservers({ItemChangeType::Inserted});
}

void
KeySignature::removed()
{
    notifyObservers({ItemChangeType::Removed});
}

void
KeySignature::removeSelf()
{
    notifyObservers({ItemChangeType::Removed});
    Spatial::removeSelf();
}

void
KeySignature::createDrawable()
{
    ItemImpl::createDrawable<DrawableKeySignature>(*this);
}

Rect
KeySignature::getFrame() const
{
    const auto drawable = ItemImpl::getDrawable(*this);
    return drawable->getFrame(getDrawPoint());
}

Region
KeySignature::getRegion() const
{
    return ItemImpl::getRegion<DrawableKeySignature>(*this);
}

void
KeySignature::update()
{
    if (getIsQuiescent()) {
        return;
    }
    notifyObservers({ItemChangeType::Updated});
}

/// \brief Calculates the effective accidental for a given key signature value, clef and slot.
AccidentalType
KeySignature::getAccidental(KeySignatureValue inKeyValue,
                            ClefType inClef,
                            int inSlot)
{
    const auto numAlterations = inKeyValue.getAccidentals();
    const int midiNoteForUnalteredSlot = Pitch::midiNoteForSlot(inSlot, inClef);
    const Pitch pitch{inClef, inSlot, midiNoteForUnalteredSlot};
    AccidentalType accidental{AccidentalType::NONE};
    
    switch (pitch.step)
    {
    case PitchStep::C:
        {
            if (numAlterations >= 2) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -6) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    case PitchStep::D:
        {
            if (numAlterations >= 4) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -4) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    case PitchStep::E:
        {
            if (numAlterations >= 6) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -2) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    case PitchStep::F:
        {
            if (numAlterations >= 1) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -7) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    case PitchStep::G:
        {
            if (numAlterations >= 3) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -5) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    case PitchStep::A:
        {
            if (numAlterations >= 5) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -3) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    case PitchStep::B:
        {
            if (numAlterations >= 7) {
                accidental = AccidentalType::SHARP;
            } else if (numAlterations <= -1) {
                accidental = AccidentalType::FLAT;
            }
            break;
        }
    default:
        {
            accidental = AccidentalType::NONE;
            break;
        }
    }
    
    return accidental;
}

/// \brief Calculates the needed spaces for the change of key.
/// \discussion This includes the space for cancellations.
/// \param inOldKey The previous key signature
/// \param inNewKey The new key signature
/// \result Number of spaces needed for new key signature.
double
KeySignature::calculateSpace(int inOldKey, int inNewKey)
{
    if (inNewKey == 0) {
        
        // Key of C
        if (inOldKey > 7) {
            inOldKey -= 7;
        }
        return inOldKey;
        
    } else if (inNewKey < 8) {
        
        // Flats
        if (inOldKey >= 8) {
            if (false) { // TODO: Show naturals preference
                return inOldKey - 7 + inNewKey;
            } else {
                return inNewKey;
            }
        }
        
        if (inOldKey <= inNewKey) {
            return inNewKey;
        }
        
        return inOldKey;
        
    } else {
        
        // Sharps
        if (inOldKey < 8) {
            if (false) {
                // TODO: Show naturals preference
                return inOldKey + inNewKey - 7;
            } else {
                return inNewKey - 7;
            }
        }
        
        if (inOldKey <= inNewKey) {
            return inNewKey - 7;
        }
        
        return inOldKey - 7;
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.


