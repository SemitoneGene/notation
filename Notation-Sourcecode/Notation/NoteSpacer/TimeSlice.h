// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#pragma once

// Local
#include "Forward.h"
#include "Geometry.h"

namespace komp
{

    /// a struct to hold info about the temporal along with
    /// the temporal itself
    class SliceMember
    {
    public:
        SliceMember(Temporal& inTemporal);
        Temporal& temporal;

        // the desirable width for a note of this type (e.g.
        // per the ratios provided by Elaine Gould)
        double widthForType;

        // the current x relative to the measure's dataX
        // in pixels
        double currentX;

        // the current frame of the temporal, in pixels,
        // moved so that it is RELATIVE to the MEASURE'S DATA X
        Rect currentFrame;

        // the amount of space, in pixels, that the
        // temporal requires to the left of its getX() value
        // to avoid collision
        double leftMargin;

        // the amount of space, in pixels, that the
        // temporal requires to the right of its getX() value
        // to avoid collision
        double rightMargin;

        // the amount of space, in pixels, that is
        // needed between this temporals getX() and the next
        // temporal's getX()
        double trailingDistance;

        // a temporal (if present) which follows this temporal
        // in the same measure, track and hand, and which (if
        // there is more than one immediately following) has
        // the largest left Margin
        Temporal* nextTemporal;

        // the frame of the next temporal, in pixels,
        // moved so that it is RELATIVE to the MEASURE'S DATA X
        Rect nextTemporalFrame;

        // the amount of space, in pixels, that the
        // next temporal requires to the left of its getX() value
        // to avoid collision
        double nextTemporalLeftMargin;
    };

    using SliceMembers = std::vector<SliceMember>;

    /// NoteSlice represents a collection of temporals
    /// which start at the same time, in the same measure,
    /// and thus need to be aligned vertically.  NoteSlice
    /// objects will be used by the NoteSpacing algorithm.
    ///
    class TimeSlice
    {
    public:
        TimeSlice(int inRoundedTime);

        int getTime() const;
        void addMember(SliceMember&& inMember);
        const SliceMembers& getMembers() const;
        SliceMembers& getMembers();
        double getX() const;
        void setX(double inValue);

    private:
        int mTime;
        SliceMembers mMembers;
        double mX;

    };

} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
