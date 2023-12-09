// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

// Self
#include "TimeSlice.h"

// Local
#include "Measure.h"
#include "Temporal.h"
#include "Throw.h"
#include "TemporalValue.h"
#include "TemporalConstants.h"

namespace komp
{
    SliceMember::SliceMember(Temporal& inTemporal)
    : temporal{inTemporal}
    , currentFrame{inTemporal.getFrame()}
    , widthForType{0.0}
    , currentX{inTemporal.getX()}
    , leftMargin{0.0}
    , rightMargin{0.0}
    , trailingDistance{0.0}
    , nextTemporal{nullptr}
    , nextTemporalFrame{}
    , nextTemporalLeftMargin{0.0}
    {
        const auto duration = inTemporal.getDuration();
        const double spacesForTypeAndDots = TemporalValue::getSpacesForTypeAndDots(duration.getTemporalType(),
                                                                                   duration.getDots());
        const double spaceSize = inTemporal.getSpaceSize();
        widthForType = spacesForTypeAndDots * spaceSize;
        const auto measureDataX = inTemporal.getLocation().getMeasure().getDataX();
        currentFrame.moveBy(-1.0 * measureDataX, 0.0);
        leftMargin = currentX - currentFrame.left;
        rightMargin = currentFrame.right - currentX;
        leftMargin += (MINIMUM_TEMPORAL_SPACES_LEFT * spaceSize);
        rightMargin += (MINIMUM_TEMPORAL_SPACES_RIGHT * spaceSize);
    }


    TimeSlice::TimeSlice(int inRoundedTime)
    : mTime{inRoundedTime}
    , mMembers{}
    {

    }


    int
    TimeSlice::getTime() const
    {
        return mTime;
    }


    void
    TimeSlice::addMember(SliceMember&& inMember)
    {
        KOMP_ASSERT(inMember.temporal.getRoundedTime() == getTime());
        mMembers.emplace_back(std::move(inMember));
    }


    const SliceMembers&
    TimeSlice::getMembers() const
    {
        return mMembers;
    }


    SliceMembers&
    TimeSlice::getMembers()
    {
        return mMembers;
    }

    
    double
    TimeSlice::getX() const
    {
        return mX;
    }


    void
    TimeSlice::setX(double inValue)
    {
        mX = inValue;
    }

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
