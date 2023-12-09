// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "FontTheme.h"

// System
#include <assert.h>
#include <string>

// Local
#include "ArticulationPlacement.h"
#include "BreathPlacement.h"
#include "Clef.h"
#include "DefaultMarkPlacement.h"
#include "DynamicPlacement.h"
#include "Engraver.h"
#include "ExpressionPlacement.h"
#include "FontManager.h"
#include "MarkData.h"
#include "MarkerType.h"
#include "Measure.h"
#include "Note.h"
#include "NoteheadType.h"
#include "Octave.h"
#include "OrnamentPlacement.h"
#include "StemType.h"
#include "Temporal.h"
#include "TechniquePlacement.h"
#include "TremoloPlacement.h"
#include "TupletType.h"
#include "WiggleType.h"

#ifdef KOMP_BUILD_APPLE
#include "FontThemeMacOS.h"
#elif KOMP_BUILD_LINUX
#include "FontThemeLinux.h"
#endif

namespace komp
{    
FontTheme& FontTheme::instance()
{
    static FontTheme instance;
    return instance;
}

void
FontTheme::configure(std::string_view inPath)
{
#ifdef KOMP_BUILD_APPLE
    mImpl = std::make_unique<FontThemeMacOS>();
#elif KOMP_BUILD_LINUX
    mImpl = std::make_unique<FontThemeLinux>();
#endif
    mImpl->configure(inPath);
    
    // Brackets
    Brace = mImpl->getCodepoint("brace");
    BracketTop = mImpl->getCodepoint("bracketTop");
    BracketBottom = mImpl->getCodepoint("bracketBottom");
    
    populateMaps();
    getMusicFontSize();
}

double
FontTheme::getMusicFontSize()
{
    if (mMusicFontSize == -1) {
        
        // Determine the ideal font size by comparing font metrics at
        // various sizes to the bounding rect metadata. Use the "glyphBBoxes"
        // information for a "noteheadBlack"
        
        const auto& notehead = NoteheadTypeMap[TemporalType::QUARTER];
        auto boundingBox = getBoundingBox(notehead);
        
        // Get the ideal values
        const auto idealWidth = boundingBox.getWidth();
        const auto idealHeight = boundingBox.getHeight();
        
        // Get the font info
        auto& fontManager = FontManager::instance();
        const auto& fontName = fontManager.getNotationFontName();
        
        // Starting at font point size 1, interate until we hit an ideal font size
        int fontSize = 1;
        
        while (true) {
            
            // Get the font metrics and the font size
            const auto& frame = fontManager.getCharacterFrame(fontName,
                                                              fontSize,
                                                              notehead,
                                                              false);
            if (frame.getWidth() > idealWidth && frame.getHeight() > idealHeight) {
                // Found one
                mMusicFontSize = fontSize;
                break;
            }
            ++fontSize;
        }
        
        // Clear the font cache as it will have cached sizes that will never be used
        fontManager.clearCache();
    }
    
    return mMusicFontSize;
}

unsigned short
FontTheme::getTupletDigit(int inDigit) const
{
    if (inDigit < 0) {
        inDigit = 0;
    } else if (inDigit > 9) {
        inDigit = 9;
    }
    
    switch (inDigit) {
    case 1: return FontTheme::instance().TupletMap[TupletType::ONE];
    case 2: return FontTheme::instance().TupletMap[TupletType::TWO];
    case 3: return FontTheme::instance().TupletMap[TupletType::THREE];
    case 4: return FontTheme::instance().TupletMap[TupletType::FOUR];
    case 5: return FontTheme::instance().TupletMap[TupletType::FIVE];
    case 6: return FontTheme::instance().TupletMap[TupletType::SIX];
    case 7: return FontTheme::instance().TupletMap[TupletType::SEVEN];
    case 8: return FontTheme::instance().TupletMap[TupletType::EIGHT];
    case 9: return FontTheme::instance().TupletMap[TupletType::NINE];
    default: return FontTheme::instance().TupletMap[TupletType::ZERO];
    }
}

unsigned short
FontTheme::getTupletDigit(std::string_view inString) const
{
    int digit = 0;
    std::stringstream ss{std::string(inString)};
    ss >> digit;
    
    if (digit < 0) {
        digit = 0;
    } else if (digit > 9) {
        digit = 9;
    }
    
    return getTupletDigit(digit);
}

DrawableChars
FontTheme::getTupletNumber(int inNumber) const
{
    DrawableChars result;
    std::stringstream ss;
    ss << inNumber;
    
    std::string str = ss.str();
    for (const auto& c : str) {
        const std::string digit{c};
        result.emplace_back(getTupletDigit(digit));
    }
    
    return result;
}

unsigned short
FontTheme::getTupletColon() const
{
    return FontTheme::instance().TupletMap[TupletType::COLON];
}

Rect
FontTheme::getBoundingBox(unsigned short inChar) const
{
    const auto& name = NameMap.at(inChar);
    return mImpl->getBoundingBox(name);
}

/// \discussion This class contains all the values for the character for a specific
/// font.  It then creates tables of characters for access by the
/// engraver.
FontTheme::FontTheme()
: mMusicFontSize(-1)
{
}

/// \brief Populate attribute to symbol maps
void
FontTheme::populateMaps()
{
    populateAccidentalMap();
    populateClefMap();
    populateClusterHeadMap();
    populateFlagsMap();
    populateHeadMap("notehead", NoteheadTypeMap);
    populateHeadMap("noteheadCircleX", CircledXHeadMap);
    populateHeadMap("noteheadDiamond", DiamondHeadMap);
    populateHeadMap("noteheadX", XHeadMap);
    populateHeadMap("noteheadPlus", PlusHeadMap);
    populateHeadMap("noteheadTriangleUp", TriangleHeadMap);
    populateHeadMap("noteheadTriangleDown", TriangleInvertedHeadMap);
    populateMarkMap();
    populateMarkerMap();
    populateNameMap();
    populateNoteMap();
    populateOctaveMap();
    populateRestMap();
    populateSlashHorzEndHeadMap();
    populateSlashVertEndHeadMap();
    populateSquareHeadMap();
    populateTimeSignatureMap();
    populateTupletMap();
    populateWiggleMap();
    populateStemPlacementMap();
}

/// \brief A small convienence function to shorten the length of the insertions
void
FontTheme::insertClef(ClefType inClefType,
                      unsigned short inCharacter,
                      int inNormalSizeSlotAdjustment,
                      double inNormalSizePixelOffset,
                      int inSmallSizeSlotAdjustment,
                      double inSmallSizePixelOffset)
{
    static constexpr const auto middleSlot = MIDDLE_SLOT;
    const auto& clefInfo = Clef::clefInfo(inClefType);
    const auto normalSizeDrawSlot = middleSlot - clefInfo.clefSymbolInfo.standardDelineationSlot() + clefInfo.delineationSlot();
    ClefTable.emplace(std::make_pair(inClefType,
                                     ClefData {
        inCharacter,
        normalSizeDrawSlot,
        inNormalSizePixelOffset,
        inSmallSizeSlotAdjustment,
        inSmallSizePixelOffset
    }));
}

void
FontTheme::populateClefMap()
{
    // Clef characters
    const unsigned short GClef = mImpl->getCodepoint("gClef");
    const unsigned short GClef8vb = mImpl->getCodepoint("gClef8vb");
    const unsigned short GClef8va = mImpl->getCodepoint("gClef8va");
    const unsigned short FClef = mImpl->getCodepoint("fClef");
    const unsigned short FClef8vb = mImpl->getCodepoint("fClef8vb");
    const unsigned short FClef8va = mImpl->getCodepoint("fClef8va");
    const unsigned short CClef = mImpl->getCodepoint("cClef");
    const unsigned short Percussion1 = mImpl->getCodepoint("unpitchedPercussionClef1");
    const unsigned short Percussion2 = mImpl->getCodepoint("unpitchedPercussionClef2");
    const unsigned short GClef15ma = mImpl->getCodepoint("gClef15ma");
    const unsigned short GClef15mb = mImpl->getCodepoint("gClef15mb");
    const unsigned short FClef15ma = mImpl->getCodepoint("fClef15ma");
    const unsigned short FClef15mb = mImpl->getCodepoint("fClef15mb");
    const unsigned short TabClef = mImpl->getCodepoint("6stringTabClef");
    
    const unsigned short UnkClef = mImpl->getCodepoint("gClefReversed");
    const int defaultAdj = 0;
    const int gAdj = -1;
    const int cAdj = 0;
    const int fAdj = 1;
    const int pAdj = 0;
    const int tAdj = 0;
    
    //                   SymbolType  | Character  | NormalSizeSlotAdjustment | NormalSizePixelOffset | SmallSizeSlotAdjustment | SmallSizePixelOffset
    insertClef(ClefType::G,            GClef,       defaultAdj,                  4.0,                    gAdj,                   0);
    insertClef(ClefType::G_LINE1,      GClef,       defaultAdj,                  0,                      gAdj,                   0);
    insertClef(ClefType::G_8VA,        GClef8va,    defaultAdj,                  4.0,                    gAdj,                   0);
    insertClef(ClefType::G_8VB,        GClef8vb,    defaultAdj,                  4.0,                    gAdj,                   0);
    insertClef(ClefType::G_15MA,       GClef15ma,   defaultAdj,                  4.0,                    gAdj,                   0);
    insertClef(ClefType::G_15MB,       GClef15mb,   defaultAdj,                  4.0,                    gAdj,                   0);
    insertClef(ClefType::F,            FClef,       defaultAdj,                 -4.0,                    fAdj,                   0);
    insertClef(ClefType::F_LINE3,      FClef,       defaultAdj,                  0,                      fAdj,                   0);
    insertClef(ClefType::F_LINE5,      FClef,       defaultAdj,                  0,                      fAdj,                   0);
    insertClef(ClefType::F_8VA,        FClef8va,    defaultAdj,                 -4.0,                    fAdj,                   0);
    insertClef(ClefType::F_8VB,        FClef8vb,    defaultAdj,                 -4.0,                    fAdj,                   0);
    insertClef(ClefType::F_15MA,       FClef15ma,   defaultAdj,                 -4.0,                    fAdj,                   0);
    insertClef(ClefType::F_15MB,       FClef15mb,   defaultAdj,                 -4.0,                    fAdj,                   0);
    insertClef(ClefType::C,            CClef,       defaultAdj,                  0,                      cAdj,                   0);
    insertClef(ClefType::C_LINE1,      CClef,       defaultAdj,                  0,                      cAdj,                   0);
    insertClef(ClefType::C_LINE2,      CClef,       defaultAdj,                  0,                      cAdj,                   0);
    insertClef(ClefType::C_LINE4,      CClef,       defaultAdj,                  0,                      cAdj,                   0);
    insertClef(ClefType::C_LINE5,      CClef,       defaultAdj,                  0,                      cAdj,                   0);
    insertClef(ClefType::PERCUSSION,   Percussion1, defaultAdj,                 -4.0,                    pAdj,                   0);
    insertClef(ClefType::PERCUSSION_2, Percussion2, defaultAdj,                 -4.0,                    pAdj,                   0);
    insertClef(ClefType::PERCUSSION_3, Percussion1, defaultAdj,                 -4.0,                    pAdj,                   0);
    insertClef(ClefType::TAB,          TabClef,     defaultAdj,                  0,                      tAdj,                   0);
    insertClef(ClefType::TAB_2,        UnkClef,     defaultAdj,                  0,                      tAdj,                   0);
    insertClef(ClefType::UNKNOWN,      UnkClef,     defaultAdj,                  0,                      cAdj,                   0);
}

/// \brief Populate the notehead map with complete note symbols
/// already loaded into the theme.
void
FontTheme::populateHeadMap(std::string_view inPrefix,
                           phmap::flat_hash_map<TemporalType,
                           unsigned short>& inMap)
{
    const unsigned short doubleWhole = mImpl->getDoubleWholeCodepoint(inPrefix);
    const unsigned short whole = mImpl->getWholeCodepoint(inPrefix);
    const unsigned short half = mImpl->getHalfCodepoint(inPrefix);
    const unsigned short notehead = mImpl->getNoteVariant(inPrefix);
    
    // Populate the table positions. Slots correspond to time values.
    inMap[TemporalType::BREVE] = doubleWhole;
    inMap[TemporalType::WHOLE] = whole;
    inMap[TemporalType::HALF] = half;
    inMap[TemporalType::QUARTER] = notehead;
    inMap[TemporalType::EIGHTH] = notehead;
    inMap[TemporalType::SIXTEENTH] = notehead;
    inMap[TemporalType::THIRTY_SECOND] = notehead;
    inMap[TemporalType::SIXTY_FOURTH] = notehead;
    inMap[TemporalType::ONE_TWENTY_EIGHTH] = notehead;
    inMap[TemporalType::TWO_FIFTY_SIXTH] = notehead;
    inMap[TemporalType::FIVE_TWELFTH] = notehead;
    inMap[TemporalType::TEN_TWENTY_FOURTH] = notehead;
}

void
FontTheme::populateClusterHeadMap()
{
    const unsigned short notehead = mImpl->getCodepoint("noteheadClusterSquareBlack");
    
    ClusterHeadMap[TemporalType::BREVE] = notehead;
    ClusterHeadMap[TemporalType::WHOLE] = notehead;
    ClusterHeadMap[TemporalType::HALF] = notehead;
    ClusterHeadMap[TemporalType::QUARTER] = notehead;
    ClusterHeadMap[TemporalType::EIGHTH] = notehead;
    ClusterHeadMap[TemporalType::SIXTEENTH] = notehead;
    ClusterHeadMap[TemporalType::THIRTY_SECOND] = notehead;
    ClusterHeadMap[TemporalType::SIXTY_FOURTH] = notehead;
    ClusterHeadMap[TemporalType::ONE_TWENTY_EIGHTH] = notehead;
    ClusterHeadMap[TemporalType::TWO_FIFTY_SIXTH] = notehead;
    ClusterHeadMap[TemporalType::FIVE_TWELFTH] = notehead;
    ClusterHeadMap[TemporalType::TEN_TWENTY_FOURTH] = notehead;
}

/// \brief Populate the note map with complete note symbols
/// already loaded into the theme.
void
FontTheme::populateNoteMap()
{
    NoteMap[TemporalType::BREVE] = mImpl->getCodepoint("noteDoubleWhole");
    NoteMap[TemporalType::WHOLE] = mImpl->getCodepoint("noteWhole");
    NoteMap[TemporalType::HALF] = mImpl->getCodepoint("noteHalfUp");
    NoteMap[TemporalType::QUARTER] = mImpl->getCodepoint("noteQuarterUp");
    NoteMap[TemporalType::EIGHTH] = mImpl->getCodepoint("note8thUp");
    NoteMap[TemporalType::SIXTEENTH] = mImpl->getCodepoint("note16thUp");
    NoteMap[TemporalType::THIRTY_SECOND] = mImpl->getCodepoint("note32ndUp");
    NoteMap[TemporalType::SIXTY_FOURTH] = mImpl->getCodepoint("note64thUp");
    NoteMap[TemporalType::ONE_TWENTY_EIGHTH] = mImpl->getCodepoint("note128thUp");
}

void
FontTheme::populateSlashHorzEndHeadMap()
{
    const unsigned short notehead = mImpl->getCodepoint("noteheadSlashHorizontalEnds");
    
    SlashHorzEndHeadMap[TemporalType::BREVE] = notehead;
    SlashHorzEndHeadMap[TemporalType::WHOLE] = notehead;
    SlashHorzEndHeadMap[TemporalType::HALF] = notehead;
    SlashHorzEndHeadMap[TemporalType::QUARTER] = notehead;
    SlashHorzEndHeadMap[TemporalType::EIGHTH] = notehead;
    SlashHorzEndHeadMap[TemporalType::SIXTEENTH] = notehead;
    SlashHorzEndHeadMap[TemporalType::THIRTY_SECOND] = notehead;
    SlashHorzEndHeadMap[TemporalType::SIXTY_FOURTH] = notehead;
    SlashHorzEndHeadMap[TemporalType::ONE_TWENTY_EIGHTH] = notehead;
    SlashHorzEndHeadMap[TemporalType::TWO_FIFTY_SIXTH] = notehead;
    SlashHorzEndHeadMap[TemporalType::FIVE_TWELFTH] = notehead;
    SlashHorzEndHeadMap[TemporalType::TEN_TWENTY_FOURTH] = notehead;
}

void
FontTheme::populateSlashVertEndHeadMap()
{
    const unsigned short notehead = mImpl->getCodepoint("noteheadSlashVerticalEndsSmall");
    
    SlashVertEndHeadMap[TemporalType::BREVE] = notehead;
    SlashVertEndHeadMap[TemporalType::WHOLE] = notehead;
    SlashVertEndHeadMap[TemporalType::HALF] = notehead;
    SlashVertEndHeadMap[TemporalType::QUARTER] = notehead;
    SlashVertEndHeadMap[TemporalType::EIGHTH] = notehead;
    SlashVertEndHeadMap[TemporalType::SIXTEENTH] = notehead;
    SlashVertEndHeadMap[TemporalType::THIRTY_SECOND] = notehead;
    SlashVertEndHeadMap[TemporalType::SIXTY_FOURTH] = notehead;
    SlashVertEndHeadMap[TemporalType::ONE_TWENTY_EIGHTH] = notehead;
    SlashVertEndHeadMap[TemporalType::TWO_FIFTY_SIXTH] = notehead;
    SlashVertEndHeadMap[TemporalType::FIVE_TWELFTH] = notehead;
    SlashVertEndHeadMap[TemporalType::TEN_TWENTY_FOURTH] = notehead;
}

/// \brief Populate the rest table with rest symbols already loaded into the theme.
void
FontTheme::populateRestMap()
{
    RestMap[TemporalType::BREVE] = mImpl->getCodepoint("restDoubleWhole");
    RestMap[TemporalType::WHOLE] = mImpl->getCodepoint("restWhole");
    RestMap[TemporalType::HALF] = mImpl->getCodepoint("restHalf");
    RestMap[TemporalType::QUARTER] = mImpl->getCodepoint("restQuarter");
    RestMap[TemporalType::EIGHTH] = mImpl->getCodepoint("rest8th");
    RestMap[TemporalType::SIXTEENTH] = mImpl->getCodepoint("rest16th");
    RestMap[TemporalType::THIRTY_SECOND] = mImpl->getCodepoint("rest32nd");
    RestMap[TemporalType::SIXTY_FOURTH] = mImpl->getCodepoint("rest64th");
    RestMap[TemporalType::ONE_TWENTY_EIGHTH] = mImpl->getCodepoint("rest128th");
    RestMap[TemporalType::TWO_FIFTY_SIXTH] = mImpl->getCodepoint("rest256th");
    RestMap[TemporalType::FIVE_TWELFTH] = mImpl->getCodepoint("rest512th");
    RestMap[TemporalType::TEN_TWENTY_FOURTH] = mImpl->getCodepoint("rest1024th");
}

void
FontTheme::populateSquareHeadMap()
{
    const unsigned short notehead = mImpl->getCodepoint("noteheadSquareBlack");
    
    SquareHeadMap[TemporalType::BREVE] = notehead;
    SquareHeadMap[TemporalType::WHOLE] = notehead;
    SquareHeadMap[TemporalType::HALF] = notehead;
    SquareHeadMap[TemporalType::QUARTER] = notehead;
    SquareHeadMap[TemporalType::EIGHTH] = notehead;
    SquareHeadMap[TemporalType::SIXTEENTH] = notehead;
    SquareHeadMap[TemporalType::THIRTY_SECOND] = notehead;
    SquareHeadMap[TemporalType::SIXTY_FOURTH] = notehead;
    SquareHeadMap[TemporalType::ONE_TWENTY_EIGHTH] = notehead;
    SquareHeadMap[TemporalType::TWO_FIFTY_SIXTH] = notehead;
    SquareHeadMap[TemporalType::FIVE_TWELFTH] = notehead;
    SquareHeadMap[TemporalType::TEN_TWENTY_FOURTH] = notehead;
}

void
FontTheme::populateTimeSignatureMap()
{
    /// This table contains the numbers used to render time signatures.
    /// It can be directly indexed by numberic value from 0 - 9.
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig0"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig1"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig2"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig3"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig4"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig5"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig6"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig7"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig8"));
    TimeSignatureTable.push_back(mImpl->getCodepoint("timeSig9"));
    
    // This map contains the cut and common time symbols
    TimeSignatureMap[TimeSignatureType::COMMON] = mImpl->getCodepoint("timeSigCommon");
    TimeSignatureMap[TimeSignatureType::CUT] = mImpl->getCodepoint("timeSigCutCommon");
}

/// \brief Populates both the SmuflMarkMap and the MarkInfoMap
void
FontTheme::addMark(std::string_view inSmuflName,
                   MarkType inMarkType,
                   double inOffsetX,
                   double inOffsetY,
                   PlacementType inPlacementType,
                   MarkCategory inMarkCategory)
{
    addMarkShared(inSmuflName,
                  inMarkType,
                  inOffsetX,
                  inOffsetY,
                  inPlacementType,
                  inMarkCategory,
                  nullptr);
}

/// \brief Populates both the SmuflMarkMap and the MarkInfoMap
void
FontTheme::addMark(std::string_view inSmuflNameOrTextExpression,
                   MarkType inMarkType,
                   double inOffsetX,
                   double inOffsetY,
                   PlacementType inPlacementType,
                   MarkCategory inMarkCategory,
                   MarkPlacementUPtr&& inMarkPlacement)
{
    addMarkShared(inSmuflNameOrTextExpression,
                  inMarkType,
                  inOffsetX,
                  inOffsetY,
                  inPlacementType,
                  inMarkCategory,
                  std::move(inMarkPlacement));
}

/// \brief Populates both the SmuflMarkMap and the MarkInfoMap
void
FontTheme::addMarkShared(std::string_view inSmuflNameOrTextExpression,
                         MarkType inMarkType,
                         double inOffsetX,
                         double inOffsetY,
                         PlacementType inPlacementType,
                         MarkCategory inMarkCategory,
                         MarkPlacementUPtr&& inMarkPlacement)
{
    if (!inMarkPlacement) {
        inMarkPlacement = std::make_unique<DefaultMarkPlacement>();
    }
    
    if (inMarkCategory == MarkCategory::EXPRESSION) {
        
        // Handle the mark type that are overloading the SMuFL name as a way of containing
        // a string that will be displayed as a mark
        
        MarkTextParams textParams;
        textParams.category = inMarkCategory;
        textParams.markType = inMarkType;
        textParams.offsetX = inOffsetX;
        textParams.offsetY = inOffsetY;
        textParams.placement = inPlacementType;
        textParams.markPlacement = std::move(inMarkPlacement);
        textParams.expressionText = inSmuflNameOrTextExpression;
        
        const MarkData markData(std::move(textParams));
        MarkInfo.emplace(inMarkType, std::move(markData));
        
    } else {
        
        unsigned short character{0};
        
        if (inSmuflNameOrTextExpression.size() > 0) {
            character = mImpl->getCodepoint(inSmuflNameOrTextExpression);
            SmuflMarkMap[inSmuflNameOrTextExpression] = inMarkType;
        }
        
        MarkSmuflParams smuflParams;
        
        smuflParams.markType = inMarkType;
        smuflParams.character = character;
        smuflParams.offsetX = inOffsetX;
        smuflParams.offsetY = inOffsetY;
        smuflParams.placement = inPlacementType;
        smuflParams.category = inMarkCategory;
        smuflParams.markPlacement = std::move(inMarkPlacement);
        smuflParams.smuflName = inSmuflNameOrTextExpression;
        
        const MarkData markData(std::move(smuflParams));
        MarkInfo.emplace(inMarkType, std::move(markData));
    }
}

/// \brief Populate the symbol table with symbols already loaded into the theme.
void
FontTheme::populateMarkMap()
{
    addMark("", MarkType::NONE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::NONE);
    addMark("accidentalFlat", MarkType::FLAT, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ACCIDENTAL);
    addMark("accidentalNatural", MarkType::NATURAL, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ACCIDENTAL);
    addMark("accidentalSharp", MarkType::SHARP, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ACCIDENTAL);
    addMark("augmentationDot", MarkType::DOT, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("breathMarkComma", MarkType::BREATH_MARK, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<BreathPlacement>());
    addMark("caesura", MarkType::CAESURA, 12, 0, PlacementType::ABOVE_STAFF, MarkCategory::FERMATA, std::make_unique<BreathPlacement>());
    addMark("articTenutoAbove", MarkType::TENUTO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articTenutoBelow", MarkType::TENUTO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articTenutoStaccatoAbove", MarkType::TENUTO_STACCATO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articTenutoStaccatoBelow", MarkType::TENUTO_STACCATO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articTenutoAccentAbove", MarkType::TENUTO_ACCENT_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articTenutoAccentBelow", MarkType::TENUTO_ACCENT_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articAccentAbove", MarkType::ACCENT_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articAccentBelow", MarkType::ACCENT_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articAccentStaccatoAbove", MarkType::ACCENT_STACCATO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articAccentStaccatoBelow", MarkType::ACCENT_STACCATO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articMarcatoAbove", MarkType::MARCATO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articMarcatoBelow", MarkType::MARCATO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articStaccatoAbove", MarkType::STACCATO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articStaccatoBelow", MarkType::STACCATO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articStaccatissimoAbove", MarkType::STACCATISSIMO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articStaccatissimoBelow", MarkType::STACCATISSIMO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articMarcatoStaccatoAbove", MarkType::MARCATO_STACCATO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articMarcatoStaccatoBelow", MarkType::MARCATO_STACCATO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articMarcatoTenutoAbove", MarkType::MARCATO_TENUTO_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articMarcatoTenutoBelow", MarkType::MARCATO_TENUTO_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articStressAbove", MarkType::STRESS_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articStressBelow", MarkType::STRESS_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articUnstressAbove", MarkType::UNSTRESS_ABOVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("articUnstressBelow", MarkType::UNSTRESS_BELOW, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ARTICULATION, std::make_unique<ArticulationPlacement>());
    addMark("brassDoitShort", MarkType::BRASS_DOIT, 1, -3, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<TechniquePlacement>());
    addMark("brassFallLipShort", MarkType::BRASS_FALLOFF, 1, 3, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<TechniquePlacement>());
    addMark("brassLiftShort", MarkType::BRASS_LIFT, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<TechniquePlacement>());
    addMark("brassMuteClosed", MarkType::BRASS_MUTE_CLOSED, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<TechniquePlacement>());
    addMark("brassPlop", MarkType::BRASS_PLOP, -6, -3, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<TechniquePlacement>());
    addMark("brassScoop", MarkType::BRASS_SCOOP, -6, 3, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<TechniquePlacement>());
    addMark("dynamicPPP", MarkType::DYNAMIC_PPP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicPP", MarkType::DYNAMIC_PP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicPiano", MarkType::DYNAMIC_P, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicMP", MarkType::DYNAMIC_MP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicMF", MarkType::DYNAMIC_MF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicForte", MarkType::DYNAMIC_F, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicFF", MarkType::DYNAMIC_FF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicFFF", MarkType::DYNAMIC_FFF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicSforzando1", MarkType::DYNAMIC_SF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicSforzato", MarkType::DYNAMIC_SFZ, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicFortePiano", MarkType::DYNAMIC_FP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicForzando", MarkType::DYNAMIC_FZ, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicSforzando", MarkType::DYNAMIC_S, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicPPPP", MarkType::DYNAMIC_PPPP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicPPPPP", MarkType::DYNAMIC_PPPPP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicPPPPPP", MarkType::DYNAMIC_PPPPPP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicFFFF", MarkType::DYNAMIC_FFFF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicFFFFF", MarkType::DYNAMIC_FFFFF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicFFFFFF", MarkType::DYNAMIC_FFFFFF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicSforzandoPiano", MarkType::DYNAMIC_SFP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicSforzandoPianissimo", MarkType::DYNAMIC_SFPP, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicRinforzando1", MarkType::DYNAMIC_RF, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicRinforzando2", MarkType::DYNAMIC_RFZ, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicSforzatoFF", MarkType::DYNAMIC_SFFZ, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("dynamicNiente", MarkType::DYNAMIC_NIENTE, 0, 0.7, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC, std::make_unique<DynamicPlacement>());
    addMark("", MarkType::DYNAMIC_OTHER, 0, 0, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC);
    addMark("", MarkType::DYNAMIC_UNKNOWN, 0, 0, PlacementType::AUTOMATIC, MarkCategory::DYNAMIC);
    addMark("fermataAbove", MarkType::FERMATA, 0, 0, PlacementType::ABOVE_STAFF, MarkCategory::FERMATA, std::make_unique<DynamicPlacement>());
    addMark("fermataBelow", MarkType::FERMATA_INVERTED, 0, 0, PlacementType::BELOW_STAFF, MarkCategory::FERMATA, std::make_unique<DynamicPlacement>());
    addMark("fermataShortAbove", MarkType::FERMATA_SHORT_ABOVE, 0, 0, PlacementType::ABOVE_STAFF, MarkCategory::FERMATA, std::make_unique<DynamicPlacement>());
    addMark("fermataShortBelow", MarkType::FERMATA_SHORT_BELOW, 0, 0, PlacementType::BELOW_STAFF, MarkCategory::FERMATA, std::make_unique<DynamicPlacement>());
    addMark("fermataLongAbove", MarkType::FERMATA_LONG_ABOVE, 0, 0, PlacementType::ABOVE_STAFF, MarkCategory::FERMATA, std::make_unique<DynamicPlacement>());
    addMark("fermataLongBelow", MarkType::FERMATA_LONG_BELOW, 0, 0, PlacementType::BELOW_STAFF, MarkCategory::FERMATA, std::make_unique<DynamicPlacement>());
    addMark("graceNoteSlashStemUp", MarkType::GRACE_SLASH_UP, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("graceNoteSlashStemDown", MarkType::GRACE_SLASH_DOWN, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("noteheadParenthesisLeft", MarkType::PAREN_LEFT, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("noteheadParenthesisRight", MarkType::PAREN_RIGHT, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("keyboardPedalPed", MarkType::PEDAL, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<DynamicPlacement>());
    addMark("keyboardPedalUp", MarkType::PEDAL_UP, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<DynamicPlacement>());
    addMark("ornamentTrill", MarkType::TRILL, 0, 0, PlacementType::ABOVE_STAFF, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("ornamentTurn", MarkType::TURN, 0, 0, PlacementType::ABOVE_STAFF, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("ornamentTurnInverted", MarkType::TURN_INVERTED, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("ornamentTurnSlash", MarkType::TURN_SLASH, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("ornamentMordent", MarkType::MORDENT, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("ornamentMordentInverted", MarkType::MORDENT_INVERTED, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<OrnamentPlacement>());
    addMark("stringsUpBow", MarkType::STRINGS_BOW_UP, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<OrnamentPlacement>());
    addMark("stringsDownBow", MarkType::STRINGS_BOW_DOWN, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<OrnamentPlacement>());
    addMark("stringsHarmonic", MarkType::STRINGS_HARMONIC, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<OrnamentPlacement>());
    addMark("stringsHalfHarmonic", MarkType::STRINGS_HARMONIC_HALF, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<OrnamentPlacement>());
    addMark("stringsHarmonic", MarkType::STRINGS_OPEN, 0, 0, PlacementType::AUTOMATIC, MarkCategory::TECHNIQUE, std::make_unique<OrnamentPlacement>());
    addMark("tremolo1", MarkType::TREMOLO_ONE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<TremoloPlacement>());
    addMark("tremolo2", MarkType::TREMOLO_TWO, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<TremoloPlacement>());
    addMark("tremolo3", MarkType::TREMOLO_THREE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<TremoloPlacement>());
    addMark("tremolo4", MarkType::TREMOLO_FOUR, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<TremoloPlacement>());
    addMark("tremolo5", MarkType::TREMOLO_FIVE, 0, 0, PlacementType::AUTOMATIC, MarkCategory::ORNAMENT, std::make_unique<TremoloPlacement>());
    
    // Expressions (drawn as text) go below this line
    addMark("accel.", MarkType::EXPRESSION_ACCELERANDO, 0, 0, PlacementType::AUTOMATIC, MarkCategory::EXPRESSION, std::make_unique<OrnamentPlacement>());
    addMark("cresc.", MarkType::EXPRESSION_CRESCENDO, 0, 0, PlacementType::AUTOMATIC, MarkCategory::EXPRESSION, std::make_unique<DynamicPlacement>());
    addMark("decresc.", MarkType::EXPRESSION_DECRESCENDO, 0, 0, PlacementType::AUTOMATIC, MarkCategory::EXPRESSION, std::make_unique<DynamicPlacement>());
    addMark("dim.", MarkType::EXPRESSION_DIM, 0, 0, PlacementType::AUTOMATIC, MarkCategory::EXPRESSION, std::make_unique<DynamicPlacement>());
    addMark("l.v.", MarkType::EXPRESSION_LV, 0, 0, PlacementType::AUTOMATIC, MarkCategory::EXPRESSION, std::make_unique<OrnamentPlacement>());
    addMark("rit.", MarkType::EXPRESSION_RITARDANDO, 0, 0, PlacementType::AUTOMATIC, MarkCategory::EXPRESSION, std::make_unique<OrnamentPlacement>());    
}

void
FontTheme::populateMarkerMap()
{
    MarkerMap[MarkerType::CODA] = (mImpl->getCodepoint("coda"));
    MarkerMap[MarkerType::CODA_SQUARE] = (mImpl->getCodepoint("codaSquare"));
    MarkerMap[MarkerType::DA_CAPO] = (mImpl->getCodepoint("daCapo"));
    MarkerMap[MarkerType::DAL_SEGNO] = (mImpl->getCodepoint("dalSegno"));
    MarkerMap[MarkerType::SEGNO] = (mImpl->getCodepoint("segno"));
}

void
FontTheme::populateNameMap()
{
    mImpl->populateNameMap(NameMap);
}

void
FontTheme::populateFlagsMap()
{
    FlagMap[TemporalFlagType::EIGHTH_UP] = (mImpl->getCodepoint("flag8thUp"));
    FlagMap[TemporalFlagType::SIXTEENTH_UP] = (mImpl->getCodepoint("flag16thUp"));
    FlagMap[TemporalFlagType::THIRTY_SECOND_UP] = (mImpl->getCodepoint("flag32ndUp"));
    FlagMap[TemporalFlagType::SIXTY_FOURTH_UP] = (mImpl->getCodepoint("flag64thUp"));
    FlagMap[TemporalFlagType::ONE_TWENTY_EIGHTH_UP] = (mImpl->getCodepoint("flag128thUp"));
    FlagMap[TemporalFlagType::TWO_FIFTY_SIXTH_UP] = (mImpl->getCodepoint("flag256thUp"));
    FlagMap[TemporalFlagType::FIVE_TWELFTH_UP] = (mImpl->getCodepoint("flag512thUp"));
    FlagMap[TemporalFlagType::TEN_TWENTY_FOURTH_UP] = (mImpl->getCodepoint("flag1024thUp"));
    FlagMap[TemporalFlagType::EIGHTH_DOWN] = (mImpl->getCodepoint("flag8thDown"));
    FlagMap[TemporalFlagType::SIXTEENTH_DOWN] = (mImpl->getCodepoint("flag16thDown"));
    FlagMap[TemporalFlagType::THIRTY_SECOND_DOWN] = (mImpl->getCodepoint("flag32ndDown"));
    FlagMap[TemporalFlagType::SIXTY_FOURTH_DOWN] = (mImpl->getCodepoint("flag64thDown"));
    FlagMap[TemporalFlagType::ONE_TWENTY_EIGHTH_DOWN] = (mImpl->getCodepoint("flag128thDown"));
    FlagMap[TemporalFlagType::TWO_FIFTY_SIXTH_DOWN] = (mImpl->getCodepoint("flag256thDown"));
    FlagMap[TemporalFlagType::FIVE_TWELFTH_DOWN] = (mImpl->getCodepoint("flag512thUp"));
    FlagMap[TemporalFlagType::TEN_TWENTY_FOURTH_DOWN] = (mImpl->getCodepoint("flag1024thDown"));
}

void
FontTheme::populateTupletMap()
{
    TupletMap[TupletType::ZERO] = mImpl->getCodepoint("tuplet0");
    TupletMap[TupletType::ONE] = mImpl->getCodepoint("tuplet1");
    TupletMap[TupletType::TWO] = mImpl->getCodepoint("tuplet2");
    TupletMap[TupletType::THREE] = mImpl->getCodepoint("tuplet3");
    TupletMap[TupletType::FOUR] = mImpl->getCodepoint("tuplet4");
    TupletMap[TupletType::FIVE] = mImpl->getCodepoint("tuplet5");
    TupletMap[TupletType::SIX] = mImpl->getCodepoint("tuplet6");
    TupletMap[TupletType::SEVEN] = mImpl->getCodepoint("tuplet7");
    TupletMap[TupletType::EIGHT] = mImpl->getCodepoint("tuplet8");
    TupletMap[TupletType::NINE] = mImpl->getCodepoint("tuplet9");
    TupletMap[TupletType::COLON] = mImpl->getCodepoint("tupletColon");
}

void
FontTheme::populateWiggleMap()
{
    WiggleMap[WiggleType::UP] = mImpl->getCodepoint("wiggleArpeggiatoUp");
    WiggleMap[WiggleType::DOWN] = mImpl->getCodepoint("wiggleArpeggiatoDown");
    WiggleMap[WiggleType::UP_ARROW] = mImpl->getCodepoint("wiggleArpeggiatoUpArrow");
    WiggleMap[WiggleType::DOWN_ARROW] = mImpl->getCodepoint("wiggleArpeggiatoDownArrow");
    WiggleMap[WiggleType::UP_SWASH] = mImpl->getCodepoint("wiggleArpeggiatoUpSwash");
    WiggleMap[WiggleType::DOWN_SWASH] = mImpl->getCodepoint("wiggleArpeggiatoDownSwash");
    WiggleMap[WiggleType::GLISSANDO] = mImpl->getCodepoint("wiggleGlissando");
}

void
FontTheme::populateStemPlacementMap()
{
    // noteheadBlack
    auto stemPlacement = mImpl->getStemPlacement("noteheadBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::NORMAL] = stemPlacement;
    }
    
    // noteheadCircleX
    stemPlacement = mImpl->getStemPlacement("noteheadCircleX");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::CIRCLED_X] = stemPlacement;
    }
    
    // noteheadDiamond
    stemPlacement = mImpl->getStemPlacement("noteheadDiamondBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::DIAMOND] = stemPlacement;
    }
    
    // noteheadX
    stemPlacement = mImpl->getStemPlacement("noteheadXBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::X] = stemPlacement;
    }
    
    // noteheadPlus
    stemPlacement = mImpl->getStemPlacement("noteheadPlusBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::PLUS] = stemPlacement;
    }
    
    // noteheadSquare
    stemPlacement = mImpl->getStemPlacement("noteheadSquareBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::SQUARE] = stemPlacement;
    }
    
    // noteheadTriangleUp
    stemPlacement = mImpl->getStemPlacement("noteheadTriangleUpBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::TRIANGLE] = stemPlacement;
    }
    
    // noteheadTriangleDown
    stemPlacement = mImpl->getStemPlacement("noteheadTriangleDownBlack");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::TRIANGLE_INVERTED] = stemPlacement;
    }
    
    // noteheadSlashVerticalEndsSmall
    stemPlacement = mImpl->getStemPlacement("noteheadSlashVerticalEndsSmall");
    if (stemPlacement.isValid()) {
        StemPlacementMap[NoteheadType::SLASH_V_END] = stemPlacement;
    }
}

void
FontTheme::addAccidental(std::string_view inSmuflName,
                         AccidentalType inAccidentalType,
                         double inOffsetX,
                         double inOffsetY)
{
    AccidentalData accidentalData(inAccidentalType,
                                  mImpl->getCodepoint(inSmuflName),
                                  inOffsetX,
                                  inOffsetY);
    AccidentalMap.emplace(inAccidentalType, std::move(accidentalData));
}

void
FontTheme::populateAccidentalMap()
{
    addAccidental("accidentalSharp", AccidentalType::SHARP, 3, 0);
    addAccidental("accidentalFlat", AccidentalType::FLAT, 4, -0.5);
    addAccidental("accidentalNatural", AccidentalType::NATURAL, 4, 0);
    addAccidental("accidentalDoubleSharp", AccidentalType::DOUBLE_SHARP, 3, 0);
    addAccidental("accidentalDoubleFlat", AccidentalType::DOUBLE_FLAT, -1, -0.5);
    addAccidental("accidentalSharpSharp", AccidentalType::SHARP_SHARP, 0, 0);
    addAccidental("accidentalNaturalSharp", AccidentalType::NATURAL_SHARP, 0, 0);
    addAccidental("accidentalNaturalFlat", AccidentalType::NATURAL_FLAT, 0, 0);
    addAccidental("accidentalQuarterToneFlat4", AccidentalType::QUARTER_FLAT, 0, 0);
    addAccidental("accidentalQuarterToneSharp4", AccidentalType::QUARTER_SHARP, 0, 0);
    addAccidental("accidentalParensLeft", AccidentalType::PAREN_LEFT, 0, 0);
    addAccidental("accidentalParensRight", AccidentalType::PAREN_RIGHT, 0, 0);
}

/// \brief Populate the SMuFL to Octave type
void
FontTheme::populateOctaveMap()
{
    OctaveMap[OctaveType::O] = mImpl->getCodepoint("ottava");
    OctaveMap[OctaveType::O_ALTA] = mImpl->getCodepoint("ottavaAlta");
    OctaveMap[OctaveType::O_BASSA] = mImpl->getCodepoint("ottavaBassa");
    OctaveMap[OctaveType::O_BASSA_BA] = mImpl->getCodepoint("ottavaBassaBa");
    OctaveMap[OctaveType::O_BASSA_VB] = mImpl->getCodepoint("ottavaBassaVb");
    OctaveMap[OctaveType::Q] = mImpl->getCodepoint("quindicesima");
    OctaveMap[OctaveType::Q_ALTA] = mImpl->getCodepoint("quindicesimaAlta");
    OctaveMap[OctaveType::Q_BASSA] = mImpl->getCodepoint("quindicesimaBassa");
    OctaveMap[OctaveType::Q_BASSA_MB] = mImpl->getCodepoint("quindicesimaBassaMb");
    OctaveMap[OctaveType::V] = mImpl->getCodepoint("ventiduesima");
    OctaveMap[OctaveType::V_ALTA] = mImpl->getCodepoint("ventiduesimaAlta");
    OctaveMap[OctaveType::V_BASSA] = mImpl->getCodepoint("ventiduesimaBassa");
    OctaveMap[OctaveType::V_BASSA_MB] = mImpl->getCodepoint("ventiduesimaBassaMb");
}

// MARK: - Utilities

unsigned short
FontTheme::getCharacterForSymbol(std::string_view inCharacter) const
{
    return mImpl->getCodepoint(inCharacter);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
