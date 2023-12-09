// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Preferences.h"

namespace komp
{
Preferences& Preferences::instance() {
    static Preferences instance;
    return instance;
}

Preferences::Preferences()
{
    ChordFont.Font = "BodoniSvtyTwoITCTT-Book";
    ChordFont.Size = 12;
    
    ExpressionFont.Font = "BodoniSvtyTwoITCTT-BookIta";
    ExpressionFont.Size = 12;
    
    LyricFont.Font = "BodoniSvtyTwoITCTT-Book";
    LyricFont.Size = 9;
    
    MeasureNumbersFont.Font = "BodoniSvtyTwoITCTT-Book";
    MeasureNumbersFont.Size = 9;
    
    StaffFont.Font = "BodoniSvtyTwoITCTT-Book";
    StaffFont.Size = 12;
    
    TempoMarkFont.Font = "BodoniSvtyTwoITCTT-Bold";
    TempoMarkFont.Size = 8;
    
    TextFont.Font = "BodoniSvtyTwoITCTT-Book";
    TextFont.Size = 10;

    TextInfo textInfo;
    textInfo.Font = "BodoniSvtyTwoITCTT-Book";
    textInfo.Size = 12;
    
    // Setup the instructions text
    TextInfo instructionsInfo;
    instructionsInfo.Font = "BodoniSvtyTwoITCTT-Book";
    instructionsInfo.Size = 12;
    Instructions[0] = instructionsInfo;
    Instructions[1] = instructionsInfo;
    Instructions[2] = instructionsInfo;
    
    // Setup the Composer text
    TextInfo composerInfo;
    composerInfo.Font = "BodoniSvtyTwoITCTT-Book";
    composerInfo.Size = 8;
    Composer[0] = composerInfo;
    Composer[1] = composerInfo;
    Composer[2] = composerInfo;
    Composer[3] = composerInfo;
    
    Header[0] = textInfo;
    Header[1] = textInfo;
    
    Footer[0] = textInfo;
    Footer[1] = textInfo;
    
    // Setup the copyright text
    TextInfo copyrightInfo;
    copyrightInfo.Font = "BodoniSvtyTwoITCTT-Book";
    copyrightInfo.Size = 8;
    
    CopyRight[0] = copyrightInfo;
    CopyRight[1] = copyrightInfo;
    CopyRight[2] = copyrightInfo;
    CopyRight[3] = copyrightInfo;
    CopyRight[4] = copyrightInfo;
    CopyRight[5] = copyrightInfo;
    
    // Set up title defaults
    Title[0].Font = "BodoniSvtyTwoITCTT-Book";
    Title[0].Size = 15;
    
    Title[1].Font = "BodoniSvtyTwoITCTT-Book";
    Title[1].Size = 9;
    
    Title[2].Font = "BodoniSvtyTwoITCTT-Book";
    Title[2].Size = 9;
    
    // Measure endings (Voltas)
    EndingFont.Font = "BodoniSvtyTwoITCTT-Book";
    EndingFont.Size = 9;
    
    // Compressed Rests
    RehearsalMarksFont.Font = "BodoniSvtyTwoITCTT-Bold";
    RehearsalMarksFont.Size = 14;

    // Rehearsal Marks
    CompressedRestFont.Font = "BodoniSvtyTwoITCTT-Bold";
    CompressedRestFont.Size = 14;

    // Tablature
    TabFont.Font = "BodoniSvtyTwoITCTT-Book";
    TabFont.Size = 10;
    
    // TODO: Phrase font
    
    mFlags[PREFS_DEBUG_SHOW_GLYPH_OUTLINES] = false;
    mFlags[PREFS_DEBUG_SHOW_ITEM_FRAMES] = false;
    mFlags[PREFS_DEBUG_SHOW_MEASURE_DATA] = false;
    mFlags[PREFS_DEBUG_SHOW_STROKE_FRAME] = false;
    mFlags[PREFS_DEBUG_SHOW_ANCHOR] = false;
    mFlags[PREFS_DEBUG_SHOW_ITEM_TIMES] = false;
    mFlags[PREFS_DEBUG_RECORD_CLASSIFIER_DATA] = false;
}


// MARK: - Debug Preferences

bool
Preferences::getShowGlyphOutlines() const
{
    return mFlags[PREFS_DEBUG_SHOW_GLYPH_OUTLINES];
}

void
Preferences::setShowGlyphOutlines(bool inValue)
{
    mFlags[PREFS_DEBUG_SHOW_GLYPH_OUTLINES] = inValue;
}

void
Preferences::showOutlines(bool inShow)
{
    mFlags[PREFS_DEBUG_SHOW_ANCHOR] = inShow;
    mFlags[PREFS_DEBUG_SHOW_GLYPH_OUTLINES] = inShow;
    mFlags[PREFS_DEBUG_SHOW_ITEM_FRAMES] = inShow;
    mFlags[PREFS_DEBUG_SHOW_MEASURE_DATA] = inShow;
    mFlags[PREFS_DEBUG_SHOW_STROKE_FRAME] = inShow;
}

void
Preferences::toggleItemTimes()
{
    mFlags[PREFS_DEBUG_SHOW_ITEM_TIMES] = !mFlags[PREFS_DEBUG_SHOW_ITEM_TIMES];
}

void
Preferences::toggleRecordClassifierData()
{
    mFlags[PREFS_DEBUG_RECORD_CLASSIFIER_DATA] = !mFlags[PREFS_DEBUG_RECORD_CLASSIFIER_DATA];
}

bool
Preferences::getShowAnchor() const
{
    return mFlags[PREFS_DEBUG_SHOW_ANCHOR];
}

bool
Preferences::getShowItemFrames() const
{
    return mFlags[PREFS_DEBUG_SHOW_ITEM_FRAMES];
}

void
Preferences::setShowItemFrames(bool inValue)
{
    mFlags[PREFS_DEBUG_SHOW_ITEM_FRAMES] = inValue;
}

bool
Preferences::getShowItemTimes() const
{
    return mFlags[PREFS_DEBUG_SHOW_ITEM_TIMES];
}

bool
Preferences::getRecordClassifierData() const
{
    return mFlags[PREFS_DEBUG_RECORD_CLASSIFIER_DATA];
}

void
Preferences::setShowItemTimes(bool inValue)
{
    mFlags[PREFS_DEBUG_SHOW_ITEM_TIMES] = inValue;
}

void
Preferences::setRecordClassifierData(bool inValue)
{
    mFlags[PREFS_DEBUG_RECORD_CLASSIFIER_DATA] = inValue;
}

bool
Preferences::getShowMeasureData() const
{
    return mFlags[PREFS_DEBUG_SHOW_MEASURE_DATA];
}

bool
Preferences::getShowStrokeFrame() const
{
    return mFlags[PREFS_DEBUG_SHOW_STROKE_FRAME];
}


bool
Preferences::playRepeats() const
{
    return true;
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
