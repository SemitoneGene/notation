// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "Instrument.h"

namespace komp
{
Instrument::Instrument()
{
}

int
Instrument::getNumStaves() const
{
    return static_cast<int>(startingClefs.size());
}

const Pitch&
Instrument::getRangeLow() const
{
    return mRangeLow;
}

Pitch&
Instrument::getRangeLow()
{
    return mRangeLow;
}

const Pitch&
Instrument::getRangeHigh() const
{
    return mRangeHigh;
}

Pitch&
Instrument::getRangeHigh()
{
    return mRangeHigh;
}

StaffType
Instrument::getStaffType() const
{
    return mStaffType;
}

void
Instrument::setStaffType(StaffType inStaffType)
{
    mStaffType = inStaffType;
}

/// Set the staff type
void setStaffType(StaffType inStaffType);


int
Instrument::getStaffLines() const
{
    return mStaffLines;
}

void
Instrument::setStaffLines(int inLineCount)
{
    mStaffLines = inLineCount;
}

void
Instrument::applyLocalization(std::string_view inName,
                              std::string_view inAbbreviation,
                              std::string_view inFamily)
{
    name = inName;
    abbreviation = inAbbreviation;
    family = inFamily;
}

SoundID
Instrument::getSoundID() const
{
    return mSoundID;
}

void
Instrument::setSoundID(SoundID inSoundID)
{
    mSoundID = inSoundID;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
