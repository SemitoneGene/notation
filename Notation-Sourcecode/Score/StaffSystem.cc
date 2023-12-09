// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "StaffSystem.h"

// Local
#include "DrawableContext.h"
#include "Measure.h"
#include "Preferences.h"
#include "Score.h"
#include "ScoreColors.h"
#include "Staff.h"
#include "Track.h"

namespace komp
{
StaffSystem::StaffSystem(const Score& inScore)
: mScore{inScore}
{
    updateMeasures();
}

void
StaffSystem::draw(const DrawableContext& inContext)
{
    configure();
    
    const auto& firstStaff = getStaves().front();
    const auto& color = ScoreColors::instance().defaultColor();
    const DrawParams dparams{
        inContext,
        { getX(), firstStaff->getScoreY() },
        { color, color }
    };
    mSystemBarline->draw(dparams);
    
    for (const auto& staff : getStaves()) {
        staff->draw(inContext);
    }
    
    const auto& measures = mScore.getMeasures();
    for (const auto& measure : measures) {
        for (const auto& staff : getStaves()) {
            measure->draw(inContext, *staff.get());
        }
    }
    
    const auto bracketX = getX();
    for (const auto& bracket : mBrackets) {
        bracket->draw(inContext, bracketX);
    }
    
    //        drawMeasureNumbers(inContext,
    //                           inTopStaff,
    //                           inBottomStaff,
    //                           inMeasureStart,
    //                           measureIndex,
    //                           measureX,
    //                           inMeasureNumberInfo);
}

void
StaffSystem::removeStaff(Track& inTrack, int inHand)
{
    const auto removeIter = getStaffIter(inTrack, inHand);
    if (removeIter == mStaves.cend()) {
        return;
    }
    
    const auto& affectedTrack = (*removeIter)->getTrack();
    
    auto iter = mStaves.erase(removeIter);
    
    // If there are more hands in the same track then the staves
    // need to be updated to reflect the removal of a prior hand.
    // for example, if we remove handIndex zero from a piano track,
    // then the remaining hand/staff (handIndex one) needs to
    // become handIndex zero (or, at least, the code involved with
    // adding and removing tracks to/from an existing score
    // requires this).
    
    if (iter != mStaves.cend()) {
        Track* currentTrack = &(*iter)->getTrack();
        while (currentTrack == &affectedTrack && iter != mStaves.cend()) {
            const auto oldHandIndex = (*iter)->getHand();
            const auto newHandIndex = oldHandIndex - 1;
            (*iter)->setHand(newHandIndex);
            ++iter;
            currentTrack = &(*iter)->getTrack();
        }
    }
}

void
StaffSystem::updateMeasures()
{
    for (const auto& s : getStaves()) {
        s->notifyMeasureChange();
    }
}

//    void
//    StaffSystem::drawMeasureNumbers(const DrawableContext& inContext,
//                                    int inTopStaff,
//                                    int inBottomStaff,
//                                    size_t inMeasureStart,
//                                    size_t inMeasureIndex,
//                                    double inMeasureX,
//                                    const MeasureNumberInfo& inMeasureNumberInfo) const
//    {
//        if (inMeasureNumberInfo.Type == MeasureNumberType::NONE) {
//            return;
//        }
//
//        const auto& prefs = Preferences::instance();
//        const auto drawY = getY();
//        const auto& score = mScore;
//        const auto firstMeasureIndex = inMeasureStart;
//
//        const auto span = inBottomStaff - inTopStaff;
//        auto topInnerIter = getStaves().cbegin() + inTopStaff;
//        auto bottomInnerIter = topInnerIter + span;
//
//        Rect barRect;
//
//        const auto& bottomInnerStaff = *bottomInnerIter;
//
//        for ( ; topInnerIter != bottomInnerIter; ++topInnerIter) {
//
//            const auto& topStaff = *topInnerIter;
//
//            if (inMeasureIndex == firstMeasureIndex) {
//
//                // Calculate where the numbers go
//                const auto offset = inMeasureNumberInfo.Offset * Staff::getSpacing();
//                if (inMeasureNumberInfo.Location == MeasureNumberLocation::BELOW) {
//
//                    // Below staff
//                    barRect.top = drawY + bottomInnerStaff->bottom() + offset + 1;
//                    barRect.bottom = barRect.top + (prefs.MeasureFont.Height + 1);
//                } else {
//
//                    // Above staff
//                    barRect.bottom = drawY + topStaff->getY() - offset - 3;
//                    barRect.top = barRect.bottom - (prefs.MeasureFont.Height + 1);
//                }
//            }
//
//            auto currentMeasure = inMeasureStart + inMeasureIndex;
//            if (currentMeasure > 0) {
//                currentMeasure = score->userMeasureNumber(currentMeasure);
//                if (currentMeasure > 0 && ((inMeasureNumberInfo.Type == MeasureNumberType::SYSTEM && inMeasureIndex == 0)
//                                        || (inMeasureNumberInfo.Type == MeasureNumberType::EVERY && currentMeasure % inMeasureNumberInfo.Increment == 0))) {
//
//                    // Draw the numeral at the start of the barline
//                    auto textX = inMeasureX;
//                    const auto textY = barRect.top;
//
//                    const auto measureText = std::to_string(currentMeasure);
//                    if (inMeasureNumberInfo.Box) {
//
//                        // At minimum, make the box a square
//                        const auto textWidth = TextWidth(prefs.MeasureFont.Font,
//                                                         prefs.MeasureFont.Size,
//                                                         measureText.c_str());
//                        const auto barWidth = std::max(textWidth, prefs.MeasureFont.Height + 1) + 2;
//
//                        barRect.left = textX;
//                        barRect.right = textX + barWidth;
//
//                        // Frame the rect
//                        // TODO: Configure the colors
//                        //inContext->setLineWidth(1.0);
//                        //inContext->setStrokeColor(color);
//                        //inContext->strokeRect(barRect);
//
//                        textX += (barWidth - textWidth + 1) / 2;
//                    }
//
//                    // Set the text color
//                    const auto& colors = ScoreColors::instance();
//                    const auto& color = colors.textColor();
//
//                    LOG_F(INFO, "Nope!");
////                    inContext->drawText(prefs.MeasureFont.Font,
////                                        prefs.MeasureFont.Size,
////                                        textX, textY,
////                                        measureText.c_str(),
////                                        color);
//                }
//            }
//        }
//    }

// MARK: - Insets

void
StaffSystem::setInsetLeft(double inValue)
{
    mInsetLeft = inValue;
    updateBracketInset();
}

// MARK: - Brackets

/// \brief Update inset based on total width of brackets
void
StaffSystem::updateBracketInset()
{
    double widest = 0;
    for (const auto& bracket : mBrackets) {
        widest = std::max(widest, bracket->getWidth());
    }
    
    mInsetLeft += widest;
}


const std::vector<SystemBracketUPtr>&
StaffSystem::getBrackets() const
{
    return mBrackets;
}

void
StaffSystem::addStaff(StaffUPtr inStaff)
{
    inStaff->setStaffSystem(this);
    mStaves.emplace_back(std::move(inStaff));
}


/// \brief Remove orphaned staves
void
StaffSystem::reconcile(const TrackUPtrVec& inTracks)
{
    std::vector<StaffUPtr> stavesToKeep;
    for (const auto& track : inTracks) {
        auto iter = mStaves.begin();
        while (iter != mStaves.end()) {
            auto& staff = *iter;
            if (&staff->getTrack() == track.get()) {
                stavesToKeep.emplace_back(std::move(staff));
                iter = mStaves.erase(iter);
            } else {
                ++iter;
            }
        }
    }
    mStaves = std::move(stavesToKeep);
    
    // Update the brackets
    mBrackets.clear();
}

double
StaffSystem::getX() const
{
    const auto pageOffsetX = 0;
    const auto margin = 0;
    const auto result = pageOffsetX + margin + mInsetLeft;
    return result;
}

void
StaffSystem::setX(double inX)
{
    const auto pageOffsetX = 0;
    const auto margin = 0;
    const auto pageMarginEdge = pageOffsetX + margin;
    const auto result = inX - pageMarginEdge;
    return setInsetLeft(result);
}

double
StaffSystem::getRight() const
{
    return getX() + getMeasureWidth();
}

double
StaffSystem::getWidth() const
{
    return getRight() - getX();
}

double
StaffSystem::getHeight() const
{
    const auto& first = mStaves.front();
    const auto& last = mStaves.back();
    return last->bottom() - first->getY();
}

/// \brief Return the combined width of all measures in the line
/// \result The total width of all measures
double
StaffSystem::getMeasureWidth() const
{
    // Get the width of all measures on the line
    double totalMeasureWidth = 0;
    for (auto index = mFirstMeasure; index <= getLastMeasure(); ++index) {
        const auto& measure = mScore.getMeasure(static_cast<int>(index));
        totalMeasureWidth += measure->getWidth();
    }
    return totalMeasureWidth;
}

/// \brief Get the index of the line in the score line array
/// \result The line index
int
StaffSystem::getIndex() const
{
    const auto& systems = mScore.getStaffSystems();
    const auto iter = std::find_if(systems.cbegin(),
                                   systems.cend(),
                                   [&](const StaffSystemUPtr& item) {
        return item.get() == this;
    });
    
    if (iter == systems.end()) {
        return -1;
    }
    
    return static_cast<int>(iter - systems.cbegin());
}

/// \brief Get an iterator to the line in the score line array
/// \result The line iterator
std::vector<StaffSystemUPtr>::const_iterator
StaffSystem::iter() const
{
    const auto& systems = mScore.getStaffSystems();
    
    const auto iter = std::find_if(systems.cbegin(),
                                   systems.cend(),
                                   [&](const StaffSystemUPtr& system) {
        return system.get() == this;
    });
    
    return iter;
}


StaffUPtrVec::const_iterator
StaffSystem::getStaffIter(const Track& inTrack, int inHand) const
{
    KOMP_ASSERT(inHand >= 0);
    KOMP_ASSERT(inHand < inTrack.getHands());
    
    auto predicate = [&](const StaffUPtr& inStaff) {
        
        if (&inStaff->getTrack() != &inTrack) {
            return false;
        }
        
        if (inStaff->getHand() != inHand) {
            return false;
        }
        
        return true;
    };
    
    return std::find_if(mStaves.cbegin(), mStaves.cend(), predicate);
    
}

Staff&
StaffSystem::getStaff(const Track& inTrack, int inHand) const
{
    const auto& iter = getStaffIter(inTrack, inHand);
    if (iter == mStaves.cend()) {
        KOMP_THROW("staff not found");
    }
    
    return *(*iter).get();
}

void
StaffSystem::clearBrackets()
{
    mBrackets.clear();
}

void
StaffSystem::addBracket(const SystemBracketInfo& inInfo)
{
    // Get the tracks
    const auto& tracks = mScore.getTracks();
    const auto& trackStart = tracks.at(inInfo.trackStart);
    const auto& trackEnd = tracks.at(inInfo.trackEnd);
    
    // Get the start and end staff
    const auto startIndex = mScore.getStaffIndex(*trackStart.get());
    const auto& staffStart = mStaves.at(startIndex);
    
    const auto endIndex = mScore.getStaffIndex(*trackEnd.get());
    const auto handIndex = trackEnd->getHands() - 1;
    const auto& staffEnd = mStaves.at(endIndex + handIndex);
    
    auto bracket = std::make_unique<SystemBracket>(inInfo.type,
                                                   inInfo.level,
                                                   *staffStart,
                                                   *staffEnd);
    mBrackets.push_back(std::move(bracket));
    
    updateBracketInset();
}

/// \brief Return the maximum width of all staff brackets
/// \result The maximum width of all brackets
double
StaffSystem::getMaximumBracketWidth() const
{
    double bracketWidth = 0;
    for (const auto& staff : mStaves) {
        bracketWidth = std::max(bracketWidth, staff->getBracketWidth());
    }
    return bracketWidth;
}

/// \brief Return the left-most position of all brackets
/// \result The left-most position of all brackets
double
StaffSystem::getLeftMostBracketPosition() const
{
    double bracketWidth = 0;
    for (const auto& staff : mStaves) {
        bracketWidth = std::max(bracketWidth, staff->getBracketWidth());
    }
    return bracketWidth;
}

void
StaffSystem::setFirstMeasure(size_t inValue)
{
    mFirstMeasure = inValue;
    updateMeasures();
}

void
StaffSystem::configure()
{
    const auto height = getHeight();
    if (mSystemBarline) {
        const auto barHeight = mSystemBarline->getHeight();
        if (height != barHeight) {
            auto barline = std::make_unique<DrawableBarline>(mSystemBarline->getType(),
                                                             height);
            barline->setZOrder(-1);
            mSystemBarline = std::move(barline);
        }
    } else {
        auto barline = std::make_unique<DrawableBarline>(BarlineType::LEFT_NORMAL,
                                                         height);
        barline->setZOrder(-1);
        mSystemBarline = std::move(barline);
    }
    
    for (const auto& bracket : mBrackets) {
        bracket->configure();
    }
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

