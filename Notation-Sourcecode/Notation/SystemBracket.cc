// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "SystemBracket.h"

// Local
#include "DrawableBracket.h"
#include "DrawableContext.h"
#include "Engraver.h"
#include "FontManager.h"
#include "FontTheme.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"
#include "StaffSystem.h"

namespace komp
{
SystemBracket::SystemBracket(SystemBracketType inType,
                             int inLevel,
                             Staff& inStart,
                             Staff& inStop)
: mType(inType)
, mLevel(0)
, mStaffStart(inStart)
, mStaffStop(inStop)
{
}

SystemBracket::SystemBracket(const SystemBracket& inSystemBracket)
: mType(inSystemBracket.mType)
, mLevel(inSystemBracket.mLevel)
, mStaffStart(inSystemBracket.mStaffStart)
, mStaffStop(inSystemBracket.mStaffStop)
{
}

/// \brief Configure a system bracket
void
SystemBracket::configure()
{
    auto update = false;
    
    if (mDrawableBracket) {
        const auto systemHeight = ceil(mStaffStop.bottom() - mStaffStart.getY());
        const auto bracketHeight = ceil(mDrawableBracket->getHeight());
        update = systemHeight != bracketHeight;
    }
    
    if (!mDrawableBracket || update) {
        auto bracket = std::make_unique<DrawableBracket>(mType,
                                                         mLevel,
                                                         mStaffStart,
                                                         mStaffStop);
        mDrawableBracket = std::move(bracket);
    }
}

/// \brief Draws a bracket
/// \param inContext The context to draw in.
/// \param inX Horizontal position to draw at.
void
SystemBracket::draw(const DrawableContext& inContext, double inX) const
{
    if (!mDrawableBracket) {
        auto bracket = std::make_unique<DrawableBracket>(mType,
                                                         mLevel,
                                                         mStaffStart,
                                                         mStaffStop);
        mDrawableBracket = std::move(bracket);
    }
    
    DrawableColors colors;
    const auto& color = ScoreColors::instance().textColor();
    colors.fillColor = color;
    colors.strokeColor = color;
    
    {
        const auto top = mStaffStart.getStaffSystem()->getY() + mStaffStart.getY();
        
        const DrawParams dparams{
            inContext,
            {inX, top},
            colors
        };
        mDrawableBracket->draw(dparams);
    }
}

/// \brief Get the width of the bracket
double
SystemBracket::getWidth() const
{
    // Measure the bracket
    const auto& engraver = Engraver::instance();
    
    if (mType == SystemBracketType::BRACKET) {
        return engraver.heavyBarlineWidth();
    } else {
        // Get the font information
        const auto fontSize = Staff::getFontSize();
        return CharacterWidth(fontSize, FontTheme::instance().Brace);
    }
}

/// \brief Get the bracket info
SystemBracketInfo
SystemBracket::getInfo() const
{
    return SystemBracketInfo(mType,
                             mLevel,
                             mStaffStart.getTrackIndex(),
                             mStaffStop.getTrackIndex());
}
}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

