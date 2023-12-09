// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"

namespace komp
{
/// Applies "note spacing" aka "justification" to the temporals
/// in a measure.
///
class NoteSpacerIntf
{
public:
    virtual ~NoteSpacerIntf() = default;
    virtual void applySpacing(Measure& outMeasure) = 0;
    
    /// poor man's factory
    static NoteSpacerIntfUPtr getDefaultStrategy();
};

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
