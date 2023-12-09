// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "KeySignatureType.h"

// System
#include <cmath>
#include <cstdlib>

// Local
#include "ClefType.h"
#include "Pitch.h"
#include "Throw.h"

namespace komp
{
    KeySignatureValue::KeySignatureValue()
    : mValue(KeySignatureType::C)
    {
        
    }


    KeySignatureValue::KeySignatureValue(KeySignatureType inValue)
    : mValue(KeySignatureType::C)
    {
        setValue(inValue);
    }

  
    KeySignatureType
    KeySignatureValue::getValue() const
    {
        return mValue;
    }
    
    
    void
    KeySignatureValue::setValue(KeySignatureType inValue)
    {
        // bad things keep happening, so checking
        // for uninitialized values
        switch (inValue) {
            case KeySignatureType::C_FLAT:
            case KeySignatureType::G_FLAT:
            case KeySignatureType::D_FLAT:
            case KeySignatureType::A_FLAT:
            case KeySignatureType::E_FLAT:
            case KeySignatureType::B_FLAT:
            case KeySignatureType::F:
            case KeySignatureType::C:
            case KeySignatureType::G:
            case KeySignatureType::D:
            case KeySignatureType::A:
            case KeySignatureType::E:
            case KeySignatureType::B:
            case KeySignatureType::F_SHARP:
            case KeySignatureType::C_SHARP:
                break;
            default:
                KOMP_THROW("bad key signature value");
        }
        mValue = inValue;
    }
    
    
    KeyInt
    KeySignatureValue::getInt() const
    {
        switch (getValue())
        {
            case KeySignatureType::C_FLAT: return 7;
            case KeySignatureType::G_FLAT: return 6;
            case KeySignatureType::D_FLAT: return 5;
            case KeySignatureType::A_FLAT: return 4;
            case KeySignatureType::E_FLAT: return 3;
            case KeySignatureType::B_FLAT: return 2;
            case KeySignatureType::F: return 1;
            case KeySignatureType::C: return 0;
            case KeySignatureType::G: return 8;
            case KeySignatureType::D: return 9;
            case KeySignatureType::A: return 10;
            case KeySignatureType::E: return 11;
            case KeySignatureType::B: return 12;
            case KeySignatureType::F_SHARP: return 13;
            case KeySignatureType::C_SHARP: return 14;
            default: return 0;
        }
    }
    
    
    void
    KeySignatureValue::setInt(KeyInt inValue)
    {
        switch (inValue)
        {
            case 7: mValue = KeySignatureType::C_FLAT; return;
            case 6: mValue = KeySignatureType::G_FLAT; return;
            case 5: mValue = KeySignatureType::D_FLAT; return;
            case 4: mValue = KeySignatureType::A_FLAT; return;
            case 3: mValue = KeySignatureType::E_FLAT; return;
            case 2: mValue = KeySignatureType::B_FLAT; return;
            case 1: mValue = KeySignatureType::F; return;
            case 0: mValue = KeySignatureType::C; return;
            case 8: mValue = KeySignatureType::G; return;
            case 9: mValue = KeySignatureType::D; return;
            case 10: mValue = KeySignatureType::A; return;
            case 11: mValue = KeySignatureType::E; return;
            case 12: mValue = KeySignatureType::B; return;
            case 13: mValue = KeySignatureType::F_SHARP; return;
            case 14: mValue = KeySignatureType::C_SHARP; return;
            default: return;
        }
    }
    
    
    int
    KeySignatureValue::getAccidentals() const
    {
        switch (getValue())
        {
            case KeySignatureType::C_FLAT: return -7;
            case KeySignatureType::G_FLAT: return -6;
            case KeySignatureType::D_FLAT: return -5;
            case KeySignatureType::A_FLAT: return -4;
            case KeySignatureType::E_FLAT: return -3;
            case KeySignatureType::B_FLAT: return -2;
            case KeySignatureType::F: return -1;
            case KeySignatureType::C: return 0;
            case KeySignatureType::G: return 1;
            case KeySignatureType::D: return 2;
            case KeySignatureType::A: return 3;
            case KeySignatureType::E: return 4;
            case KeySignatureType::B: return 5;
            case KeySignatureType::F_SHARP: return 6;
            case KeySignatureType::C_SHARP: return 7;
            default: return 7;
        }
    }
    
    
    void
    KeySignatureValue::setAccidentals(int inValue)
    {
        switch (inValue)
        {
            case -7: mValue = KeySignatureType::C_FLAT; return;
            case -6: mValue = KeySignatureType::G_FLAT; return;
            case -5: mValue = KeySignatureType::D_FLAT; return;
            case -4: mValue = KeySignatureType::A_FLAT; return;
            case -3: mValue = KeySignatureType::E_FLAT; return;
            case -2: mValue = KeySignatureType::B_FLAT; return;
            case -1: mValue = KeySignatureType::F; return;
            case 0: mValue = KeySignatureType::C; return;
            case 1: mValue = KeySignatureType::G; return;
            case 2: mValue = KeySignatureType::D; return;
            case 3: mValue = KeySignatureType::A; return;
            case 4: mValue = KeySignatureType::E; return;
            case 5: mValue = KeySignatureType::B; return;
            case 6: mValue = KeySignatureType::F_SHARP; return;
            case 7: mValue = KeySignatureType::C_SHARP; return;
            default: return;
        }
    }


    std::vector<int>
    KeySignatureValue::calculateSlots(ClefType inClef) const
    {
        // this 'algorithm' is reverse-engineered from actual music notation.
        // it is not readily apparent if there is a simpler algorithmic approach
        // to the seemingly aesthetic choices that are made in music notation
        // for the placement of accidentals in key signatures
        
        const auto accidentals = getAccidentals();
        if (accidentals == 0) {
            return std::vector<int>{};
        }
        
        std::vector<int> slots;
        const bool isFlats = accidentals < 0;
        const bool isSharps = !isFlats;
        
        const auto firstStep = isFlats ? PitchStep::B : PitchStep::F;
        
        const auto slotBottomLine = 2; // bottom line of 5-line staff
        const auto slotTopLine = 10;   // top line of 5-line staff
        
        auto isFirstSlotFound = false;
        auto slot = 6; // start in the middle and go up first

        // search for the starting slot, first look above the middle
        // staff line, then look below the middle staff line
        
        if (!isFirstSlotFound) {
            for ( ; slot <= slotTopLine; ++slot) {
                const Pitch pitch{inClef, slot};
                if (pitch.step == firstStep) {
                    isFirstSlotFound = true;
                    break;
                }
            }
        }
        
        if (!isFirstSlotFound) {
            for (slot = 6; slot >= slotBottomLine; --slot) {
                Pitch pitch{inClef, slot};
                if (pitch.step == firstStep) {
                    isFirstSlotFound = true;
                    break;
                }
            }
        }

        KOMP_ASSERT(isFirstSlotFound);
        
        auto maxSlot = slotTopLine;
        auto minSlot = slotBottomLine;

        // if the key uses flats, and the starting slot is 4 then we
        // need to allow an accidental below staff line 1 (at slot 1)
        const auto isSlot4Flats = (slot == 4) && isFlats;
        if (isSlot4Flats) {
            minSlot = slotBottomLine - 1;
        }
        
        // if the key uses sharps, and the starting slot is 10 then
        // we need to make the bottom line of the staff unavailable
        // and we need to allow an accidental on slot 11
        const auto isSlot10Sharps = (slot == 10) && isSharps;
        if (isSlot10Sharps) {
            minSlot = slotBottomLine + 1;
            maxSlot = slotTopLine + 1;
        }
        
        // if the key uses sharps, and the starting slot is 6 then we
        // need to make fifth line unavailble
        const bool isSlot6Sharps = (slot == 6) && isSharps;

        // if the key uses sharps, and the starting slot is 8 then we
        // need to make fifth line unavailble
        const auto isSlot8Sharps = (slot == 8) && isSharps;

        if (isSlot6Sharps || isSlot8Sharps) {
            maxSlot = slotTopLine - 1;
            minSlot = slotBottomLine + 1;
        }
        
        // if the key uses sharps, and the starting slot is 7 then we
        // need to make the bottom two slots unavailable
        const auto isSlot7Sharps = (slot == 7) && isSharps;

        // if the key uses sharps, and the starting slot is 4 then we
        // need to make the bottom two slots unavailable
        const auto isSlot4Sharps = (slot == 4) && isSharps;
        if (isSlot4Sharps || isSlot7Sharps) {
            minSlot = slotBottomLine + 2;
        }
        
        // if the key uses sharps, and the starting slot is 5 then we
        // need to make fifth line and the fourth space unavailble
        const auto isSlot5Sharps = (slot == 5) && isSharps;
        if (isSlot5Sharps) {
            maxSlot = slotTopLine - 2;
        }
        
        // populate the output vector
        slots.emplace_back(slot);
        
        int evenJump = 0;
        int oddJump = 0;
        
        if (isFlats) {
            evenJump = slot <= 7 ? -4 : 3;
            oddJump = slot <= 7 ? 3 : -4;
        } else {
            evenJump = slot <= 7 ? -3 : 4;
            oddJump = slot <= 7 ? 4 : -3;
        }
        
        const auto numAccidentals = std::abs(accidentals);
        for (int i = 1; i < numAccidentals; ++i) {
            
            const bool isEven = (i % 2) == 0;
            const bool isOdd = !isEven;
            
            if (isEven) {
                slot += evenJump;
            } else if (isOdd) {
                slot += oddJump;
            }
            
            if (slot > maxSlot) {
                slot -= 7;
            } else if (slot < minSlot) {
                slot += 7;
            }
            
            slots.emplace_back(slot);
        }
        
        return slots;
    }
    
    
    bool operator==(const KeySignatureValue& l, const KeySignatureValue& r)
    {
        return l.getValue() == r.getValue();
    }
    
    
    bool operator!=(const KeySignatureValue& l, const KeySignatureValue& r)
    {
        return l.getValue() != r.getValue();
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
