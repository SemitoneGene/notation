// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <string>

namespace komp
{
    enum class ClefSymbolType
    {
        UNKNOWN,
        G,
        F,
        C,
        PERCUSSION,
        TAB
    };
    
    enum class ClefType
    {
        G = 0,        ///< Treble
        G_LINE1,      ///< "French" was G5
        G_8VA,        ///< 8va Treble (8 up)
        G_8VB,        ///< 8vb Treble (8 down)
        G_15MA,       ///< G clef quindicesima alta
        G_15MB,       ///< G clef quindicesima bassa (was G4)
        F,            ///< Bass
        F_LINE3,      ///< "Varbaritone" F on third line
        F_LINE5,      ///< "Subbass" F on fifth line
        F_8VA,        ///< 8va F (8 up)
        F_8VB,        ///< 8vb Bass (8 down)
        F_15MA,       ///< F quindicesima alta (15 up)
        F_15MB,       ///< F quindicesima bassa (down 15)
        C,            ///< Alto / Viola (C Line 3)
        C_LINE1,      ///< "Soprano" C-Clef first line
        C_LINE2,      ///< "Mezzo Soprano" C-Clef second-line
        C_LINE4,      ///< Tenor
        C_LINE5,      ///< "Baritone" C-Clef fifth-line
        PERCUSSION,   ///< Unpitched Percussion (Double lines)
        PERCUSSION_2, ///< Unpitched Percussion (Empty rectangle)
        PERCUSSION_3, ///< Unpitched Percussion (Double lines)
        TAB,          ///< Tablature
        TAB_2,        ///< ?
        UNKNOWN       ///< ?
    };
    
    inline std::string getClefTypeString(ClefType value)
    {
        switch (value) {
            case ClefType::G: return "G";
            case ClefType::G_LINE1: return "G_LINE1";
            case ClefType::G_8VA: return "G_8VA";
            case ClefType::G_8VB: return "G_8VB";
            case ClefType::G_15MA: return "G_15MA";
            case ClefType::G_15MB: return "G_15MB";
            case ClefType::F: return "F";
            case ClefType::F_LINE3: return "F_LINE3";
            case ClefType::F_LINE5: return "F_LINE5";
            case ClefType::F_8VA: return "F_8VA";
            case ClefType::F_8VB: return "F_8VB";
            case ClefType::F_15MA: return "F_15MA";
            case ClefType::F_15MB: return "F_15MB";
            case ClefType::C: return "C";
            case ClefType::C_LINE1: return "C_LINE1";
            case ClefType::C_LINE2: return "C_LINE2";
            case ClefType::C_LINE4: return "C_LINE4";
            case ClefType::C_LINE5: return "C_LINE5";
            case ClefType::PERCUSSION: return "PERCUSSION";
            case ClefType::PERCUSSION_2: return "PERCUSSION_2";
            case ClefType::PERCUSSION_3: return "PERCUSSION";
            case ClefType::TAB: return "TAB";
            case ClefType::TAB_2: return "TAB_2";
            case ClefType::UNKNOWN: return "UNKNOWN";
            default: return "UNKNOWN";
        }
    }
    
    inline ClefType parseClefTypeString(const std::string& value)
    {
        if (value == "G") {
            return ClefType::G;
        } else if (value == "G_LINE1") {
            return ClefType::G_LINE1;
        } else if (value == "G_8VA") {
            return ClefType::G_8VA;
        } else if (value == "G_8VB") {
            return ClefType::G_8VB;
        } else if (value == "G_15MA") {
            return ClefType::G_15MA;
        } else if (value == "G_15MB") {
            return ClefType::G_15MB;
        } else if (value == "F") {
            return ClefType::F;
        } else if (value == "F_LINE3") {
            return ClefType::F_LINE3;
        } else if (value == "F_LINE5") {
            return ClefType::F_LINE5;
        } else if (value == "F_8VA") {
            return ClefType::F_8VA;
        } else if (value == "F_8VB") {
            return ClefType::F_8VB;
        } else if (value == "F_15MA") {
            return ClefType::F_15MA;
        } else if (value == "F_15MB") {
            return ClefType::F_15MB;
        } else if (value == "C") {
            return ClefType::C;
        } else if (value == "C_LINE1") {
            return ClefType::C_LINE1;
        } else if (value == "C_LINE2") {
            return ClefType::C_LINE2;
        } else if (value == "C_LINE4") {
            return ClefType::C_LINE4;
        } else if (value == "C_LINE5") {
            return ClefType::C_LINE5;
        } else if (value == "PERCUSSION") {
            return ClefType::PERCUSSION;
        } else if (value == "PERCUSSION_2") {
            return ClefType::PERCUSSION_2;
        } else if (value == "PERCUSSION_3") {
            return ClefType::PERCUSSION_3;
        } else if (value == "TAB") {
            return ClefType::TAB;
        } else if (value == "TAB_2") {
            return ClefType::TAB_2;
        } else if (value == "UNKNOWN") {
            return ClefType::UNKNOWN;
        } else {
            return ClefType::UNKNOWN;
        }
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
