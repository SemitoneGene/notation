// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{

class MeasureObserver
{
public:
    virtual ~MeasureObserver() = default;
    
    virtual void itemAdded(const Measure& inMeasure, Item& inItem) = 0;
    virtual void itemRemoved(const Measure& inMeasure, Item& inItem) = 0;
};

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
