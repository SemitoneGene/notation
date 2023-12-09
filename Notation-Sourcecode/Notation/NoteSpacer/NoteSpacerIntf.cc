// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "NoteSpacerIntf.h"

// Local
#include "NoteSpacer.h"


namespace komp
{

NoteSpacerIntfUPtr
NoteSpacerIntf::getDefaultStrategy()
{
    return std::make_unique<NoteSpacer>();
}

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
