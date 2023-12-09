// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local frameworks
#include <parallel_hashmap/phmap.h>

// Local
#include "DrawableTextChar.h"
#include "Forward.h"
#include "MarkData.h"
#include "StemType.h"

// System
#include <map>
#include <string>
#include <vector>

// Forward declarations
namespace komp
{
enum class AccidentalType;
enum class ArpeggiateType;
enum class ClefType;
enum class MarkCategory;
enum class MarkType;
enum class MarkerType;
enum class NoteheadType;
enum class OctaveType;
enum class PedalType;
enum class PlacementType;
enum class TemporalFlagType;
enum class TemporalType;
enum class TimeSignatureType;
enum class TupletType;
enum class WiggleType;

class FontThemeLinux;
class FontThemeMacOS;
}

namespace komp
{
// Types
struct AccidentalData
{
    AccidentalData(AccidentalType inType,
                   unsigned short inSymbol,
                   double inOffsetX,
                   double inOffsetY)
    : type(inType)
    , character(inSymbol)
    , offsetX(inOffsetX)
    , offsetY(inOffsetY)
    {
    }
    
    AccidentalType type;
    unsigned short character;
    double offsetX;
    double offsetY;
};

struct ClefData
{
    ClefData(unsigned short inSymbol,
             int inNormalSizeDrawSlot,
             double inNormalSizePixelOffset,
             int inSmallSizeDrawSlot,
             double inSmallSizePixelOffset)
    : Symbol(inSymbol)
    , NormalSizeDrawSlot(inNormalSizeDrawSlot)
    , NormalSizePixelOffset(inNormalSizePixelOffset)
    , SmallSizeSlotOffset(inSmallSizeDrawSlot)
    , SmallSizePixelOffset(inSmallSizePixelOffset)
    {
    }
    
    // The font codepoint
    unsigned short Symbol;
    
    // The offset (denominated in slots) needed to draw
    // a full-sized (line-start) clef in the correct position.
    // for Treble, Bass, and Viola clefs, the number is 6
    // indicating that the glyph is to be vertically centered
    // in the five-line staff.  Note NormalSizeDrawSlot should not
    // be confused with the focal line of the clef.
    int NormalSizeDrawSlot;
    
    // The pixel offset value to be applied to the slot to pixel
    // value. This is done as not all clef glyphs will be positioned
    // correctly using the default drawing location.
    double NormalSizePixelOffset;
    
    // When drawing the clef in "small" size, e.g. for a mid-
    // measure clef change or courtesy clef, add the
    // SmallSizeDrawSlot amount to the NormalSizeDrawSlot to
    // get the correct position.
    int SmallSizeSlotOffset;
    
    // The pixel offset value to be applied to the slot to pixel
    // value. This is done as not all clef glyphs will be positioned
    // correctly using the default drawing location.
    double SmallSizePixelOffset;
    
    // convenience - use this to get the drawing slot for small clefs
    inline int smallSizeDrawSlot() const { return NormalSizeDrawSlot + SmallSizeSlotOffset; }
};

class FontTheme
{
public:
    static FontTheme& instance();
    
    double getMusicFontSize();
    
    // Brackets
    unsigned short Brace;
    unsigned short BracketTop;
    unsigned short BracketBottom;
    
    // Tables
    std::vector<unsigned short> TimeSignatureTable;
    
    // Maps
    phmap::flat_hash_map<AccidentalType, AccidentalData> AccidentalMap;
    phmap::flat_hash_map<TemporalType, unsigned short> CircledXHeadMap;
    phmap::flat_hash_map<TemporalType, unsigned short> ClusterHeadMap;
    phmap::flat_hash_map<ClefType, ClefData> ClefTable;
    phmap::flat_hash_map<TemporalType, unsigned short> DiamondHeadMap;
    phmap::flat_hash_map<TemporalFlagType, unsigned short> FlagMap;
    phmap::flat_hash_map<MarkType, MarkData> MarkInfo;
    phmap::flat_hash_map<MarkerType, unsigned short> MarkerMap;
    phmap::flat_hash_map<unsigned short, std::string> NameMap;
    phmap::flat_hash_map<TemporalType, unsigned short> NoteheadTypeMap;
    phmap::flat_hash_map<TemporalType, unsigned short> NoteMap;
    phmap::flat_hash_map<OctaveType, unsigned short> OctaveMap;
    phmap::flat_hash_map<TemporalType, unsigned short> PlusHeadMap;
    phmap::flat_hash_map<TemporalType, unsigned short> RestMap;
    phmap::flat_hash_map<TemporalType, unsigned short> SlashHorzEndHeadMap;
    phmap::flat_hash_map<TemporalType, unsigned short> SlashVertEndHeadMap;
    phmap::flat_hash_map<std::string, MarkType> SmuflMarkMap;
    phmap::flat_hash_map<TemporalType, unsigned short> SquareHeadMap;
    phmap::flat_hash_map<TimeSignatureType, unsigned short> TimeSignatureMap;
    phmap::flat_hash_map<TemporalType, unsigned short> TriangleHeadMap;
    phmap::flat_hash_map<TemporalType, unsigned short> TriangleInvertedHeadMap;
    phmap::flat_hash_map<TupletType, unsigned short> TupletMap;
    phmap::flat_hash_map<WiggleType, unsigned short> WiggleMap;
    phmap::flat_hash_map<TemporalType, unsigned short> XHeadMap;
    phmap::flat_hash_map<NoteheadType, StemPlacement> StemPlacementMap;
    
    unsigned short getTupletDigit(int inDigit) const;
    unsigned short getTupletDigit(std::string_view inString) const;
    DrawableChars getTupletNumber(int inNumber) const;
    unsigned short getTupletColon() const;
    
    Rect getBoundingBox(unsigned short inChar) const;
    unsigned short getCharacterForSymbol(std::string_view inCharacter) const;
    
private:
    friend class FontManager;
    
    FontTheme();
    
    void configure(std::string_view inPath);
    
    void populateMaps();
    void populateAccidentalMap();
    void populateClefMap();
    void populateClusterHeadMap();
    void populateFlagsMap();
    void populateHeadMap(std::string_view inPrefix,
                         phmap::flat_hash_map<TemporalType,
                         unsigned short>& inMap);
    void populateMarkMap();
    void populateMarkerMap();
    void populateNameMap();
    void populateNoteMap();
    void populateOctaveMap();
    void populateRestMap();
    void populateSlashHorzEndHeadMap();
    void populateSlashVertEndHeadMap();
    void populateSquareHeadMap();
    void populateTimeSignatureMap();
    void populateTupletMap();
    void populateWiggleMap();
    void populateStemPlacementMap();
    
    void insertClef(ClefType inClefType,
                    unsigned short inCharacter,
                    int inNormalSizeSlotAdjustment,
                    double inNormalSizePixelOffset,
                    int inSmallSizeSlotAdjustment,
                    double inSmallSizePixelOffset);
    
    void addAccidental(std::string_view inSmuflName,
                       AccidentalType inAccidentalType,
                       double inOffsetX,
                       double inOffsetY);
    
    void addMark(std::string_view inSmuflName,
                 MarkType inMarkType,
                 double inOffsetX,
                 double inOffsetY,
                 PlacementType inPlacementType,
                 MarkCategory inMarkCategory);
    
    void addMark(std::string_view inSmuflNameOrTextExpression,
                 MarkType inMarkType,
                 double inOffsetX,
                 double inOffsetY,
                 PlacementType inPlacementType,
                 MarkCategory inMarkCategory,
                 MarkPlacementUPtr&& inMarkPlacement);
    
    void addMarkShared(std::string_view inSmuflNameOrTextExpression,
                       MarkType inMarkType,
                       double inOffsetX,
                       double inOffsetY,
                       PlacementType inPlacementType,
                       MarkCategory inMarkCategory,
                       MarkPlacementUPtr&& inMarkPlacement);
    
    double mMusicFontSize;
    
#ifdef KOMP_BUILD_APPLE
    std::unique_ptr<FontThemeMacOS> mImpl;
#elif KOMP_BUILD_LINUX
    std::unique_ptr<FontThemeLinux> mImpl;
#endif
};

} //namespace


// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
