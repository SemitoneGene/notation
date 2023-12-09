// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ScoreColors.h"

// System
#include <string.h>

namespace komp
{
ScoreColors& ScoreColors::instance() {
    static ScoreColors instance;
    return instance;
}

ScoreColors::ScoreColors()
{
}

const DrawableColor&
ScoreColors::defaultColor() const
{
    return mDarkMode ? DefaultColorDark : DefaultColorLight;
}

const DrawableColor&
ScoreColors::textColor() const
{
    return mDarkMode ? TextColorDark : TextColorLight;
}

const DrawableColor&
ScoreColors::voiceToColor(size_t inVoice) const
{
    switch (inVoice)
    {
    case 0:
        return mDarkMode ? VoiceOneColorDark : VoiceOneColorLight;
        
    case 1:
        return mDarkMode ? VoiceTwoColorDark : VoiceTwoColorLight;
        
    case 2:
        return mDarkMode ? VoiceThreeColorDark : VoiceThreeColorLight;
        
    case 3:
        return mDarkMode ? VoiceFourColorDark : VoiceFourColorLight;
        
    case 4:
        return mDarkMode ? VoiceFiveColorDark : VoiceFiveColorLight;
        
    case 5:
        return mDarkMode ? VoiceSixColorDark : VoiceSixColorLight;
        
    case 6:
        return mDarkMode ? VoiceSevenColorDark : VoiceSevenColorLight;
        
    case 7:
        return mDarkMode ? VoiceEightColorDark : VoiceEightColorLight;
        
    default:
        return mDarkMode ? VoiceOneColorDark : VoiceOneColorLight;
    }
}

DrawableColors
ScoreColors::getColorsForState(bool isSelected,
                               int inVoice) const
{
    DrawableColors colors;
    if (isSelected) {
        const auto& color = mDarkMode ? SelectedItemColorDark : SelectedItemColorLight;
        colors.fillColor = color;
        colors.strokeColor = color;
    } else {
        const auto& color = voiceToColor(inVoice);
        colors.fillColor = color;
        colors.strokeColor = color;
    }
    return colors;
}

void
ScoreColors::setDarkMode(bool inDark)
{
    mDarkMode = inDark;
}

void
ScoreColors::reset()
{
    VoiceOneColorLight = DrawableColor{0, 0, 0};
    VoiceTwoColorLight = DrawableColor{170, 31, 55};
    VoiceThreeColorLight = DrawableColor{52, 112, 92};
    VoiceFourColorLight = DrawableColor{45, 86, 138};
    VoiceFiveColorLight = DrawableColor{253, 165, 10};
    VoiceSixColorLight = DrawableColor{45, 171, 2};
    VoiceSevenColorLight = DrawableColor{239, 197, 0};
    VoiceEightColorLight = DrawableColor{252, 93, 211};
    DefaultColorLight = DrawableColor{0, 0, 0};
    DisabledColorLight = DrawableColor{0.6f, 0.6f, 0.6f};
    SelectedItemColorLight = DrawableColor{0, 120, 255};
    TextColorLight = DrawableColor{DefaultColorLight};

    VoiceOneColorDark = DrawableColor{124, 124, 124};
    VoiceTwoColorDark = DrawableColor{170, 31, 55};
    VoiceThreeColorDark = DrawableColor{52, 112, 92};
    VoiceFourColorDark = DrawableColor{45, 86, 138};
    VoiceFiveColorLight = DrawableColor{253, 165, 10};
    VoiceSixColorDark = DrawableColor{45, 171, 2};
    VoiceSevenColorLight = DrawableColor{239, 197, 0};
    VoiceEightColorDark = DrawableColor{252, 93, 211};
    DefaultColorDark = DrawableColor{124, 124, 124};
    DisabledColorDark = DrawableColor{0.6f, 0.6f, 0.6f};
    SelectedItemColorDark = DrawableColor{0, 120, 255};
    TextColorDark = DrawableColor{DefaultColorDark};
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

