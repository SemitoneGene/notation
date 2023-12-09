// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "PlaybackNote.h"

// Local
#include "Constants.h"

namespace
{
static constexpr const double MaxVelocity = 127.0;
}

namespace komp
{
PlaybackNote::PlaybackNote()
: mIsSilent{false}
, mStartTime{0.0}
, mPitch{}
, mDurations{}
, mLoudness{0.0}
, mOffLoudness{0.0}
{
    
}


bool
PlaybackNote::getIsSilent() const
{
    return mIsSilent;
}


double
PlaybackNote::getTimeBase() const
{
    // TODO: use this in the future if TimeBase becomes configurable
    return static_cast<double>(TIME_BASE);
}


double
PlaybackNote::getStartTime() const
{
    return mStartTime;
}


double
PlaybackNote::getTimeDuration() const
{
    // Multiple durations generally indicate then existence
    // of a tie chain.  We will accumulate the durations of
    // all the durations. We will use the playback-adjusted
    // clocks for the last duration, but the full duration
    // for any durations that come before it
        
    if (mDurations.size() == 0) {
        return 0.0;
    }
    
    const auto& lastDuration = mDurations.back();
    double clocks = 0.0;
    
    for (const auto& duration : mDurations) {
        
        if (&duration != &lastDuration) {
            clocks += duration.getClocks();
        } else {
            clocks += duration.getSustainClocks();
        }
    }
    
    return clocks;
}


double
PlaybackNote::getStopTime() const
{
    return getStartTime() + getTimeDuration();
}


const Pitch&
PlaybackNote::getPitch() const
{
    return mPitch;
}


void
PlaybackNote::setIsSilent(bool inValue)
{
    mIsSilent = inValue;
}


void
PlaybackNote::setTimeBase(double inTimeTimeBase)
{
    // TODO: use this in the future if TimeBase becomes configurable
    // mTimeBase = std::max(1.0, inTimeTimeBase);
}


void
PlaybackNote::setStartTime(double inTime)
{
    mStartTime = std::max(0.0, inTime);
}


void
PlaybackNote::setPitch(Pitch inPitch)
{
    mPitch = std::move(inPitch);
}

int
PlaybackNote::getVelocity() const
{
    const auto value = MaxVelocity * static_cast<double>(getLoudness());
    return static_cast<int>(std::ceil(value - 0.5));
}

void
PlaybackNote::setVelocity(int inValue)
{
    inValue = std::max(inValue, 0);
    inValue = std::min(inValue, static_cast<int>(MaxVelocity));
    const auto value = static_cast<double>(inValue) / MaxVelocity;
    setLoudness(value);
}

int
PlaybackNote::getOffVelocity() const
{
    const auto value = MaxVelocity * static_cast<double>(getOffLoudness());
    return static_cast<int>(std::ceil(value - 0.5));
}

void
PlaybackNote::setOffVelocity(int inValue)
{
    inValue = std::max(inValue, 0);
    inValue = std::min(inValue, static_cast<int>(MaxVelocity));
    const auto value = static_cast<double>(inValue) / MaxVelocity;
    setOffLoudness(value);
}

double
PlaybackNote::getLoudness() const
{
    return mLoudness;
}


double
PlaybackNote::getOffLoudness() const
{
    return mOffLoudness;
}


void
PlaybackNote::addDuration(Duration inDuration)
{
    mDurations.emplace_back(std::move(inDuration));
}


void
PlaybackNote::setLoudness(double inValue)
{
    mLoudness = std::max(inValue, 0.0);
    mLoudness = std::min(mLoudness, 1.0);
}


void
PlaybackNote::setOffLoudness(double inValue)
{
    mOffLoudness = std::max(inValue, 0.0);
    mOffLoudness = std::min(mOffLoudness, 1.0);
}

}

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
