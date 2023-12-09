// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Measure.h"
#include "Staff.h"

namespace komp
{

// MARK: - PasteItems

struct PasteItemsContext
{
    PasteItemsContext() = delete;
    
    PasteItemsContext(const ItemRefVec& inItems,
                      Measure& inMeasure,
                      Staff& inStaff,
                      double inX)
    : items{inItems}
    , measure{inMeasure}
    , staff{inStaff}
    , startX{inX}
    {
    }
    
    // The items to be pasted
    ItemRefVec items;
    
    /// The target measure of the paste
    MeasureRef measure;
    
    /// The target staff of the paste
    StaffRef staff;
    
    /// The position the insert begins at
    double startX;
};

struct PasteItemsParams
{
    PasteItemsParams() = delete;
    
    PasteItemsParams(PasteItemsContext inOrigin,
                     PasteItemsContext inDestination)
    : origin(inOrigin)
    , destination(inDestination)
    {
    }
    
    /// The measures and staves in the clipboard
    PasteItemsContext origin;
    
    /// The location where the clipboarded will be pasted
    PasteItemsContext destination;
};

// MARK: - PasteRange

struct PasteRangeContext
{
    PasteRangeContext() = delete;
    
    PasteRangeContext(Measure& inStartMeasure,
                      Staff& inStartStaff,
                      Measure& inEndMeasure,
                      Staff& inEndStaff)
    : startMeasure(inStartMeasure)
    , startStaff(inStartStaff)
    , endMeasure(inEndMeasure)
    , endStaff(inEndStaff)
    {
    }
    
    /// The first measure of the clipboard selection
    MeasureRef startMeasure;
    
    /// The last measure of the clipboard selection
    MeasureRef endMeasure;
    
    /// The top staff of the clipboard selection
    StaffRef startStaff;
    
    /// The bottom staff of the clipboard selection
    StaffRef endStaff;
};

struct PasteRangeParams
{
    PasteRangeParams() = delete;
    
    PasteRangeParams(PasteRangeContext inOrigin,
                     PasteRangeContext inDestination)
    : origin(inOrigin)
    , destination(inDestination)
    {
    }
    
    /// The measures and staves in the clipboard
    PasteRangeContext origin;
    
    /// The location where the clipboarded will be pasted
    PasteRangeContext destination;
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
