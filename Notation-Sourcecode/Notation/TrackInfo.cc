// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TrackInfo.h"

namespace komp
{

TrackInfo::TrackInfo()
{
}

TrackInfo::TrackInfo(Instrument inInstrument, std::string_view inUuid)
: mInstrument{std::move(inInstrument)}
{
    if (!inUuid.empty()) {
        mUuid = inUuid;
    }
}

std::string_view
TrackInfo::getName() const
{
    return mInstrument.name;
}


void
TrackInfo::setName(std::string inName)
{
    mInstrument.name = std::move(inName);
}


std::string_view
TrackInfo::getAbbreviation() const
{
    return mInstrument.abbreviation;
}


void
TrackInfo::setAbbreviation(std::string inAbbreviation)
{
    mInstrument.abbreviation = std::move(inAbbreviation);
}


int
TrackInfo::getNumStaves() const
{
    return mInstrument.getNumStaves();
}

void
TrackInfo::addStaffInfo(StaffInfo inStaffInfo)
{
    mInstrument.startingClefs.push_back(inStaffInfo.clefType);
}


void
TrackInfo::addStaffInfo(ClefType inClefType)
{
    mInstrument.startingClefs.push_back(inClefType);
}


void
TrackInfo::clearStaffInfoList()
{
    mInstrument.startingClefs.clear();
}


SoundID
TrackInfo::getSoundID() const
{
    return mInstrument.getSoundID();
}


void
TrackInfo::setSoundID(SoundID inSoundID)
{
    mInstrument.setSoundID(inSoundID);
}

const Instrument&
TrackInfo::getInstrument() const
{
    return mInstrument;
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
