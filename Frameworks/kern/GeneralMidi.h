// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <chrono>
#include <limits>

namespace kern
{
    /// This enum is being used as if each enumeration were an integer constant.
    /// As such, do not alter this enum in any way, it should never be changed.
    /// Use GeneralMidiToInt and IntToGeneralMidi to safely cast to and from int.
    ///
    enum class GeneralMidiProgram
    {
        UNKNOWN = 0,
        ACOUSTIC_GRAND_PIANO = 1,
        BRIGHT_ACOUSTIC_PIANO = 2,
        ELECTRIC_GRAND_PIANO = 3,
        HONKY_TONK_PIANO = 4,
        ELECTRIC_PIANO_1 = 5,
        ELECTRIC_PIANO_2 = 6,
        HARPSICHORD = 7,
        CLAVI = 8,
        CELESTA = 9,
        GLOCKENSPIEL = 10,
        MUSIC_BOX = 11,
        VIBRAPHONE = 12,
        MARIMBA = 13,
        XYLOPHONE = 14,
        TUBULAR_BELLS = 15,
        DULCIMER = 16,
        DRAWBAR_ORGAN = 17,
        PERCUSSIVE_ORGAN = 18,
        ROCK_ORGAN = 19,
        CHURCH_ORGAN = 20,
        REED_ORGAN = 21,
        ACCORDION = 22,
        HARMONICA = 23,
        TANGO_ACCORDION = 24,
        ACOUSTIC_GUITAR_NYLON = 25,
        ACOUSTIC_GUITAR_STEEL = 26,
        ELECTRIC_GUITAR_JAZZ = 27,
        ELECTRIC_GUITAR_CLEAN = 28,
        ELECTRIC_GUITAR_MUTED = 29,
        OVERDRIVEN_GUITAR = 30,
        DISTORTION_GUITAR = 31,
        GUITAR_HARMONICS = 32,
        ACOUSTIC_BASS = 33,
        ELECTRIC_BASS_FINGER = 34,
        ELECTRIC_BASS_PICK = 35,
        FRETLESS_BASS = 36,
        SLAP_BASS_1 = 37,
        SLAP_BASS_2 = 38,
        SYNTH_BASS_1 = 39,
        SYNTH_BASS_2 = 40,
        VIOLIN = 41,
        VIOLA = 42,
        CELLO = 43,
        CONTRABASS = 44,
        TREMOLO_STRINGS = 45,
        PIZZICATO_STRINGS = 46,
        ORCHESTRAL_HARP = 47,
        TIMPANI = 48,
        STRING_ENSEMBLE_1 = 49,
        STRING_ENSEMBLE_2 = 50,
        SYNTHSTRINGS_1 = 51,
        SYNTHSTRINGS_2 = 52,
        CHOIR_AAHS = 53,
        VOICE_OOHS = 54,
        SYNTH_VOICE = 55,
        ORCHESTRA_HIT = 56,
        TRUMPET = 57,
        TROMBONE = 58,
        TUBA = 59,
        MUTED_TRUMPET = 60,
        FRENCH_HORN = 61,
        BRASS_SECTION = 62,
        SYNTHBRASS_1 = 63,
        SYNTHBRASS_2 = 64,
        SOPRANO_SAX = 65,
        ALTO_SAX = 66,
        TENOR_SAX = 67,
        BARITONE_SAX = 68,
        OBOE = 69,
        ENGLISH_HORN = 70,
        BASSOON = 71,
        CLARINET = 72,
        PICCOLO = 73,
        FLUTE = 74,
        RECORDER = 75,
        PAN_FLUTE = 76,
        BLOWN_BOTTLE = 77,
        SHAKUHACHI = 78,
        WHISTLE = 79,
        OCARINA = 80,
        LEAD_1_SQUARE = 81,
        LEAD_2_SAWTOOTH = 82,
        LEAD_3_CALLIOPE = 83,
        LEAD_4_CHIFF = 84,
        LEAD_5_CHARANG = 85,
        LEAD_6_VOICE = 86,
        LEAD_7_FIFTHS = 87,
        LEAD_8_BASS_LEAD = 88,
        PAD_1_NEW_AGE = 89,
        PAD_2_WARM = 90,
        PAD_3_POLYSYNTH = 91,
        PAD_4_CHOIR = 92,
        PAD_5_BOWED = 93,
        PAD_6_METALLIC = 94,
        PAD_7_HALO = 95,
        PAD_8_SWEEP = 96,
        FX_1_RAIN = 97,
        FX_2_SOUNDTRACK = 98,
        FX_3_CRYSTAL = 99,
        FX_4_ATMOSPHERE = 100,
        FX_5_BRIGHTNESS = 101,
        FX_6_GOBLINS = 102,
        FX_7_ECHOES = 103,
        FX_8_SCI_FI = 104,
        SITAR = 105,
        BANJO = 106,
        SHAMISEN = 107,
        KOTO = 108,
        KALIMBA = 109,
        BAG_PIPE = 110,
        FIDDLE = 111,
        SHANAI = 112,
        TINKLE_BELL = 113,
        AGOGO = 114,
        STEEL_DRUMS = 115,
        WOODBLOCK = 116,
        TAIKO_DRUM = 117,
        MELODIC_TOM = 118,
        SYNTH_DRUM = 119,
        REVERSE_CYMBAL = 120,
        GUITAR_FRET_NOISE = 121,
        BREATH_NOISE = 122,
        SEASHORE = 123,
        BIRD_TWEET = 124,
        TELEPHONE_RING = 125,
        HELICOPTER = 126,
        APPLAUSE = 127,
        GUNSHOT = 128,
    };

    constexpr const char* const GM_STRING_UNKNOWN = "UNKNOWN";
    constexpr const char* const GM_STRING_ACOUSTIC_GRAND_PIANO = "Acoustic Grand Piano";
    constexpr const char* const GM_STRING_BRIGHT_ACOUSTIC_PIANO = "Bright Acoustic Piano";
    constexpr const char* const GM_STRING_ELECTRIC_GRAND_PIANO = "Electric Grand Piano";
    constexpr const char* const GM_STRING_HONKY_TONK_PIANO = "Honky-tonk Piano";
    constexpr const char* const GM_STRING_ELECTRIC_PIANO_1 = "Electric Piano 1";
    constexpr const char* const GM_STRING_ELECTRIC_PIANO_2 = "Electric Piano 2";
    constexpr const char* const GM_STRING_HARPSICHORD = "Harpsichord";
    constexpr const char* const GM_STRING_CLAVI = "Clavi";
    constexpr const char* const GM_STRING_CELESTA = "Celesta";
    constexpr const char* const GM_STRING_GLOCKENSPIEL = "Glockenspiel";
    constexpr const char* const GM_STRING_MUSIC_BOX = "Music Box";
    constexpr const char* const GM_STRING_VIBRAPHONE = "Vibraphone";
    constexpr const char* const GM_STRING_MARIMBA = "Marimba";
    constexpr const char* const GM_STRING_XYLOPHONE = "Xylophone";
    constexpr const char* const GM_STRING_TUBULAR_BELLS = "Tubular Bells";
    constexpr const char* const GM_STRING_DULCIMER = "Dulcimer";
    constexpr const char* const GM_STRING_DRAWBAR_ORGAN = "Drawbar Organ";
    constexpr const char* const GM_STRING_PERCUSSIVE_ORGAN = "Percussive Organ";
    constexpr const char* const GM_STRING_ROCK_ORGAN = "Rock Organ";
    constexpr const char* const GM_STRING_CHURCH_ORGAN = "Church Organ";
    constexpr const char* const GM_STRING_REED_ORGAN = "Reed Organ";
    constexpr const char* const GM_STRING_ACCORDION = "Accordion";
    constexpr const char* const GM_STRING_HARMONICA = "Harmonica";
    constexpr const char* const GM_STRING_TANGO_ACCORDION = "Tango Accordion";
    constexpr const char* const GM_STRING_ACOUSTIC_GUITAR_NYLON = "Acoustic Guitar (nylon)";
    constexpr const char* const GM_STRING_ACOUSTIC_GUITAR_STEEL = "Acoustic Guitar (steel)";
    constexpr const char* const GM_STRING_ELECTRIC_GUITAR_JAZZ = "Electric Guitar (jazz)";
    constexpr const char* const GM_STRING_ELECTRIC_GUITAR_CLEAN = "Electric Guitar (clean)";
    constexpr const char* const GM_STRING_ELECTRIC_GUITAR_MUTED = "Electric Guitar (muted)";
    constexpr const char* const GM_STRING_OVERDRIVEN_GUITAR = "Overdriven Guitar";
    constexpr const char* const GM_STRING_DISTORTION_GUITAR = "Distortion Guitar";
    constexpr const char* const GM_STRING_GUITAR_HARMONICS = "Guitar harmonics";
    constexpr const char* const GM_STRING_ACOUSTIC_BASS = "Acoustic Bass";
    constexpr const char* const GM_STRING_ELECTRIC_BASS_FINGER = "Electric Bass (finger)";
    constexpr const char* const GM_STRING_ELECTRIC_BASS_PICK = "Electric Bass (pick)";
    constexpr const char* const GM_STRING_FRETLESS_BASS = "Fretless Bass";
    constexpr const char* const GM_STRING_SLAP_BASS_1 = "Slap Bass 1";
    constexpr const char* const GM_STRING_SLAP_BASS_2 = "Slap Bass 2";
    constexpr const char* const GM_STRING_SYNTH_BASS_1 = "Synth Bass 1";
    constexpr const char* const GM_STRING_SYNTH_BASS_2 = "Synth Bass 2";
    constexpr const char* const GM_STRING_VIOLIN = "Violin";
    constexpr const char* const GM_STRING_VIOLA = "Viola";
    constexpr const char* const GM_STRING_CELLO = "Cello";
    constexpr const char* const GM_STRING_CONTRABASS = "Contrabass";
    constexpr const char* const GM_STRING_TREMOLO_STRINGS = "Tremolo Strings";
    constexpr const char* const GM_STRING_PIZZICATO_STRINGS = "Pizzicato Strings";
    constexpr const char* const GM_STRING_ORCHESTRAL_HARP = "Orchestral Harp";
    constexpr const char* const GM_STRING_TIMPANI = "Timpani";
    constexpr const char* const GM_STRING_STRING_ENSEMBLE_1 = "String Ensemble 1";
    constexpr const char* const GM_STRING_STRING_ENSEMBLE_2 = "String Ensemble 2";
    constexpr const char* const GM_STRING_SYNTHSTRINGS_1 = "SynthStrings 1";
    constexpr const char* const GM_STRING_SYNTHSTRINGS_2 = "SynthStrings 2";
    constexpr const char* const GM_STRING_CHOIR_AAHS = "Choir Aahs";
    constexpr const char* const GM_STRING_VOICE_OOHS = "Voice Oohs";
    constexpr const char* const GM_STRING_SYNTH_VOICE = "Synth Voice";
    constexpr const char* const GM_STRING_ORCHESTRA_HIT = "Orchestra Hit";
    constexpr const char* const GM_STRING_TRUMPET = "Trumpet";
    constexpr const char* const GM_STRING_TROMBONE = "Trombone";
    constexpr const char* const GM_STRING_TUBA = "Tuba";
    constexpr const char* const GM_STRING_MUTED_TRUMPET = "Muted Trumpet";
    constexpr const char* const GM_STRING_FRENCH_HORN = "French Horn";
    constexpr const char* const GM_STRING_BRASS_SECTION = "Brass Section";
    constexpr const char* const GM_STRING_SYNTHBRASS_1 = "SynthBrass 1";
    constexpr const char* const GM_STRING_SYNTHBRASS_2 = "SynthBrass 2";
    constexpr const char* const GM_STRING_SOPRANO_SAX = "Soprano Sax";
    constexpr const char* const GM_STRING_ALTO_SAX = "Alto Sax";
    constexpr const char* const GM_STRING_TENOR_SAX = "Tenor Sax";
    constexpr const char* const GM_STRING_BARITONE_SAX = "Baritone Sax";
    constexpr const char* const GM_STRING_OBOE = "Oboe";
    constexpr const char* const GM_STRING_ENGLISH_HORN = "English Horn";
    constexpr const char* const GM_STRING_BASSOON = "Bassoon";
    constexpr const char* const GM_STRING_CLARINET = "Clarinet";
    constexpr const char* const GM_STRING_PICCOLO = "Piccolo";
    constexpr const char* const GM_STRING_FLUTE = "Flute";
    constexpr const char* const GM_STRING_RECORDER = "Recorder";
    constexpr const char* const GM_STRING_PAN_FLUTE = "Pan Flute";
    constexpr const char* const GM_STRING_BLOWN_BOTTLE = "Blown Bottle";
    constexpr const char* const GM_STRING_SHAKUHACHI = "Shakuhachi";
    constexpr const char* const GM_STRING_WHISTLE = "Whistle";
    constexpr const char* const GM_STRING_OCARINA = "Ocarina";
    constexpr const char* const GM_STRING_LEAD_1_SQUARE = "Lead 1 (square)";
    constexpr const char* const GM_STRING_LEAD_2_SAWTOOTH = "Lead 2 (sawtooth)";
    constexpr const char* const GM_STRING_LEAD_3_CALLIOPE = "Lead 3 (calliope)";
    constexpr const char* const GM_STRING_LEAD_4_CHIFF = "Lead 4 (chiff)";
    constexpr const char* const GM_STRING_LEAD_5_CHARANG = "Lead 5 (charang)";
    constexpr const char* const GM_STRING_LEAD_6_VOICE = "Lead 6 (voice)";
    constexpr const char* const GM_STRING_LEAD_7_FIFTHS = "Lead 7 (fifths)";
    constexpr const char* const GM_STRING_LEAD_8_BASS_LEAD = "Lead 8 (bass + lead)";
    constexpr const char* const GM_STRING_PAD_1_NEW_AGE = "Pad 1 (new age)";
    constexpr const char* const GM_STRING_PAD_2_WARM = "Pad 2 (warm)";
    constexpr const char* const GM_STRING_PAD_3_POLYSYNTH = "Pad 3 (polysynth)";
    constexpr const char* const GM_STRING_PAD_4_CHOIR = "Pad 4 (choir)";
    constexpr const char* const GM_STRING_PAD_5_BOWED = "Pad 5 (bowed)";
    constexpr const char* const GM_STRING_PAD_6_METALLIC = "Pad 6 (metallic)";
    constexpr const char* const GM_STRING_PAD_7_HALO = "Pad 7 (halo)";
    constexpr const char* const GM_STRING_PAD_8_SWEEP = "Pad 8 (sweep)";
    constexpr const char* const GM_STRING_FX_1_RAIN = "FX 1 (rain)";
    constexpr const char* const GM_STRING_FX_2_SOUNDTRACK = "FX 2 (soundtrack)";
    constexpr const char* const GM_STRING_FX_3_CRYSTAL = "FX 3 (crystal)";
    constexpr const char* const GM_STRING_FX_4_ATMOSPHERE = "FX 4 (atmosphere)";
    constexpr const char* const GM_STRING_FX_5_BRIGHTNESS = "FX 5 (brightness)";
    constexpr const char* const GM_STRING_FX_6_GOBLINS = "FX 6 (goblins)";
    constexpr const char* const GM_STRING_FX_7_ECHOES = "FX 7 (echoes)";
    constexpr const char* const GM_STRING_FX_8_SCI_FI = "FX 8 (sci-fi)";
    constexpr const char* const GM_STRING_SITAR = "Sitar";
    constexpr const char* const GM_STRING_BANJO = "Banjo";
    constexpr const char* const GM_STRING_SHAMISEN = "Shamisen";
    constexpr const char* const GM_STRING_KOTO = "Koto";
    constexpr const char* const GM_STRING_KALIMBA = "Kalimba";
    constexpr const char* const GM_STRING_BAG_PIPE = "Bag pipe";
    constexpr const char* const GM_STRING_FIDDLE = "Fiddle";
    constexpr const char* const GM_STRING_SHANAI = "Shanai";
    constexpr const char* const GM_STRING_TINKLE_BELL = "Tinkle Bell";
    constexpr const char* const GM_STRING_AGOGO = "Agogo";
    constexpr const char* const GM_STRING_STEEL_DRUMS = "Steel Drums";
    constexpr const char* const GM_STRING_WOODBLOCK = "Woodblock";
    constexpr const char* const GM_STRING_TAIKO_DRUM = "Taiko Drum";
    constexpr const char* const GM_STRING_MELODIC_TOM = "Melodic Tom";
    constexpr const char* const GM_STRING_SYNTH_DRUM = "Synth Drum";
    constexpr const char* const GM_STRING_REVERSE_CYMBAL = "Reverse Cymbal";
    constexpr const char* const GM_STRING_GUITAR_FRET_NOISE = "Guitar Fret Noise";
    constexpr const char* const GM_STRING_BREATH_NOISE = "Breath Noise";
    constexpr const char* const GM_STRING_SEASHORE = "Seashore";
    constexpr const char* const GM_STRING_BIRD_TWEET = "Bird Tweet";
    constexpr const char* const GM_STRING_TELEPHONE_RING = "Telephone Ring";
    constexpr const char* const GM_STRING_HELICOPTER = "Helicopter";
    constexpr const char* const GM_STRING_APPLAUSE = "Applause";
    constexpr const char* const GM_STRING_GUNSHOT = "Gunshot";

    inline const char* const GeneralMidiToString( GeneralMidiProgram inValue )
    {
        switch (inValue)
        {
            case GeneralMidiProgram::UNKNOWN: return GM_STRING_UNKNOWN;
            case GeneralMidiProgram::ACOUSTIC_GRAND_PIANO: return GM_STRING_ACOUSTIC_GRAND_PIANO;
            case GeneralMidiProgram::BRIGHT_ACOUSTIC_PIANO: return GM_STRING_BRIGHT_ACOUSTIC_PIANO;
            case GeneralMidiProgram::ELECTRIC_GRAND_PIANO: return GM_STRING_ELECTRIC_GRAND_PIANO;
            case GeneralMidiProgram::HONKY_TONK_PIANO: return GM_STRING_HONKY_TONK_PIANO;
            case GeneralMidiProgram::ELECTRIC_PIANO_1: return GM_STRING_ELECTRIC_PIANO_1;
            case GeneralMidiProgram::ELECTRIC_PIANO_2: return GM_STRING_ELECTRIC_PIANO_2;
            case GeneralMidiProgram::HARPSICHORD: return GM_STRING_HARPSICHORD;
            case GeneralMidiProgram::CLAVI: return GM_STRING_CLAVI;
            case GeneralMidiProgram::CELESTA: return GM_STRING_CELESTA;
            case GeneralMidiProgram::GLOCKENSPIEL: return GM_STRING_GLOCKENSPIEL;
            case GeneralMidiProgram::MUSIC_BOX: return GM_STRING_MUSIC_BOX;
            case GeneralMidiProgram::VIBRAPHONE: return GM_STRING_VIBRAPHONE;
            case GeneralMidiProgram::MARIMBA: return GM_STRING_MARIMBA;
            case GeneralMidiProgram::XYLOPHONE: return GM_STRING_XYLOPHONE;
            case GeneralMidiProgram::TUBULAR_BELLS: return GM_STRING_TUBULAR_BELLS;
            case GeneralMidiProgram::DULCIMER: return GM_STRING_DULCIMER;
            case GeneralMidiProgram::DRAWBAR_ORGAN: return GM_STRING_DRAWBAR_ORGAN;
            case GeneralMidiProgram::PERCUSSIVE_ORGAN: return GM_STRING_PERCUSSIVE_ORGAN;
            case GeneralMidiProgram::ROCK_ORGAN: return GM_STRING_ROCK_ORGAN;
            case GeneralMidiProgram::CHURCH_ORGAN: return GM_STRING_CHURCH_ORGAN;
            case GeneralMidiProgram::REED_ORGAN: return GM_STRING_REED_ORGAN;
            case GeneralMidiProgram::ACCORDION: return GM_STRING_ACCORDION;
            case GeneralMidiProgram::HARMONICA: return GM_STRING_HARMONICA;
            case GeneralMidiProgram::TANGO_ACCORDION: return GM_STRING_TANGO_ACCORDION;
            case GeneralMidiProgram::ACOUSTIC_GUITAR_NYLON: return GM_STRING_ACOUSTIC_GUITAR_NYLON;
            case GeneralMidiProgram::ACOUSTIC_GUITAR_STEEL: return GM_STRING_ACOUSTIC_GUITAR_STEEL;
            case GeneralMidiProgram::ELECTRIC_GUITAR_JAZZ: return GM_STRING_ELECTRIC_GUITAR_JAZZ;
            case GeneralMidiProgram::ELECTRIC_GUITAR_CLEAN: return GM_STRING_ELECTRIC_GUITAR_CLEAN;
            case GeneralMidiProgram::ELECTRIC_GUITAR_MUTED: return GM_STRING_ELECTRIC_GUITAR_MUTED;
            case GeneralMidiProgram::OVERDRIVEN_GUITAR: return GM_STRING_OVERDRIVEN_GUITAR;
            case GeneralMidiProgram::DISTORTION_GUITAR: return GM_STRING_DISTORTION_GUITAR;
            case GeneralMidiProgram::GUITAR_HARMONICS: return GM_STRING_GUITAR_HARMONICS;
            case GeneralMidiProgram::ACOUSTIC_BASS: return GM_STRING_ACOUSTIC_BASS;
            case GeneralMidiProgram::ELECTRIC_BASS_FINGER: return GM_STRING_ELECTRIC_BASS_FINGER;
            case GeneralMidiProgram::ELECTRIC_BASS_PICK: return GM_STRING_ELECTRIC_BASS_PICK;
            case GeneralMidiProgram::FRETLESS_BASS: return GM_STRING_FRETLESS_BASS;
            case GeneralMidiProgram::SLAP_BASS_1: return GM_STRING_SLAP_BASS_1;
            case GeneralMidiProgram::SLAP_BASS_2: return GM_STRING_SLAP_BASS_2;
            case GeneralMidiProgram::SYNTH_BASS_1: return GM_STRING_SYNTH_BASS_1;
            case GeneralMidiProgram::SYNTH_BASS_2: return GM_STRING_SYNTH_BASS_2;
            case GeneralMidiProgram::VIOLIN: return GM_STRING_VIOLIN;
            case GeneralMidiProgram::VIOLA: return GM_STRING_VIOLA;
            case GeneralMidiProgram::CELLO: return GM_STRING_CELLO;
            case GeneralMidiProgram::CONTRABASS: return GM_STRING_CONTRABASS;
            case GeneralMidiProgram::TREMOLO_STRINGS: return GM_STRING_TREMOLO_STRINGS;
            case GeneralMidiProgram::PIZZICATO_STRINGS: return GM_STRING_PIZZICATO_STRINGS;
            case GeneralMidiProgram::ORCHESTRAL_HARP: return GM_STRING_ORCHESTRAL_HARP;
            case GeneralMidiProgram::TIMPANI: return GM_STRING_TIMPANI;
            case GeneralMidiProgram::STRING_ENSEMBLE_1: return GM_STRING_STRING_ENSEMBLE_1;
            case GeneralMidiProgram::STRING_ENSEMBLE_2: return GM_STRING_STRING_ENSEMBLE_2;
            case GeneralMidiProgram::SYNTHSTRINGS_1: return GM_STRING_SYNTHSTRINGS_1;
            case GeneralMidiProgram::SYNTHSTRINGS_2: return GM_STRING_SYNTHSTRINGS_2;
            case GeneralMidiProgram::CHOIR_AAHS: return GM_STRING_CHOIR_AAHS;
            case GeneralMidiProgram::VOICE_OOHS: return GM_STRING_VOICE_OOHS;
            case GeneralMidiProgram::SYNTH_VOICE: return GM_STRING_SYNTH_VOICE;
            case GeneralMidiProgram::ORCHESTRA_HIT: return GM_STRING_ORCHESTRA_HIT;
            case GeneralMidiProgram::TRUMPET: return GM_STRING_TRUMPET;
            case GeneralMidiProgram::TROMBONE: return GM_STRING_TROMBONE;
            case GeneralMidiProgram::TUBA: return GM_STRING_TUBA;
            case GeneralMidiProgram::MUTED_TRUMPET: return GM_STRING_MUTED_TRUMPET;
            case GeneralMidiProgram::FRENCH_HORN: return GM_STRING_FRENCH_HORN;
            case GeneralMidiProgram::BRASS_SECTION: return GM_STRING_BRASS_SECTION;
            case GeneralMidiProgram::SYNTHBRASS_1: return GM_STRING_SYNTHBRASS_1;
            case GeneralMidiProgram::SYNTHBRASS_2: return GM_STRING_SYNTHBRASS_2;
            case GeneralMidiProgram::SOPRANO_SAX: return GM_STRING_SOPRANO_SAX;
            case GeneralMidiProgram::ALTO_SAX: return GM_STRING_ALTO_SAX;
            case GeneralMidiProgram::TENOR_SAX: return GM_STRING_TENOR_SAX;
            case GeneralMidiProgram::BARITONE_SAX: return GM_STRING_BARITONE_SAX;
            case GeneralMidiProgram::OBOE: return GM_STRING_OBOE;
            case GeneralMidiProgram::ENGLISH_HORN: return GM_STRING_ENGLISH_HORN;
            case GeneralMidiProgram::BASSOON: return GM_STRING_BASSOON;
            case GeneralMidiProgram::CLARINET: return GM_STRING_CLARINET;
            case GeneralMidiProgram::PICCOLO: return GM_STRING_PICCOLO;
            case GeneralMidiProgram::FLUTE: return GM_STRING_FLUTE;
            case GeneralMidiProgram::RECORDER: return GM_STRING_RECORDER;
            case GeneralMidiProgram::PAN_FLUTE: return GM_STRING_PAN_FLUTE;
            case GeneralMidiProgram::BLOWN_BOTTLE: return GM_STRING_BLOWN_BOTTLE;
            case GeneralMidiProgram::SHAKUHACHI: return GM_STRING_SHAKUHACHI;
            case GeneralMidiProgram::WHISTLE: return GM_STRING_WHISTLE;
            case GeneralMidiProgram::OCARINA: return GM_STRING_OCARINA;
            case GeneralMidiProgram::LEAD_1_SQUARE: return GM_STRING_LEAD_1_SQUARE;
            case GeneralMidiProgram::LEAD_2_SAWTOOTH: return GM_STRING_LEAD_2_SAWTOOTH;
            case GeneralMidiProgram::LEAD_3_CALLIOPE: return GM_STRING_LEAD_3_CALLIOPE;
            case GeneralMidiProgram::LEAD_4_CHIFF: return GM_STRING_LEAD_4_CHIFF;
            case GeneralMidiProgram::LEAD_5_CHARANG: return GM_STRING_LEAD_5_CHARANG;
            case GeneralMidiProgram::LEAD_6_VOICE: return GM_STRING_LEAD_6_VOICE;
            case GeneralMidiProgram::LEAD_7_FIFTHS: return GM_STRING_LEAD_7_FIFTHS;
            case GeneralMidiProgram::LEAD_8_BASS_LEAD: return GM_STRING_LEAD_8_BASS_LEAD;
            case GeneralMidiProgram::PAD_1_NEW_AGE: return GM_STRING_PAD_1_NEW_AGE;
            case GeneralMidiProgram::PAD_2_WARM: return GM_STRING_PAD_2_WARM;
            case GeneralMidiProgram::PAD_3_POLYSYNTH: return GM_STRING_PAD_3_POLYSYNTH;
            case GeneralMidiProgram::PAD_4_CHOIR: return GM_STRING_PAD_4_CHOIR;
            case GeneralMidiProgram::PAD_5_BOWED: return GM_STRING_PAD_5_BOWED;
            case GeneralMidiProgram::PAD_6_METALLIC: return GM_STRING_PAD_6_METALLIC;
            case GeneralMidiProgram::PAD_7_HALO: return GM_STRING_PAD_7_HALO;
            case GeneralMidiProgram::PAD_8_SWEEP: return GM_STRING_PAD_8_SWEEP;
            case GeneralMidiProgram::FX_1_RAIN: return GM_STRING_FX_1_RAIN;
            case GeneralMidiProgram::FX_2_SOUNDTRACK: return GM_STRING_FX_2_SOUNDTRACK;
            case GeneralMidiProgram::FX_3_CRYSTAL: return GM_STRING_FX_3_CRYSTAL;
            case GeneralMidiProgram::FX_4_ATMOSPHERE: return GM_STRING_FX_4_ATMOSPHERE;
            case GeneralMidiProgram::FX_5_BRIGHTNESS: return GM_STRING_FX_5_BRIGHTNESS;
            case GeneralMidiProgram::FX_6_GOBLINS: return GM_STRING_FX_6_GOBLINS;
            case GeneralMidiProgram::FX_7_ECHOES: return GM_STRING_FX_7_ECHOES;
            case GeneralMidiProgram::FX_8_SCI_FI: return GM_STRING_FX_8_SCI_FI;
            case GeneralMidiProgram::SITAR: return GM_STRING_SITAR;
            case GeneralMidiProgram::BANJO: return GM_STRING_BANJO;
            case GeneralMidiProgram::SHAMISEN: return GM_STRING_SHAMISEN;
            case GeneralMidiProgram::KOTO: return GM_STRING_KOTO;
            case GeneralMidiProgram::KALIMBA: return GM_STRING_KALIMBA;
            case GeneralMidiProgram::BAG_PIPE: return GM_STRING_BAG_PIPE;
            case GeneralMidiProgram::FIDDLE: return GM_STRING_FIDDLE;
            case GeneralMidiProgram::SHANAI: return GM_STRING_SHANAI;
            case GeneralMidiProgram::TINKLE_BELL: return GM_STRING_TINKLE_BELL;
            case GeneralMidiProgram::AGOGO: return GM_STRING_AGOGO;
            case GeneralMidiProgram::STEEL_DRUMS: return GM_STRING_STEEL_DRUMS;
            case GeneralMidiProgram::WOODBLOCK: return GM_STRING_WOODBLOCK;
            case GeneralMidiProgram::TAIKO_DRUM: return GM_STRING_TAIKO_DRUM;
            case GeneralMidiProgram::MELODIC_TOM: return GM_STRING_MELODIC_TOM;
            case GeneralMidiProgram::SYNTH_DRUM: return GM_STRING_SYNTH_DRUM;
            case GeneralMidiProgram::REVERSE_CYMBAL: return GM_STRING_REVERSE_CYMBAL;
            case GeneralMidiProgram::GUITAR_FRET_NOISE: return GM_STRING_GUITAR_FRET_NOISE;
            case GeneralMidiProgram::BREATH_NOISE: return GM_STRING_BREATH_NOISE;
            case GeneralMidiProgram::SEASHORE: return GM_STRING_SEASHORE;
            case GeneralMidiProgram::BIRD_TWEET: return GM_STRING_BIRD_TWEET;
            case GeneralMidiProgram::TELEPHONE_RING: return GM_STRING_TELEPHONE_RING;
            case GeneralMidiProgram::HELICOPTER: return GM_STRING_HELICOPTER;
            case GeneralMidiProgram::APPLAUSE: return GM_STRING_APPLAUSE;
            case GeneralMidiProgram::GUNSHOT: return GM_STRING_GUNSHOT;
            default: break;
        }

        return GM_STRING_UNKNOWN;
    }

    inline GeneralMidiProgram StringToGeneralMidiProgram( const char* const inString )
    {
        if( std::strcmp( inString, GM_STRING_UNKNOWN ) == 0 )
        {
            return GeneralMidiProgram::UNKNOWN;
        }
        else if( std::strcmp( inString, GM_STRING_ACOUSTIC_GRAND_PIANO ) )
        {
            return GeneralMidiProgram::ACOUSTIC_GRAND_PIANO;
        }
        else if( std::strcmp( inString, GM_STRING_BRIGHT_ACOUSTIC_PIANO ) )
        {
            return GeneralMidiProgram::BRIGHT_ACOUSTIC_PIANO;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_GRAND_PIANO ) )
        {
            return GeneralMidiProgram::ELECTRIC_GRAND_PIANO;
        }
        else if( std::strcmp( inString, GM_STRING_HONKY_TONK_PIANO ) )
        {
            return GeneralMidiProgram::HONKY_TONK_PIANO;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_PIANO_1 ) )
        {
            return GeneralMidiProgram::ELECTRIC_PIANO_1;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_PIANO_2 ) )
        {
            return GeneralMidiProgram::ELECTRIC_PIANO_2;
        }
        else if( std::strcmp( inString, GM_STRING_HARPSICHORD ) )
        {
            return GeneralMidiProgram::HARPSICHORD;
        }
        else if( std::strcmp( inString, GM_STRING_CLAVI ) )
        {
            return GeneralMidiProgram::CLAVI;
        }
        else if( std::strcmp( inString, GM_STRING_CELESTA ) )
        {
            return GeneralMidiProgram::CELESTA;
        }
        else if( std::strcmp( inString, GM_STRING_GLOCKENSPIEL ) )
        {
            return GeneralMidiProgram::GLOCKENSPIEL;
        }
        else if( std::strcmp( inString, GM_STRING_MUSIC_BOX ) )
        {
            return GeneralMidiProgram::MUSIC_BOX;
        }
        else if( std::strcmp( inString, GM_STRING_VIBRAPHONE ) )
        {
            return GeneralMidiProgram::VIBRAPHONE;
        }
        else if( std::strcmp( inString, GM_STRING_MARIMBA ) )
        {
            return GeneralMidiProgram::MARIMBA;
        }
        else if( std::strcmp( inString, GM_STRING_XYLOPHONE ) )
        {
            return GeneralMidiProgram::XYLOPHONE;
        }
        else if( std::strcmp( inString, GM_STRING_TUBULAR_BELLS ) )
        {
            return GeneralMidiProgram::TUBULAR_BELLS;
        }
        else if( std::strcmp( inString, GM_STRING_DULCIMER ) )
        {
            return GeneralMidiProgram::DULCIMER;
        }
        else if( std::strcmp( inString, GM_STRING_DRAWBAR_ORGAN ) )
        {
            return GeneralMidiProgram::DRAWBAR_ORGAN;
        }
        else if( std::strcmp( inString, GM_STRING_PERCUSSIVE_ORGAN ) )
        {
            return GeneralMidiProgram::PERCUSSIVE_ORGAN;
        }
        else if( std::strcmp( inString, GM_STRING_ROCK_ORGAN ) )
        {
            return GeneralMidiProgram::ROCK_ORGAN;
        }
        else if( std::strcmp( inString, GM_STRING_CHURCH_ORGAN ) )
        {
            return GeneralMidiProgram::CHURCH_ORGAN;
        }
        else if( std::strcmp( inString, GM_STRING_REED_ORGAN ) )
        {
            return GeneralMidiProgram::REED_ORGAN;
        }
        else if( std::strcmp( inString, GM_STRING_ACCORDION ) )
        {
            return GeneralMidiProgram::ACCORDION;
        }
        else if( std::strcmp( inString, GM_STRING_HARMONICA ) )
        {
            return GeneralMidiProgram::HARMONICA;
        }
        else if( std::strcmp( inString, GM_STRING_TANGO_ACCORDION ) )
        {
            return GeneralMidiProgram::TANGO_ACCORDION;
        }
        else if( std::strcmp( inString, GM_STRING_ACOUSTIC_GUITAR_NYLON ) )
        {
            return GeneralMidiProgram::ACOUSTIC_GUITAR_NYLON;
        }
        else if( std::strcmp( inString, GM_STRING_ACOUSTIC_GUITAR_STEEL ) )
        {
            return GeneralMidiProgram::ACOUSTIC_GUITAR_STEEL;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_GUITAR_JAZZ ) )
        {
            return GeneralMidiProgram::ELECTRIC_GUITAR_JAZZ;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_GUITAR_CLEAN ) )
        {
            return GeneralMidiProgram::ELECTRIC_GUITAR_CLEAN;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_GUITAR_MUTED ) )
        {
            return GeneralMidiProgram::ELECTRIC_GUITAR_MUTED;
        }
        else if( std::strcmp( inString, GM_STRING_OVERDRIVEN_GUITAR ) )
        {
            return GeneralMidiProgram::OVERDRIVEN_GUITAR;
        }
        else if( std::strcmp( inString, GM_STRING_DISTORTION_GUITAR ) )
        {
            return GeneralMidiProgram::DISTORTION_GUITAR;
        }
        else if( std::strcmp( inString, GM_STRING_GUITAR_HARMONICS ) )
        {
            return GeneralMidiProgram::GUITAR_HARMONICS;
        }
        else if( std::strcmp( inString, GM_STRING_ACOUSTIC_BASS ) )
        {
            return GeneralMidiProgram::ACOUSTIC_BASS;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_BASS_FINGER ) )
        {
            return GeneralMidiProgram::ELECTRIC_BASS_FINGER;
        }
        else if( std::strcmp( inString, GM_STRING_ELECTRIC_BASS_PICK ) )
        {
            return GeneralMidiProgram::ELECTRIC_BASS_PICK;
        }
        else if( std::strcmp( inString, GM_STRING_FRETLESS_BASS ) )
        {
            return GeneralMidiProgram::FRETLESS_BASS;
        }
        else if( std::strcmp( inString, GM_STRING_SLAP_BASS_1 ) )
        {
            return GeneralMidiProgram::SLAP_BASS_1;
        }
        else if( std::strcmp( inString, GM_STRING_SLAP_BASS_2 ) )
        {
            return GeneralMidiProgram::SLAP_BASS_2;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTH_BASS_1 ) )
        {
            return GeneralMidiProgram::SYNTH_BASS_1;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTH_BASS_2 ) )
        {
            return GeneralMidiProgram::SYNTH_BASS_2;
        }
        else if( std::strcmp( inString, GM_STRING_VIOLIN ) )
        {
            return GeneralMidiProgram::VIOLIN;
        }
        else if( std::strcmp( inString, GM_STRING_VIOLA ) )
        {
            return GeneralMidiProgram::VIOLA;
        }
        else if( std::strcmp( inString, GM_STRING_CELLO ) )
        {
            return GeneralMidiProgram::CELLO;
        }
        else if( std::strcmp( inString, GM_STRING_CONTRABASS ) )
        {
            return GeneralMidiProgram::CONTRABASS;
        }
        else if( std::strcmp( inString, GM_STRING_TREMOLO_STRINGS ) )
        {
            return GeneralMidiProgram::TREMOLO_STRINGS;
        }
        else if( std::strcmp( inString, GM_STRING_PIZZICATO_STRINGS ) )
        {
            return GeneralMidiProgram::PIZZICATO_STRINGS;
        }
        else if( std::strcmp( inString, GM_STRING_ORCHESTRAL_HARP ) )
        {
            return GeneralMidiProgram::ORCHESTRAL_HARP;
        }
        else if( std::strcmp( inString, GM_STRING_TIMPANI ) )
        {
            return GeneralMidiProgram::TIMPANI;
        }
        else if( std::strcmp( inString, GM_STRING_STRING_ENSEMBLE_1 ) )
        {
            return GeneralMidiProgram::STRING_ENSEMBLE_1;
        }
        else if( std::strcmp( inString, GM_STRING_STRING_ENSEMBLE_2 ) )
        {
            return GeneralMidiProgram::STRING_ENSEMBLE_2;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTHSTRINGS_1 ) )
        {
            return GeneralMidiProgram::SYNTHSTRINGS_1;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTHSTRINGS_2 ) )
        {
            return GeneralMidiProgram::SYNTHSTRINGS_2;
        }
        else if( std::strcmp( inString, GM_STRING_CHOIR_AAHS ) )
        {
            return GeneralMidiProgram::CHOIR_AAHS;
        }
        else if( std::strcmp( inString, GM_STRING_VOICE_OOHS ) )
        {
            return GeneralMidiProgram::VOICE_OOHS;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTH_VOICE ) )
        {
            return GeneralMidiProgram::SYNTH_VOICE;
        }
        else if( std::strcmp( inString, GM_STRING_ORCHESTRA_HIT ) )
        {
            return GeneralMidiProgram::ORCHESTRA_HIT;
        }
        else if( std::strcmp( inString, GM_STRING_TRUMPET ) )
        {
            return GeneralMidiProgram::TRUMPET;
        }
        else if( std::strcmp( inString, GM_STRING_TROMBONE ) )
        {
            return GeneralMidiProgram::TROMBONE;
        }
        else if( std::strcmp( inString, GM_STRING_TUBA ) )
        {
            return GeneralMidiProgram::TUBA;
        }
        else if( std::strcmp( inString, GM_STRING_MUTED_TRUMPET ) )
        {
            return GeneralMidiProgram::MUTED_TRUMPET;
        }
        else if( std::strcmp( inString, GM_STRING_FRENCH_HORN ) )
        {
            return GeneralMidiProgram::FRENCH_HORN;
        }
        else if( std::strcmp( inString, GM_STRING_BRASS_SECTION ) )
        {
            return GeneralMidiProgram::BRASS_SECTION;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTHBRASS_1 ) )
        {
            return GeneralMidiProgram::SYNTHBRASS_1;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTHBRASS_2 ) )
        {
            return GeneralMidiProgram::SYNTHBRASS_2;
        }
        else if( std::strcmp( inString, GM_STRING_SOPRANO_SAX ) )
        {
            return GeneralMidiProgram::SOPRANO_SAX;
        }
        else if( std::strcmp( inString, GM_STRING_ALTO_SAX ) )
        {
            return GeneralMidiProgram::ALTO_SAX;
        }
        else if( std::strcmp( inString, GM_STRING_TENOR_SAX ) )
        {
            return GeneralMidiProgram::TENOR_SAX;
        }
        else if( std::strcmp( inString, GM_STRING_BARITONE_SAX ) )
        {
            return GeneralMidiProgram::BARITONE_SAX;
        }
        else if( std::strcmp( inString, GM_STRING_OBOE ) )
        {
            return GeneralMidiProgram::OBOE;
        }
        else if( std::strcmp( inString, GM_STRING_ENGLISH_HORN ) )
        {
            return GeneralMidiProgram::ENGLISH_HORN;
        }
        else if( std::strcmp( inString, GM_STRING_BASSOON ) )
        {
            return GeneralMidiProgram::BASSOON;
        }
        else if( std::strcmp( inString, GM_STRING_CLARINET ) )
        {
            return GeneralMidiProgram::CLARINET;
        }
        else if( std::strcmp( inString, GM_STRING_PICCOLO ) )
        {
            return GeneralMidiProgram::PICCOLO;
        }
        else if( std::strcmp( inString, GM_STRING_FLUTE ) )
        {
            return GeneralMidiProgram::FLUTE;
        }
        else if( std::strcmp( inString, GM_STRING_RECORDER ) )
        {
            return GeneralMidiProgram::RECORDER;
        }
        else if( std::strcmp( inString, GM_STRING_PAN_FLUTE ) )
        {
            return GeneralMidiProgram::PAN_FLUTE;
        }
        else if( std::strcmp( inString, GM_STRING_BLOWN_BOTTLE ) )
        {
            return GeneralMidiProgram::BLOWN_BOTTLE;
        }
        else if( std::strcmp( inString, GM_STRING_SHAKUHACHI ) )
        {
            return GeneralMidiProgram::SHAKUHACHI;
        }
        else if( std::strcmp( inString, GM_STRING_WHISTLE ) )
        {
            return GeneralMidiProgram::WHISTLE;
        }
        else if( std::strcmp( inString, GM_STRING_OCARINA ) )
        {
            return GeneralMidiProgram::OCARINA;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_1_SQUARE ) )
        {
            return GeneralMidiProgram::LEAD_1_SQUARE;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_2_SAWTOOTH ) )
        {
            return GeneralMidiProgram::LEAD_2_SAWTOOTH;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_3_CALLIOPE ) )
        {
            return GeneralMidiProgram::LEAD_3_CALLIOPE;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_4_CHIFF ) )
        {
            return GeneralMidiProgram::LEAD_4_CHIFF;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_5_CHARANG ) )
        {
            return GeneralMidiProgram::LEAD_5_CHARANG;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_6_VOICE ) )
        {
            return GeneralMidiProgram::LEAD_6_VOICE;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_7_FIFTHS ) )
        {
            return GeneralMidiProgram::LEAD_7_FIFTHS;
        }
        else if( std::strcmp( inString, GM_STRING_LEAD_8_BASS_LEAD ) )
        {
            return GeneralMidiProgram::LEAD_8_BASS_LEAD;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_1_NEW_AGE ) )
        {
            return GeneralMidiProgram::PAD_1_NEW_AGE;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_2_WARM ) )
        {
            return GeneralMidiProgram::PAD_2_WARM;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_3_POLYSYNTH ) )
        {
            return GeneralMidiProgram::PAD_3_POLYSYNTH;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_4_CHOIR ) )
        {
            return GeneralMidiProgram::PAD_4_CHOIR;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_5_BOWED ) )
        {
            return GeneralMidiProgram::PAD_5_BOWED;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_6_METALLIC ) )
        {
            return GeneralMidiProgram::PAD_6_METALLIC;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_7_HALO ) )
        {
            return GeneralMidiProgram::PAD_7_HALO;
        }
        else if( std::strcmp( inString, GM_STRING_PAD_8_SWEEP ) )
        {
            return GeneralMidiProgram::PAD_8_SWEEP;
        }
        else if( std::strcmp( inString, GM_STRING_FX_1_RAIN ) )
        {
            return GeneralMidiProgram::FX_1_RAIN;
        }
        else if( std::strcmp( inString, GM_STRING_FX_2_SOUNDTRACK ) )
        {
            return GeneralMidiProgram::FX_2_SOUNDTRACK;
        }
        else if( std::strcmp( inString, GM_STRING_FX_3_CRYSTAL ) )
        {
            return GeneralMidiProgram::FX_3_CRYSTAL;
        }
        else if( std::strcmp( inString, GM_STRING_FX_4_ATMOSPHERE ) )
        {
            return GeneralMidiProgram::FX_4_ATMOSPHERE;
        }
        else if( std::strcmp( inString, GM_STRING_FX_5_BRIGHTNESS ) )
        {
            return GeneralMidiProgram::FX_5_BRIGHTNESS;
        }
        else if( std::strcmp( inString, GM_STRING_FX_6_GOBLINS ) )
        {
            return GeneralMidiProgram::FX_6_GOBLINS;
        }
        else if( std::strcmp( inString, GM_STRING_FX_7_ECHOES ) )
        {
            return GeneralMidiProgram::FX_7_ECHOES;
        }
        else if( std::strcmp( inString, GM_STRING_FX_8_SCI_FI ) )
        {
            return GeneralMidiProgram::FX_8_SCI_FI;
        }
        else if( std::strcmp( inString, GM_STRING_SITAR ) )
        {
            return GeneralMidiProgram::SITAR;
        }
        else if( std::strcmp( inString, GM_STRING_BANJO ) )
        {
            return GeneralMidiProgram::BANJO;
        }
        else if( std::strcmp( inString, GM_STRING_SHAMISEN ) )
        {
            return GeneralMidiProgram::SHAMISEN;
        }
        else if( std::strcmp( inString, GM_STRING_KOTO ) )
        {
            return GeneralMidiProgram::KOTO;
        }
        else if( std::strcmp( inString, GM_STRING_KALIMBA ) )
        {
            return GeneralMidiProgram::KALIMBA;
        }
        else if( std::strcmp( inString, GM_STRING_BAG_PIPE ) )
        {
            return GeneralMidiProgram::BAG_PIPE;
        }
        else if( std::strcmp( inString, GM_STRING_FIDDLE ) )
        {
            return GeneralMidiProgram::FIDDLE;
        }
        else if( std::strcmp( inString, GM_STRING_SHANAI ) )
        {
            return GeneralMidiProgram::SHANAI;
        }
        else if( std::strcmp( inString, GM_STRING_TINKLE_BELL ) )
        {
            return GeneralMidiProgram::TINKLE_BELL;
        }
        else if( std::strcmp( inString, GM_STRING_AGOGO ) )
        {
            return GeneralMidiProgram::AGOGO;
        }
        else if( std::strcmp( inString, GM_STRING_STEEL_DRUMS ) )
        {
            return GeneralMidiProgram::STEEL_DRUMS;
        }
        else if( std::strcmp( inString, GM_STRING_WOODBLOCK ) )
        {
            return GeneralMidiProgram::WOODBLOCK;
        }
        else if( std::strcmp( inString, GM_STRING_TAIKO_DRUM ) )
        {
            return GeneralMidiProgram::TAIKO_DRUM;
        }
        else if( std::strcmp( inString, GM_STRING_MELODIC_TOM ) )
        {
            return GeneralMidiProgram::MELODIC_TOM;
        }
        else if( std::strcmp( inString, GM_STRING_SYNTH_DRUM ) )
        {
            return GeneralMidiProgram::SYNTH_DRUM;
        }
        else if( std::strcmp( inString, GM_STRING_REVERSE_CYMBAL ) )
        {
            return GeneralMidiProgram::REVERSE_CYMBAL;
        }
        else if( std::strcmp( inString, GM_STRING_GUITAR_FRET_NOISE ) )
        {
            return GeneralMidiProgram::GUITAR_FRET_NOISE;
        }
        else if( std::strcmp( inString, GM_STRING_BREATH_NOISE ) )
        {
            return GeneralMidiProgram::BREATH_NOISE;
        }
        else if( std::strcmp( inString, GM_STRING_SEASHORE ) )
        {
            return GeneralMidiProgram::SEASHORE;
        }
        else if( std::strcmp( inString, GM_STRING_BIRD_TWEET ) )
        {
            return GeneralMidiProgram::BIRD_TWEET;
        }
        else if( std::strcmp( inString, GM_STRING_TELEPHONE_RING ) )
        {
            return GeneralMidiProgram::TELEPHONE_RING;
        }
        else if( std::strcmp( inString, GM_STRING_HELICOPTER ) )
        {
            return GeneralMidiProgram::HELICOPTER;
        }
        else if( std::strcmp( inString, GM_STRING_APPLAUSE ) )
        {
            return GeneralMidiProgram::APPLAUSE;
        }
        else if( std::strcmp( inString, GM_STRING_GUNSHOT ) )
        {
            return GeneralMidiProgram::GUNSHOT;
        }

        return GeneralMidiProgram::UNKNOWN;
    }

    inline GeneralMidiProgram IntToGeneralMidi(int inValue )
    {
        if( inValue < 1 || inValue > 128 )
        {
            return GeneralMidiProgram::UNKNOWN;
        }
        else
        {
            return static_cast<GeneralMidiProgram>( inValue );
        }
    }

    inline int GeneralMidiToInt( GeneralMidiProgram inValue )
    {
        const auto result = static_cast<int>( inValue );
        if( result < 1 || result > 128 )
        {
            return 0;
        }

        return result;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
