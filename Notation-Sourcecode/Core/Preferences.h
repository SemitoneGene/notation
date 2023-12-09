// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <bitset>

// Local
#include "MeasureNumberInfo.h"
#include "Text.h"
#include "Track.h"

namespace komp
{
    class Preferences
    {
    public:        
        static Preferences& instance();
        
        Preferences();
        
        void showOutlines(bool inShow);
        void toggleItemTimes();
        void toggleRecordClassifierData();
        
        bool playRepeats() const;
        
        // Debug settings
        bool getShowGlyphOutlines() const;
        void setShowGlyphOutlines(bool inValue);
        bool getShowAnchor() const;
        bool getShowItemFrames() const;
        void setShowItemFrames(bool inValue);
        bool getShowMeasureData() const;
        bool getShowStrokeFrame() const;
        bool getShowItemTimes() const;
        void setShowItemTimes(bool inValue);
        bool getRecordClassifierData() const;
        void setRecordClassifierData(bool inValue);

        TextInfo ChordFont;
        TextInfo ExpressionFont;
        TextInfo LyricFont;
        TextInfo MeasureNumbersFont;
        TextInfo StaffFont;
        TextInfo TempoMarkFont;
        TextInfo RehearsalMarksFont;
        TextInfo EndingFont;
        TextInfo CompressedRestFont;
        TextInfo TabFont;
        TextInfo TempoFont;
        TextInfo TextFont;

        MeasureNumberInfo MeasureNumbers;
        
        TextInfo Title[3];
        TextInfo Instructions[3];
        TextInfo Composer[4];
        TextInfo Header[2];
        TextInfo Footer[2];
        TextInfo CopyRight[6];
        
    private:
        
        enum {
            PREFS_DEBUG_SHOW_ANCHOR,
            PREFS_DEBUG_SHOW_GLYPH_OUTLINES,
            PREFS_DEBUG_SHOW_ITEM_FRAMES,
            PREFS_DEBUG_SHOW_ITEM_TIMES,
            PREFS_DEBUG_RECORD_CLASSIFIER_DATA,
            PREFS_DEBUG_SHOW_MEASURE_DATA,
            PREFS_DEBUG_SHOW_STROKE_FRAME,
            PREFS_TOTAL
        };
        
        std::bitset<PREFS_TOTAL> mFlags;
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
