// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "DrawableChordAtomFactory.h"

// Local
#include "Throw.h"
#include "FontManager.h"
#include "FontTheme.h"

namespace komp
{
	DrawableChordAtomFactory::DrawableChordAtomFactory()
    : mMap{}
    {
        constexpr const double DEFAULT_NORMAL_FONT_SIZE = 12.0;
        constexpr const double DEFAULT_SUPERSCRIPT_FONT_FACTOR = 0.70;
        constexpr const double DEFAULT_SUPERSCRIPT_FONT_SIZE = DEFAULT_NORMAL_FONT_SIZE * DEFAULT_SUPERSCRIPT_FONT_FACTOR;
        constexpr const double DEFAULT_NORMAL_X_OFFSET = 0.0;
        constexpr const double DEFAULT_SUPERSCRIPT_X_OFFSET = 0.0;
        constexpr const double DEFAULT_NORMAL_Y_OFFSET = 0.0;
        constexpr const double DEFAULT_SUPERSCRIPT_Y_OFFSET = -3.5;
        constexpr const auto SMUFL_ENUM_VALUE = ChordSymbolCharFont::SMUFL;
        constexpr const auto TEXT_FONT_ENUM_VALUE = ChordSymbolCharFont::TEXT;

        DrawableChordAtomParams params;

        const auto& fontManager = FontManager::instance();
        const auto& smuflFont = fontManager.getNotationFontName();
        const auto& textFont = fontManager.getTextFontName();
        DrawableChars characters;
        DrawableTextChar character{'O'};
        DrawableTextCharOffsets charOffsets;

        //////////////
        // ROOTS /////
        //////////////

        constexpr const double ROOT_MARGIN_LEFT = 0.2;
        constexpr const double ROOT_MARGIN_RIGHT = 0.2;

        bool useTextFont = true;
        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_A;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'A', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "A";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_B;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'B', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "B";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_C;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'C', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "C";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_D;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'D', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "D";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_E;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'E', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "E";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_F;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'F', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "F";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ROOT;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ROOT_G;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = ROOT_MARGIN_LEFT;
        charOffsets.rightMargin = ROOT_MARGIN_RIGHT;
        character = DrawableTextChar{'G', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "G";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        /////////////////
        // ACCIDENTALS //
        /////////////////

        useTextFont = false;
        const auto& fontTheme = FontTheme::instance();
        constexpr const double ACCIDENTAL_FONT_SIZE_FACTOR = 1.25;
        constexpr const double ACCIDENTAL_Y_ADJUST = -6.0;

        params.chordSymbolCharType = ChordSymbolCharType::ACCIDENTAL;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ACCIDENTAL_DOUBLE_FLAT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 2.0;
        charOffsets.leftMargin = 0.25;
        charOffsets.rightMargin = 0.25;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("accidentalDoubleFlat"), charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "bb";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET + ACCIDENTAL_Y_ADJUST;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET + (ACCIDENTAL_Y_ADJUST * DEFAULT_SUPERSCRIPT_FONT_FACTOR);
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ACCIDENTAL;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ACCIDENTAL_DOUBLE_SHARP;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.5;
        charOffsets.yOffset = 1.0;
        charOffsets.leftMargin = 0.25;
        charOffsets.rightMargin = 0.25;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("accidentalDoubleSharp"), charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "x";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET + ACCIDENTAL_Y_ADJUST;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET + (ACCIDENTAL_Y_ADJUST * DEFAULT_SUPERSCRIPT_FONT_FACTOR);
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ACCIDENTAL;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ACCIDENTAL_FLAT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 2.0;
        charOffsets.leftMargin = 0.25;
        charOffsets.rightMargin = 0.25;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("accidentalFlat"), charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "b";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET + ACCIDENTAL_Y_ADJUST;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET + (ACCIDENTAL_Y_ADJUST * DEFAULT_SUPERSCRIPT_FONT_FACTOR);
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ACCIDENTAL;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ACCIDENTAL_NATURAL;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 1.5;
        charOffsets.leftMargin = 0.25;
        charOffsets.rightMargin = 0.25;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("accidentalNatural"), charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "n";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET + ACCIDENTAL_Y_ADJUST;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET + (ACCIDENTAL_Y_ADJUST * DEFAULT_SUPERSCRIPT_FONT_FACTOR);
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::ACCIDENTAL;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::ACCIDENTAL_SHARP;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 1.5;
        charOffsets.leftMargin = 0.25;
        charOffsets.rightMargin = 0.25;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("accidentalSharp"), charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "#";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET + ACCIDENTAL_Y_ADJUST;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET + (ACCIDENTAL_Y_ADJUST * DEFAULT_SUPERSCRIPT_FONT_FACTOR);
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * ACCIDENTAL_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        /////////////////
        // NUMBERS //////
        /////////////////

        useTextFont = true;
        constexpr const double NUMBERS_FONT_SIZE_FACTOR = 0.9;
//        constexpr const double NUMBERS_Y_ADJUST = 0.0;
        constexpr const double NUMBER_LEFT_MARGIN = 0.1;
        constexpr const double NUMBER_RIGHT_MARGIN = 0.5;

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_2;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'2', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "2";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_4;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'4', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "4";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_5;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'5', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "5";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_6;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'6', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "6";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_7;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'7', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "7";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_9;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'9', charOffsets};
        characters.push_back(character);
        params.text = characters;
        params.asciiText = "9";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_11;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'1', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'1', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "11";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::EXTENSION;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::NUMBER_13;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'1', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = NUMBER_LEFT_MARGIN;
        charOffsets.rightMargin = NUMBER_RIGHT_MARGIN;
        character = DrawableTextChar{'3', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "13";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * NUMBERS_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);
        insert(params);

        //////////////////////////
        // QUALITIES - TEXT //////
        //////////////////////////

        useTextFont = true;
        constexpr const double TEXT_QUALITIES_FONT_SIZE_FACTOR = 0.9;
//        constexpr const double TEXT_QUALITIES_Y_ADJUST = 0.0;

        // aug
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_AUG_TEXT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.2;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'a', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'u', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.2;
        character = DrawableTextChar{'g', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "aug";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        // dim
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_DIM_TEXT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.2;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'d', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'i', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.2;
        character = DrawableTextChar{'m', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "dim";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        // m
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_LOWERCASE_M;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = true;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.3;
        charOffsets.rightMargin = 0.3;
        character = DrawableTextChar{'m', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "m";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        // M
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_UPPERCASE_M;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = true;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.3;
        charOffsets.rightMargin = 0.3;
        character = DrawableTextChar{'M', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "M";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        // sus
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_SUS_TEXT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.2;
        charOffsets.rightMargin = 0.9;
        character = DrawableTextChar{'s', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'u', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.5;
        character = DrawableTextChar{'s', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "sus";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        // Maj
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_MAJ_TEXT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.2;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'M', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'a', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.2;
        character = DrawableTextChar{'j', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = CHORD_SYMBOL_STRING_MAJ_TEXT;
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        // min
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_MIN_TEXT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.2;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'m', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.05;
        character = DrawableTextChar{'i', charOffsets};
        characters.push_back(character);

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 0.05;
        charOffsets.rightMargin = 0.2;
        character = DrawableTextChar{'n', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "min";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TEXT_QUALITIES_FONT_SIZE_FACTOR;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        //////////////////////////
        // QUALITIES - SYMBOLIC //
        //////////////////////////

        useTextFont = false;

        constexpr const double AUG_SIZE = 0.8;
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_AUG_SYMBOL;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = -0.5;
        charOffsets.leftMargin = 0.3;
        charOffsets.rightMargin = 0.3;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymAugmented"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "+";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * AUG_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * AUG_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        constexpr const double DIM_SIZE = 0.6;
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_DIM_SYMBOL;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = -1.5;
        charOffsets.leftMargin = 0.3;
        charOffsets.rightMargin = 0.3;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymDiminished"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "o";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * DIM_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * DIM_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_HALF_DIM_SYMBOL;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = -1.5;
        charOffsets.leftMargin = 0.3;
        charOffsets.rightMargin = 0.3;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymHalfDiminished"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "*";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * DIM_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * DIM_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        constexpr const double TRIANGLE_SIZE = 1.0;
        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_MAJOR_SYMBOL;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = -0.5;
        charOffsets.leftMargin = 0.1;
        charOffsets.rightMargin = 0.1;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymMajorSeventh"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "^";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * TRIANGLE_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * TRIANGLE_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::QUALITY;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::QUALITY_MINOR_SYMBOL;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        constexpr const double DASH_SIZE = 0.8;
        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = -2.0;
        charOffsets.leftMargin = 0.75;
        charOffsets.rightMargin = 0.5;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymMinor"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "-";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * DASH_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * DASH_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        ///////////////
        // PARENS /////
        ///////////////

        useTextFont = false;
        constexpr const double PARENS_SIZE = 0.5;

        params.chordSymbolCharType = ChordSymbolCharType::PARENS;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::PARENS_LEFT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 2.0;
        charOffsets.leftMargin = 1.0;
        charOffsets.rightMargin = 1.0;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymParensLeftTall"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "(";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * PARENS_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * PARENS_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

        params.chordSymbolCharType = ChordSymbolCharType::PARENS;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::PARENS_RIGHT;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 2.0;
        charOffsets.leftMargin = 1.0;
        charOffsets.rightMargin = 1.0;
        character = DrawableTextChar{fontTheme.getCharacterForSymbol("csymParensRightTall"), charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = ")";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * PARENS_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * PARENS_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);
        insert(params);

        ///////////////
        // SLASH //////
        ///////////////

        useTextFont = true;
        constexpr const double SLASH_SIZE = 1.1;
        
        params.chordSymbolCharType = ChordSymbolCharType::SLASH;
        params.chordSymbolChar.chordSymbolCharValue = ChordSymbolCharValue::SLASH;
        params.chordSymbolCharFont = useTextFont ? TEXT_FONT_ENUM_VALUE : SMUFL_ENUM_VALUE;
        params.isShortened = false;
        params.chordSymbolChar.isSuperscript = false;
        characters.clear();

        charOffsets = DrawableTextCharOffsets{};
        charOffsets.xOffset = 0.0;
        charOffsets.yOffset = 0.0;
        charOffsets.leftMargin = 1.0;
        charOffsets.rightMargin = 1.0;
        character = DrawableTextChar{'/', charOffsets};
        characters.push_back(character);

        params.text = characters;
        params.asciiText = "/";
        params.xNormalOffset = DEFAULT_NORMAL_X_OFFSET;
        params.yNormalOffset = DEFAULT_NORMAL_Y_OFFSET;
        params.normalFontSize = DEFAULT_NORMAL_FONT_SIZE * SLASH_SIZE;
        params.fontName = useTextFont ? textFont : smuflFont;
        params.xSuperscriptOffset = DEFAULT_SUPERSCRIPT_X_OFFSET;
        params.ySuperscriptOffset = DEFAULT_SUPERSCRIPT_Y_OFFSET;
        params.superscriptFontSize = DEFAULT_SUPERSCRIPT_FONT_SIZE * SLASH_SIZE;
        insert(params);
        params.chordSymbolChar.isSuperscript = true;
        insert(params);

    }


	const DrawableChordAtom&
	DrawableChordAtomFactory::getDrawableChordAtom(const ChordSymbolChar& inChar)
	{
        const auto& map = getInstance().mMap;
        const auto iter = map.find(inChar);
        KOMP_ASSERT(iter != map.cend());
        return iter->second;
	}


    const DrawableChordAtom&
    DrawableChordAtomFactory::getDrawableChordAtom(ChordSymbolCharValue inCharValue,
                                                   bool inIsSuperscript)
    {
        return getDrawableChordAtom(ChordSymbolChar{inCharValue, inIsSuperscript});
    }


	const DrawableChordAtomFactory&
	DrawableChordAtomFactory::getInstance()
	{
        static DrawableChordAtomFactory instance;
        return instance;
	}


    void
    DrawableChordAtomFactory::insert(const DrawableChordAtomParams& inParams)
    {
        mMap.emplace(inParams.chordSymbolChar, inParams);
    }

    
    const std::map<const ChordSymbolChar, const DrawableChordAtom>&
    DrawableChordAtomFactory::getDrawableChordAtoms()
    {
        const auto& instance = getInstance();
        const auto& map = instance.mMap;
        return map;
    }
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
