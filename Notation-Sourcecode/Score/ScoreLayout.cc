// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "ScoreLayout.h"

// Local
#include "Engraver.h"
#include "FontManager.h"
#include "Measure.h"
#include "Note.h"
#include "NoteSpacerIntf.h"
#include "Preferences.h"
#include "Rest.h"
#include "Score.h"
#include "ScoreColors.h"
#include "ScoreCreationInfo.h"
#include "ScoreLayoutInfo.h"
#include "Staff.h"
#include "StaffSystem.h"
#include "Track.h"

namespace komp
{
// MARK: - ScoreLayout

ScoreLayout::ScoreLayout(Score& inScore, const ScoreInfo& inInfo)
: mScore(inScore)
{
    mLayoutParams.systemInset = inInfo.systemInset;
    
    initTextInfo();
    
    // The score info may specify multiple systems per page, but linear layout
    // only has a single system on the "page".  Set the measure count of the
    // sole line to be measuresPerLine * linesPerPage
    // Add the default line
    insertSystem(0, inInfo);
}

/// \brief Layout and justify the entire score.


void
ScoreLayout::layout()
{
    ScoreLayout::StaffMetrics upperMetrics;
    ScoreLayout::StaffMetrics lowerMetrics;
    
    for (const auto& system : mStaffSystems) {
        
        for (const auto& staff : system->getStaves()) {
            
            // Get the measure span
            const auto firstIndex = static_cast<int>(system->getFirstMeasure());
            const auto& firstMeasure = mScore.getMeasure(firstIndex);
            auto iter = firstMeasure->iter();
            
            const auto lastIndex = static_cast<int>(system->getLastMeasure());
            const auto& lastMeasure = mScore.getMeasure(lastIndex);
            const auto endIter = lastMeasure->iter();
            
            // Get the interstitial items heights
            RectList upperFrames;
            RectList lowerFrames;
            for ( ; iter <= endIter; ++iter) {
                const auto& measure = *iter;
                upperFrames.emplace_back(measure->getInterstaffUpperFrame(*staff));
                lowerFrames.emplace_back(measure->getInterstaffLowerFrame(*staff));
            }
            upperMetrics.emplace_back(staff.get(), upperFrames);
            lowerMetrics.emplace_back(staff.get(), lowerFrames);
        }
    }
    
    KOMP_ASSERT(lowerMetrics.size() == upperMetrics.size());
    
    // Set the vertical position of the staves
    auto posY = 0;
    
    for (auto index = 1; index < lowerMetrics.size(); ++index) {
        
        const auto staffAboveMetrics = lowerMetrics[index - 1];
        const auto currentStaffMetrics = upperMetrics[index];
        
        const auto staff = currentStaffMetrics.first;
        
        // Get the greatest height of all the interstaff space below the staff
        // that is above the current staff.
        auto greatestBelow = 0.0;
        for (auto& interStaffAboveBelow : staffAboveMetrics.second) {
            greatestBelow = std::max(greatestBelow, interStaffAboveBelow.getHeight());
        }

        // Get the greatest height of all the interstaff space that is
        // above the current staff
        auto greatestAbove = 0.0;
        for (auto& interStaffCurrentAbove : currentStaffMetrics.second) {
            greatestAbove = std::max(greatestAbove, interStaffCurrentAbove.getHeight());
        }

        const auto totalInterstaffSpace = greatestBelow
                                        + greatestAbove
                                        + staff->getHeight();
        
        if (totalInterstaffSpace > mLayoutParams.interstaffSpace) {
            posY += totalInterstaffSpace;
            staff->setY(posY + staff->getSpacing());
        } else {
            posY += mLayoutParams.interstaffSpace;
            staff->setY(posY);
        }
    }
    
    mStaffSystems.front()->setY(mLayoutParams.marginTop);
    
    mScore.update();
    
}

/// \brief Draw all score elements
void
ScoreLayout::draw(const DrawableContext& inContext)
{
    for (const auto& staffSystem : mStaffSystems) {
        staffSystem->draw(inContext);
    }
    
    {
        //
        // Title
        //
        
        const auto& color = ScoreColors::instance().defaultColor();
        const DrawParams dparams{
            inContext,
            { 350, 10 },
            { color, color }
        };
        
        const auto& title = getTitle(0);
        mTitle.setString(title.Text);
        
        const auto& font = Preferences::instance().Title[0];
        mTitle.setFontName(font.Font);
        mTitle.setFontSize(font.Size);
        mTitle.draw(dparams);
    }
    
    {
        //
        // Composer
        //
        
        const auto& color = ScoreColors::instance().defaultColor();
        const DrawParams dparams{
            inContext,
            { 500, 35 },
            { color, color }
        };
        
        const auto& title = getComposer(0);
        mComposer.setString(title.Text);
        
        const auto& font = Preferences::instance().Composer[0];
        mComposer.setFontName(font.Font);
        mComposer.setFontSize(font.Size);
        mComposer.draw(dparams);
    }
    
    {
        //
        // Arranger
        //
        
        const auto& color = ScoreColors::instance().defaultColor();
        const DrawParams dparams{
            inContext,
            { 500, 45 },
            { color, color }
        };
        
        const auto& title = getComposer(1);
        mArranger.setString(title.Text);
        
        const auto& font = Preferences::instance().Composer[1];
        mArranger.setFontName(font.Font);
        mArranger.setFontSize(font.Size);
        mArranger.draw(dparams);
    }
}

/// \brief Get the dimensions that contain all the score items
/// \result The dimensions
Size
ScoreLayout::getContentSize() const
{
    const auto& firstLine = mStaffSystems.front();
    
    const auto width = firstLine->getInsetLeft()
    + firstLine->getWidth()
    + firstLine->getInsetRight();
    
    const auto& lastLine = mStaffSystems.back();
    const auto height = lastLine->getY() + lastLine->getHeight();
    
    const auto contentSize = Size(width + mLayoutParams.marginRight,
                                  height + mLayoutParams.marginBottom);
    
    return contentSize;
}

// Collect a list of alll measure metrics per staff. We will use these
// metrics to determine the distance of items above the top staffline
// and under the bottom staff line.
ScoreLayout::StaffMetrics
ScoreLayout::collectMetrics() const
{
    StaffMetrics metrics;
    
    for (const auto& system : mStaffSystems) {
        
        for (const auto& staff : system->getStaves()) {
            
            // Get the measure span
            const auto firstIndex = static_cast<int>(system->getFirstMeasure());
            const auto& firstMeasure = mScore.getMeasure(firstIndex);
            auto iter = firstMeasure->iter();
            
            const auto lastIndex = static_cast<int>(system->getLastMeasure());
            const auto& lastMeasure = mScore.getMeasure(lastIndex);
            const auto endIter = lastMeasure->iter();
            
            // Calculate the frame of the staff measure
            ScoreLayout::RectList measureFrames;
            for ( ; iter <= endIter; ++iter) {
                const auto& measure = *iter;
                measureFrames.emplace_back(measure->getFrame(*staff));
            }
            metrics.emplace_back(staff.get(), measureFrames);
        }
    }
    return metrics;
}

void
ScoreLayout::getExtents(const ScoreLayout::RectList& inExtents,
                        double& outTop,
                        double& outBottom) const
{
    for (auto measureIndex = 0; measureIndex < inExtents.size(); ++measureIndex) {
        const auto& previousRect = inExtents.at(measureIndex);
        outTop = std::min(outTop, previousRect.top);
        outBottom = std::max(outBottom, previousRect.bottom);
    }
}

// MARK: - Brackets

void
ScoreLayout::clearBrackets()
{
    const auto& system = mStaffSystems.front();
    system->clearBrackets();
}

void
ScoreLayout::addBracket(const SystemBracketInfo& inInfo)
{
    const auto& system = mStaffSystems.front();
    system->addBracket(inInfo);
}

// MARK: - Lines

/// \brief Inserts a staff system into the score
void
ScoreLayout::insertSystem(const size_t inStaffSystemIndex,
                          const ScoreInfo& inInfo)
{
    auto system = std::make_unique<StaffSystem>(mScore);
    system->setInsetLeft(inInfo.systemInset);
    system->setInsetRight(0);
    system->setMeasureCount(inInfo.measuresPerLine * inInfo.linesPerPage);
    
    const auto iter = mStaffSystems.cbegin() + inStaffSystemIndex;
    mStaffSystems.insert(iter, std::move(system));
}

/// \brief Update the system indices for the pages
void
ScoreLayout::updateSystems()
{
    // Get the total number of measures in the score
    const auto measureCount = mScore.getMeasureCount();
    
    // Update all the line state
    for (const auto& line : mStaffSystems) {
        
        line->setFirstMeasure(0);
        line->setMeasureCount(measureCount);
        
        for (const auto& staff : line->getStaves()) {
            staff->setStaffSystem(line.get());
        }
    }
}

// MARK: - Staves

void
ScoreLayout::indentStaves(double inSystemInset,
                          double inNameInset,
                          bool inShowName)
{
    int lineIndex = 0;
    
    for (const auto& system : mStaffSystems) {
        system->setInsetLeft(inSystemInset);
        
        if (inShowName && lineIndex == 0) {
            system->setInsetLeft(inNameInset);
        }
        
        ++lineIndex;
    }
}

/// \brief Finds the line and staff that cursor is pointing to.
/// \param inPoint The cursor location
/// \result The closest staff to the point or nullptr if none found
Staff*
ScoreLayout::findStaff(const Point& inPoint) const
{
    Staff* previousStaff = nullptr;
    Staff* staff = nullptr;
    
    double lastBottom = 0;
    
    // Find the line and staff the point is at
    for (const auto& line : mStaffSystems) {
        
        for (const auto& lineStaff : line->getStaves()) {
            
            staff = lineStaff.get();
            
            // Get the staff coordinates in line space
            auto staffTop = line->getY() + staff->getY();
            const auto staffBottom = line->getY() + staff->bottom();
            
            // Check the current staff
            if (inPoint.y >= staffTop && inPoint.y <= staffBottom) {
                return staff;
            }
            
            if (previousStaff) {
                
                const auto distanceBetweenStaves = staffTop - lastBottom;
                const auto midPoint = staffTop - (distanceBetweenStaves / 2.0);
                
                if (inPoint.y < midPoint) {
                    return previousStaff;
                }
            }
            
            lastBottom = staffBottom;
            
            // Remember for next compare
            previousStaff = staff;
        }
    }
    
    return staff;
}

/// \brief Updates the number of pages, lines on each page, measure in each
/// line and total number of measures based on ScoreLayoutInfo param
void
ScoreLayout::updateLayoutInfo(const ScoreLayoutInfo& inLayoutInfo)
{
    KOMP_ASSERT(!mStaffSystems.empty());
    
    const auto totalMeasures = inLayoutInfo.numMeasuresInScore();
    if (totalMeasures < mScore.getMeasureCount()
        && inLayoutInfo.deleteExtraMeasures) {
        // delete extra measures
        mScore.measuresErase(mScore.getMeasuresBegin() + totalMeasures,
                             mScore.getMeasuresEnd());
    } else if (totalMeasures > mScore.getMeasureCount()) {
        // add measures
        for (auto index = mScore.getMeasureCount(); index < totalMeasures; ++index) {
            mScore.validateMeasure(index);
        }
    }
}

// MARK: - Text Items

void
ScoreLayout::initTextInfo()
{
    const auto& prefs = Preferences::instance();
    
    // Set the title info
    for (auto index = 0; index < mTitleText.size(); ++index) {
        mTitleText[index].FontInfo = prefs.Title[index];
    }
    
    // Set the instructions
    for (auto index = 0; index < mInstructionsText.size(); ++index) {
        mInstructionsText[index].FontInfo = prefs.Instructions[index];
    }
    
    // Set the composer info
    for (auto index = 0; index < mComposerText.size(); ++index) {
        mComposerText[index].FontInfo = prefs.Composer[index];
    }
    
    // Set the header info
    for (auto index = 0; index < mHeaderText.size(); ++index) {
        
        if (index == 0) {
            // Setup first header to be the current page number
            mHeaderText[0].Text = "#P";
        }
        
        mHeaderText[index].FontInfo = prefs.Header[index];
    }
    
    // Set the footer information
    for (auto index = 0; index < mFooterText.size(); ++index) {
        
        if (index == 0) {
            // Setup first header to be the current page number
            mFooterText[0].Text = "#P";
        }
        
        mFooterText[index].FontInfo = prefs.Footer[index];
    }
    
    // Set the copyright info
    for (auto index = 0; index < mCopyRightText.size(); ++index) {
        mCopyRightText[index].FontInfo = prefs.CopyRight[index];
    }
}

void
ScoreLayout::setTitle(std::string_view inText, size_t inLine)
{
    assert(inLine < mTitleText.size());
    mTitleText[inLine].Text = inText;
}

const TextRecord&
ScoreLayout::getTitle(size_t inLine) const
{
    assert(inLine < mTitleText.size());
    return mTitleText[inLine];
}

void
ScoreLayout::setComposer(std::string_view inText, size_t inLine)
{
    assert(inLine < mComposerText.size());
    mComposerText[inLine].Text = inText;
}

const TextRecord&
ScoreLayout::getComposer(size_t inLine) const
{
    assert(inLine < mComposerText.size());
    return mComposerText[inLine];
}

void
ScoreLayout::setInstructions(std::string_view inText, size_t inLine)
{
    assert(inLine < mInstructionsText.size());
    mInstructionsText[inLine].Text = inText;
}

const TextRecord&
ScoreLayout::getInstructions(size_t inLine) const
{
    assert(inLine < mInstructionsText.size());
    return mInstructionsText[inLine];
}

void
ScoreLayout::setHeader(std::string_view inText, size_t inLine)
{
    assert(inLine < mHeaderText.size());
    mHeaderText[inLine].Text = inText;
}

const TextRecord&
ScoreLayout::getHeader(size_t inLine) const
{
    assert(inLine < mHeaderText.size());
    return mHeaderText[inLine];
}

void
ScoreLayout::setFooter(std::string_view inText, size_t inLine)
{
    assert(inLine < mFooterText.size());
    mFooterText[inLine].Text = inText;
}

const TextRecord&
ScoreLayout::getFooter(size_t inLine) const
{
    assert(inLine < mFooterText.size());
    return mFooterText[inLine];
}

void
ScoreLayout::setCopyright(std::string_view inText, size_t inLine)
{
    assert(inLine < mCopyRightText.size());
    mCopyRightText[inLine].Text = inText;
}

const TextRecord&
ScoreLayout::getCopyRight(size_t inLine) const
{
    assert(inLine < mCopyRightText.size());
    return mCopyRightText[inLine];
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
