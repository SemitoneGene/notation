// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "EngraverLinux.h"

// Local
#include "FontManager.h"
#include "Text.h"

namespace komp
{
/// \brief Measure the text.
/// \param inText The string to measure
/// \result The width and height of the character.
komp::Size
EngraverLinux::MeasureText(std::string_view inFontName,
                           const double inFontSize,
                           const char* inText)
{
    auto& fontManager = komp::FontManager::instance();
    auto font = fontManager.getFont(inFontName, inFontSize);
    font.setPointSizeF(inFontSize);

    const QFontMetrics fontMetrics(font);
    const QString text(inText);
        
    const auto boundingBox = fontMetrics.boundingRect(text);
    
    const auto width = ceil(boundingBox.width());
    const auto height = ceil(boundingBox.height());
    
    return komp::Size{width, height};
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
