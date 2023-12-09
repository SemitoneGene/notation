// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// System
#include <numeric>
#include <vector>

namespace komp
{
struct StaffSystemLayoutInfo
{
    int numMeasures;
    
    StaffSystemLayoutInfo()
    : numMeasures{4}
    {}
    
    explicit StaffSystemLayoutInfo(int inNumMeasures)
    : numMeasures{inNumMeasures}
    {}
};

struct MeasureAccumulator
{
    double operator() (double result, const StaffSystemLayoutInfo& inInfo)
    {
        return result + inInfo.numMeasures;
    }
};

struct PageLayoutInfo
{
    std::vector<StaffSystemLayoutInfo> staffSystems;
    
    inline int numMeasuresOnPage() const {
        
        if (staffSystems.empty()) {
            return 0;
        }
        
        return std::accumulate(staffSystems.begin(),
                               staffSystems.end(),
                               0,
                               MeasureAccumulator());
    }
};

struct PageAccumulator
{
    double operator() (double result, const PageLayoutInfo& inInfo)
    {
        return result + inInfo.numMeasuresOnPage();
    }
};

struct SystemsAccumulator
{
    double operator() (double result, const PageLayoutInfo& inInfo)
    {
        return result + inInfo.staffSystems.size();
    }
};

struct ScoreLayoutInfo
{
    // TODO: remove this and put it somewhere better
    int pageHeight;
    int pageWidth;
    double leftMarginSize;
    double rightMarginSize;
    double topMarginSize;
    double bottomMarginSize;
    std::vector<PageLayoutInfo> pages;
    
    // if there are leftover measures(line, pages)
    // we can either delete them or leave them alone
    bool deleteExtraMeasures;
    bool deleteExtraLines;
    bool deleteExtraPages;
    
    inline int numMeasuresInScore() const {
        
        if (pages.empty()) {
            return 0;
        }
        
        return std::accumulate(pages.begin(),
                               pages.end(),
                               0,
                               PageAccumulator());
    }
    
    inline int numStaffSystemsInScore() const {
        if (pages.empty()) {
            return 0;
        }
        
        return std::accumulate(pages.begin(),
                               pages.end(),
                               0,
                               SystemsAccumulator());
    }
    
    inline bool isPageRectValid() const {
        // TODO: verify that the page rect will
        // be within the bounds of the page size
        return true;
    }
    
    ScoreLayoutInfo()
    : pageHeight(static_cast<int>(static_cast<double>(PIXELS_PER_INCH) * 8.5))
    , pageWidth(PIXELS_PER_INCH * 11.0)
    , leftMarginSize(0.5f * static_cast<double>(PIXELS_PER_INCH))
    , rightMarginSize(0.5f * static_cast<double>(PIXELS_PER_INCH))
    , topMarginSize(0.5f * static_cast<double>(PIXELS_PER_INCH))
    , bottomMarginSize(0.5f * static_cast<double>(PIXELS_PER_INCH))
    , deleteExtraMeasures(true)
    , deleteExtraLines(true)
    , deleteExtraPages(true)
    {}
};
}
