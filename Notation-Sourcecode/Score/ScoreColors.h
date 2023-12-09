// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <vector>

// Local
#include "Constants.h"
#include "DrawableColors.h"

namespace komp
{
class ScoreColors
{
public:
    static ScoreColors& instance();
    
    const DrawableColor& defaultColor() const;
    const DrawableColor& textColor() const;
    
    const DrawableColor& voiceToColor(size_t inVoice) const;
    
    DrawableColors getColorsForState(bool isSelected,
                                     int inVoice) const;
    
    void setDarkMode(bool inDark);
    
    void reset();
    
private:
    ScoreColors();
    
    DrawableColor VoiceOneColorLight{0.0, 0.0, 0.0};
    DrawableColor VoiceTwoColorLight{170, 31, 55};
    DrawableColor VoiceThreeColorLight{52, 112, 92};
    DrawableColor VoiceFourColorLight{45, 86, 138};
    DrawableColor VoiceFiveColorLight{253, 165, 10};
    DrawableColor VoiceSixColorLight{45, 171, 2};
    DrawableColor VoiceSevenColorLight{239, 197, 0};
    DrawableColor VoiceEightColorLight{252, 93, 211};
    DrawableColor ChordSymbolColorLight{0.0, 0.0, 0.0};
    DrawableColor DefaultColorLight{0.0, 0.0, 0.0};
    DrawableColor DisabledColorLight{0.6f, 0.6f, 0.6f};
    DrawableColor SelectedItemColorLight{0, 120, 255};
    DrawableColor TextColorLight{0.0, 0.0, 0.0};

    DrawableColor VoiceOneColorDark{124, 124, 124};
    DrawableColor VoiceTwoColorDark{170, 31, 55};
    DrawableColor VoiceThreeColorDark{52, 112, 92};
    DrawableColor VoiceFourColorDark{45, 86, 138};
    DrawableColor VoiceFiveColorDark{253, 165, 10};
    DrawableColor VoiceSixColorDark{45, 171, 2};
    DrawableColor VoiceSevenColorDark{239, 197, 0};
    DrawableColor VoiceEightColorDark{252, 93, 211};
    DrawableColor ChordSymbolColorDark{124, 124, 124};
    DrawableColor DefaultColorDark{124, 124, 124};
    DrawableColor DisabledColorDark{0.6f, 0.6f, 0.6f};
    DrawableColor SelectedItemColorDark{0, 120, 255};
    DrawableColor TextColorDark{124, 124, 124};

    bool mDarkMode{false};
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
