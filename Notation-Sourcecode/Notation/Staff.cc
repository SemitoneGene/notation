// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Staff.h"

// Local
#include "Constants.h"
#include "DrawParams.h"
#include "Engraver.h"
#include "FontTheme.h"
#include "Item.h"
#include "KeySignature.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "StaffSystem.h"

namespace komp
{
// TODO: Make these configurable
static const double LYRICS_BASE = -2.0;
static const double STAFF_NAME_OFFSET = 4.0;

static const double NOTE_STAFF_SPACING = 4.0;                       // pixels (0.25 em)
static const double NOTE_SLOT_SIZE = NOTE_STAFF_SPACING / 2.0;

static const double TAB_STAFF_SPACING = 6.0;
//static const double TAB_SLOT_SIZE = TAB_STAFF_SPACING / 2.0;

Staff::Staff(const Score& inScore,
             Track& inTrack,
             StaffSystem* inStaffSystem,
             StaffType inType,
             int inLineCount,
             int inHand)
: mScore(inScore)
, mStaffType(inType)
, mLineCount{inLineCount}
, mHand(inHand)
, mTrack(inTrack)
, mStaffSystem(inStaffSystem)
, mUpdateLines(false)
{
    for (int index = 0; index < MAX_VOICES; ++index) {
        mLyricPositions[index] = 0;
    }
    
    switch (mStaffType)
    {
    case StaffType::NOTATION:
        {
            mSpacing = NOTE_STAFF_SPACING;
            mHeight = mSpacing * (getStaffLineCount() - 1);
            mSlotSize = mSpacing / 2.0;
            break;
        }
        
    case StaffType::TABLATURE:
        {
            mSpacing = TAB_STAFF_SPACING;
            mSlotSize = mSpacing / 2.0;
            
            // Tab staff height depends on the number of strings
            mHeight = 999999; //(mTrack.getTabTuning().Strings - 1) * mSpacing;
            
            // Single string staff must be at least one unit high
            if (mHeight == 0) {
                mHeight = 1;
            }
            break;
        }
        
    case StaffType::PERCUSSION:
        {
            mSpacing = NOTE_STAFF_SPACING;
            mHeight = mSpacing * 4;
            mSlotSize = mSpacing / 2.0;
            break;
        }
    }
    
    // TODO: Be smarter about lyric font heights here?
    auto lyricPosition = LYRICS_BASE;
    for (auto voiceIndex = 0; voiceIndex < MAX_VOICES; ++voiceIndex) {
        setLyricPosition(voiceIndex, lyricPosition);
        lyricPosition += LYRICS_BASE * 2;
    }
    
    setY(0);
}

void
Staff::notifyMeasureChange()
{
#ifdef DEBUG
    const auto& system = getStaffSystem();
    const auto index = system->getFirstMeasure();
    const auto measureCount = mScore.getMeasureCount();
    
    if (index >= measureCount) {
        LOG_F(ERROR, "Measure out of range");
    } else {
        const auto newMeasureIndex = static_cast<int>(index);
        KOMP_ASSERT(newMeasureIndex >= 0);
        KOMP_ASSERT(newMeasureIndex < measureCount);
    }
#endif
    mUpdateLines = true;
    updateStaffName();
}

// MARK: - Drawing

void
Staff::draw(const DrawableContext& inContext)
{
    if (mUpdateLines) {
        mLines.clearState();
        mLines.update(*this);
        mUpdateLines = false;
    }
        
    DrawableColors colors;
    const auto& color = ScoreColors::instance().defaultColor();
    colors.fillColor = color;
    colors.strokeColor = color;
    
    const DrawParams drawParams {
        inContext,
        Point(mStaffSystem->getX(),
              mStaffSystem->getY() + getY()),
        colors
    };
    mLines.setZOrder(-2);
    mLines.draw(drawParams);
        
    drawStaffName(inContext, drawParams);
}

void
Staff::updateStaffName()
{
    if (!mScore.getShowInstrumentNames()) {
        return;
    }
    
    if (mTrack.getInstrument().name.empty()) {
        return;
    }
    
    const auto hand = getHand();
    const auto hands = mTrack.getHands();
    const auto lastHand = hands - 1;
    const auto handIsValid = hand == 0 || hand == lastHand;
    if (!handIsValid) {
        return;
    }
    
    // Set the track name. If the track has more than a hand, just set the first hand.
    if (hands == 1 || hand != lastHand) {
        const auto& font = Preferences::instance().StaffFont;
        const auto& trackName = mTrack.getInstrument().name;
        mName.setString(trackName.c_str());
        mName.setFontName(font.Font);
        mName.setFontSize(font.Size);
    }
}

bool
Staff::shouldShowStaffName() const
{
    return mScore.getShowInstrumentNames() && !mName.getString().empty();
}

void
Staff::drawStaffName(const DrawableContext& inContext,
                     const DrawParams& inDrawParams)
{
    if (!shouldShowStaffName()) {
        return;
    }
    
    const auto& font = Preferences::instance().StaffFont;
    const auto& trackName = mTrack.getInstrument().name;
    const auto textSize = MeasureText(font.Font,
                                      font.Size,
                                      trackName.c_str());
    
    // Set the text horizonal position
    auto nameX = inDrawParams.drawPoint.x - (textSize.width / 2.0);
    nameX -= mStaffSystem->getMaximumBracketWidth();
    nameX -= STAFF_NAME_OFFSET;

    const auto hands = mTrack.getHands();
    auto nameY = 0.0;
    if (hands == 1) {
        // Center the name on the single staff
        nameY = getScoreY() + (getHeight() / 2.0);
    } else {
        // Position the text in the middle of all the hand staffs
        const auto& firstStaff = mStaffSystem->getStaff(mTrack, 0);
        const auto& lastStaff = mStaffSystem->getStaff(mTrack, hands - 1);
        const auto top = firstStaff.getScoreY();
        const auto bottom = lastStaff.getScoreY() + lastStaff.getHeight();
        const auto center = (bottom - top) / 2.0;
        nameY = top + center;
    }
    
    const auto& textColor = ScoreColors::instance().textColor();
    DrawableColors textColors;
    textColors.fillColor = textColor;
    textColors.strokeColor = textColor;
    const DrawParams tparams {
        inContext,
        Point(nameX, nameY),
        textColors
    };
    mName.draw(tparams);
}

// MARK: - Positioning

/// \brief Set the position of the staff in line coordinate space
/// \param inY The new staff vertical position
void
Staff::setY(double inY)
{
    mY = inY;
    mBaseY = mY + mHeight + mSpacing;
    
    const auto& staffSystem = getStaffSystem();
    const auto firstMeasure = staffSystem->getFirstMeasure();
    const auto measureCount = staffSystem->getMeasureCount();
    const auto scoreMeasureCount = mScore.getMeasureCount();
    
    for (int i = 0; i < measureCount; ++i) {
        const auto measureIndex = firstMeasure + i;
        if (measureIndex < scoreMeasureCount) {
            const auto& measure = mScore.getMeasure(measureIndex);
            const auto& items = measure->allItems();
            for (const auto& item : items) {
                item.get().notifyStaffYChanged();
            }
        }
    }
}


double
Staff::getScoreY() const
{
    const auto systemY = getStaffSystem()->getY();
    return systemY + getY();
}


double
Staff::getScoreBottom() const
{
    const auto myY = getScoreY();
    return myY + mHeight;
}


double
Staff::getScoreMiddle() const
{
    return slotToPixel(6);
}


/// \brief Converts the pixel to a staff slot
/// \param inPixel The pixel y value
/// \result A position value.
double
Staff::pixelToSlot(double inPixel) const
{
    const auto lineY = mStaffSystem->getY();
    const auto staffPixel = lineY + mBaseY - inPixel;
    const auto adjustedPixel = staffPixel / mSlotSize;
    const auto slot = std::ceil(adjustedPixel - 0.5);
    return slot;
}

/// \brief Converts a slot into a measured pixel position.
/// \param inSlot The slot position
/// \result A position value.
double
Staff::slotToPixel(int inSlot) const
{
    const auto lineY = mStaffSystem->getY();
    const auto delta = (inSlot * mSpacing);
    const auto offsetY = lineY + mBaseY;
    return offsetY - delta / 2.0;
}

/// \brief Converts a slot into a measured pixel position.
/// \param inSlot The slot position
/// \result A position value.
double
Staff::slotToPixel(double inSlot) const
{
    const auto lineY = mStaffSystem->getY();
    const auto delta = (inSlot * mSpacing);
    const auto offsetY = lineY + mBaseY;
    return offsetY - delta / 2.0;
}

// MARK: - Dragging

/// \brief A staff may have items that cannot be dragged; i.e. unpitched percussion
/// \result True if items can be dragged

bool
Staff::itemsAreDraggable() const
{
    return mStaffType != StaffType::PERCUSSION;
}

// MARK: - Accessors

double
Staff::getFontSize()
{
    const auto musicFontSize = FontTheme::instance().getMusicFontSize();
    return musicFontSize;
}

double
Staff::getY() const
{
    return mY;
}

int
Staff::getLyricYPos(int inVoice) const
{
    if (inVoice == -1) {
        inVoice = 0;
    }
    
    const auto systemY = mStaffSystem->getY() + baseY();
    const auto lyricPos = getLyricPosition(inVoice) * getSpacing();
    return (systemY - lyricPos) / 2.0;
}

char
Staff::getLyricPosition(int inVoice) const
{
    return mLyricPositions[inVoice];
}

void
Staff::setLyricPosition(int inVoice, int inValue)
{
    mLyricPositions[inVoice] = inValue;
}

int
Staff::getStaffLineCount() const
{
    return mLineCount;
}

int
Staff::getTopLineSlot() const
{
    const auto numLines = getStaffLineCount();
    const auto result = MIDDLE_SLOT + numLines - 1;
    return result;
}

int
Staff::getBottomLineSlot() const
{
    const auto topDistance = getTopLineSlot() - MIDDLE_SLOT;
    const auto result = MIDDLE_SLOT - topDistance;
    return result;
}

bool
Staff::getIsLegerAbove(int inSlot) const
{
    const auto firstLeger = getTopLineSlot() + 2;
    return inSlot >= firstLeger;
}

bool
Staff::getIsLegerBelow(int inSlot) const
{
    const auto firstLeger = getBottomLineSlot() - 2;
    return inSlot <= firstLeger;
}

int
Staff::getBaselineSlotBelow() const
{
    return getBottomLineSlot() - 6;
}

int
Staff::getBaselineSlotAbove() const
{
    return getTopLineSlot() + 6;
}

int
Staff::getIndex() const
{
    const auto& staves = mStaffSystem->getStaves();
    const auto iter = std::find_if(staves.cbegin(),
                                   staves.cend(),
                                   [&](const StaffUPtr& item) {
        return item.get() == this;
    });
    if (iter == staves.cend()) {
        LOG_F(WARNING, "Unable to find staff index");
        return -1;
    }
    
    return static_cast<int>(iter - staves.cbegin());
}

int
Staff::getTrackIndex() const
{
    return mScore.getTrackIndex(mTrack);
}

double
Staff::getSpacing()
{
    return NOTE_STAFF_SPACING;
}

double
Staff::getSlotSize()
{
    return NOTE_SLOT_SIZE;
}

/// \brief Return the width of any brackets that contain the staff
/// \result The bracket width or zero if the staff is not bracketed
double
Staff::getBracketWidth() const
{
    const auto index = getIndex();
    for (const auto& bracket : mStaffSystem->getBrackets()) {
        const auto startIndex = bracket->getStartStaff().getIndex();
        const auto stopIndex = bracket->getStopStaff().getIndex();
        if (index >= startIndex || index <= stopIndex) {
            return Staff::getSpacing();
        }
    }
    
    return 0;
}

/// \brief Given a staff, return an adjusted rect that represents a slot space.
/// \discussion This rect is used by varous routines to determine if a stroke in
/// in a note space.
/// \result The staff slot rect
komp::Rect
Staff::getSlotFrame()
{
    const auto staffSpacing = komp::Staff::getSpacing();
    return komp::Rect(0, 0, staffSpacing, staffSpacing);
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

