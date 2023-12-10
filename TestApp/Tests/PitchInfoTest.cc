// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Test Framework
#include "Cpul/cpulTestHarness.h"
#include "TestControl.h"

// Komp
#include "Pitch.h"
#include "Clef.h"

#include "TestSupport/TestFileRepository.h"

#define KOMP_ENABLE_THESE_TESTS
#if defined KOMP_ENABLE_ALL_TESTS || defined KOMP_ENABLE_THESE_TESTS

using namespace komp;

TEST( Slot_Treble_C4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, 0 };
    auto expected = 0;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_Cs4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, 1 };
    auto expected = 0;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_Cb4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, -1 };
    auto expected = 0;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_D4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, 0 };
    auto expected = 1;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_Db4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, -1 };
    auto expected = 1;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_Ds4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, 1 };
    auto expected = 1;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_Eb5, Pitch )
{
    auto pitch = Pitch{ PitchStep::E, 5, -1 };
    auto expected = 9;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Treble_F2, Pitch )
{
    auto pitch = Pitch{ PitchStep::F, 2, 0 };
    auto expected = -11;
    auto actual = pitch.slot( ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Bass_C4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, 0 };
    auto expected = 12;
    auto actual = pitch.slot( ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Bass_D4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, 0 };
    auto expected = 13;
    auto actual = pitch.slot( ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Bass_Eb5, Pitch )
{
    auto pitch = Pitch{ PitchStep::E, 5, -1 };
    auto expected = 21;
    auto actual = pitch.slot( ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Bass_F2, Pitch )
{
    auto pitch = Pitch{ PitchStep::F, 2, 0 };
    auto expected = 1;
    auto actual = pitch.slot( ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Alto_C4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, 0 };
    auto expected = 6;
    auto actual = pitch.slot( ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Alto_D4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, 0 };
    auto expected = 7;
    auto actual = pitch.slot( ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Alto_Eb5, Pitch )
{
    auto pitch = Pitch{ PitchStep::E, 5, -1 };
    auto expected = 15;
    auto actual = pitch.slot( ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_Alto_F2, Pitch )
{
    auto pitch = Pitch{ PitchStep::F, 2, 0 };
    auto expected = -5;
    auto actual = pitch.slot( ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_G_8VA, Pitch )
{
    auto pitch = Pitch{ PitchStep::F, 5, 0 };
    auto expected = 3;
    auto actual = pitch.slot( ClefType::G_8VA );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( Slot_F_8VB, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 1, 0 };
    auto expected = -2;
    auto actual = pitch.slot( ClefType::F_8VB );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_C4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, 0 };
    auto expected = 60;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Cs4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, 1 };
    auto expected = 61;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Cb4, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, 4, -1 };
    auto expected = 59;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_D4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, 0 };
    auto expected = 62;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Db4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, -1 };
    auto expected = 61;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Ds4, Pitch )
{
    auto pitch = Pitch{ PitchStep::D, 4, 1 };
    auto expected = 63;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Eb5, Pitch )
{
    auto pitch = Pitch{ PitchStep::E, 5, -1 };
    auto expected = 75;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_F2, Pitch )
{
    auto pitch = Pitch{ PitchStep::E, 5, -1 };
    auto expected = 75;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Cminus1, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, -1, 0 };
    auto expected = 0;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Cminus1doubleFlat, Pitch )
{
    auto pitch = Pitch{ PitchStep::C, -1, -2 };
    auto expected = -2;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Fx7, Pitch )
{
    auto pitch = Pitch{ PitchStep::F, 7, 2 };
    auto expected = 103;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Gb8, Pitch )
{
    auto pitch = Pitch{ PitchStep::G, 8, -1 };
    auto expected = 114;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Bb2, Pitch )
{
    auto pitch = Pitch{ PitchStep::B, 2, -1 };
    auto expected = 46;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( MidiNote_Bsharp6, Pitch )
{
    auto pitch = Pitch{ PitchStep::B, 6, 1 };
    auto expected = 96;
    auto actual = pitch.getMidiNote();
    CHECK_EQUAL( expected, actual );
}
T_END


 TEST( fromKomp_clef_G_Ab4, Pitch )
 {
     // Ab4
     const auto clef = ClefType::G;
     const int slot = 5;
     const int midiNote = 68;

     const auto expectedStep = PitchStep::A;
     const int expectedAlter = -1;
     const int expectedOctave = 4;

     const auto pitch = Pitch{ clef, slot, midiNote };
     CHECK_EQUAL( expectedAlter, pitch.alter );
     CHECK_EQUAL( expectedOctave, pitch.octave );
     CHECK( expectedStep == pitch.step );
 }
 T_END


 TEST( fromKomp_clef_F_Dsharp2, Pitch )
 {
     // D#2
     const auto clef = ClefType::F;
     const int slot = -1;
     const int midiNote = 39;

     const auto expectedStep = PitchStep::D;
     const int expectedAlter = 1;
     const int expectedOctave = 2;

     const auto pitch = Pitch{ clef, slot, midiNote };
     CHECK_EQUAL( expectedAlter, pitch.alter );
     CHECK_EQUAL( expectedOctave, pitch.octave );
     CHECK( expectedStep == pitch.step );
 }
 T_END


 TEST( fromKomp_clef_C_Fb3, Pitch )
 {
     // Fb3
     const auto clef = ClefType::F;
     const int slot = 8;
     const int midiNote = 52;

     const auto expectedStep = PitchStep::F;
     const int expectedAlter = -1;
     const int expectedOctave = 3;

     const auto pitch = Pitch{ clef, slot, midiNote };
     CHECK_EQUAL( expectedAlter, pitch.alter );
     CHECK_EQUAL( expectedOctave, pitch.octave );
     CHECK( expectedStep == pitch.step );
 }
 T_END


 TEST( fromKomp_clef_G_8VA_Bsharp5, Pitch )
 {
     // B#5
     const auto clef = ClefType::G_8VA;
     const int slot = 6;
     const int midiNote = 84;

     const auto expectedStep = PitchStep::B;
     const int expectedAlter = 1;
     const int expectedOctave = 5;

     const auto pitch = Pitch{ clef, slot, midiNote };
     CHECK_EQUAL( expectedAlter, pitch.alter );
     CHECK_EQUAL( expectedOctave, pitch.octave );
     CHECK( expectedStep == pitch.step );
 }
 T_END


 TEST( fromKomp_clef_G_AVeryFlat4, Pitch )
 {
     // Ab4
     const auto clef = ClefType::G;
     const int slot = 5;
     const int midiNote = 56;

     const auto expectedStep = PitchStep::A;
     const int expectedAlter = -13;
     const int expectedOctave = 4;

     const auto pitch = Pitch{ clef, slot, midiNote };
     CHECK_EQUAL( expectedAlter, pitch.alter );
     CHECK_EQUAL( expectedOctave, pitch.octave );
     CHECK( expectedStep == pitch.step );
 }
T_END


TEST( fromKomp_clef_G_Bb2, Pitch )
{
    // Bb2
    const auto clef = ClefType::G;
    const int slot = -8;
    const int midiNote = 46;
    
    const auto expectedStep = PitchStep::B;
    const int expectedAlter = -1;
    const int expectedOctave = 2;
    
    const auto pitch = Pitch{ clef, slot, midiNote };
    CHECK_EQUAL( expectedAlter, pitch.alter );
    CHECK_EQUAL( expectedOctave, pitch.octave );
    CHECK( expectedStep == pitch.step );
}
T_END


TEST( fromKomp_clef_G_Bsharp4, Pitch )
{
    // B#4 (i.e. == C5)
    const auto clef = ClefType::G;
    const int slot = 6;
    const int midiNote = 72;
    
    const auto expectedStep = PitchStep::B;
    const int expectedAlter = 1;
    const int expectedOctave = 4;
    
    const auto pitch = Pitch{ clef, slot, midiNote };
    CHECK_EQUAL( expectedAlter, pitch.alter );
    CHECK_EQUAL( expectedOctave, pitch.octave );
    CHECK( expectedStep == pitch.step );
}
T_END


TEST( fromKomp_clef_G_BdoubleSharp4, Pitch )
{
    // Bx4 (i.e. == C#5)
    const auto clef = ClefType::G;
    const int slot = 6;
    const int midiNote = 73;
    
    const auto expectedStep = PitchStep::B;
    const int expectedAlter = 2;
    const int expectedOctave = 4;
    
    const auto pitch = Pitch{ clef, slot, midiNote };
    CHECK_EQUAL( expectedAlter, pitch.alter );
    CHECK_EQUAL( expectedOctave, pitch.octave );
    CHECK( expectedStep == pitch.step );
}
T_END


TEST( fromKomp_clef_G_Cb5, Pitch )
{
    // Cb5 (i.e. == B4)
    const auto clef = ClefType::G;
    const int slot = 7;
    const int midiNote = 71;
    
    const auto expectedStep = PitchStep::C;
    const int expectedAlter = -1;
    const int expectedOctave = 5;
    
    const auto pitch = Pitch{ clef, slot, midiNote };
    CHECK_EQUAL( expectedAlter, pitch.alter );
    CHECK_EQUAL( expectedOctave, pitch.octave );
    CHECK( expectedStep == pitch.step );
}
T_END


TEST( midiNoteForSlot_slot_2_clef_G, Pitch )
{
    int expected = 64;
    int actual = Pitch::midiNoteForSlot( 2, ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n2_clef_G, Pitch )
{
    int expected = 57;
    int actual = Pitch::midiNoteForSlot( -2, ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_9_clef_G, Pitch )
{
    int expected = 76;
    int actual = Pitch::midiNoteForSlot( 9, ClefType::G );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_2_clef_G_8VA, Pitch )
{
    int expected = 64 + 12;
    int actual = Pitch::midiNoteForSlot( 2, ClefType::G_8VA );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n2_clef_G_8VA, Pitch )
{
    int expected = 57 + 12;
    int actual = Pitch::midiNoteForSlot( -2, ClefType::G_8VA );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_9_clef_G_8VA, Pitch )
{
    int expected = 76 + 12;
    int actual = Pitch::midiNoteForSlot( 9, ClefType::G_8VA );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_2_clef_C, Pitch )
{
    int expected = 53;
    int actual = Pitch::midiNoteForSlot( 2, ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n2_clef_C, Pitch )
{
    int expected = 47;
    int actual = Pitch::midiNoteForSlot( -2, ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_9_clef_C, Pitch )
{
    int expected = 65;
    int actual = Pitch::midiNoteForSlot( 9, ClefType::C );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_2_clef_F_15MB, Pitch )
{
    int expected = 43 - 24;
    int actual = Pitch::midiNoteForSlot( 2, ClefType::F_15MB );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n2_clef_F_15MB, Pitch )
{
    int expected = 36 - 24;
    int actual = Pitch::midiNoteForSlot( -2, ClefType::F_15MB );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_9_clef_F_15MB, Pitch )
{
    int expected = 55 - 24;
    int actual = Pitch::midiNoteForSlot( 9, ClefType::F_15MB );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_12_clef_F, Pitch )
{
    int expected = 60;
    int actual = Pitch::midiNoteForSlot( 12, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_11_clef_F, Pitch )
{
    int expected = 59;
    int actual = Pitch::midiNoteForSlot( 11, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_10_clef_F, Pitch )
{
    int expected = 57;
    int actual = Pitch::midiNoteForSlot( 10, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_9_clef_F, Pitch )
{
    int expected = 55;
    int actual = Pitch::midiNoteForSlot( 9, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_8_clef_F, Pitch )
{
    int expected = 53;
    int actual = Pitch::midiNoteForSlot( 8, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_7_clef_F, Pitch )
{
    int expected = 52;
    int actual = Pitch::midiNoteForSlot( 7, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_6_clef_F, Pitch )
{
    // D3
    int expected = 50;
    int actual = Pitch::midiNoteForSlot( 6, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_5_clef_F, Pitch )
{
    // C3
    int expected = 48;
    int actual = Pitch::midiNoteForSlot( 5, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_4_clef_F, Pitch )
{
    // B2
    int expected = 47;
    int actual = Pitch::midiNoteForSlot( 4, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_3_clef_F, Pitch )
{
    // A2
    int expected = 45;
    int actual = Pitch::midiNoteForSlot( 3, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_2_clef_F, Pitch )
{
    // G2
    int expected = 43;
    int actual = Pitch::midiNoteForSlot( 2, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_1_clef_F, Pitch )
{
    // F2
    int expected = 41;
    int actual = Pitch::midiNoteForSlot( 1, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_0_clef_F, Pitch )
{
    // E2
    int expected = 40;
    int actual = Pitch::midiNoteForSlot( 0, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n1_clef_F, Pitch )
{
    // D2
    int expected = 38;
    int actual = Pitch::midiNoteForSlot( -1, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n2_clef_F, Pitch )
{
    // C2
    int expected = 36;
    int actual = Pitch::midiNoteForSlot( -2, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n3_clef_F, Pitch )
{
    // B1
    int expected = 35;
    int actual = Pitch::midiNoteForSlot( -3, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n4_clef_F, Pitch )
{
    // A1
    int expected = 33;
    int actual = Pitch::midiNoteForSlot( -4, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n5_clef_F, Pitch )
{
    // G1
    int expected = 31;
    int actual = Pitch::midiNoteForSlot( -5, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n6_clef_F, Pitch )
{
    // F1
    int expected = 29;
    int actual = Pitch::midiNoteForSlot( -6, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( midiNoteForSlot_slot_n7_clef_F, Pitch )
{
    // E1
    int expected = 28;
    int actual = Pitch::midiNoteForSlot( -7, ClefType::F );
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( setString_Good_Gs6, Pitch )
{
    const std::string input = "G#4";
    const bool isSuccessExpected = true;
    const PitchStep expectedPitchStep = PitchStep::G;
    const int expectedOctave = 4;
    const int expectedAlter = 1;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Good_FbN1, Pitch )
{
    const std::string input = "Fb-1";
    const bool isSuccessExpected = true;
    const PitchStep expectedPitchStep = PitchStep::F;
    const int expectedOctave = -1;
    const int expectedAlter = -1;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Good_Ax0, Pitch )
{
    const std::string input = "Ax0";
    const bool isSuccessExpected = true;
    const PitchStep expectedPitchStep = PitchStep::A;
    const int expectedOctave = 0;
    const int expectedAlter = 2;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Good_Ebb9, Pitch )
{
    const std::string input = "Ebb9";
    const bool isSuccessExpected = true;
    const PitchStep expectedPitchStep = PitchStep::E;
    const int expectedOctave = 9;
    const int expectedAlter = -2;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Good_FbbN1, Pitch )
{
    const std::string input = "Fbb-1";
    const bool isSuccessExpected = true;
    const PitchStep expectedPitchStep = PitchStep::F;
    const int expectedOctave = -1;
    const int expectedAlter = -2;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Good_D5, Pitch )
{
    const std::string input = "D5";
    const bool isSuccessExpected = true;
    const PitchStep expectedPitchStep = PitchStep::D;
    const int expectedOctave = 5;
    const int expectedAlter = 0;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Bad_Poop, Pitch )
{
    const std::string input = "Poop";
    const bool isSuccessExpected = false;
    const PitchStep expectedPitchStep = PitchStep::C;
    const int expectedOctave = 4;
    const int expectedAlter = 0;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Bad_Cs, Pitch )
{
    const std::string input = "C#";
    const bool isSuccessExpected = false;
    const PitchStep expectedPitchStep = PitchStep::C;
    const int expectedOctave = 4;
    const int expectedAlter = 0;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( setString_Bad_CP1, Pitch )
{
    const std::string input = "C+1";
    const bool isSuccessExpected = false;
    const PitchStep expectedPitchStep = PitchStep::C;
    const int expectedOctave = 4;
    const int expectedAlter = 0;
    Pitch actual;
    const bool isSuccessActual = actual.setString(input);
    CHECK_EQUAL( isSuccessExpected, isSuccessActual );
    CHECK_EQUAL( expectedOctave, actual.octave );
    CHECK_EQUAL( expectedAlter, actual.alter );
    CHECK( expectedPitchStep == actual.step );
}
T_END


TEST( getString_C4, Pitch )
{
    const std::string expected = "C4";
    Pitch p;
    p.step = PitchStep::C;
    p.octave = 4;
    p.alter = 0;
    const std::string actual = p.getString();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( getString_BbN1, Pitch )
{
    const std::string expected = "Bb-1";
    Pitch p;
    p.step = PitchStep::B;
    p.octave = -1;
    p.alter = -1;
    const std::string actual = p.getString();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( getString_Dx5, Pitch )
{
    const std::string expected = "Dx5";
    Pitch p;
    p.step = PitchStep::D;
    p.octave = 5;
    p.alter = 2;
    const std::string actual = p.getString();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( getString_Ebb0, Pitch )
{
    const std::string expected = "Ebb0";
    Pitch p;
    p.step = PitchStep::E;
    p.octave = 0;
    p.alter = -2;
    const std::string actual = p.getString();
    CHECK_EQUAL( expected, actual );
}
T_END


TEST( getString_Fs6, Pitch )
{
    const std::string expected = "F#6";
    Pitch p;
    p.step = PitchStep::F;
    p.octave = 6;
    p.alter = 1;
    const std::string actual = p.getString();
    CHECK_EQUAL( expected, actual );
}
T_END

#endif

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
